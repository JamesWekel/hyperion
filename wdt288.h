/* WDT288.H    (C) Copyright Jan Jaeger, 1999-2012                   */
/*              (C) Copyright Roger Bowler, 1999-2012                */
/*                                                                   */
/*              Watchdog Timer - Diagnose 0x288                      */
/*            (Control Virtual Machine Time Bomb)                    */
/*                                                                   */
/*   Released under "The Q Public License Version 1"                 */
/*   (http://www.hercules-390.org/herclic.html) as modifications to  */
/*   Hercules.                                                       */

#ifndef _WDT288_H_
#define _WDT288_H_

/*-----------------------------------------------------------*/
/*  Diagnose 288: Watchdog Timer  support                    */
/*-----------------------------------------------------------*/

/* Diag 288 : watchdog timer headers                         */
/*-----------------------------------------------------------*/
/* Start of headers and defines copied from                  */
/* https://github.com/torvalds/linux                         */
/*      file 'arch/s390/include/asm/diag288.h' and           */
/*      file 'drivers/watchdog/diag288_wdt.c                 */
/* The copied headers have been modified                     */
/*-----------------------------------------------------------*/

/* constants */
#define WDT288_MIN_INTERVAL 15	    /* Minimal time supported by diag288 */
#define WDT288_MAX_INTERVAL 3600    /* One hour should be enough - pure estimation */

#define WDT288_MAX_CMDLEN   240     /* Max length of cmds string */

#define WDT288_DEFAULT_TIMEOUT 30
#define WDT288_US_SLEEP     250000

/* Diag 0x288 Function codes - init, change, cancel */
#define WDT288_FUNC_INIT    0
#define WDT288_FUNC_CHANGE  1
#define WDT288_FUNC_CANCEL  2

/* Helpers */
#define WDT288_IS_DISABLED          ( sysblk.wdt288_enabled == 0 )
#define WDT288_IS_ENABLED           ( sysblk.wdt288_enabled == 1 )
#define WDT288_IS_ENABLED_INACTIVE  ( sysblk.wdt288_enabled == 1 && sysblk.wdt288_active == 0 )
#define WDT288_IS_ENABLED_ACTIVE    ( sysblk.wdt288_enabled == 1 && sysblk.wdt288_active == 1 )
#define WDT288_IS_SHUTDOWN          ( sysblk.wdt288_canceled == 1 )

#define OBTAIN_WDT288_LOCK()        obtain_lock(  &sysblk.wdt288_lock )
#define RELEASE_WDT288_LOCK()       release_lock( &sysblk.wdt288_lock )

/* Functions in module wdt288.c */
/* ---------------------------- */
/* used in Diag 0x288 and       */
/* used in hsccmd.c for 'wdt'   */
/* ---------------------------- */
int wdt288_init( U32 timeout );
int wdt288_change( U32 timeout );
int wdt288_cancel( U32 timeout );

U64 wdt288_get_expire_time( U32 timeout );
U32 wdt288_get_timeout( );
int wdt288_set_cmdsep( char );
int wdt288_set_disabled( );
int wdt288_set_enabled( );
int wdt288_set_expiry_cmds( char* );
int wdt288_show_status( );
int wdt288_test_expire( );

#endif /* _WDT288_H_ */
