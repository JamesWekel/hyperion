/* WDT288.C    (C) Copyright Jan Jaeger, 1999-2012                   */
/*              (C) Copyright Roger Bowler, 1999-2012                */
/*                                                                   */
/*              Watchdog Timer - Diagnose 0x288                      */
/*            (Control Virtual Machine Time Bomb)                    */
/*                                                                   */
/*   Released under "The Q Public License Version 1"                 */
/*   (http://www.hercules-390.org/herclic.html) as modifications to  */
/*   Hercules.                                                       */

#include "hstdinc.h"
#define _WDT288_C_
#define _HENGINE_DLL_

#include "hercules.h"
#include "opcode.h"
#include "inline.h"

#include "wdt288.h"

/* forward references */
int wdt288_do_cmds( char*, char );

/*-----------------------------------------------------------*/
/* Diagnose 288: Watchdog Timer: ASYNC Action Commands Thread*/
/*-----------------------------------------------------------*/
/* Note: the  wdt288_doing_cmds flag indicates if this       */
/* thread is executing commands                              */
/*-----------------------------------------------------------*/
static void* wdt288_action_cmds_thread( void* arg )
{
    int   rc;
    char* cmdline;
    char  cmdsep;

    UNREFERENCED( arg );
    UNREFERENCED( rc );

    if (sysblk.wdt288_debug)
        logmsg( ">>>> wdt288_action_cmds_thread - started\n" );

    /* Set action command thread priority to Server thread          */
    /* priority in order to action commands if guest OS has not     */
    /* reset the timer and the timeout expires.                     */
    SET_THREAD_PRIORITY( sysblk.srvprio, sysblk.qos_user_initiated );

    /* Mark the thread as active */
    OBTAIN_WDT288_LOCK();
    {
        if ( sysblk.wdt288_doing_cmds )
        {
            /* already doing cmds - ignore this request */
            WRMSG( HHC01959, "I", "wdt288_action_cmds_thread: already doing cmds, ignoring request"  );
            RELEASE_WDT288_LOCK();
            return NULL;
        }

        sysblk.wdt288_doing_cmds = TRUE;
        cmdline = strdup( sysblk.wdt288_cmds );
        cmdsep = sysblk.wdt288_cmdsep;

    }
    RELEASE_WDT288_LOCK();

    WRMSG( HHC01958, "I", "Timer Expired. Executing", cmdline );
    wdt288_do_cmds( cmdline, cmdsep );

    /* Mark the thread as done */
    OBTAIN_WDT288_LOCK();
    {
        sysblk.wdt288_doing_cmds = FALSE;
        free (cmdline);
    }
    RELEASE_WDT288_LOCK();

    if (sysblk.wdt288_debug)
        logmsg( ">>>> wdt288_action_cmds_thread - ended\n" );
    return NULL;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Watchdog Timer Thread (wdt288_thread)      */
/*-----------------------------------------------------------*/
/* Note: the sysblk.wdt288_active flag indicates if the      */
/* timer is active and is only set when the thread is        */
/* running                                                   */
/*-----------------------------------------------------------*/
static void* wdt288_thread( void* arg )
{
    int   rc;
    TID   tid;

    UNREFERENCED( arg );
    UNREFERENCED( rc );

    LOG_THREAD_BEGIN( WDT288_THREAD_NAME );

    /* Set watchdog priority to Server thread priority              */
    /* in order to action commands if guest OS has not reset        */
    /* the timer and the timeout expires. No impact as it           */
    /* should just be waiting anyway until a timeout.               */
    SET_THREAD_PRIORITY( sysblk.srvprio, sysblk.qos_user_initiated );

    /* Mark the timer as active */
    OBTAIN_WDT288_LOCK();
    {
        sysblk.wdt288_active = TRUE;
    }
    RELEASE_WDT288_LOCK();

    /* Watchdog thread main loop - wait for timer to be canceled */
    while ( true )
    {
        OBTAIN_WDT288_LOCK();
        {
            /* timer was closed/cancelled or we are in shutdown mode?*/
            if ( sysblk.wdt288_canceled || sysblk.shutdown)
            {
                break;
            }

            /* timer expired? and we are not already doing cmds */
            if ( sysblk.wdt288_expire_time < wdt288_get_expire_time( 0 ) && !sysblk.wdt288_doing_cmds )
            {
                rc = create_thread( &tid, DETACHED, wdt288_action_cmds_thread,
                                    NULL, "WDT288 do cmds");
                if(rc)
                    WRMSG(HHC00102, "E", strerror(rc));
            }
        }
        RELEASE_WDT288_LOCK();
        USLEEP( WDT288_US_SLEEP );
    }

    /* timer ended; reset state to inactive*/
    /* already have lock */
    sysblk.wdt288_canceled = FALSE;
    sysblk.wdt288_active = FALSE;
    sysblk.wdt288_tid = 0;
    sysblk.wdt288_expire_time = 0;

    RELEASE_WDT288_LOCK();

    LOG_THREAD_END( WDT288_THREAD_NAME );

    return NULL;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Helpers                                    */
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/*  Diagnose 288: Initialize Timer                           */
/*-----------------------------------------------------------*/
int wdt288_init( U32 timeout )
{
    int rc = 0;
    int n;

     /* Initialize the watchdog timer state */
    sysblk.wdt288_canceled = FALSE;
    sysblk.wdt288_active = FALSE;
    sysblk.wdt288_expire_time = wdt288_get_expire_time( timeout );

    /* Start the watchdog thread */
    rc = create_thread( &sysblk.wdt288_tid, DETACHED,
        wdt288_thread, NULL, WDT288_THREAD_NAME );
    if (rc)
    {
        // "Error in function create_thread(): %s"
        WRMSG( HHC00102, "E", strerror( rc ));
        return rc;
    }

    /* Note: in a Diag 288 instruction so only a short wait for timer startup */
    n = 10;
    while ( n-- >  0 )
    {
        usleep( WDT288_US_SLEEP );

        OBTAIN_WDT288_LOCK();
        {
            if ( sysblk.wdt288_active == TRUE)
            {
                RELEASE_WDT288_LOCK();
                break;
            }
        }
        RELEASE_WDT288_LOCK();
    }
    if (n <= 0)
    {
        // something happened; force cancel (without lock)
        sysblk.wdt288_canceled = TRUE;
        rc = -1;
    }

    return rc;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Reset/Change Timer Timeout                 */
/*-----------------------------------------------------------*/
int wdt288_change( U32 timeout )
{
    int rc = 0;

    OBTAIN_WDT288_LOCK();
    {
        sysblk.wdt288_expire_time = wdt288_get_expire_time( timeout );
    }
    RELEASE_WDT288_LOCK();

    return rc;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Cancel/close Timer                         */
/*-----------------------------------------------------------*/
int wdt288_cancel( U32 timeout )
{
    int rc = 0;
    int n;

    UNREFERENCED( timeout );

    /* initiate timer thread shutdown*/
    OBTAIN_WDT288_LOCK();
    {
        sysblk.wdt288_canceled = TRUE;
    }
    RELEASE_WDT288_LOCK();

    /* Note: in a Diag 288 instruction so only a short wait for timer to stop */
    n = 10;
    while ( n-- >  0 )
    {
        usleep( WDT288_US_SLEEP );

        OBTAIN_WDT288_LOCK();
        {
            if ( sysblk.wdt288_active == FALSE)
            {
                RELEASE_WDT288_LOCK();
                break;
            }
        }
        RELEASE_WDT288_LOCK();
    }
    if (n <= 0)  rc = -1;

    return rc;
}

/*-----------------------------------------------------------*/
/*  Timer Expired: Process Commands  (recursive)             */
/*-----------------------------------------------------------*/
/* reference: cmdtab.c: the_real_panel_command(char* cmdline)*/
/*-----------------------------------------------------------*/
/* Note: cmdline is modified                                 */
/*-----------------------------------------------------------*/
int wdt288_do_cmds( char* cmdline, char cmdsep )
{
    int rc = 0;                         /* Return Code from command  */

    if (sysblk.wdt288_debug)
        logmsg( ">>>>> wdt288_do_cmds- executing commands: %s\n", cmdline );

    /* Handle command separation */
    if (cmdsep)
    {
        /* Does cmdline contain any separator characters? */
        char*  firstsep  = strchr( cmdline, cmdsep );
        if (firstsep)
        {
            /* Yes! Must process commands separately */
            size_t  sepcharindex  = (firstsep - cmdline);
            char*   first_cmd     = strdup( cmdline );

            /* Mark end of first command */
            first_cmd[ sepcharindex ] = 0;

            TRIM( first_cmd );
            if (strncasecmp( first_cmd, "PAUSE", 5 ) == 0)
            {
                /* PAUSE command - pause for specified number of seconds */
                char*  arg = first_cmd + 5;
                int     pause_seconds = atoi( arg );

                if (pause_seconds > 0)
                {
                    // "Diagnose 0x288 Watchdog Timer: %s: %d"
                    WRMSG( HHC01955, "I", "pausing for seconds", pause_seconds);
                    SLEEP( pause_seconds );
                }
                else
                {
                    // "Diagnose 0x288 Watchdog Timer: %s: %d"
                    WRMSG( HHC01955, "E", "pause seconds value <= 0", pause_seconds);
                }
            }

            else if (strncasecmp( first_cmd, "WDT", 3 ) == 0)
            {
                // "Diagnose 0x288 Watchdog Timer: %s: %s"
                WRMSG( HHC01958, "I", "ignoring wdt command", first_cmd);
            }

            else
            {
                /* Process command */
                if (strlen( first_cmd ) > 0)
                    panel_command( first_cmd );
            }

            /* Process remaining command(s) */
            rc = wdt288_do_cmds( first_cmd + sepcharindex + 1, cmdsep );

            /* Prevent memory leak */
            free( first_cmd );

            /* Return with retcode of second command */
            return rc;
        }
    }

    /* Only one command  */
    rc =  (int) (uintptr_t) panel_command( cmdline );

    return rc;
}

// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns)/1000)

/*-----------------------------------------------------------*/
/*  Diagnose 288: Get Expire Time (microseconds)             */
/*-----------------------------------------------------------*/
U64 wdt288_get_expire_time( U32 timeout )
{
    U64 us          = 0;

    {
    #if defined( __linux__ )
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec + 500);

    #else
        // microsecond resolution getimeofday
        struct timeval  tv;
        gettimeofday( &tv, NULL );
        us = SEC_TO_US((uint64_t)tv.tv_sec) + tv.tv_usec;

    #endif
    }

    return us + SEC_TO_US(timeout);
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Get Timeout (remaining seconds)            */
/*-----------------------------------------------------------*/
U32 wdt288_get_timeout(  )
{
    U64 us          = 0;
    if (sysblk.wdt288_expire_time > 0)
    {
        us = sysblk.wdt288_expire_time - wdt288_get_expire_time( 0 );

        return (U32)(us / SEC_TO_US(1));
    }

    /* timer is not active */
    return 0;

}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Test Timer Expiry                          */
/*-----------------------------------------------------------*/
/* return: 0: OK, 1: not active                              */
int wdt288_test_expire( )
{
    int rc;

    /* expire the active watchdog timer */
    OBTAIN_WDT288_LOCK();
    {
        if ( WDT288_IS_ENABLED_ACTIVE )
        {
            // Note: only get here for wdt_cmd. Issue message
            // before the actual expiry happens
            WRMSG( HHC02204 , "I", "WDT", "expired" );

            sysblk.wdt288_expire_time = 0;
            RELEASE_WDT288_LOCK();

            usleep( WDT288_US_SLEEP + WDT288_US_SLEEP/10 );     // 110% of timer thread

            OBTAIN_WDT288_LOCK();
            sysblk.wdt288_expire_time = wdt288_get_expire_time( WDT288_DEFAULT_TIMEOUT );

            rc = 0;
        }

        else
        {
            /* timer is not active */
            rc = 1;
        }
    }
    RELEASE_WDT288_LOCK();

    return rc;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Set Expiry Commands                        */
/*-----------------------------------------------------------*/
/* return: 0: OK                                             */
/*        -1: enabled; cmds can not be null                  */
/*        -2: malloc error                                   */
/*        -3: cmds > WDT288_MAX_CMDLEN                       */
/*-----------------------------------------------------------*/
int wdt288_set_expiry_cmds( char* cmds )
{
    int cmdslen = 0;

    if (cmds != NULL)
        cmdslen = strlen( cmds );

    if ( cmdslen > WDT288_MAX_CMDLEN )
        return -3;

    OBTAIN_WDT288_LOCK();
    {
        if ( WDT288_IS_ENABLED && cmdslen == 0 )
        {
            // "Watchdog timer is enabled, commands cannot be empty"
            RELEASE_WDT288_LOCK();
            return -1;
        }

        /* already have cmds? */
        if ( sysblk.wdt288_cmds != NULL )
        {
            free( sysblk.wdt288_cmds );
            sysblk.wdt288_cmds = NULL;
        }

        /* set new cmds */
        if ( cmdslen > 0 )
        {
            sysblk.wdt288_cmds = malloc( cmdslen + 1 );
            if (sysblk.wdt288_cmds == NULL)
            {
                RELEASE_WDT288_LOCK();
                return -2;
            }
            strncpy( sysblk.wdt288_cmds, cmds, cmdslen );
            sysblk.wdt288_cmds[cmdslen] = '\0';
        }
    }
    RELEASE_WDT288_LOCK();

    return 0;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Set Command Separator                      */
/*-----------------------------------------------------------*/
/* return: 0: OK                                             */
/*-----------------------------------------------------------*/
int wdt288_set_cmdsep( char cmdsep )
{
    OBTAIN_WDT288_LOCK();
    {
        sysblk.wdt288_cmdsep = cmdsep;
    }
    RELEASE_WDT288_LOCK();
    return 0;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Set Enabled                                */
/*-----------------------------------------------------------*/
/* return: 0: OK                                             */
/*        -1: disabled; cmds can not be null                 */
/*-----------------------------------------------------------*/
int wdt288_set_enabled( )
{
    int rc;

    OBTAIN_WDT288_LOCK();
    {
        if ( WDT288_IS_DISABLED && sysblk.wdt288_cmds == NULL )
        {
            //no commands have been defined; can not be enabled
            rc = -1;
        }

        else
        {
            sysblk.wdt288_enabled = true;
            rc = 0;
        }
    }
    RELEASE_WDT288_LOCK();
    return rc;
}


/*-----------------------------------------------------------*/
/*  Diagnose 288: Set Disabled                               */
/*-----------------------------------------------------------*/
/* return: 0: OK                                             */
/*        -1: enabled-active, can not be disabled            */
/*-----------------------------------------------------------*/
int wdt288_set_disabled( )
{
    int rc;

    OBTAIN_WDT288_LOCK();
    {
        if ( WDT288_IS_ENABLED_ACTIVE )
        {
            // "Watchdog timer is enabled-active, can not be disabled"
            rc = -1;
        }

        else
        {
            sysblk.wdt288_enabled = false;
            rc = 0;
        }
    }
    RELEASE_WDT288_LOCK();
    return rc;
}

/*-----------------------------------------------------------*/
/*  Diagnose 288: Show Status                                */
/*-----------------------------------------------------------*/
/* return: 0: OK                                             */
/*-----------------------------------------------------------*/
int wdt288_show_status( )
{
    char buffer[1024];

    char status_timeout[128];
    char status_cmds[512];
    char status_cmdsep[32];
    char* status_active;

    OBTAIN_WDT288_LOCK();
    {
        status_active = WDT288_IS_ENABLED_ACTIVE ? "enabled-active"    :
                            WDT288_IS_ENABLED        ? "enabled-inactive"  : "disabled";

        // TIMEOUT
        if ( WDT288_IS_ENABLED_ACTIVE )
            sprintf(status_timeout, "timeout: \t%d seconds", wdt288_get_timeout() );
        else
            sprintf(status_timeout, "timeout: \t(none)" );

        // CMDSEP
        if ( sysblk.wdt288_cmdsep != '\0' )
            sprintf(status_cmdsep, "cmdsep: \t'%c'", sysblk.wdt288_cmdsep );
        else
            sprintf(status_cmdsep, "cmdsep: \t(none)" );

        // CMDS
        if ( sysblk.wdt288_cmds != NULL )
            sprintf(status_cmds, "cmds: \t\t\"%s\"", sysblk.wdt288_cmds );
        else
            sprintf(status_cmds, "cmds: \t\t(none)" );

        sprintf(buffer, "status: \n\tstate:\t\t%s\n \t%s\n \t%s\n \t%s",
            status_active,
            status_timeout,
            status_cmdsep,
            status_cmds
            );
        WRMSG( HHC01959, "I", buffer);
    }
    RELEASE_WDT288_LOCK();
    return 0;
}