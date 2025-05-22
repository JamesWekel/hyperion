/* ZVECTOR.C    (C) Copyright Jan Jaeger, 1999-2012                  */
/*              (C) Copyright Roger Bowler, 1999-2012                */
/*              z/Arch Vector Operations                             */
/*                                                                   */
/*   Released under "The Q Public License Version 1"                 */
/*   (http://www.hercules-390.org/herclic.html) as modifications to  */
/*   Hercules.                                                       */

/* Interpretive Execution - (C) Copyright Jan Jaeger, 1999-2012      */
/* z/Architecture support - (C) Copyright Jan Jaeger, 1999-2012      */

#include "hstdinc.h"
#define _ZVECTOR_C_
#define _HENGINE_DLL_

#include "hercules.h"
#include "opcode.h"
#include "inline.h"

/* ====================================================================== */
/* ZVECTOR_END macro for debugging Vector instructions                    */
/* Note: block comments are used to avoid gcc                             */
/*       warning: multi-line comment [-Wcomment]                          */

// To display the results of all Vector instructions, uncomment the
// following three lines:-

/*
    #undef ZVECTOR_END
    #define ZVECTOR_END(_regs)                                      \
                ARCH_DEP(display_inst) (_regs, inst);
*/

// To display the results of specific Vector instructions, uncomment
// and modify the following four lines:-

/*
    #undef ZVECTOR_END
    #define ZVECTOR_END(_regs)                                      \
            if (inst[5] == 0x3E || inst[5] == 0x36)                 \
                ARCH_DEP(display_inst) (_regs, inst);
*/
/* ====================================================================== */

#if defined( FEATURE_129_ZVECTOR_FACILITY )

  // #undef FEATURE_V128_SSE

    /* ARM AArch64 processor? */
    #if defined( __aarch64__ ) &&  defined( __ARM_NEON )

        /* Gcc 10+ / Clang 11+ */
        #if ( (defined( __clang_major__ ) && __clang_major__ >= 11  ) ||   \
            (defined( __GNUC__ ) && __GNUC__ >= 10  )                    \
            )

            // use sse2neon for translation of SSE interinsics to Arm Neon intrinsics
            #define SSE2NEON_SUPPRESS_WARNINGS
            #include "sse2neon.h"
            #define FEATURE_V128_NEON 1
            #define FEATURE_V128_SSE 1
        #endif

    #endif

  /* compile debug message: are we using intrinsics? */
  #if 1
    #if defined( FEATURE_V128_SSE )
      #pragma message("FEATURE_V128_SSE is defined.  Using intrinsics." )
    #else
      #pragma message("No intrinsics are included for optimization; only compiler optimization")
    #endif

    #if defined( FEATURE_HW_CLMUL )
        #pragma message("FEATURE_HW_CLMU is defined.  Using intrinsics." )
    #else
        #pragma message("No FEATURE_HW_CLMU intrinsics are included for optimization; only compiler optimization")
    #endif
  #endif

/*------------------------------------------------------------------------*/
/* See zvector2.c for the following Vector instructions.                  */
/*------------------------------------------------------------------------*/
/* E601 VLEBRH  - VECTOR LOAD BYTE REVERSED ELEMENT (16)          [VRX]   */
/* E602 VLEBRG  - VECTOR LOAD BYTE REVERSED ELEMENT (64)          [VRX]   */
/* E603 VLEBRF  - VECTOR LOAD BYTE REVERSED ELEMENT (32)          [VRX]   */
/* E604 VLLEBRZ - VECTOR LOAD BYTE REVERSED ELEMENT AND ZERO      [VRX]   */
/* E605 VLBRREP - VECTOR LOAD BYTE REVERSED ELEMENT AND REPLICATE [VRX]   */
/* E606 VLBR    - VECTOR LOAD BYTE REVERSED ELEMENTS              [VRX]   */
/* E607 VLER    - VECTOR LOAD ELEMENTS REVERSED                   [VRX]   */
/* E609 VSTEBRH - VECTOR STORE BYTE REVERSED ELEMENT (16)         [VRX]   */
/* E60A VSTEBRG - VECTOR STORE BYTE REVERSED ELEMENT (64)         [VRX]   */
/* E60B VSTEBRF - VECTOR STORE BYTE REVERSED ELEMENT (32)         [VRX]   */
/* E60E VSTBR   - VECTOR STORE BYTE REVERSED ELEMENTS             [VRX]   */
/* E60F VSTER   - VECTOR STORE ELEMENTS REVERSED                  [VRX]   */
/* E634 VPKZ    - VECTOR PACK ZONED                               [VSI]   */
/* E635 VLRL    - VECTOR LOAD RIGHTMOST WITH LENGTH               [VSI]   */
/* E637 VLRLR   - VECTOR LOAD RIGHTMOST WITH LENGTH (reg)         [VRS-d] */
/* E63C VUPKZ   - VECTOR UNPACK ZONED                             [VSI]   */
/* E63D VSTRL   - VECTOR STORE RIGHTMOST WITH LENGTH              [VSI]   */
/* E63F VSTRLR  - VECTOR STORE RIGHTMOST WITH LENGTH (reg)        [VRS-d] */
/* E649 VLIP    - VECTOR LOAD IMMEDIATE DECIMAL                   [VRI-h] */
/* E650 VCVB    - VECTOR CONVERT TO BINARY (32)                   [VRR-i] */
/* E651 VCLZDP  - VECTOR COUNT LEADING ZERO DIGITS                [VRR-k] */
/* E652 VCVBG   - VECTOR CONVERT TO BINARY (64)                   [VRR-i] */
/* E654 VUPKZH  - VECTOR UNPACK ZONED HIGH                        [VRR-k] */
/* E658 VCVD    - VECTOR CONVERT TO DECIMAL (32)                  [VRI-i] */
/* E659 VSRP    - VECTOR SHIFT AND ROUND DECIMAL                  [VRi-g] */
/* E65A VCVDG   - VECTOR CONVERT TO DECIMAL (64)                  [VRI-i] */
/* E65B VPSOP   - VECTOR PERFORM SIGN OPERATION DECIMAL           [VRI-g] */
/* E65C VUPKZL  - VECTOR UNPACK ZONED LOW                         [VRR-k] */
/* E65F VTP     - VECTOR TEST DECIMAL                             [VRR-g] */
/* E670 VPKZR   - VECTOR PACK ZONED REGISTER                      [VRI-f] */
/* E671 VAP     - VECTOR ADD DECIMAL                              [VRI-f] */
/* E672 VSRPR   - VECTOR SHIFT AND ROUND DECIMAL REGISTER         [VRI-f] */
/* E673 VSP     - VECTOR SUBTRACT DECIMAL                         [VRI-f] */
/* E674 VSCHP   - DECIMAL SCALE AND CONVERT TO HFP                [VRR-b] */
/* E677 VCP     - VECTOR COMPARE DECIMAL                          [VRR-h] */
/* E678 VMP     - VECTOR MULTIPLY DECIMAL                         [VRI-f] */
/* E679 VMSP    - VECTOR MULTIPLY AND SHIFT DECIMAL               [VRI-f] */
/* E67A VDP     - VECTOR DIVIDE DECIMAL                           [VRI-f] */
/* E67B VRP     - VECTOR REMAINDER DECIMAL                        [VRI-f] */
/* E67C VSCSHP  - DECIMAL SCALE AND CONVERT AND SPLIT TO HFP      [VRR-b] */
/* E67D VCSPH   - VECTOR CONVERT HFP TO SCALED DECIMAL            [VRR-j] */
/* E67E VSDP    - VECTOR SHIFT AND DIVIDE DECIMAL                 [VRI-f] */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* See nnpa.c for the following Specialized-Function-Assist instructions. */
/*------------------------------------------------------------------------*/
/* E655 VCNF   - VECTOR FP CONVERT TO NNP                         [VRR-a] */
/* E656 VCLFNH - VECTOR FP CONVERT AND LENGTHEN FROM NNP HIGH     [VRR_a] */
/* E65D VCFN   - VECTOR FP CONVERT FROM NNP                       [VRR-a] */
/* E65E VCLFNL - VECTOR FP CONVERT AND LENGTHEN FROM NNP LOW      [VRR-a] */
/* E675 VCRNF  - VECTOR FP CONVERT AND ROUND TO NNP               [VRR-c] */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* See ieee.c for the following Vector Floating-Point instructions.       */
/*------------------------------------------------------------------------*/
/* E74A VFTCI  - Vector FP Test Data Class Immediate              [VRI-e] */
/* E78E VFMS   - Vector FP Multiply and Subtract                  [VRR-e] */
/* E78F VFMA   - Vector FP Multiply and Add                       [VRR-e] */
/* E79E VFNMS  - Vector FP Negative Multiply And Subtract         [VRR-e] */
/* E79F VFNMA  - Vector FP Negative Multiply And Add              [VRR-e] */
/* E7C0 VCLFP  - Vector FP Convert To Logical (short BFP to 32)   [VRR-a] */
/* E7C0 VCLGD  - Vector FP Convert To Logical (long BFP to 64)    [VRR-a] */
/* E7C1 VCFPL  - Vector FP Convert From Logical (32 to short BFP) [VRR-a] */
/* E7C1 VCDLG  - Vector FP Convert From Logical (64 to long BFP)  [VRR-a] */
/* E7C2 VCSFP  - Vector FP Convert To Fixed (short BFP to 32)     [VRR-a] */
/* E7C2 VCGD   - Vector FP Convert To Fixed (long BFP to 64)      [VRR-a] */
/* E7C3 VCFPS  - Vector FP Convert From Fixed (32 to short BFP)   [VRR-a] */
/* E7C3 VCDG   - Vector FP Convert From Fixed (64 to long BFP)    [VRR-a] */
/* E7C4 VFLL   - Vector FP Load Lengthened                        [VRR-a] */
/* E7C5 VFLR   - Vector FP Load Rounded                           [VRR-a] */
/* E7C7 VFI    - Vector Load FP Integer                           [VRR-a] */
/* E7CA WFK    - Vector FP Compare and Signal Scalar              [VRR-a] */
/* E7CB WFC    - Vector FP Compare Scalar                         [VRR-a] */
/* E7CC VFPSO  - Vector FP Perform Sign Operation                 [VRR-a] */
/* E7CE VFSQ   - Vector FP Square Root                            [VRR-a] */
/* E7E2 VFS    - Vector FP Subtract                               [VRR-c] */
/* E7E3 VFA    - Vector FP Add                                    [VRR-c] */
/* E7E5 VFD    - Vector FP Divide                                 [VRR-c] */
/* E7E7 VFM    - Vector FP Multiply                               [VRR-c] */
/* E7E8 VFCE   - Vector FP Compare Equal                          [VRR-c] */
/* E7EA VFCHE  - Vector FP Compare High or Equal                  [VRR-c] */
/* E7EB VFCH   - Vector FP Compare High                           [VRR-c] */
/* E7EE VFMIN  - Vector FP Minimum                                [VRR-c] */
/* E7EF VFMAX  - Vector FP Maximum                                [VRR-c] */
/*------------------------------------------------------------------------*/


/*===================================================================*/
/* Achitecture Independent Routines                                  */
/*===================================================================*/

#if !defined(_ZVECTOR_ARCH_INDEPENDENT_)
#define _ZVECTOR_ARCH_INDEPENDENT_


/*-------------------------------------------------------------------*/
/* 128 bit types                                                     */
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/* are the compiler 128 bit types available?                         */
/*-------------------------------------------------------------------*/
#if defined( __SIZEOF_INT128__ )
    #define _USE_128_
#endif

/*-------------------------------------------------------------------*/
/* U128                                                              */
/*-------------------------------------------------------------------*/
typedef union {
        QW   Q;
#if defined( _USE_128_ )
    unsigned __int128 u_128;
#endif
        U64  u_64[2];
        U32  u_32[4];
        U16  u_16[8];
        U8   u_8[16];

#if defined( _USE_128_ )
    __int128 s_128;
#endif
        S64  s_64[2];
        S32  s_32[4];
        S16  s_16[8];
        S8   s_8[16];

#if defined( FEATURE_V128_SSE )
        __m128i V;      // intrinsic type vector
#endif

}  U128  ;

/*===================================================================*/
/* U128 Arithmetic (add, sub, mul)                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/* U128 Add: return a + b                                            */
/*-------------------------------------------------------------------*/
static inline U128 U128_add( U128 a, U128 b)
{
#if defined( _USE_128_ )
    U128 temp;                           /* temp (return) value      */

    temp.u_128 =  a.u_128 + b.u_128;
    return temp;

#else
    U128 temp;                           /* temp (return) value      */

    temp.Q.D.H.D =  a.Q.D.H.D + b.Q.D.H.D;
    temp.Q.D.L.D =  a.Q.D.L.D + b.Q.D.L.D;
    if (temp.Q.D.L.D < b.Q.D.L.D) temp.Q.D.H.D++;
    return temp;
#endif
}

/*-------------------------------------------------------------------*/
/* U128 Subtract: return a - b                                       */
/*-------------------------------------------------------------------*/
static inline U128 U128_sub( U128 a, U128 b)
{
#if defined( _USE_128_ )
    U128 temp;                           /* temp (return) value      */

    temp.u_128 =  a.u_128 - b.u_128;
    return temp;

#else
    U128 temp;                           /* temp (return) value      */

    temp.Q.D.H.D =  a.Q.D.H.D - b.Q.D.H.D;
    if (a.Q.D.L.D < b.Q.D.L.D) temp.Q.D.H.D--;
    temp.Q.D.L.D =  a.Q.D.L.D - b.Q.D.L.D;

    return temp;
#endif
}

/*-------------------------------------------------------------------*/
/* U128: U64 * U64 Multiply: return a * b (overflow ignored)         */
/*                                                                   */
/* Very simple, standard approach to arithmetic multiply             */
/*                                                                   */
/*                                                                   */
/*-------------------------------------------------------------------*/
static inline U128 U64_mul (U64 aa, U64 bb)
{
#if defined( _USE_128_)
    U128 temp;                           /* temp (return) value      */

    temp.u_128 =  (unsigned __int128) aa * bb;
    return temp;

#else
    DW a;                                /* arg 'aa' as DW            */
    DW b;                                /* arg 'bb' as DW            */
    DW t64;                              /* temp                      */
    U128 r;                              /* U128 multiply result      */
    U128 t128;                           /* temp                      */

    /* initialize result */
    r.Q.D.H.D = 0UL;
    r.Q.D.L.D = 0UL;

    /* zero check */
    if (aa == 0 || bb == 0) return r;

    /* arguments as DWs */
    a.D = aa;
    b.D = bb;

    /* a low 32 x b low 32 */
    if ( a.F.L.F != 0 && b.F.L.F!= 0 )
    {
        r.Q.D.L.D = (U64) a.F.L.F * (U64) b.F.L.F;
    }

    /* a high 32 x b low 32 */
    if ( a.F.H.F != 0 && b.F.L.F!= 0 )
    {
        t64.D =  (U64) a.F.H.F * (U64) b.F.L.F;
        t128.Q.D.H.D = 0UL;
        t128.Q.D.L.D = 0UL;
        t128.Q.D.H.F.L.F = t64.F.H.F;
        t128.Q.D.L.F.H.F = t64.F.L.F;
        r = U128_add( r, t128 );
    }

    /* a low 32 x b high 32 */
    if ( a.F.L.F != 0 && b.F.H.F!= 0 )
    {
        t64.D =  (U64) a.F.L.F * (U64) b.F.H.F;
        t128.Q.D.H.D = 0UL;
        t128.Q.D.L.D = 0UL;
        t128.Q.D.H.F.L.F = t64.F.H.F;
        t128.Q.D.L.F.H.F = t64.F.L.F;
        r = U128_add( r, t128 );
    }

    /* a high 32 x b high 32 */
    if ( a.F.H.F != 0 && b.F.H.F!= 0 )
    {
        t64.D =  (U64) a.F.H.F * (U64) b.F.H.F;
        t128.Q.D.H.D = 0UL;
        t128.Q.D.L.D = 0UL;
        t128.Q.D.H.F.L.F = t64.F.L.F;
        t128.Q.D.H.F.H.F = t64.F.H.F;
        r = U128_add( r, t128 );
    }

    return r;
#endif

}

/*-------------------------------------------------------------------*/
/* U128 * U32 Multiply: return a * b (overflow ignored)              */
/*                                                                   */
/* Very simple, standard approach to arithmetic multiply             */
/*                                                                   */
/*                                                                   */
/*-------------------------------------------------------------------*/
static inline U128 U128_U32_mul( U128 a, U32 b)
{
#if defined( _USE_128_ )
    U128 temp;                           /* temp (return) value      */

    temp.u_128 =  a.u_128 * b;
    return temp;

#else
    U128 r;                           /* return value                */
    U64 t;                            /* temp                        */


    /* initialize result */
    r.Q.D.H.D = 0UL;
    r.Q.D.L.D = 0UL;

    if (b == 0) return r;

    /* 1st 32 bits : LL */
    if (a.Q.F.LL.F != 0) r.Q.D.L.D = (U64) a.Q.F.LL.F * (U64) b;

    /* 2nd 32 bits : LH */
    if( a.Q.F.LH.F != 0)
    {
        t = (U64) a.Q.F.LH.F  * (U64) b  +  (U64) r.Q.F.LH.F;
        r.Q.F.LH.F = t & 0xFFFFFFFFUL;
        r.Q.F.HL.F = t >> 32;
    }

    /* 3rd 32 bits : HL */
    if( a.Q.F.HL.F != 0)
    {
        t = (U64) a.Q.F.HL.F  * (U64) b  +  (U64) r.Q.F.HL.F;
        r.Q.F.HL.F = t & 0xFFFFFFFFUL;
        r.Q.F.HH.F = t >> 32;
    }

    /* 4th 32 bits : HH */
    if( a.Q.F.HH.F != 0)
    {
        t = (U64) a.Q.F.HH.F  * (U64) b  +  (U64) r.Q.F.HH.F;
        r.Q.F.HH.F = t & 0xFFFFFFFFUL;
    }
    return r;
#endif
}

/*-------------------------------------------------------------------*/
/* Debug helper for U128                                             */
/*                                                                   */
/* Input:                                                            */
/*      msg     pointer to logmsg context string                     */
/*      u       U128 number                                          */
/*                                                                   */
/*-------------------------------------------------------------------*/
static inline void u128_logmsg(const char * msg, U128 u)
{
    printf("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", msg, u.Q.D.H.D, u.Q.D.L.D);
}

/* QW access helpers */
#if defined(WORDS_BIGENDIAN)
  #define QW_D(_i)  d[(_i)]                  /* Doubleword           */
  #define QW_F(_i)  f[(_i)]                  /* Fullword             */
  #define QW_H(_i)  h[(_i)]                  /* Halfword             */
  #define QW_B(_i)  b[(_i)]                  /* Byte                 */
#else
  #define QW_D(_i)  d[1-(_i)]                /* Doubleword           */
  #define QW_F(_i)  f[3-(_i)]                /* Fullword             */
  #define QW_H(_i)  h[7-(_i)]                /* Halfword             */
  #define QW_B(_i)  b[15-(_i)]               /* Byte                 */
#endif

#if defined ( FEATURE_V128_SSE )
/* ================================================================= */
/* intrinsic optimization helper routines                            */
/* ================================================================= */

/* ignore "unused-function" warnings */
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__GNUC__) && defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-function"
#endif

/* ================================================================= */
/* prototypes: return __m128i intrinsic vector                       */
/* ================================================================= */
static inline __m128i v128_add_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_add_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_add_64( const __m128i v2, const __m128i v3 );
static inline __m128i v128_add_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_add_compute_carry_16( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_add_compute_carry_32( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_add_compute_carry_8( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_and( const __m128i a, const __m128i b );
static inline __m128i v128_andnot( const __m128i a, const __m128i b );
static inline __m128i v128_average_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_average_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_average_logical_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_average_logical_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_bit_to_byte_mask(U16 bitmap);
static inline __m128i v128_bswap( const __m128i a  );
static inline __m128i v128_compare_equal_16( const __m128i a, const __m128i b );
static inline __m128i v128_compare_equal_32( const __m128i a, const __m128i b );
static inline __m128i v128_compare_equal_64( const __m128i a, const __m128i b );
static inline __m128i v128_compare_equal_8( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_16( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_32( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_64( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_8( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_logical_16( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_logical_32( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_logical_64( const __m128i a, const __m128i b );
static inline __m128i v128_compare_high_logical_8( const __m128i a, const __m128i b );
static inline __m128i v128_complement_16( const __m128i a );
static inline __m128i v128_complement_32( const __m128i a );
static inline __m128i v128_complement_64( const __m128i a );
static inline __m128i v128_complement_8( const __m128i a );
static inline __m128i v128_find_any_equal_16( const __m128i source, const __m128i compare_to );
static inline __m128i v128_find_any_equal_32( const __m128i source, const __m128i compare_to );
static inline __m128i v128_find_any_equal_8( const __m128i source, const __m128i compare_to );
static inline __m128i v128_find_equal_16( const __m128i vec_a, const __m128i vec_b );
static inline __m128i v128_find_equal_32( const __m128i vec_a, const __m128i vec_b );
static inline __m128i v128_find_equal_8( const __m128i vec_a, const __m128i vec_b );
static inline __m128i v128_find_equal_zero_16( const __m128i vec_a );
static inline __m128i v128_find_equal_zero_32( const __m128i vec_a );
static inline __m128i v128_find_equal_zero_8( const __m128i vec_a );
static inline __m128i v128_isolate_string_16(__m128i string);
static inline __m128i v128_isolate_string_8(__m128i string);
static inline __m128i v128_maximum_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_maximum_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_maximum_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_maximum_logical_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_maximum_logical_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_maximum_logical_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_merge_high_16( const __m128i a, const __m128i b );
static inline __m128i v128_merge_high_32( const __m128i a, const __m128i b );
static inline __m128i v128_merge_high_64( const __m128i a, const __m128i b );
static inline __m128i v128_merge_high_8( const __m128i a, const __m128i b );
static inline __m128i v128_merge_low_16( const __m128i a, const __m128i b );
static inline __m128i v128_merge_low_32( const __m128i a, const __m128i b );
static inline __m128i v128_merge_low_64( const __m128i a, const __m128i b );
static inline __m128i v128_merge_low_8( const __m128i a, const __m128i b );
static inline __m128i v128_minimum_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_minimum_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_minimum_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_minimum_logical_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_minimum_logical_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_minimum_logical_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_multiply_add_even_16( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_even_32( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_high_16( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_high_32( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_high_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_even_16( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_even_32( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_even_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_high_32( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_high_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_logical_odd_16( const __m128i v2, const __m128i v3, const __m128i v4  );
static inline __m128i v128_multiply_add_logical_odd_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_low_16( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_low_32( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_low_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_add_odd_16( const __m128i v2, const __m128i v3, const __m128i v4  );
static inline __m128i v128_multiply_add_odd_32( const __m128i v2, const __m128i v3, const __m128i v4  );
static inline __m128i v128_multiply_add_odd_8( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_multiply_even_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_even_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_even_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_high_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_high_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_even_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_even_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_even_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_high_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_high_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_high_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_odd_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_odd_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_logical_odd_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_low_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_low_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_low_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_odd_16( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_odd_32( const __m128i v2, const __m128i v3  );
static inline __m128i v128_multiply_odd_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_not( const __m128i a );
static inline __m128i v128_one();
static inline __m128i v128_or( const __m128i a, const __m128i b );
static inline __m128i v128_pack_16(__m128i a, __m128i b);
static inline __m128i v128_pack_32(__m128i a, __m128i b);
static inline __m128i v128_pack_64(__m128i a, __m128i b);
static inline __m128i v128_pack_logical_saturate_16(__m128i a, __m128i b);
static inline __m128i v128_pack_logical_saturate_32(__m128i a, __m128i b);
static inline __m128i v128_pack_logical_saturate_64(__m128i a, __m128i b);
static inline __m128i v128_pack_saturate_16(__m128i a, __m128i b);
static inline __m128i v128_pack_saturate_32(__m128i a, __m128i b);
static inline __m128i v128_pack_saturate_64(__m128i a, __m128i b);
static inline __m128i v128_permute( const __m128i v2, const __m128i v3, const __m128i v4 );
static inline __m128i v128_positive_16( const __m128i v2 );
static inline __m128i v128_positive_32( const __m128i v2 );
static inline __m128i v128_positive_8( const __m128i v2 );
static inline __m128i v128_replicate_16( U16 value );
static inline __m128i v128_replicate_32( U32 value );
static inline __m128i v128_replicate_64( U64 value );
static inline __m128i v128_replicate_8( U8 value );
static inline __m128i v128_rotate_and_insert_under_mask_16( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  );
static inline __m128i v128_rotate_and_insert_under_mask_32( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  );
static inline __m128i v128_rotate_and_insert_under_mask_64( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  );
static inline __m128i v128_rotate_and_insert_under_mask_8( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  );
static inline __m128i v128_set(const U64 value);
static inline __m128i v128_shift_left_double_byte( const __m128i a, const __m128i b, int shift );
static inline __m128i v128_shift_left_vector_8( const __m128i v2, const __m128i v3  );
static inline __m128i v128_shift_right_arithmetic_byte( const __m128i a, int shift );
static inline __m128i v128_shift_right_logical_byte( const __m128i a, int shift );
static inline __m128i v128_sign_extend_16_64( const __m128i a );
static inline __m128i v128_sign_extend_32_64( const __m128i a );
static inline __m128i v128_sign_extend_8_64( const __m128i a );
static inline __m128i v128_sll_16( const __m128i a, const U32 count );
static inline __m128i v128_sll_32( const __m128i a, const U32 count );
static inline __m128i v128_sll_64( const __m128i a, const U32 count );
static inline __m128i v128_sll_8( const __m128i a, const U32 count );
static inline __m128i v128_sra_16( const __m128i a, const U32 count );
static inline __m128i v128_sra_32( const __m128i a, const U32 count );
static inline __m128i v128_sra_8( const __m128i a, const U32 count );
static inline __m128i v128_srl_16( const __m128i a, const U32 count );
static inline __m128i v128_srl_32( const __m128i a, const U32 count );
static inline __m128i v128_srl_64( const __m128i a, const U32 count );
static inline __m128i v128_srl_8( const __m128i a, const U32 count );
static inline __m128i v128_subtract_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_subtract_32( const __m128i v2, const __m128i v3 );
static inline __m128i v128_subtract_64( const __m128i v2, const __m128i v3 );
static inline __m128i v128_subtract_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_subtract_compute_borrow_ind_16( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_subtract_compute_borrow_ind_32( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_subtract_compute_borrow_ind_8( const __m128i v2 , const __m128i v3 );
static inline __m128i v128_sum_across_doubleword_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_sum_across_word_16( const __m128i v2, const __m128i v3 );
static inline __m128i v128_sum_across_word_8( const __m128i v2, const __m128i v3 );
static inline __m128i v128_unpack_high_16( const __m128i a );
static inline __m128i v128_unpack_high_32( const __m128i a );
static inline __m128i v128_unpack_high_8( const __m128i a );
static inline __m128i v128_unpack_logical_high_16( const __m128i a );
static inline __m128i v128_unpack_logical_high_32( const __m128i a );
static inline __m128i v128_unpack_logical_high_8( const __m128i a );
static inline __m128i v128_unpack_logical_low_16( const __m128i a );
static inline __m128i v128_unpack_logical_low_32( const __m128i a );
static inline __m128i v128_unpack_logical_low_8( const __m128i a );
static inline __m128i v128_unpack_low_16( const __m128i a );
static inline __m128i v128_unpack_low_32( const __m128i a );
static inline __m128i v128_unpack_low_8( const __m128i a );
static inline __m128i v128_xor( const __m128i a, const __m128i b );
static inline __m128i v128_zero();

/* ================================================================= */
/* prototypes: return int                                            */
/* ================================================================= */
static inline int v128_compare_equal_cc( const __m128i mask );
static inline int v128_compare_high_cc( const __m128i mask );
static inline int v128_compare_high_logical_cc( const __m128i mask );
static inline int v128_find_first_equal_16( const __m128i source , const __m128i compare_to );
static inline int v128_find_first_equal_32( const __m128i vec_a , const __m128i vec_b );
static inline int v128_find_first_equal_8( const __m128i source , const __m128i compare_to );
static inline int v128_find_first_in_vector_16( const __m128i source, const U16 halfword );
static inline int v128_find_first_in_vector_32( const __m128i vector, const U32 word );
static inline int v128_find_first_in_vector_8( const __m128i source, const U8 byte );
static inline int v128_find_first_not_equal_16( const __m128i vec_a , const __m128i vec_b );
static inline int v128_find_first_not_equal_32( const __m128i vec_a , const __m128i vec_b );
static inline int v128_find_first_not_equal_8( const __m128i vec_a , const __m128i vec_b );
static inline int v128_find_first_substring_16( const __m128i v2_str, const int v2_len, const __m128i v3_substr, const int v3_len  );
static inline int v128_find_first_substring_32( const __m128i v2_str, int v2_len, const __m128i v3_substr, int v3_len  );
static inline int v128_find_first_substring_8( const __m128i v2_str, const int v2_len, const __m128i v3_substr, const int v3_len  );
static inline int v128_find_first_zero_in_vector_16( const __m128i vector );
static inline int v128_find_first_zero_in_vector_32( const __m128i vector );
static inline int v128_find_first_zero_in_vector_8( const __m128i vector );
static inline int v128_pack_logical_saturate_cc_16(__m128i a, __m128i b);
static inline int v128_pack_logical_saturate_cc_32(__m128i a, __m128i b);
static inline int v128_pack_logical_saturate_cc_64(__m128i a, __m128i b);
static inline int v128_pack_saturate_cc_16(__m128i a, __m128i b);
static inline int v128_pack_saturate_cc_32(__m128i a, __m128i b);
static inline int v128_pack_saturate_cc_64(__m128i a, __m128i b);

/**
 * v128_zero
 *      return a 128-bit 'zero' vector
 */
static inline __m128i v128_zero()
{
    return _mm_setzero_si128 ();
}

/**
 * v128_one
 *      return a 128-bit 'one' vector
 */
static inline __m128i v128_one()
{
    return _mm_set_epi64x( 0, 1 );
}

/**
 * v128_set
 *      return a 128-bit vector with U64 value
 */
static inline __m128i v128_set(const U64 value)
{
    return _mm_set_epi64x( 0, value );
}

/**
 * v128_bswap
 *      return a 128-bit vector with bytes reversed
 *
 */
static inline __m128i v128_bswap( const __m128i a  )
{
    __m128i swapmask = _mm_set_epi8( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    return _mm_shuffle_epi8( a,  swapmask );
}


/**
 * v128_find_first_equal_xx
 *      find the index of the first byte (_8), halfword (_16) or word (_32)
 *      where the source vector element is equal to the compare_to vector
 *      element.
 *
 *      If an equal element is not found, the return value is:
 *          16 for byte (_8),
 *           8 for halfword (_16)
 *           4 for word (_32)
 */

static inline int v128_find_first_equal_8( const __m128i source , const __m128i compare_to )
{
    int index = _mm_cmpestri ( compare_to, 16, source, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT );
    if ( index == 16 )
        return 16;              /* not found */
    else
        return 15 - index;      /* convert to big-endian */
}

static inline int v128_find_first_equal_16( const __m128i source , const __m128i compare_to )
{
    int index = _mm_cmpestri ( compare_to, 8, source, 8, _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT );
    if (index == 8)
        return 8;              /* not found */
    else
        return 7 - index;      /* convert to big-endian */
}

static inline int v128_find_first_equal_32( const __m128i vec_a , const __m128i vec_b )
{
    int i;
    QW  v_a, v_b;

    //required for MSVC: 'type cast': cannot convert from 'const __m128i' to 'QW'
    v_a.v = vec_a;
    v_b.v = vec_b;

    for (i=0; i < 4; i++)
    {
        if ( v_a.QW_F(i)  == v_b.QW_F(i) )
            return i;
    }
    return 4;
}

/**
 * v128_find_first_not_equal_xx
 *      find the index of the first byte (_8), halfword (_16) or word (_32)
 *      where the source vector element is not equal to the compare_to vector
 *      element.
 *
 *      If a not-equal element is not found, the return value is:
 *          16 for byte (_8),
 *           8 for halfword (_16)
 *           4 for word (_32)
 */

static inline int v128_find_first_not_equal_8( const __m128i vec_a , const __m128i vec_b )
{
    // Compare bytes for equality
    // __m128i mask = _mm_cmpeq_epi8( vec_a, vec_b );

    // int index = _mm_cmpestri ( _mm_set1_epi8( 0x00 ), 16, mask, 16,
    //                     _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT );
    int index = _mm_cmpestri ( vec_b, 16, vec_a, 16,
                        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT | _SIDD_NEGATIVE_POLARITY);
    if (index == 16)
        return 16;              /* not found */
    else
        return 15 - index;      /* convert to big-endian */

}

static inline int v128_find_first_not_equal_16( const __m128i vec_a , const __m128i vec_b )
{
    // Compare bytes for equality
    __m128i mask = _mm_cmpeq_epi16( vec_a, vec_b );

    int index = _mm_cmpestri ( _mm_set1_epi16( 0x0000 ), 8, mask, 8,
                        _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT );
    if ( index == 8 )
        return 8;               /* not found */
    else
        return 7 - index;       /* convert to big-endian */
}

static inline int v128_find_first_not_equal_32( const __m128i vec_a , const __m128i vec_b )
{
    int i;
    QW  v_a, v_b;

    //required for MSVC: 'type cast': cannot convert from 'const __m128i' to 'QW'
    v_a.v = vec_a;
    v_b.v = vec_b;

    for (i=0; i < 4; i++)
    {
        if ( v_a.QW_F(i)  != v_b.QW_F(i) )
            return i;
    }
    return 4;
}

/**
 * v128_find_first_equal_xx
 *      find the index of the first byte (_8), halfword (_16) or word (_32)
 *      in the vector that is equal to the parameter.
 *
 *      If the parameter is not found, the return value is:
 *          16 for byte (_8),
 *           8 for halfword (_16)
 *           4 for word (_32)
 */

static inline int v128_find_first_in_vector_8( const __m128i source, const U8 byte )
{
    // Create a vector with 16 instances of the byte to find
    __m128i compare_to = _mm_set1_epi8( byte );

    int index = _mm_cmpestri ( compare_to, 16, source, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT );
    if ( index == 16 )
        return 16;              /* not found */
    else
        return 15 - index;      /* convert to big-endian */
}

static inline int v128_find_first_in_vector_16( const __m128i source, const U16 halfword )
{
    // Create a vector with 8 instances of the halfword to find
    __m128i compare_to = _mm_set1_epi16( halfword );

    int index = _mm_cmpestri ( compare_to, 8, source, 8, _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_MOST_SIGNIFICANT) ;
    if ( index == 8 )
        return 8;              /* not found */
    else
        return 7 - index;      /* convert to big-endian */
}

static inline int v128_find_first_in_vector_32( const __m128i vector, const U32 word )
{
    int i;
    QW  v_a;

    //required for MSVC: 'type cast': cannot convert from 'const __m128i' to 'QW'
    v_a.v = vector;

    for (i=0; i < 4; i++)
    {
        if ( v_a.QW_F(i) == word )
            return i;
    }
    return 4;
}

/**
 * v128_find_first_zero_in_vector_xx
 *      find the index of the first zero byte (_8), halfword (_16) or word (_32)
 *      in the vector.
 *
 *      If zero is not found, the return value is:
 *          16 for byte (_8),
 *           8 for halfword (_16)
 *           4 for word (_32)
 */

static inline int v128_find_first_zero_in_vector_8( const __m128i vector )
{
    return v128_find_first_in_vector_8( vector, 0 );
}

static inline int v128_find_first_zero_in_vector_16( const __m128i vector )
{
    return v128_find_first_in_vector_16( vector, 0 );
}

static inline int v128_find_first_zero_in_vector_32( const __m128i vector )
{
    return v128_find_first_in_vector_32( vector, 0 );
}

/**
 * v128_find_any_equal_xx
 *      return a vector byte mask where a byte (_8), halfword (_16) or word (_32)
 *      element of the source vector is equal to any byte (_8), halfword (_16) or
 *      word (_32) element in the compare_to vector.
 *
 *      For each byte (_8), halfword (_16) or word (_32) of the source vector, the
 *      result byte mask vector will contain:
 *           byte (_8)      : 0xFF if found, otherwise 0x00
 *           halfword (_16) : 0xFFFF if found, otherwise 0x0000
 *           word (_32)     : 0xFFFFFFFF if found, otherwise 0x00000000
 */

static inline __m128i v128_find_any_equal_8( const __m128i source, const __m128i compare_to )
{
    return _mm_cmpestrm ( compare_to, 16, source, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_UNIT_MASK);
}

static inline __m128i v128_find_any_equal_16( const __m128i source, const __m128i compare_to )
{
    return _mm_cmpestrm ( compare_to, 8, source, 8, _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_UNIT_MASK);
}

static inline __m128i v128_find_any_equal_32( const __m128i source, const __m128i compare_to )
{
    int i;
    __m128i temp;
    QW      result;
    QW      qw_s;

    //required for MSVC: 'type cast': cannot convert from 'const __m128i' to 'QW'
    qw_s.v = source;

    result.v = v128_zero();
    for (i=0; i < 4; i++)
    {
        temp = _mm_set1_epi32( qw_s.QW_F(i) );
        temp = _mm_cmpeq_epi32( compare_to, temp );

        if ( !_mm_testz_si128( temp, temp ) )
        {
            result.QW_F(i) = 0xFFFFFFFF;
        }
    }
    return result.v;
}

/**
 * v128_find_equal_xx
 *      return a vector byte mask where a byte (_8), halfword (_16) or word (_32)
 *      element of the source vector is equal to corresponding element in the
 *      compare_to vector.
 *
 *      For each byte (_8), halfword (_16) or word (_32) of the source vector, the
 *      result byte mask vector will contain:
 *           byte (_8)      : 0xFF if equal, otherwise 0x00
 *           halfword (_16) : 0xFFFF if equal, otherwise 0x0000
 *           word (_32)     : 0xFFFFFFFF if equal, otherwise 0x00000000
 */

static inline __m128i v128_find_equal_8( const __m128i vec_a, const __m128i vec_b )
{
    return _mm_cmpeq_epi8( vec_a, vec_b );
}

static inline __m128i v128_find_equal_16( const __m128i vec_a, const __m128i vec_b )
{
    return _mm_cmpeq_epi16( vec_a, vec_b );
}

static inline __m128i v128_find_equal_32( const __m128i vec_a, const __m128i vec_b )
{
    return _mm_cmpeq_epi32( vec_a, vec_b );
}

/**
 * v128_find_equal_zero_xx
 *      return a vector byte mask where a byte (_8), halfword (_16) or word (_32)
 *      element of the source vector is equal to zero.
 *
 *      For each byte (_8), halfword (_16) or word (_32) of the source vector, the
 *      result byte mask vector will contain:
 *           byte (_8)      : 0xFF if zero, otherwise 0x00
 *           halfword (_16) : 0xFFFF if zero, otherwise 0x0000
 *           word (_32)     : 0xFFFFFFFF if zero, otherwise 0x00000000
 */

static inline __m128i v128_find_equal_zero_8( const __m128i vec_a )
{
    return  _mm_cmpeq_epi8( vec_a, _mm_set1_epi8( 0 ) );
}

static inline __m128i v128_find_equal_zero_16( const __m128i vec_a )
{
    return  _mm_cmpeq_epi16( vec_a, _mm_set1_epi16( 0 ) );
}

static inline __m128i v128_find_equal_zero_32( const __m128i vec_a )
{
    return  _mm_cmpeq_epi32( vec_a, _mm_set1_epi32( 0 ) );
}

/**
 * v128_bit_to_byte_mask
 *      return a vector byte mask from a 16-bit bit mask
 *      For each bit mask, the byte mask is 0xFF.
 *
 * reference:: https://stackoverflow.com/questions/67201469/convert-16-bits-mask-to-16-bytes-mask
 */
static inline __m128i v128_bit_to_byte_mask(U16 bitmap)
{
    const __m128i shuffle = _mm_setr_epi32( 0, 0, 0x01010101, 0x01010101);

    const __m128i bitselect = _mm_setr_epi8(
        1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1U<<7,
        1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1U<<7 );

    __m128i v = _mm_shuffle_epi8( _mm_cvtsi32_si128( bitmap ), shuffle );  // SSSE3 pshufb

    v = _mm_and_si128( v, bitselect );
    v = _mm_cmpeq_epi8( v, bitselect );       // non-zero -> 0xFF  :  0 -> 0x00

    return v;
}

/**
 * v128_isolate_string_xx
 *      copy non-zero vector elements up to a zero element;
 *      zero remaining elements
 */
static inline __m128i v128_isolate_string_8(__m128i string)
{
    __m128i mask;

    //find a zero element
    int fzi = v128_find_first_zero_in_vector_8( string );

    // generate mask to isolate
    fzi = 16 - fzi;
    mask = _mm_cmpestrm ( string, 16, string, fzi,
                _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_UNIT_MASK | _SIDD_NEGATIVE_POLARITY );

    // printf(" v128_isolate_string_8: fzi=%d\n", fzi );
    // u128_logmsg(" v128_isolate_string_8: mask: ", (U128) mask );

    return _mm_and_si128( string, mask);
}

static inline __m128i v128_isolate_string_16(__m128i string)
{
    __m128i mask;

    //find a zero element
    int fzi = v128_find_first_zero_in_vector_16( string );

    // generate mask to isolate
    fzi = 8 - fzi;
    mask = _mm_cmpestrm ( string, 8, string, fzi,
                _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_UNIT_MASK | _SIDD_NEGATIVE_POLARITY );

    return _mm_and_si128( string, mask);
}

/**
 * v128_replicate_xx
 *      generate a vector with all elements the same value
 */

static inline __m128i v128_replicate_8( U8 value )
{
    return _mm_set1_epi8( value);
}

static inline __m128i v128_replicate_16( U16 value )
{
    return _mm_set1_epi16( value);
}

static inline __m128i v128_replicate_32( U32 value )
{
    return _mm_set1_epi32( value);
}

static inline __m128i v128_replicate_64( U64 value )
{
    return _mm_set1_epi64x( value );
}

//=============================================================
// v128 logical functions
// - not, and, andnot, or, xor
//=============================================================
static inline __m128i v128_not( const __m128i a )
{
    return _mm_xor_si128( a, _mm_set1_epi8( 0xFF ) );
}

static inline __m128i v128_and( const __m128i a, const __m128i b )
{
    return _mm_and_si128( a, b );
}

// programmers note: b is Complemented! ie: a & ~b
static inline __m128i v128_andnot( const __m128i a, const __m128i b )
{
    return _mm_andnot_si128( b, a );
}

static inline __m128i v128_or( const __m128i a, const __m128i b )
{
    return _mm_or_si128( a, b );
}

static inline __m128i v128_xor( const __m128i a, const __m128i b )
{
    return _mm_xor_si128( a, b );
}

//=============================================================
// v128 shift left/right
// - sll, srl, sra
//=============================================================
// element shift left logical
static inline __m128i v128_sll_8( const __m128i a, const U32 count )
{
    return v128_and( _mm_slli_epi64( a, count ), _mm_set1_epi8( 0xFF << count ) );
}

static inline __m128i v128_sll_16( const __m128i a, const U32 count )
{
    return _mm_sll_epi16( a, v128_set( count ) );
}

static inline __m128i v128_sll_32( const __m128i a, const U32 count )
{
    return _mm_sll_epi32( a, v128_set( count ) );
}

static inline __m128i v128_sll_64( const __m128i a, const U32 count )
{
    return _mm_sll_epi64( a, v128_set( count ) );
}

// element shift right logical
static inline __m128i v128_srl_8( const __m128i a, const U32 count )
{
    return v128_and( _mm_srli_epi64( a, count ), _mm_set1_epi8( 0xFF >> count ) );
}

static inline __m128i v128_srl_16( const __m128i a, const U32 count )
{
    return _mm_srl_epi16( a, v128_set( count ) );
}

static inline __m128i v128_srl_32( const __m128i a, const U32 count )
{
    return _mm_srl_epi32( a, v128_set( count ) );
}

static inline __m128i v128_srl_64( const __m128i a, const U32 count )
{
    return _mm_srl_epi64( a, v128_set( count ) );
}

// element shift right arithmetic
static inline __m128i v128_sra_8( const __m128i a, const U32 count )
{   __m128i  hi, lo;

    //     Convert signed 8-bit elements to signed 16-bit elements,
    //     right shift arithmetic, make U8, and repack to 8-bit elements
    hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( a, 8) );
    lo = _mm_cvtepi8_epi16( a );

    hi = _mm_srai_epi16(hi, count);
    hi = v128_and( hi, _mm_set1_epi16( 0x0FF ) );

    lo = _mm_srai_epi16(lo, count);
    lo = v128_and( lo, _mm_set1_epi16( 0x0FF ) );

    return _mm_packus_epi16( lo, hi );
}

static inline __m128i v128_sra_16( const __m128i a, const U32 count )
{
    return _mm_sra_epi16( a, v128_set( count ) );
}

static inline __m128i v128_sra_32( const __m128i a, const U32 count )
{
    return _mm_sra_epi32( a, v128_set( count ) );
}

//=============================================================
// v128 sign extend
//=============================================================
// element sign extend byte to double
static inline __m128i v128_sign_extend_8_64( const __m128i a )
{
    __m128i mask = _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, 0);

    return _mm_cvtepi8_epi64( _mm_shuffle_epi8( a , mask ) );
}

static inline __m128i v128_sign_extend_16_64( const __m128i a )
{
    __m128i mask = _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, 8, 1, 0);
    return _mm_cvtepi16_epi64( _mm_shuffle_epi8( a , mask ) );
}

static inline __m128i v128_sign_extend_32_64( const __m128i a )
{
    return _mm_cvtepi32_epi64( _mm_shuffle_epi32( a ,0x08 ) );
}

//=============================================================
// v128 merge low
//=============================================================
// element merge low
static inline __m128i v128_merge_low_8( const __m128i a, const __m128i b )
{
    return _mm_unpacklo_epi8( b, a );
}

static inline __m128i v128_merge_low_16( const __m128i a, const __m128i b )
{
    return _mm_unpacklo_epi16( b, a );
}

static inline __m128i v128_merge_low_32( const __m128i a, const __m128i b )
{
    return _mm_unpacklo_epi32( b, a );
}

static inline __m128i v128_merge_low_64( const __m128i a, const __m128i b )
{
    return _mm_unpacklo_epi64( b, a );
}

//=============================================================
// v128 merge high
//=============================================================
// element merge high
static inline __m128i v128_merge_high_8( const __m128i a, const __m128i b )
{
    return _mm_unpackhi_epi8( b, a );
}

static inline __m128i v128_merge_high_16( const __m128i a, const __m128i b )
{
    return _mm_unpackhi_epi16( b, a );
}

static inline __m128i v128_merge_high_32( const __m128i a, const __m128i b )
{
    return _mm_unpackhi_epi32( b, a );
}

static inline __m128i v128_merge_high_64( const __m128i a, const __m128i b )
{
    return _mm_unpackhi_epi64( b, a );
}

//=============================================================
// v128 complement
//=============================================================
// element complement
static inline __m128i v128_complement_8( const __m128i a )
{
    return _mm_add_epi8( v128_not( a ), _mm_set1_epi8( 1 ) );
}

static inline __m128i v128_complement_16( const __m128i a )
{
    return _mm_add_epi16( v128_not( a ), _mm_set1_epi16( 1 ) );
}

static inline __m128i v128_complement_32( const __m128i a )
{
    return _mm_add_epi32( v128_not( a ), _mm_set1_epi32( 1 ) );
}

static inline __m128i v128_complement_64( const __m128i a )
{
    return _mm_add_epi64( v128_not( a ), _mm_set1_epi64x( 1 ) );
}


//=============================================================
// v128 pack
//=============================================================
static inline __m128i v128_pack_16(__m128i a, __m128i b)
{
    __m128i mask, lo, hi;

    mask = _mm_set_epi8( 14, 12, 10, 8, 6, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1);
    hi = _mm_shuffle_epi8( a , mask );
    //u128_logmsg(" hi", (U128) hi);

    mask = _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 );
    lo = _mm_shuffle_epi8( b , mask );
    //u128_logmsg(" lo", (U128) lo);

    return _mm_or_si128(lo, hi); // Combine the results
}

static inline __m128i v128_pack_32(__m128i a, __m128i b)
{
    __m128i mask, lo, hi;

    mask = _mm_set_epi8( 13, 12, 9, 8, 5, 4, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1 );
    hi = _mm_shuffle_epi8( a , mask );
    //u128_logmsg(" hi", (U128) hi);

    mask = _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 );
    lo = _mm_shuffle_epi8( b , mask );
    //u128_logmsg(" lo", (U128) lo);

    return _mm_or_si128(lo, hi); // Combine the results
}

static inline __m128i v128_pack_64(__m128i a, __m128i b)
{
    __m128i mask, lo, hi;

    mask = _mm_set_epi8( 11, 10, 9, 8, 3, 2, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1 );
    hi = _mm_shuffle_epi8( a , mask );
     //u128_logmsg(" hi", (U128) hi);

    mask = _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 );
    lo = _mm_shuffle_epi8( b , mask );
    //u128_logmsg(" lo", (U128) lo);

    return _mm_or_si128(lo, hi); // Combine the results
}


//=============================================================
// v128 pack saturate
//=============================================================
static inline __m128i v128_pack_saturate_16(__m128i a, __m128i b)
{
    __m128i mask_sat, mask_sat_pos, mask_sat_neg;
    __m128i lo, hi;

    /* hi vector - a */
    mask_sat_pos = _mm_cmpgt_epi16( a, _mm_set1_epi16( 0x007F ) );
    mask_sat_neg = _mm_cmplt_epi16( a, _mm_set1_epi16( 0xFF80 ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    hi = v128_andnot( a, mask_sat );                                       /* not saturated elements */
    hi = v128_or(hi, v128_and( mask_sat_pos, _mm_set1_epi16( 0x7F ) ) );   /* positive saturated elements*/
    hi = v128_or(hi, v128_and( mask_sat_neg, _mm_set1_epi16( 0x80 ) ) );   /* negative saturated elements*/

    mask_sat_pos = _mm_cmpgt_epi16( b, _mm_set1_epi16( 0x007F ) );
    mask_sat_neg = _mm_cmplt_epi16( b, _mm_set1_epi16( 0xFF80 ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    lo = v128_andnot( b, mask_sat );                                       /* not saturated elements */
    lo = v128_or(lo, v128_and( mask_sat_pos, _mm_set1_epi16( 0x7F ) ) );   /* positive saturated elements*/
    lo = v128_or(lo, v128_and( mask_sat_neg, _mm_set1_epi16( 0x80 ) ) );   /* negative saturated elements*/

    return v128_pack_16( hi, lo);
}

static inline __m128i v128_pack_saturate_32(__m128i a, __m128i b)
{
    __m128i mask_sat, mask_sat_pos, mask_sat_neg;
    __m128i lo, hi;

    /* hi vector - a */
    mask_sat_pos = _mm_cmpgt_epi32( a, _mm_set1_epi32( 0x00007FFF ) );
    mask_sat_neg = _mm_cmplt_epi32( a, _mm_set1_epi32( 0xFFFF8000 ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    hi = v128_andnot( a, mask_sat );                                         /* not saturated elements */
    hi = v128_or(hi, v128_and( mask_sat_pos, _mm_set1_epi32( 0x7FFF ) ) );   /* positive saturated elements*/
    hi = v128_or(hi, v128_and( mask_sat_neg, _mm_set1_epi32( 0x8000 ) ) );   /* negative saturated elements*/

    /* lo vector - b */
    mask_sat_pos = _mm_cmpgt_epi32( b, _mm_set1_epi32( 0x00007FFF ) );
    mask_sat_neg = _mm_cmplt_epi32( b, _mm_set1_epi32( 0xFFFF8000 ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    lo = v128_andnot( b, mask_sat );                                         /* not saturated elements */
    lo = v128_or(lo, v128_and( mask_sat_pos, _mm_set1_epi32( 0x7FFF ) ) );   /* positive saturated elements*/
    lo = v128_or(lo, v128_and( mask_sat_neg, _mm_set1_epi32( 0x8000 ) ) );   /* nagative saturated elements*/

    return v128_pack_32( hi, lo);
}

static inline __m128i v128_pack_saturate_64(__m128i a, __m128i b)
{
    __m128i mask_sat, mask_sat_pos, mask_sat_neg;
    __m128i lo, hi;

    /* hi vector - a */
    /* note: no cmplt for epi64 */
    mask_sat_pos = _mm_cmpgt_epi64( a, _mm_set1_epi64x( 0x000000007FFFFFFFull ) );
    mask_sat_neg = v128_not( _mm_cmpgt_epi64( a, _mm_set1_epi64x( 0xFFFFFFFF7FFFFFFFull ) ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    hi = v128_andnot( a, mask_sat );                                              /* not saturated elements */
    hi = v128_or(hi, v128_and( mask_sat_pos, _mm_set1_epi64x( 0x7FFFFFFF ) ) );   /* positive saturated elements*/
    hi = v128_or(hi, v128_and( mask_sat_neg, _mm_set1_epi64x( 0x80000000 ) ) );   /* negative saturated elements*/

    /* lo vector - b */
    /* note: no cmplt for epi64 */
    mask_sat_pos = _mm_cmpgt_epi64( b, _mm_set1_epi64x( 0x000000007FFFFFFFull ) );
    mask_sat_neg = v128_not( _mm_cmpgt_epi64( b, _mm_set1_epi64x( 0xFFFFFFFF7FFFFFFFull ) ) );
    mask_sat = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    lo = v128_andnot( b, mask_sat );                                              /* not saturated elements */
    lo = v128_or(lo, v128_and( mask_sat_pos, _mm_set1_epi64x( 0x7FFFFFFF ) ) );   /* positive saturated elements*/
    lo = v128_or(lo, v128_and( mask_sat_neg, _mm_set1_epi64x( 0x80000000 ) ) );   /* nagative saturated elements*/

    return v128_pack_64( hi, lo);
}

//=============================================================
// v128 pack saturate condition code
//=============================================================
static inline int v128_pack_saturate_cc_16(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i mask_sat_pos, mask_sat_neg;
    int     cc;

    /* hi vector - a */
    mask_sat_pos = _mm_cmpgt_epi16( a, _mm_set1_epi16( 0x007F ) );
    mask_sat_neg = _mm_cmplt_epi16( a, _mm_set1_epi16( 0xFF80 ) );
    mask_hi = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    mask_sat_pos = _mm_cmpgt_epi16( b, _mm_set1_epi16( 0x007F ) );
    mask_sat_neg = _mm_cmplt_epi16( b, _mm_set1_epi16( 0xFF80 ) );
    mask_lo = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_16( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}

static inline int v128_pack_saturate_cc_32(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i mask_sat_pos, mask_sat_neg;
    int     cc;

    /* hi vector - a */
    mask_sat_pos = _mm_cmpgt_epi32( a, _mm_set1_epi32( 0x00007FFF ) );
    mask_sat_neg = _mm_cmplt_epi32( a, _mm_set1_epi32( 0xFFFF8000 ) );
    mask_hi = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    mask_sat_pos = _mm_cmpgt_epi32( b, _mm_set1_epi32( 0x00007FFF ) );
    mask_sat_neg = _mm_cmplt_epi32( b, _mm_set1_epi32( 0xFFFF8000 ) );
    mask_lo = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_32( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}

static inline int v128_pack_saturate_cc_64(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i mask_sat_pos, mask_sat_neg;
    int     cc;

    /* hi vector - a */
    /* note: no cmplt for epi64 */
    mask_sat_pos = _mm_cmpgt_epi64( a, _mm_set1_epi64x( 0x000000007FFFFFFFull ) );
    mask_sat_neg = v128_not( _mm_cmpgt_epi64( a, _mm_set1_epi64x( 0xFFFFFFFF7FFFFFFFull ) ) );
    mask_hi = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    /* note: no cmplt for epi64 */
    mask_sat_pos = _mm_cmpgt_epi64( b, _mm_set1_epi64x( 0x000000007FFFFFFFull ) );
    mask_sat_neg = v128_not( _mm_cmpgt_epi64( b, _mm_set1_epi64x( 0xFFFFFFFF7FFFFFFFull ) ) );
    mask_lo = _mm_or_si128( mask_sat_pos, mask_sat_neg);
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_64( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}


//=============================================================
// v128 pack logical saturate
//=============================================================
static inline __m128i v128_pack_logical_saturate_16(__m128i a, __m128i b)
{
    __m128i mask_sat;
    __m128i temp, lo, hi;

    /* hi vector - a */
    /* zero low half of half-words */
    temp = v128_and( a, _mm_set_epi8( -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 ) );
    mask_sat = v128_not( _mm_cmpeq_epi16( temp, v128_zero() ) );        /* satuaration mask; upper half non-zero */
    hi = v128_andnot( a, mask_sat );                                    /* not saturated elements */
    hi = v128_or( hi, v128_and( mask_sat, _mm_set1_epi16( 0xFF ) ) );   /* saturated elements */
    //u128_logmsg(" mask_hi ", (U128) mask_sat );

    /* lo vector - b */
    /* zero low half of half-words */
    temp = v128_and( b, _mm_set_epi8( -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 ) );
    mask_sat = v128_not( _mm_cmpeq_epi16( temp, v128_zero() ) );        /* satuaration mask; upper half non-zero */
    lo = v128_andnot( b, mask_sat );                                    /* not saturated elements */
    lo = v128_or( lo, v128_and( mask_sat, _mm_set1_epi16( 0xFF ) ) );   /* saturated elements */
    //u128_logmsg(" mask_lo ", (U128) mask_sat );

    return v128_pack_16( hi, lo);
}

static inline __m128i v128_pack_logical_saturate_32(__m128i a, __m128i b)
{
    __m128i mask_sat;
    __m128i temp, lo, hi;

    /* hi vector - a */
    temp = v128_and( a, _mm_set_epi16( -1, 0, -1, 0, -1, 0, -1, 0 ) );    /* zero low half of words */
    mask_sat = v128_not( _mm_cmpeq_epi32( temp, v128_zero() ) );          /* satuaration mask; upper half non-zero */
    hi = v128_andnot( a, mask_sat );                                      /* not saturated elements */
    hi = v128_or( hi, v128_and( mask_sat, _mm_set1_epi32( 0xFFFF ) ) );   /* saturated elements */
    //u128_logmsg(" mask_hi ", (U128) mask_sat );

    /* lo vector - b */
    temp = v128_and( b, _mm_set_epi16( -1, 0, -1, 0, -1, 0, -1, 0 ) );    /* zero low half of words */
    mask_sat = v128_not( _mm_cmpeq_epi32( temp, v128_zero() ) );          /* satuaration mask; upper half non-zero */
    lo = v128_andnot( b, mask_sat );                                      /* not saturated elements */
    lo = v128_or( lo, v128_and( mask_sat, _mm_set1_epi32( 0xFFFF ) ) );   /* saturated elements */
    //u128_logmsg(" mask_lo ", (U128) mask_sat );

    return v128_pack_32( hi, lo);
}

static inline __m128i v128_pack_logical_saturate_64(__m128i a, __m128i b)
{
    __m128i mask_sat;
    __m128i temp, lo, hi;

    /* hi vector - a */
    temp = v128_and( a, _mm_set_epi32( -1, 0, -1, 0 ) );                   /* zero low half of double words */
    mask_sat = v128_not( _mm_cmpeq_epi64( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    hi = v128_andnot( a, mask_sat );                                             /* not saturated elements */
    hi = v128_or(hi, v128_and( mask_sat, _mm_set1_epi64x( 0xFFFFFFFF ) ) );          /* saturated elements */
    //u128_logmsg(" mask_hi ", (U128) mask_sat );

    /* lo vector - b */
    temp = v128_and( b, _mm_set_epi32( -1, 0, -1, 0 ) );                   /* zero low half of double words */
    mask_sat = v128_not( _mm_cmpeq_epi64( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    lo = v128_andnot( b, mask_sat );                                             /* not saturated elements */
    lo = v128_or(lo, v128_and( mask_sat, _mm_set1_epi64x( 0xFFFFFFFF ) ) );          /* saturated elements */
    //u128_logmsg(" mask_lo ", (U128) mask_sat );

    return v128_pack_64( hi, lo);
}

//=============================================================
// v128 pack saturate condition code
//=============================================================
static inline int v128_pack_logical_saturate_cc_16(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i temp;
    int     cc;

    /* hi vector - a */
    /* zero low half of half-words */
    temp = v128_and( a, _mm_set_epi8( -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 ) );
    mask_hi = v128_not( _mm_cmpeq_epi16( temp, v128_zero() ) );
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    temp = v128_and( b, _mm_set_epi8( -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 ) );
    mask_lo = v128_not( _mm_cmpeq_epi16( temp, v128_zero() ) );
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_16( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}

static inline int v128_pack_logical_saturate_cc_32(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i temp;
    int     cc;

    /* hi vector - a */
    temp = v128_and( a, _mm_set_epi16( -1, 0, -1, 0, -1, 0, -1, 0 ) );          /* zero low half of words */
    mask_hi = v128_not( _mm_cmpeq_epi32( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    temp = v128_and( b, _mm_set_epi16( -1, 0, -1, 0, -1, 0, -1, 0 ) );         /* zero low half of  words */
    mask_lo = v128_not( _mm_cmpeq_epi32( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_32( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}

static inline int v128_pack_logical_saturate_cc_64(__m128i a, __m128i b)
{
    __m128i mask, mask_hi, mask_lo;
    __m128i temp;
    int     cc;

    /* hi vector - a */
    temp = v128_and( a, _mm_set_epi32( -1, 0, -1, 0 ) );                   /* zero low half of double words */
    mask_hi = v128_not( _mm_cmpeq_epi64( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    //u128_logmsg(" mask_hi ", (U128) mask_hi );

    /* lo vector - b */
    temp = v128_and( b, _mm_set_epi32( -1, 0, -1, 0 ) );                   /* zero low half of double words */
    mask_lo = v128_not( _mm_cmpeq_epi64( temp, v128_zero() ) );  /* satuaration mask; upper half non-zero */
    //u128_logmsg(" mask_lo ", (U128) mask_lo );

    mask = v128_pack_64( mask_hi, mask_lo);
    //u128_logmsg(" mask   ", (U128) mask );

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 0;                // No saturation
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 3;            // Saturation on all elements
        else
            cc = 1;            // At least one but not all elements saturated
    }

    return cc;
}

//=============================================================
// v128 unpack logical low
//=============================================================
// element unpack logical low
static inline __m128i v128_unpack_logical_low_8( const __m128i a )
{
    return _mm_cvtepu8_epi16( a );
}

static inline __m128i v128_unpack_logical_low_16( const __m128i a )
{
    return _mm_cvtepu16_epi32( a );
}

static inline __m128i v128_unpack_logical_low_32( const __m128i a )
{
    return _mm_cvtepu32_epi64( a );
}

//=============================================================
// v128 unpack logical high
//=============================================================
// element unpack logical high
static inline __m128i v128_unpack_logical_high_8( const __m128i a )
{
    return _mm_cvtepu8_epi16( _mm_shuffle_epi32 ( a, 0x0E ) );
}

static inline __m128i v128_unpack_logical_high_16( const __m128i a )
{
    return _mm_cvtepu16_epi32( _mm_shuffle_epi32 ( a, 0x0E )  );
}

static inline __m128i v128_unpack_logical_high_32( const __m128i a )
{
    return _mm_cvtepu32_epi64( _mm_shuffle_epi32 ( a, 0x0E )  );
}

//=============================================================
// v128 unpack low
//=============================================================
// element unpack low
static inline __m128i v128_unpack_low_8( const __m128i a )
{
    return _mm_cvtepi8_epi16( a );
}

static inline __m128i v128_unpack_low_16( const __m128i a )
{
    return _mm_cvtepi16_epi32( a );
}

static inline __m128i v128_unpack_low_32( const __m128i a )
{
    return _mm_cvtepi32_epi64( a );
}

//=============================================================
// v128 unpack high
//=============================================================
// element unpack high
static inline __m128i v128_unpack_high_8( const __m128i a )
{
    return _mm_cvtepi8_epi16( _mm_shuffle_epi32 ( a, 0x0E ) );
}

static inline __m128i v128_unpack_high_16( const __m128i a )
{
    return _mm_cvtepi16_epi32( _mm_shuffle_epi32 ( a, 0x0E )  );
}

static inline __m128i v128_unpack_high_32( const __m128i a )
{
    return _mm_cvtepi32_epi64( _mm_shuffle_epi32 ( a, 0x0E )  );
}

//=============================================================
// v128 compare equal
//=============================================================
// element compare equal
static inline __m128i v128_compare_equal_8( const __m128i a, const __m128i b )
{
    return _mm_cmpeq_epi8( a, b );
}

static inline __m128i v128_compare_equal_16( const __m128i a, const __m128i b )
{
    return _mm_cmpeq_epi16( a, b );
}

static inline __m128i v128_compare_equal_32( const __m128i a, const __m128i b )
{
    return _mm_cmpeq_epi32( a, b );
}

static inline __m128i v128_compare_equal_64( const __m128i a, const __m128i b )
{
    return _mm_cmpeq_epi64( a, b );
}

//=============================================================
// v128 compare equal condition code
//=============================================================
static inline int v128_compare_equal_cc( const __m128i mask )
{
    int     cc;

    if ( _mm_testz_si128 ( mask, _mm_set1_epi64x(-1) ) )
    {
        cc = 3;         // No elements equal
    }
    else
    {
        if ( _mm_movemask_epi8( mask ) == 0xFFFF )
            cc = 0;     // All equal
        else
            cc = 1;     // At least one but not all elements equal
    }

    return cc;
}


//=============================================================
// v128 compare high
//=============================================================
// element compare high
static inline __m128i v128_compare_high_8( const __m128i a, const __m128i b )
{
    return _mm_cmpgt_epi8( a, b );
}

static inline __m128i v128_compare_high_16( const __m128i a, const __m128i b )
{
    return _mm_cmpgt_epi16( a, b );
}

static inline __m128i v128_compare_high_32( const __m128i a, const __m128i b )
{
    return _mm_cmpgt_epi32( a, b );
}

static inline __m128i v128_compare_high_64( const __m128i a, const __m128i b )
{
    return _mm_cmpgt_epi64( a, b );
}

//=============================================================
// v128 compare hign condition code
//=============================================================
static inline int v128_compare_high_cc( const __m128i mask )
{
    return v128_compare_equal_cc ( mask );
}


//=============================================================
// v128 compare high logical
//=============================================================
// reference: https://stackoverflow.com/questions/56526082/is-there-a-way-to-subtract-packed-unsigned-doublewords-saturated-on-x86-using
// element compare high
static inline __m128i v128_compare_high_logical_8( const __m128i a, const __m128i b )
{
    const __m128i highbit = _mm_set1_epi8( 0x80 );

    return _mm_cmpgt_epi8( _mm_xor_si128( a, highbit ), _mm_xor_si128( b, highbit ) );
}

static inline __m128i v128_compare_high_logical_16( const __m128i a, const __m128i b )
{
    const __m128i highbit = _mm_set1_epi16( 0x8000 );

    return _mm_cmpgt_epi16( _mm_xor_si128( a, highbit ), _mm_xor_si128( b, highbit ) );
}

static inline __m128i v128_compare_high_logical_32( const __m128i a, const __m128i b )
{
    const __m128i highbit = _mm_set1_epi32( 0x80000000 );

    return _mm_cmpgt_epi32( _mm_xor_si128( a, highbit ), _mm_xor_si128( b, highbit ) );
}

static inline __m128i v128_compare_high_logical_64( const __m128i a, const __m128i b )
{
    const __m128i highest = _mm_set1_epi64x( 0x8000000000000000ull );

    return _mm_cmpgt_epi64( _mm_xor_si128( a, highest ), _mm_xor_si128( b, highest ) );
}

//=============================================================
// v128 compare hign logical condition code
//=============================================================
static inline int v128_compare_high_logical_cc( const __m128i mask )
{
    return v128_compare_equal_cc ( mask );
}

/*
Note: v128 shift left/right ... byte: use little endian array indexes!!
*/

//=============================================================
// v128 shift left byte
//=============================================================
static inline __m128i  v128_shift_left_byte( const __m128i a, int shift )
{
    // Predefined constant shuffle masks for 0-15 byte shift
    // note: -1 is a zero byte
    static union  { U8 b[16][16]; __m128i v[16];} shiftmask =
     {
          0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
         -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
         -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
         -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
         -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
         -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
         -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
         -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,
         -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,
         -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,  3,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  2,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0
    };

    // clamp shift count between 0 and 15
    shift = shift & 0xF;

    // shift/shuffle the source
    return _mm_shuffle_epi8(a, shiftmask.v[shift] );
}

//=============================================================
// v128 shift left double byte
//=============================================================
static inline __m128i v128_shift_left_double_byte( const __m128i a, const __m128i b, int shift )
{
    __m128i temphi, templo;

    shift = shift & 0x0F ;
    if (shift == 0)
        return a;

    temphi = v128_shift_left_byte( a, shift);
    templo = v128_shift_right_logical_byte( b, 16 - shift);

    return v128_or (temphi, templo) ;
}

//=============================================================
// v128 shift right logical byte
//=============================================================
static inline __m128i v128_shift_right_logical_byte( const __m128i a, int shift )
{
    // Predefined constant shuffle masks for 0-15 byte shift
    // note: -1 is a zero byte
    static union  { U8 b[16][16]; __m128i v[16];} shiftmask =
     {
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
         1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1,
         2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1,
         3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1,
         4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1,
         5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
         6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1,
         7,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1,
         8,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1,
         9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    // Clamp shift count between 0 and 15
    shift = shift & 0xF;

    // Shuffle the source vector
    return _mm_shuffle_epi8(a, shiftmask.v[shift] );
}
//=============================================================
// v128 shift right arithmetic byte
//=============================================================
static inline __m128i v128_shift_right_arithmetic_byte( const __m128i a, int shift )
{
    QW      temp;
    BOOL    neg ;

    temp.v = a;
    neg    = ( temp.QW_B(0) & 0x80 ) != 0;
    shift  = shift & 0x0F;

    temp.v = v128_shift_right_logical_byte( temp.v , shift);
    if (neg && shift > 0)
    {
        temp.v = v128_or( temp.v, v128_shift_left_byte( _mm_set1_epi64x(-1), 16 - shift) );
    }

    return temp.v ;
}

//=============================================================
// v128 permute
//=============================================================
static inline __m128i v128_permute( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i mask_v4_gt15, shuf_v4, tv4;
    __m128i result_v2, result_v3;

    tv4 = v128_and( v4, _mm_set1_epi8( 0x1f ) );
    mask_v4_gt15 = _mm_cmpgt_epi8( tv4, _mm_set1_epi8 (15) );

    shuf_v4 = v128_and( _mm_sub_epi8 ( _mm_set1_epi8 (31), tv4 ), mask_v4_gt15 ) ;
    result_v3 = _mm_shuffle_epi8( v3, shuf_v4);

    shuf_v4 = v128_andnot( _mm_sub_epi8 ( _mm_set1_epi8 (15), tv4 ), mask_v4_gt15 ) ;
    result_v2 = _mm_shuffle_epi8( v2, shuf_v4 );

    return _mm_blendv_epi8( result_v2, result_v3, mask_v4_gt15 );
}


//=============================================================
// v128 bit permute
//=============================================================
static inline int  v128_bit_permute( const __m128i v2, const __m128i v3 )
{
    __m128i  bit_in_byte, byte_in_sv, bit_in_byte_mask, bit_mask;
    __m128i  temp;

    // get byte and bit_in_byte of the source vector
    bit_in_byte = v128_and( v3, _mm_set1_epi8( 0x07 ) );
    byte_in_sv = v128_srl_8( v3, 3 );

    // get source vector bytes
    temp = v128_permute( v2, v128_zero(), byte_in_sv );

    bit_mask = _mm_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 );
    bit_in_byte_mask = _mm_shuffle_epi8( bit_mask, bit_in_byte);

    temp = v128_and( temp, bit_in_byte_mask);
    temp = v128_not( _mm_cmpeq_epi8( temp, v128_zero() ) );

    return _mm_movemask_epi8( temp );
}


//=============================================================
// v128 multiply low
//=============================================================
static inline __m128i v128_multiply_low_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    //     Convert 8-bit elements to unsigned 16-bit elements,
    //     multiply, make U8, and repack to 8-bit elements
    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu8_epi16( v3 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi);
    temphi = v128_and( temphi, _mm_set1_epi16( 0x0FF ) );
    templo = _mm_mullo_epi16 ( v2lo, v3lo);
    templo = v128_and( templo, _mm_set1_epi16( 0x0FF ) );

    return _mm_packus_epi16( templo, temphi );
}

static inline __m128i v128_multiply_low_16( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    //     Convert unsigned 16-bit elements to signed 32-bit elements,
    //     multiply, make U16, and repack to 8-bit elements
    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2 );

    v3hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu16_epi32( v3 );

    temphi = _mm_mullo_epi32 ( v2hi, v3hi);
    temphi = v128_and( temphi, _mm_set1_epi32( 0xFFFF ) );
    templo = _mm_mullo_epi32 ( v2lo, v3lo);
    templo = v128_and( templo, _mm_set1_epi32( 0xFFFF ) );

    return _mm_packus_epi32( templo, temphi );
}

static inline __m128i v128_multiply_low_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    //     shuffle 32-bii elements to low part of double,
    //     usigned multiply, reshuffle low 32-bits of double,
    //     combine hi and low elements
    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    temphi = _mm_mul_epu32( v2hi, v3hi);
    temphi = _mm_shuffle_epi32( temphi, 0x08 );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epu32( v2lo, v3lo);
    templo = _mm_shuffle_epi32( templo, 0x08 );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}



//=============================================================
// v128 multiply high
//=============================================================
static inline __m128i v128_multiply_high_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    //     Convert 8-bit elements to signed 16-bit elements,
    //     multiply, suffle upper bytes, and combine hi/lo
    v2hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepi8_epi16( v2 );

    v3hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepi8_epi16( v3 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi);
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1 ));


    templo = _mm_mullo_epi16 ( v2lo, v3lo);
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));

    return v128_or( templo, temphi );
}

static  inline __m128i  v128_multiply_high_16( const __m128i v2, const __m128i v3  )
{
    return _mm_mulhi_epi16( v2, v3 );
}


static inline __m128i v128_multiply_high_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    temphi = _mm_mul_epi32( v2hi, v3hi);
    temphi = _mm_shuffle_epi32( temphi, 0x0D );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epi32( v2lo, v3lo);
    templo = _mm_shuffle_epi32( templo, 0x0D );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}


//=============================================================
// v128 multiply logical high
//=============================================================
static inline __m128i v128_multiply_logical_high_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    //     Convert unsigned 8-bit elements to signed 16-bit elements,
    //     multiply, suffle upper bytes, and combine hi/lo
    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu8_epi16( v3 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi);
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_mullo_epi16 ( v2lo, v3lo);
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));

    return v128_or( templo, temphi );
}

static inline __m128i v128_multiply_logical_high_16( const __m128i v2, const __m128i v3  )
{
    return _mm_mulhi_epu16( v2, v3 );
}

static inline __m128i v128_multiply_logical_high_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i  temphi, templo;

    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    temphi = _mm_mul_epu32( v2hi, v3hi);
    temphi = _mm_shuffle_epi32( temphi, 0x0D );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epu32( v2lo, v3lo);
    templo = _mm_shuffle_epi32( templo, 0x0D );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}


//=============================================================
// v128 multiple add low
//=============================================================
static inline __m128i v128_multiply_add_low_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert unsigned 8-bit elements to signed 16-bit elements,
    //     multiply and add, make U8, and repack to 8-bit elements

    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu8_epi16( v3 );

    v4hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepu8_epi16( v4 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi );
    temphi = _mm_add_epi16( temphi, v4hi );
    temphi = v128_and( temphi, _mm_set1_epi16( 0x0FF ) );

    templo = _mm_mullo_epi16 ( v2lo, v3lo );
    templo = _mm_add_epi16( templo, v4lo );
    templo = v128_and( templo, _mm_set1_epi16( 0x0FF ) );

    return _mm_packus_epi16( templo, temphi );
}

static inline __m128i v128_multiply_add_low_16( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert unsigned 16-bit elements to signed 32-bit elements,
    //     multiply & add, make U16, and repack to 8-bit elements

    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2  );

    v3hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu16_epi32( v3 );

    v4hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepu16_epi32( v4 );

    temphi = _mm_mullo_epi32 ( v2hi, v3hi );
    temphi = _mm_add_epi32( temphi, v4hi );
    temphi = v128_and( temphi, _mm_set1_epi32( 0xFFFF ) );
    templo = _mm_mullo_epi32 ( v2lo, v3lo);
    templo = _mm_add_epi32( templo, v4lo );
    templo = v128_and( templo, _mm_set1_epi32( 0xFFFF ) );

    return _mm_packus_epi32( templo, temphi );
}


static inline __m128i v128_multiply_add_low_32( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    v4hi = _mm_shuffle_epi32( v4, 0x32 );
    v4hi = v128_and( v4hi, _mm_set_epi32( 0, -1, 0, -1) );
    v4lo = _mm_shuffle_epi32( v4, 0x10 );
    v4lo = v128_and( v4lo, _mm_set_epi32( 0, -1, 0, -1) );

    temphi = _mm_mul_epu32( v2hi, v3hi) ;
    temphi = _mm_add_epi64( temphi, v4hi );
    temphi = _mm_shuffle_epi32( temphi, 0x08 );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epu32( v2lo, v3lo);
    templo = _mm_add_epi64( templo, v4lo );
    templo = _mm_shuffle_epi32( templo, 0x08 );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}



//=============================================================
// v128 multiple add logical high
//=============================================================
static inline __m128i v128_multiply_add_logical_high_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert unsigned 8-bit elements to signed 16-bit elements,
    //     multiply and add, make U8, and repack to 8-bit elements
    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu8_epi16( v3 );

    v4hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepu8_epi16( v4 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi );
    temphi = _mm_add_epi16( temphi, v4hi );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1,-1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_mullo_epi16 ( v2lo, v3lo );
    templo = _mm_add_epi16( templo, v4lo );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));

    return v128_or( templo, temphi );;
}

static  inline __m128i  v128_multiply_add_logical_high_16( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert unsigned 16-bit elements to signed 32-bit elements,
    //     multiply & add, make U16, and repack to 8-bit elements

    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2  );

    v3hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu16_epi32( v3 );

    v4hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepu16_epi32( v4 );

    temphi = _mm_mullo_epi32( v2hi, v3hi );
    temphi = _mm_add_epi32( temphi, v4hi );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 14, 11, 10, 7, 6, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_mullo_epi32( v2lo, v3lo);
    templo = _mm_add_epi32( templo, v4lo );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));

    return v128_or( templo, temphi );
}


static inline __m128i v128_multiply_add_logical_high_32( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    v4hi = _mm_shuffle_epi32( v4, 0x32 );
    v4hi = v128_and( v4hi, _mm_set_epi32( 0, -1, 0, -1) );
    v4lo = _mm_shuffle_epi32( v4, 0x10 );
    v4lo = v128_and( v4lo, _mm_set_epi32( 0, -1, 0, -1) );

    temphi = _mm_mul_epu32( v2hi, v3hi) ;
    temphi = _mm_add_epi64( temphi, v4hi );
    temphi = _mm_shuffle_epi32( temphi, 0x0D );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epu32( v2lo, v3lo);
    templo = _mm_add_epi64( templo, v4lo );
    templo = _mm_shuffle_epi32( templo, 0x0D );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}


//=============================================================
// v128 multiple add high
//=============================================================
static inline __m128i v128_multiply_add_high_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert signed 8-bit elements to signed 16-bit elements,
    //     multiply and add, repack high byte to 8-bit elements
    v2hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepi8_epi16( v2 );

    v3hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepi8_epi16( v3 );

    v4hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepi8_epi16( v4 );

    temphi = _mm_mullo_epi16 ( v2hi, v3hi );
    temphi = _mm_add_epi16( temphi, v4hi );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_mullo_epi16 ( v2lo, v3lo );
    templo = _mm_add_epi16( templo, v4lo );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));

    return v128_or( templo, temphi );;
}

static inline __m128i v128_multiply_add_high_16( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    //     Convert signed 16-bit elements to signed 32-bit elements,
    //     multiply & add, and repack hign halfword to 16-bit elements
    v2hi = _mm_cvtepi16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepi16_epi32( v2  );

    v3hi = _mm_cvtepi16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepi16_epi32( v3 );

    v4hi = _mm_cvtepi16_epi32( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepi16_epi32( v4 );

    temphi = _mm_mullo_epi32( v2hi, v3hi );
    temphi = _mm_add_epi32( temphi, v4hi );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 14, 11, 10, 7, 6, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_mullo_epi32( v2lo, v3lo);
    templo = _mm_add_epi32( templo, v4lo );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));

    return v128_or( templo, temphi );
}

static inline __m128i v128_multiply_add_high_32( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo, v4hi, v4lo;
    __m128i  temphi, templo;

    v2hi = _mm_shuffle_epi32( v2, 0x32 );
    v2lo = _mm_shuffle_epi32( v2, 0x10 );

    v3hi = _mm_shuffle_epi32( v3, 0x32 );
    v3lo = _mm_shuffle_epi32( v3, 0x10 );

    v4hi = _mm_cvtepi32_epi64( _mm_bsrli_si128( v4, 8) );
    v4lo = _mm_cvtepi32_epi64( v4 );

    temphi = _mm_mul_epi32( v2hi, v3hi) ;
    temphi = _mm_add_epi64( temphi, v4hi );
    temphi = _mm_shuffle_epi32( temphi, 0x0D );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_mul_epi32( v2lo, v3lo);
    templo = _mm_add_epi64( templo, v4lo );
    templo = _mm_shuffle_epi32( templo, 0x0D );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}


//=============================================================
// v128 multiply logical even
//=============================================================
static inline __m128i v128_multiply_logical_even_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v2ev = _mm_cvtepu8_epi16( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v3ev = _mm_cvtepu8_epi16( v3ev );

    return  _mm_mullo_epi16 ( v2ev, v3ev);
}

static inline __m128i v128_multiply_logical_even_16( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v2ev = _mm_cvtepu16_epi32( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v3ev = _mm_cvtepu16_epi32( v3ev );

    return  _mm_mullo_epi32 ( v2ev, v3ev);
}


static inline __m128i v128_multiply_logical_even_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v2ev = _mm_cvtepu32_epi64( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v3ev = _mm_cvtepu32_epi64( v3ev );

    return  _mm_mul_epu32 ( v2ev, v3ev);
}

//=============================================================
// v128 multiply logical odd
//=============================================================
static inline __m128i v128_multiply_logical_odd_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v2od = _mm_cvtepu8_epi16( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v3od = _mm_cvtepu8_epi16( v3od );

    return  _mm_mullo_epi16 ( v2od, v3od);
}

static inline __m128i v128_multiply_logical_odd_16( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v2od = _mm_cvtepu16_epi32( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v3od = _mm_cvtepu16_epi32( v3od );

    return  _mm_mullo_epi32 ( v2od, v3od);
}


static inline __m128i v128_multiply_logical_odd_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v2od = _mm_cvtepu32_epi64( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v3od = _mm_cvtepu32_epi64( v3od );

    return  _mm_mul_epu32 ( v2od, v3od);
}



//=============================================================
// v128 multiply even
//=============================================================
static inline __m128i v128_multiply_even_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v2ev = _mm_cvtepi8_epi16( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v3ev = _mm_cvtepi8_epi16( v3ev );

    return  _mm_mullo_epi16 ( v2ev, v3ev);
}

static inline __m128i v128_multiply_even_16( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v2ev = _mm_cvtepi16_epi32( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v3ev = _mm_cvtepi16_epi32( v3ev );

    return  _mm_mullo_epi32 ( v2ev, v3ev);
}


static inline __m128i v128_multiply_even_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2ev, v3ev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v2ev = _mm_cvtepi32_epi64( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v3ev = _mm_cvtepi32_epi64( v3ev );

    return  _mm_mul_epi32 ( v2ev, v3ev);
}

//=============================================================
// v128 multiply odd
//=============================================================
static inline __m128i v128_multiply_odd_8( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v2od = _mm_cvtepi8_epi16( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v3od = _mm_cvtepi8_epi16( v3od );

    return  _mm_mullo_epi16 ( v2od, v3od);
}

static inline __m128i v128_multiply_odd_16( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v2od = _mm_cvtepi16_epi32( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1,  13, 12, 9, 8, 5, 4, 1, 0 ));
    v3od = _mm_cvtepi16_epi32( v3od );

    return  _mm_mullo_epi32 ( v2od, v3od);
}


static inline __m128i v128_multiply_odd_32( const __m128i v2, const __m128i v3  )
{
    __m128i  v2od, v3od;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v2od = _mm_cvtepi32_epi64( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v3od = _mm_cvtepi32_epi64( v3od );

    return  _mm_mul_epi32 ( v2od, v3od);
}


//=============================================================
// v128 multiply add logical even
//=============================================================
static inline __m128i v128_multiply_add_logical_even_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v2ev = _mm_cvtepu8_epi16( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v3ev = _mm_cvtepu8_epi16( v3ev );

    tempev = _mm_mullo_epi16 ( v2ev, v3ev );
    tempev = _mm_add_epi16( tempev, v4 );

    return tempev;
}

static inline __m128i v128_multiply_add_logical_even_16( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v2ev = _mm_cvtepu16_epi32( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v3ev = _mm_cvtepu16_epi32( v3ev );

    tempev = _mm_mullo_epi32 ( v2ev, v3ev );
    tempev = _mm_add_epi32( tempev, v4 );

    return tempev;
}


static inline __m128i v128_multiply_add_logical_even_32( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v2ev = _mm_cvtepu32_epi64( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v3ev = _mm_cvtepu32_epi64( v3ev );

    tempev = _mm_mul_epu32 ( v2ev, v3ev);
    tempev = _mm_add_epi64( tempev, v4 );

    return  tempev;
}

//=============================================================
// v128 multiply add logical odd
//=============================================================

static inline __m128i v128_multiply_add_logical_odd_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v2od = _mm_cvtepu8_epi16( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v3od = _mm_cvtepu8_epi16( v3od );

    tempod = _mm_mullo_epi16 ( v2od, v3od );
    tempod = _mm_add_epi16( tempod, v4 );

    return tempod;
}

static inline __m128i v128_multiply_add_logical_odd_16( const __m128i v2, const __m128i v3, const __m128i v4  )
{
    __m128i  v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v2od = _mm_cvtepu16_epi32( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v3od = _mm_cvtepu16_epi32( v3od );

    tempod = _mm_mullo_epi32 ( v2od, v3od );
    tempod = _mm_add_epi32( tempod, v4 );

    return tempod;
}


static  inline __m128i  v128_multiply_add_logical_odd_32( const __m128i v2, const __m128i v3, const __m128i v4  )
{
    __m128i  v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v2od = _mm_cvtepu32_epi64( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v3od = _mm_cvtepu32_epi64( v3od );

    tempod = _mm_mul_epu32 ( v2od, v3od );
    tempod = _mm_add_epi64( tempod, v4 );

    return tempod;
}


//=============================================================
// v128 multiply add even
//=============================================================
static inline  __m128i  v128_multiply_add_even_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v2ev = _mm_cvtepi8_epi16( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));
    v3ev = _mm_cvtepi8_epi16( v3ev );

    tempev = _mm_mullo_epi16 ( v2ev, v3ev );
    tempev = _mm_add_epi16( tempev, v4 );

    return tempev;
}

static inline __m128i v128_multiply_add_even_16( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v2ev = _mm_cvtepi16_epi32( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));
    v3ev = _mm_cvtepi16_epi32( v3ev );

    tempev = _mm_mullo_epi32 ( v2ev, v3ev );
    tempev = _mm_add_epi32( tempev, v4 );

    return tempev;
}


static inline __m128i v128_multiply_add_even_32( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i  v2ev, v3ev;
    __m128i  tempev;

    v2ev = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v2ev = _mm_cvtepi32_epi64( v2ev );

    v3ev = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 7, 6, 5, 4 ));
    v3ev = _mm_cvtepi32_epi64( v3ev );

    tempev = _mm_mul_epi32 ( v2ev, v3ev);
    tempev = _mm_add_epi64( tempev, v4 );

    return  tempev;
}

//=============================================================
// v128 multiply add odd
//=============================================================

static inline __m128i v128_multiply_add_odd_8( const __m128i v2, const __m128i v3, const __m128i v4 )
{
    __m128i v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v2od = _mm_cvtepi8_epi16( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    v3od = _mm_cvtepi8_epi16( v3od );

    tempod = _mm_mullo_epi16 ( v2od, v3od );
    tempod = _mm_add_epi16( tempod, v4 );

    return tempod;
}

static inline __m128i v128_multiply_add_odd_16( const __m128i v2, const __m128i v3, const __m128i v4  )
{
    __m128i  v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v2od = _mm_cvtepi16_epi32( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    v3od = _mm_cvtepi16_epi32( v3od );

    tempod = _mm_mullo_epi32 ( v2od, v3od );
    tempod = _mm_add_epi32( tempod, v4 );

    return tempod;
}


static inline __m128i v128_multiply_add_odd_32( const __m128i v2, const __m128i v3, const __m128i v4  )
{
    __m128i  v2od, v3od;
    __m128i tempod;

    v2od = _mm_shuffle_epi8( v2, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v2od = _mm_cvtepi32_epi64( v2od );

    v3od = _mm_shuffle_epi8( v3, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 11, 10, 9, 8, 3, 2, 1, 0 ));
    v3od = _mm_cvtepi32_epi64( v3od );

    tempod = _mm_mul_epi32 ( v2od, v3od );
    tempod = _mm_add_epi64( tempod, v4 );

    return tempod;
}

//=============================================================
// v128 average
//=============================================================

static inline __m128i v128_average_8( const __m128i v2, const __m128i v3 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i temphi, templo;

    v2hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepi8_epi16( v2 );

    v3hi = _mm_cvtepi8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepi8_epi16( v3 );

    temphi = _mm_add_epi16( v2hi, v3hi);
    temphi = _mm_add_epi16( temphi, _mm_set1_epi16(1) );
    temphi = _mm_srai_epi16 ( temphi, 1 );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 14, 12, 10, 8, 6, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1 ));
    //temphi = v128_and( temphi, _mm_set_epi64x( -1, 0) );

    templo = _mm_add_epi16( v2lo, v3lo);
    templo = _mm_add_epi16( templo, _mm_set1_epi16(1) );
    templo = _mm_srai_epi16 ( templo, 1 );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 14, 12, 10, 8, 6, 4, 2, 0 ));
    //templo = v128_and( templo, _mm_set_epi64x( 0, -1) );

    return v128_or( templo, temphi );
}

static inline __m128i v128_average_16( const __m128i v2, const __m128i v3 )
{
    __m128i  v2hi, v2lo, v3hi, v3lo;
    __m128i temphi, templo;

    v2hi = _mm_cvtepi16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepi16_epi32( v2 );

    v3hi = _mm_cvtepi16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepi16_epi32( v3 );

    temphi = _mm_add_epi32( v2hi, v3hi);
    temphi = _mm_add_epi32( temphi, _mm_set1_epi32(1) );
    temphi = _mm_srai_epi32 ( temphi, 1 );
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 13, 12, 9, 8, 5, 4, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1 ));
    //temphi = v128_and( temphi, _mm_set_epi64x( -1, 0) );

    templo = _mm_add_epi32( v2lo, v3lo);
    templo = _mm_add_epi32( templo, _mm_set1_epi32(1) );
    templo = _mm_srai_epi32 ( templo, 1 );
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 13, 12, 9, 8, 5, 4, 1, 0 ));
    //templo = v128_and( templo, _mm_set_epi64x( 0, -1) );

    return v128_or( templo, temphi );
}

//=============================================================
// v128 average logical
//=============================================================

static inline __m128i v128_average_logical_8( const __m128i v2, const __m128i v3 )
{
    return _mm_avg_epu8( v2, v3 );
}

static inline __m128i v128_average_logical_16( const __m128i v2, const __m128i v3 )
{
    return _mm_avg_epu16( v2, v3 );
}

//=============================================================
// v128 positive
//=============================================================

static inline __m128i v128_positive_8( const __m128i v2 )
{
    return _mm_abs_epi8( v2 );
}

static inline __m128i v128_positive_16( const __m128i v2 )
{
    return _mm_abs_epi16( v2 );
}

static inline __m128i v128_positive_32( const __m128i v2 )
{
    return _mm_abs_epi32( v2 );
}


//=============================================================
// v128 maximum
//=============================================================

static inline __m128i v128_maximum_8( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epi8( v2, v3 );
}

static inline __m128i v128_maximum_16( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epi16( v2, v3 );
}

static inline __m128i v128_maximum_32( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epi32( v2, v3 );
}

//=============================================================
// v128 minimum
//=============================================================

static inline __m128i v128_minimum_8( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epi8( v2, v3 );
}

static inline __m128i v128_minimum_16( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epi16( v2, v3 );
}

static inline __m128i v128_minimum_32( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epi32( v2, v3 );
}


//=============================================================
// v128 maximum logical
//=============================================================

static inline __m128i v128_maximum_logical_8( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epu8( v2, v3 );
}

static inline __m128i v128_maximum_logical_16( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epu16( v2, v3 );
}

static inline __m128i v128_maximum_logical_32( const __m128i v2, const __m128i v3 )
{
    return _mm_max_epu32( v2, v3 );
}

//=============================================================
// v128 minimum logical
//=============================================================

static inline __m128i v128_minimum_logical_8( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epu8( v2, v3 );
}

static inline __m128i v128_minimum_logical_16( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epu16( v2, v3 );
}

static inline __m128i v128_minimum_logical_32( const __m128i v2, const __m128i v3 )
{
    return _mm_min_epu32( v2, v3 );
}

//=============================================================
// v128 add
//=============================================================

static inline __m128i v128_add_8( const __m128i v2, const __m128i v3 )
{
    return _mm_add_epi8( v2, v3 );
}

static inline __m128i v128_add_16( const __m128i v2, const __m128i v3 )
{
    return _mm_add_epi16( v2, v3 );
}

static inline __m128i v128_add_32( const __m128i v2, const __m128i v3 )
{
    return _mm_add_epi32( v2, v3 );
}

static inline __m128i v128_add_64( const __m128i v2, const __m128i v3 )
{
    return _mm_add_epi64( v2, v3 );
}

//=============================================================
// v128 subtract
//=============================================================

static inline __m128i v128_subtract_8( const __m128i v2, const __m128i v3 )
{
    return _mm_sub_epi8( v2, v3 );
}

static inline __m128i v128_subtract_16( const __m128i v2, const __m128i v3 )
{
    return _mm_sub_epi16( v2, v3 );
}

static inline __m128i v128_subtract_32( const __m128i v2, const __m128i v3 )
{
    return _mm_sub_epi32( v2, v3 );
}

static inline __m128i v128_subtract_64( const __m128i v2, const __m128i v3 )
{
    return _mm_sub_epi64( v2, v3 );
}


//=============================================================
// v128 sum across word
//=============================================================

static inline __m128i v128_sum_across_word_8( const __m128i v2, const __m128i v3 )
{
    __m128i  v2hi, v2lo, v3b;
    __m128i tempsum;

    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3b = v128_and( v3, _mm_set1_epi32( 0xFF ) );

    tempsum = _mm_hadd_epi16(v2lo, v2hi);
    tempsum = _mm_hadd_epi16(tempsum, v128_zero() );

    tempsum = _mm_cvtepu16_epi32( tempsum );
    tempsum = _mm_add_epi32( tempsum, v3b );

    return tempsum;
}

static inline __m128i v128_sum_across_word_16( const __m128i v2, const __m128i v3 )
{
    __m128i  v2hi, v2lo, v3w;
    __m128i tempsum;

    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2 );

    v3w = v128_and( v3, _mm_set1_epi32( 0xFFFF ) );

    tempsum = _mm_hadd_epi32(v2lo, v2hi);
    tempsum = _mm_add_epi32( tempsum, v3w );

    return tempsum;
}

//=============================================================
// v128 sum across doubleword
//=============================================================

static inline __m128i v128_sum_across_doubleword_16( const __m128i v2, const __m128i v3 )
{
    __m128i  v2hi, v2lo, v3dw;
    __m128i tempsum;

    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2 );

    v3dw = v128_and( v3, _mm_set1_epi64x( 0xFFFF ) );

    tempsum = _mm_hadd_epi32(v2lo, v2hi);
    tempsum = _mm_hadd_epi32(tempsum, v128_zero() );
    tempsum = _mm_cvtepu32_epi64( tempsum );
    tempsum = _mm_add_epi64( tempsum, v3dw );

    return tempsum;
}


//=============================================================
// v128 vector subtract compute borrow indication
//=============================================================

static inline __m128i v128_subtract_compute_borrow_ind_8( const __m128i v2 , const __m128i v3 )
{
    __m128i  min_mask, ne_mask;
    __m128i  temp;

    min_mask = _mm_cmpeq_epi8( v2, _mm_min_epu8( v2, v3) );
    ne_mask =  v128_not( _mm_cmpeq_epi8( v2, v3 ) );

    temp =  v128_and( min_mask, ne_mask );      // 0xFF mask where v2.b < v3.b
    temp = v128_and ( v128_not(temp), _mm_set1_epi8( 0x01) );

    return temp;
}

static inline __m128i v128_subtract_compute_borrow_ind_16( const __m128i v2 , const __m128i v3 )
{
    __m128i  min_mask, ne_mask;
    __m128i  temp;

    min_mask = _mm_cmpeq_epi16( v2, _mm_min_epu16( v2, v3) );
    ne_mask =  v128_not( _mm_cmpeq_epi16( v2, v3 ) );

    temp =  v128_and( min_mask, ne_mask );      // 0xFFFF mask where v2.b < v3.b
    temp = v128_and ( v128_not(temp), _mm_set1_epi16( 0x01) );

    return temp;
}

static inline __m128i v128_subtract_compute_borrow_ind_32( const __m128i v2 , const __m128i v3 )
{
    __m128i  min_mask, ne_mask;
    __m128i  temp;

    min_mask = _mm_cmpeq_epi32( v2, _mm_min_epu32( v2, v3) );
    ne_mask =  v128_not( _mm_cmpeq_epi32( v2, v3 ) );

    temp =  v128_and( min_mask, ne_mask );      // 0xFFFFFFFF mask where v2.b < v3.b
    temp = v128_and ( v128_not(temp), _mm_set1_epi32( 0x01) );

    return temp;
}

//=============================================================
// v128 vector add compute carry
//=============================================================
static inline __m128i v128_add_compute_carry_8( const __m128i v2 , const __m128i v3 )
{
    __m128i  v2lo, v2hi, v3lo, v3hi;
    __m128i  templo, temphi;

    v2hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu8_epi16( v2 );

    v3hi = _mm_cvtepu8_epi16( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu8_epi16( v3 );

    temphi = _mm_add_epi16 ( v2hi, v3hi);
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_add_epi16 ( v2lo, v3lo);
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 13, 11, 9, 7, 5, 3, 1 ));

    return v128_or( templo, temphi );
}

static inline __m128i v128_add_compute_carry_16( const __m128i v2 , const __m128i v3 )
{
    __m128i  v2lo, v2hi, v3lo, v3hi;
    __m128i  templo, temphi;

    v2hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu16_epi32( v2 );

    v3hi = _mm_cvtepu16_epi32( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu16_epi32( v3 );

    temphi = _mm_add_epi32 ( v2hi, v3hi);
    temphi = _mm_shuffle_epi8( temphi, _mm_set_epi8( 15, 14, 11, 10, 7, 6, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1 ));

    templo = _mm_add_epi32 ( v2lo, v3lo);
    templo = _mm_shuffle_epi8( templo, _mm_set_epi8( -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 11, 10, 7, 6, 3, 2 ));

    return v128_or( templo, temphi );
}

static inline __m128i v128_add_compute_carry_32( const __m128i v2 , const __m128i v3 )
{
    __m128i  v2lo, v2hi, v3lo, v3hi;
    __m128i  templo, temphi;

    v2hi = _mm_cvtepu32_epi64( _mm_bsrli_si128( v2, 8) );
    v2lo = _mm_cvtepu32_epi64( v2 );

    v3hi = _mm_cvtepu32_epi64( _mm_bsrli_si128( v3, 8) );
    v3lo = _mm_cvtepu32_epi64( v3 );

    temphi = _mm_add_epi64 ( v2hi, v3hi);
    temphi = _mm_shuffle_epi32( temphi, 0x0D );
    temphi = _mm_bslli_si128( temphi, 8);

    templo = _mm_add_epi64 ( v2lo, v3lo);
    templo = _mm_shuffle_epi32( templo, 0x0D );
    templo = v128_and( templo, _mm_set_epi32( 0, 0, -1, -1) );

    return v128_or( templo, temphi );
}


//=============================================================
// v128 Rotate and Insert Under Mask
//=============================================================
static inline __m128i v128_rotate_and_insert_under_mask_8( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  )
{
    int sl, sr;
    __m128i  v2sl, v2sr, temp1, temp2;

    sl = rot % 8;
    sr = 8 - sl;

    v2sl = v128_sll_8( v2, sl);
    v2sr = v128_srl_8( v2, sr);
    temp1 = v128_and( v128_or ( v2sl, v2sr ), v3);
    temp2 = v128_andnot( v1, v3 );

    return v128_or( temp1, temp2);
}

static inline __m128i v128_rotate_and_insert_under_mask_16( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  )
{
    int sl, sr;
    __m128i  v2sl, v2sr, temp1, temp2;

    sl = rot % 16;
    sr = 16 - sl;

    v2sl = v128_sll_16( v2, sl);
    v2sr = v128_srl_16( v2, sr);
    temp1 = v128_and( v128_or ( v2sl, v2sr ), v3);
    temp2 = v128_andnot( v1, v3 );

    return v128_or( temp1, temp2);
}

static inline __m128i v128_rotate_and_insert_under_mask_32( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  )
{
    int sl, sr;
    __m128i  v2sl, v2sr, temp1, temp2;

    sl = rot % 32;
    sr = 32 - sl;

    v2sl = v128_sll_32( v2, sl);
    v2sr = v128_srl_32( v2, sr);
    temp1 = v128_and( v128_or ( v2sl, v2sr ), v3);
    temp2 = v128_andnot( v1, v3 );

    return v128_or( temp1, temp2);
}

static inline __m128i v128_rotate_and_insert_under_mask_64( const __m128i v1, const __m128i v2, const __m128i v3, const int rot  )
{
    int sl, sr;
    __m128i  v2sl, v2sr, temp1, temp2;

    sl = rot % 64;
    sr = 64 - sl;

    v2sl = v128_sll_64( v2, sl);
    v2sr = v128_srl_64( v2, sr);
    temp1 = v128_and( v128_or ( v2sl, v2sr ), v3);
    temp2 = v128_andnot( v1, v3 );

    return v128_or( temp1, temp2);
}


//=============================================================
// v128 find substring
//=============================================================


static inline int v128_find_first_substring_8( const __m128i v2_str, const int v2_len, const __m128i v3_substr, const int v3_len  )
{
    __m128i tempv2_str, tempv3_substr;

    tempv2_str    = v128_bswap ( v2_str );
    tempv3_substr = v128_bswap ( v3_substr );

    return  _mm_cmpestri(tempv3_substr, v3_len, tempv2_str, v2_len,
                   _SIDD_UBYTE_OPS
                | _SIDD_CMP_EQUAL_ORDERED
                | _SIDD_LEAST_SIGNIFICANT
                );
}

static inline int v128_find_first_substring_16( const __m128i v2_str, const int v2_len, const __m128i v3_substr, const int v3_len  )
{
    __m128i tempv2_str, tempv3_substr;

    tempv2_str    = v128_bswap ( v2_str );
    tempv3_substr = v128_bswap ( v3_substr );

    return  _mm_cmpestri(tempv3_substr, v3_len, tempv2_str, v2_len,
                  _SIDD_UWORD_OPS
                | _SIDD_CMP_EQUAL_ORDERED
                | _SIDD_LEAST_SIGNIFICANT
                );
}

static inline int v128_find_first_substring_32( const __m128i v2_str, int v2_len, const __m128i v3_substr, int v3_len  )
{
    QW tempv2_str, tempv3_substr;
    int i, j;
    bool fnd;

    tempv2_str.v    = v2_str ;
    tempv3_substr.v = v3_substr;

    if ( v2_len >= 4 )  v2_len = 4;
    if ( v3_len >= 4 )  v3_len = 4;

    for (i=0; i < v2_len; i++)
    {
        fnd = true;

        for (j=0; j < v3_len && i+j < 4; j++)
        {
            if ( tempv2_str.QW_F(i+j) != tempv3_substr.QW_F(j) )
            {
                fnd = false;
                break;
            }
        }

        if (fnd)
            return i;
    }

    return 4;
}

//=============================================================
// v128 shift left vector
//=============================================================
static inline __m128i v128_shift_left_vector_8( const __m128i v2, const __m128i v3  )
{
    __m128i tempv3;
//  avoid clang warning: implicit conversion from 'int' to 'char' changes value from 128 to -128 [-Wconstant-conversion]
//  values are unsigned for v128_multiply_low_8
//  __m128i mulshift = _mm_set_epi8(  -1, -1, -1, -1, -1, -1, -1, -1,  128,   64,   32,   16,    8,    4,    2,   1 );
    __m128i mulshift = _mm_set_epi8(  -1, -1, -1, -1, -1, -1, -1, -1, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 );

    tempv3 = v128_and( v3, _mm_set1_epi8( 0x07 ) );
    tempv3 = _mm_shuffle_epi8( mulshift,  tempv3 );

    return v128_multiply_low_8( v2, tempv3 );
}

/* restore "unused-function" warnings */
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#elif defined(__GNUC__) && defined(__clang__)
    #pragma clang diagnostic pop
#endif

#endif /* defined( FEATURE_V128_SSE )*/

/*-------------------------------------------------------------------*/
/* Galois Field Multiply                                             */
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/* Galois Field(2) 32-bit Multiply                                   */
/*                                                                   */
/* Input:                                                            */
/*      m1      32-bit multiply operand                              */
/*      m2      32-bit multiply operand                              */
/*                                                                   */
/* Returns:                                                          */
/*              64-bit GF(2) multiply result                         */
/*                                                                   */
/* version depends on whether intrinsics are being used              */
/*-------------------------------------------------------------------*/
static inline U64 gf_mul_32( U32 m1, U32 m2)
{
#if defined( FEATURE_V128_SSE ) && defined( FEATURE_HW_CLMUL )

    if (sysblk.have_PCLMULQDQ)
    {
        /* intrinsic GF 64-bit multiply */
        QW  mm1;                      /* U128 m1                       */
        QW  mm2;                      /* U128 m2                       */
        QW  acc;                      /* U128 accumulator              */

        mm1.v = v128_zero();
        mm1.D.L.D = m1;
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_32 mm1.v", mm1.D.H.D, mm1.D.L.D);

        mm2.v = v128_zero();
        mm2.D.L.D = m2;
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_32 mm2.v", mm2.D.H.D, mm2.D.L.D);

        acc.v =  _mm_clmulepi64_si128 ( mm1.v, mm2.v, 0);
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_32 acc.v", acc.D.H.D, acc.D.L.D);

        return acc.D.L.D;

        // return _mm_extract_epi64(
        //             _mm_clmulepi64_si128(
        //                 _mm_set_epi64x ( 0, m1 ),
        //                 _mm_set_epi64x ( 0, m2 ),
        //                 0
        //             ),
        //             0
        //         );

    }
    else

#endif  //  !(defined( FEATURE_V128_SSE ) && defined( FEATURE_HW_CLMUL )), or
        // "PCLMULQDQ" instruction unavailable
    {
        int     i;                    /* loop index                      */
        U32     myerU32;              /* multiplier                      */
        U64     mcandU64;             /* multiplicand                    */
        U64     accu64;               /* accumulator                     */

        accu64 = 0;

        /* select muliplier with fewest 'right most' bits */
        /* to exit loop soonest                           */
        if (m1 < m2)
        {
            mcandU64 = m2;
            myerU32  = m1;
        }
        else
        {
            mcandU64 = m1;
            myerU32  = m2;
        }

        /* galois multiply - no overflow */
        for (i=0; i < 32 && myerU32 !=0; i++)
        {
            if ( myerU32 & 0x01 )
                accu64 ^= mcandU64;
            myerU32  >>= 1;
            mcandU64 <<=1;
        }

        return accu64;
    }
}

/*-------------------------------------------------------------------*/
/* Galois Field(2) 64-bit Multiply                                   */
/*                                                                   */
/* Input:                                                            */
/*      m1          64-bit multiply operand                          */
/*      m2          64-bit multiply operand                          */
/*      accu128h    pointer to high 64 bits of result                */
/*      accu128l    pointer to low 64 bits of result                 */
/*                                                                   */
/* version depends on whether intrinsics are being used              */
/*-------------------------------------------------------------------*/
static inline void gf_mul_64( U64 m1, U64 m2, U64* accu128h, U64* accu128l)
{
#if defined( FEATURE_V128_SSE ) && defined( FEATURE_HW_CLMUL )

    if (sysblk.have_PCLMULQDQ)
    {
        /* intrinsic GF 64-bit multiply */
        QW  mm1;                      /* U128 m1                       */
        QW  mm2;                      /* U128 m2                       */
        QW  acc;                      /* U128 accumulator              */

        mm1.v = v128_zero();
        mm1.D.L.D = m1;
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_64 mm1.v", mm1.D.H.D, mm1.D.L.D);

        mm2.v = v128_zero();
        mm2.D.L.D = m2;
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_64 mm2.v", mm2.D.H.D, mm2.D.L.D);

        acc.v =  _mm_clmulepi64_si128 ( mm1.v, mm2.v, 0);
            //logmsg("%s: u128=%16.16"PRIX64".%16.16"PRIX64" \n", "gf_mul_64 acc.v", acc.D.H.D, acc.D.L.D);

        *accu128h = acc.D.H.D;
        *accu128l = acc.D.L.D;
    }
    else

#endif  //  !(defined( FEATURE_V128_SSE ) && defined( FEATURE_HW_CLMUL )), or
        // "PCLMULQDQ" instruction unavailable
    {
        /* portable C: GF 64-bit multiply */
        int     i;                    /* loop index                      */
        U64     myerU64;              /* doublewword multiplier          */
        U64     mcandU128h;           /* doublewword multiplicand - high */
        U64     mcandU128l;           /* doublewword multiplicand - low  */

        *accu128h = 0;
        *accu128l = 0;

        /* select muliplier with fewest 'right most' bits */
        /* to exit loop soonest                           */
        if (m1 < m2)
        {
            mcandU128h  = 0;
            mcandU128l = m2;
            myerU64  = m1;
        }
        else
        {
            mcandU128h  = 0;
            mcandU128l = m1;
            myerU64  = m2;
        }

        /* galois multiply - no overflow */
        for (i=0; i < 64 && myerU64 !=0; i++)
        {
            if ( myerU64 & 0x01 )
            {
                *accu128h ^= mcandU128h;
                *accu128l ^= mcandU128l;
            }
            myerU64  >>= 1;

            /* U128: shift left 1 bit*/
            mcandU128h = (mcandU128h << 1) | (mcandU128l >> 63);
            mcandU128l <<= 1;
        }
    }
}

#endif /*!defined(_ZVECTOR_ARCH_INDEPENDENT_)*/

/*===================================================================*/
/* Achitecture Dependent Routines / Instructions                     */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/* E700 VLEB   - Vector Load Element (8)                       [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_8 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    regs->VR_B( v1, m3 ) = ARCH_DEP( vfetchb )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E701 VLEH   - Vector Load Element (16)                      [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_16 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 7)                    /* M3 > 7 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    regs->VR_H( v1, m3 ) = ARCH_DEP( vfetch2 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E702 VLEG   - Vector Load Element (64)                      [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_64 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 1)                    /* M3 > 1 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    regs->VR_D( v1, m3 ) = ARCH_DEP( vfetch8 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E703 VLEF   - Vector Load Element (32)                      [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_32 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 3)                    /* M3 > 3 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    regs->VR_F( v1, m3 ) = ARCH_DEP( vfetch4 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E704 VLLEZ  - Vector Load Logical Element and Zero          [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_logical_element_and_zero )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

#if defined( FEATURE_V128_SSE )
    regs->VR_Q( v1 ).v = v128_zero();
#else
    regs->VR_D(v1, 0) = 0x00;
    regs->VR_D(v1, 1) = 0x00;
#endif

    switch (m3)
    {
    case 0: regs->VR_B( v1, 7 ) = ARCH_DEP( vfetchb )( effective_addr2, b2, regs ); break;
    case 1: regs->VR_H( v1, 3 ) = ARCH_DEP( vfetch2 )( effective_addr2, b2, regs ); break;
    case 2: regs->VR_F( v1, 1 ) = ARCH_DEP( vfetch4 )( effective_addr2, b2, regs ); break;
    case 3: regs->VR_D( v1, 0 ) = ARCH_DEP( vfetch8 )( effective_addr2, b2, regs ); break;
    case 6: regs->VR_F( v1, 0 ) = ARCH_DEP( vfetch4 )( effective_addr2, b2, regs ); break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E705 VLREP  - Vector Load and Replicate                     [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_and_replicate )
{
    int     v1, m3, x2, b2, i;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    switch (m3)
    {
    case 0:
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_8( ARCH_DEP( vfetchb )( effective_addr2, b2, regs ) );
        #else
            regs->VR_B( v1, 0 ) = ARCH_DEP( vfetchb )( effective_addr2, b2, regs );
            for (i=1; i < 16; i++)
                regs->VR_B( v1, i ) = regs->VR_B( v1, 0 );
        #endif
        break;
    case 1:
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_16( ARCH_DEP( vfetch2 )( effective_addr2, b2, regs ) );
        #else
            regs->VR_H( v1, 0 ) = ARCH_DEP( vfetch2 )( effective_addr2, b2, regs );
            for (i=1; i < 8; i++)
                regs->VR_H( v1, i ) = regs->VR_H( v1, 0 );
        #endif
        break;
    case 2:
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_32( ARCH_DEP( vfetch4 )( effective_addr2, b2, regs ) );
        #else
            regs->VR_F( v1, 0 ) = ARCH_DEP( vfetch4 )( effective_addr2, b2, regs );
            for (i=1; i < 4; i++)
                regs->VR_F( v1, i ) = regs->VR_F( v1, 0 );
        #endif
        break;
    case 3:
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_64( ARCH_DEP( vfetch8 )( effective_addr2, b2, regs ) );
        #else
            regs->VR_D( v1, 0 ) = ARCH_DEP( vfetch8 )( effective_addr2, b2, regs );
            regs->VR_D( v1, 1 ) = regs->VR_D( v1, 0 );
        #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E706 VL     - Vector Load                                   [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3);

    /* m3 - Alignment Hint: not used */
    UNREFERENCED( m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    regs->VR_Q( v1 ) = ARCH_DEP( vfetch16 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E707 VLBB   - Vector Load To Block Boundary                 [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_to_block_boundary )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2, boundary_addr;
    U64     boundary;
    U64     length;
    QW      temp;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 6)                    /* M3 > 6 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    boundary = 64 << m3; /* 0: 64 Byte, 1: 128 Byte, 2: 256 Byte, 3: 512 Byte,
                            4: 1 K-byte, 5: 2 K-Byte, 6: 4 K-Byte */

    boundary_addr = (effective_addr2 + boundary) & ~(boundary - 1);

    length = boundary_addr - effective_addr2;
    if (length > 16) length = 16;
    length--;

    memset(&temp, 0x00, sizeof(temp));

    ARCH_DEP( vfetchc )( &temp, (U32)length, effective_addr2, b2, regs );

    regs->VR_Q( v1 ) = CSWAP128( temp );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E708 VSTEB  - Vector Store Element (8)                      [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_element_8 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    ARCH_DEP( vstoreb )( regs->VR_B( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E709 VSTEH  - Vector Store Element (16)                     [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_element_16 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 7)                    /* M3 > 7 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    ARCH_DEP( vstore2 )( regs->VR_H( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E70A VSTEG  - Vector Store Element (64)                     [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_element_64 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 1)                    /* M3 > 1 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    ARCH_DEP( vstore8 )( regs->VR_D( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E70B VSTEF  - Vector Store Element (32)                     [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_element_32 )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 3)                    /* M3 > 3 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    ARCH_DEP( vstore4 )( regs->VR_F( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E70E VST    - Vector Store                                  [VRX] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store )
{
    int     v1, m3, x2, b2;
    VADR    effective_addr2;

    VRX( inst, regs, v1, x2, b2, effective_addr2, m3 );

    /* m3 - Alignment Hint: not used */
    UNREFERENCED( m3 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    ARCH_DEP( vstore16 )( regs->VR_Q( v1 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E712 VGEG   - Vector Gather Element (64)                    [VRV] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_gather_element_64 )
{
    int      v1, v2, b2, d2, m3;
    VADR     effective_addr2;

    VRV( inst, regs, v1, v2, b2, d2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 1)                    /* M3 > 3 => Specification excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    effective_addr2 = d2;

    if (b2)
        effective_addr2 += regs->GR( b2 );

    effective_addr2 += regs->VR_D( v2, m3 );
    effective_addr2 &= ADDRESS_MAXWRAP( regs );

    PER_ZEROADDR_XCHECK( regs, b2 );

    regs->VR_D( v1, m3 ) = ARCH_DEP( vfetch8 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E713 VGEF   - Vector Gather Element (32)                    [VRV] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_gather_element_32 )
{
    int      v1, v2, b2, d2, m3;
    VADR     effective_addr2;

    VRV( inst, regs, v1, v2, b2, d2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 3)                    /* M3 > 3 => Specification excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    effective_addr2 = d2;

    if (b2)
        effective_addr2 += regs->GR( b2 );

    effective_addr2 += regs->VR_F( v2, m3 );
    effective_addr2 &= ADDRESS_MAXWRAP( regs );

    PER_ZEROADDR_XCHECK( regs, b2 );

    regs->VR_F( v1, m3 ) = ARCH_DEP( vfetch4 )( effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E71A VSCEG  - Vector Scatter Element (64)                   [VRV] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_scatter_element_64 )
{
    int      v1, v2, b2, d2, m3;
    VADR     effective_addr2;

    VRV( inst, regs, v1, v2, b2, d2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 1)                    /* M3 > 3 => Specification excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    effective_addr2 = d2;

    if (b2)
        effective_addr2 += regs->GR( b2 );

    effective_addr2 += regs->VR_D( v2, m3 );
    effective_addr2 &= ADDRESS_MAXWRAP( regs );

    PER_ZEROADDR_XCHECK( regs, b2 );

    ARCH_DEP( vstore8 )( regs->VR_D( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E71B VSCEF  - Vector Scatter Element (32)                   [VRV] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_scatter_element_32 )
{
    int      v1, v2, b2, d2, m3;
    VADR     effective_addr2;

    VRV( inst, regs, v1, v2, b2, d2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 3)                    /* M3 > 3 => Specification excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    effective_addr2 = d2;

    if (b2)
        effective_addr2 += regs->GR( b2 );

    effective_addr2 += regs->VR_F( v2, m3 );
    effective_addr2 &= ADDRESS_MAXWRAP( regs );

    PER_ZEROADDR_XCHECK( regs, b2 );

    ARCH_DEP( vstore4 )( regs->VR_F( v1, m3 ), effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E721 VLGV   - Vector Load GR from VR Element              [VRS-c] */
/*-------------------------------------------------------------------*/
/*                                                                   */
/* In PoP (SA22-7832-13), for VLGV we can read:                      */
/*   If the index specified by the second-operand address is         */
/*   greater than the highest numbered element in the third          */
/*   operand, of the specified element size, the result in the       */
/*   first operand is unpredictable.                                 */
/*                                                                   */
/* However, empirical evidence suggests that any index larger than   */
/* the highest numbered element is treated as the modulo of the      */
/* highest numbered element. This may be model dependant behaviour,  */
/* but this implementation will follow a models (z15) behaviour.     */
/*                                                                   */
DEF_INST( vector_load_gr_from_vr_element )
{
    int     r1, v3, b2, m4;
    VADR    effective_addr2;
    int     i;

    VRS_C( inst, regs, r1, v3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    i = effective_addr2 & 0xFFF;  // Isolate the element index

    switch (m4)
    {
    case 0:  /* Byte */
        i %= 16;
        regs->GR( r1 ) = regs->VR_B( v3, i );
        break;
    case 1:  /* Halfword */
        i %= 8;
        regs->GR( r1 ) = regs->VR_H( v3, i );
        break;
    case 2:  /* Word */
        i %= 4;
        regs->GR( r1 ) = regs->VR_F( v3, i );
        break;
    case 3:  /* Doubleword */
        i %= 2;
        regs->GR( r1 ) = regs->VR_D( v3, i );
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E722 VLVG   - Vector Load VR Element from GR              [VRS-b] */
/*-------------------------------------------------------------------*/
/*                                                                   */
/* In PoP (SA22-7832-13), for VLVG we can read:                      */
/*   If the index, specified by the second-operand address, is       */
/*   greater than the highest numbered element in the first          */
/*   operand, of the specified element size, it is unpredictable     */
/*   which element, if any, is replaced.                             */
/*                                                                   */
/* However, empirical evidence suggests that any index larger than   */
/* the highest numbered element is treated as the modulo of the      */
/* highest numbered element. This may be model dependant behaviour,  */
/* but this implementation will follow a models (z15) behaviour.     */
/*                                                                   */
DEF_INST( vector_load_vr_element_from_gr )
{
    int     v1, r3, b2, m4;
    VADR    effective_addr2;
    int     i;

    VRS_B( inst, regs, v1, r3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    i = effective_addr2 & 0xFFF;  // Isolate the element index

    switch (m4)
    {
    case 0:  /* Byte */
        i %= 16;
        regs->VR_B( v1, i ) = regs->GR_LHLCL( r3 );
        break;
    case 1:  /* Halfword */
        i %= 8;
        regs->VR_H( v1, i ) = regs->GR_LHL  ( r3 );
        break;
    case 2:  /* Word */
        i %= 4;
        regs->VR_F( v1, i ) = regs->GR_L    ( r3 );
        break;
    case 3:  /* Doubleword */
        i %= 2;
        regs->VR_D( v1, i ) = regs->GR_G    ( r3 );
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E727 LCBB   - Load Count To Block Boundary                  [RXE] */
/*-------------------------------------------------------------------*/
DEF_INST( load_count_to_block_boundary )
{
    int     r1, x2, b2, m3;
    VADR    effective_addr2, boundary_addr;
    U64     boundary;
    U64     length;

    RXE_M3( inst, regs, r1, x2, b2, effective_addr2, m3 );

    PER_ZEROADDR_XCHECK2( regs, x2, b2 );

    if (m3 > 6)                    /* M3 > 6 => Specficitcation excp */
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    boundary = 64 << m3; /* 0: 64 Byte, 1: 128 Byte, 2: 256 Byte, 3: 512 Byte,
                            4: 1 K-byte, 5: 2 K-Byte, 6: 4 K-Byte */

    boundary_addr = (effective_addr2 + boundary) & ~(boundary - 1);

    length = boundary_addr - effective_addr2;
    if (length > 16) length = 16;

    regs->GR_L( r1 ) = (U32)length;
    regs->psw.cc = (length == 16) ? 0 : 3;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E730 VESL   - Vector Element Shift Left                   [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_left )
{
    int     v1, v3, b2, m4, shift, i;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    shift = effective_addr2 & 0xFFF;  // Isolate number of bit positions

    switch (m4)
    {
    case 0:
        shift %= 8;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sll_8( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B( v1, i ) = regs->VR_B( v3, i ) << shift;
        #endif
        break;
    case 1:
        shift %= 16;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sll_16( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H( v1, i ) = regs->VR_H( v3, i ) << shift;
        #endif
        break;
    case 2:
        shift %= 32;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sll_32( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F( v1, i ) = regs->VR_F( v3, i ) << shift;
        #endif
        break;
    case 3:
        shift %= 64;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sll_64( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D( v1, i ) = regs->VR_D( v3, i ) << shift;
        #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E733 VERLL  - Vector Element Rotate Left Logical          [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_rotate_left_logical )
{
    int     v1, v3, b2, m4, i, rotl, rotr;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    rotl = effective_addr2 & 0xFFF;  // Isolate number of bit positions

    switch (m4)
    {
    case 0:
        rotl %= 8;
        rotr = -rotl & 7;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_or( v128_sll_8( regs->VR_Q(v3).v, rotl ), v128_srl_8( regs->VR_Q(v3).v, rotr ) );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B( v1, i ) = (regs->VR_B( v3, i ) << rotl) | (regs->VR_B( v3, i ) >> rotr);
        #endif
        break;
    case 1:
        rotl %= 16;
        rotr = -rotl & 15;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_or( v128_sll_16( regs->VR_Q(v3).v, rotl ), v128_srl_16( regs->VR_Q(v3).v, rotr ) );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H( v1, i ) = (regs->VR_H( v3, i ) << rotl) | (regs->VR_H( v3, i ) >> rotr);
        #endif
        break;
    case 2:
        rotl %= 32;
        rotr = -rotl & 31;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_or( v128_sll_32( regs->VR_Q(v3).v, rotl ), v128_srl_32( regs->VR_Q(v3).v, rotr ) );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F( v1, i ) = (regs->VR_F( v3, i ) << rotl) | (regs->VR_F( v3, i ) >> rotr);
        #endif
        break;
    case 3:
        rotl %= 64;
        rotr = -rotl & 63;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_or( v128_sll_64( regs->VR_Q(v3).v, rotl ), v128_srl_64( regs->VR_Q(v3).v, rotr ) );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D( v1, i ) = (regs->VR_D( v3, i ) << rotl) | (regs->VR_D( v3, i ) >> rotr);
        #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E736 VLM    - Vector Load Multiple                        [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_multiple )
{
    int     v1, v3, b2, m4, i;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    /* m4 - Alignment Hint: not used */
    UNREFERENCED( m4 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK( regs, b2 );

    if (v3 < v1 || (v3 - v1 + 1) > 16)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    for (i=v1; i <= v3; i++)
    {
        regs->VR_Q( i ) = ARCH_DEP( vfetch16 )( effective_addr2, b2, regs );
        effective_addr2 += 16;
        effective_addr2 &= ADDRESS_MAXWRAP( regs );
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E737 VLL    - Vector Load With Length                     [VRS-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_with_length )
{
    int     v1, r3, b2, m4;
    VADR    effective_addr2;
    U32     length;
    QW      temp;

    VRS_B( inst, regs, v1, r3, b2, effective_addr2, m4 );

    /* m4 - Alignment Hint: not used */
    UNREFERENCED( m4 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK( regs, b2 );

    length = regs->GR_L(r3);
    if (length > 15) length = 15;

    memset(&temp, 0x00, sizeof(temp));

    ARCH_DEP( vfetchc )( &temp, length, effective_addr2, b2, regs );

    regs->VR_Q( v1 ) = CSWAP128( temp );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E738 VESRL  - Vector Element Shift Right Logical          [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_right_logical )
{
    int     v1, v3, b2, m4, i, shift;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    shift = effective_addr2 & 0xFFF;  // Isolate number of bit positions

    switch (m4)
    {
    case 0:
        shift %= 8;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_srl_8( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B( v1, i ) = regs->VR_B( v3, i ) >> shift;
        #endif
        break;
    case 1:
        shift %= 16;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_srl_16( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H( v1, i ) = regs->VR_H( v3, i ) >> shift;
        #endif
        break;
    case 2:
        shift %= 32;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_srl_32( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F( v1, i ) = regs->VR_F( v3, i ) >> shift;
        #endif
        break;
    case 3:
        shift %= 64;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_srl_64( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D( v1, i ) = regs->VR_D( v3, i ) >> shift;
        #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E73A VESRA  - Vector Element Shift Right Arithmetic       [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_right_arithmetic )
{
    int     v1, v3, b2, m4, shift, i;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    ZVECTOR_CHECK( regs );

    shift = effective_addr2 & 0xFFF;  // Isolate number of bit positions

    switch (m4)
    {
    case 0:
        shift %= 8;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sra_8( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B( v1, i ) = (S8) regs->VR_B( v3, i ) >> shift;
        #endif
        break;
    case 1:
        shift %= 16;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sra_16( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H( v1, i ) = (S16) regs->VR_H( v3, i ) >> shift;
        #endif
        break;
    case 2:
        shift %= 32;
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_sra_32( regs->VR_Q(v3).v, shift );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F( v1, i ) = (S32) regs->VR_F( v3, i ) >> shift;
        #endif
        break;
    case 3:
        shift %= 64;
        for (i=0; i < 2; i++)
            regs->VR_D( v1, i ) = (S64) regs->VR_D( v3, i ) >> shift;
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E73E VSTM   - Vector Store Multiple                       [VRS-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_multiple )
{
    int     v1, v3, b2, m4, i;
    VADR    effective_addr2;

    VRS_A( inst, regs, v1, v3, b2, effective_addr2, m4 );

    /* m4 - Alignment Hint: not used */
    UNREFERENCED( m4 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK( regs, b2 );

    if (v3 < v1 || (v3 - v1 + 1) > 16)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    for (i = v1; i <= v3; i++)
    {
        ARCH_DEP( vstore16 )( regs->VR_Q( i ), effective_addr2, b2, regs );
        effective_addr2 += 16;
        effective_addr2 &= ADDRESS_MAXWRAP( regs );
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E73F VSTL   - Vector Store With Length                    [VRS-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_store_with_length )
{
    int     v1, r3, b2, m4;
    VADR    effective_addr2;
    U32     length;
    QW      temp;

    VRS_B( inst, regs, v1, r3, b2, effective_addr2, m4 );

    /* m4 is not part of this instruction */
    UNREFERENCED( m4 );

    ZVECTOR_CHECK( regs );
    PER_ZEROADDR_XCHECK( regs, b2 );

    length = regs->GR_L(r3);
    if (length > 15) length = 15;

    temp = CSWAP128( regs->VR_Q( v1 ) );

    ARCH_DEP( vstorec )( &temp, length , effective_addr2, b2, regs );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E740 VLEIB  - Vector Load Element Immediate (8)           [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_immediate_8 )
{
    int     v1, i2, m3;

    VRI_A( inst, regs, v1, i2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 15)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    regs->VR_B(v1,m3) = i2 & 0xff;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E741 VLEIH  - Vector Load Element Immediate (16)          [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_immediate_16 )
{
    int     v1, i2, m3;

    VRI_A( inst, regs, v1, i2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 7)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    regs->VR_H(v1, m3) = i2;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E742 VLEIG  - Vector Load Element Immediate (64)          [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_immediate_64 )
{
    int     v1, i2, m3;

    VRI_A( inst, regs, v1, i2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 1)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    if (i2 & 0x8000)
        i2 |= 0xFFFF0000;

    regs->VR_D(v1, m3) = (S64) i2;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E743 VLEIF  - Vector Load Element Immediate (32)          [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_element_immediate_32 )
{
    int     v1, i2, m3;

    VRI_A( inst, regs, v1, i2, m3 );

    ZVECTOR_CHECK( regs );

    if (m3 > 3)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    if (i2 & 0x8000)
        i2 |= 0xFFFF0000;

    regs->VR_F(v1, m3) = i2;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E744 VGBM   - Vector Generate Byte Mask                   [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_generate_byte_mask )
{
    int     v1, i2, m3, i;

    VRI_A( inst, regs, v1, i2, m3 );

    /* m3 is not part of this instruction */
    UNREFERENCED( m3 );

    ZVECTOR_CHECK( regs );

    #if defined ( FEATURE_V128_SSE )
        /* remove GCC warnings and MSVC error */
        i = 0;
        UNREFERENCED( i );

        regs->VR_Q(v1).v = v128_bit_to_byte_mask ( i2 );

    #else

        for (i=0; i < 16; i++)
            regs->VR_B(v1, i) = (i2 & (0x1 << (15 - i))) ? 0xff : 0x00;

    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E745 VREPI  - Vector Replicate Immediate                  [VRI-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_replicate_immediate )
{
    int     v1, i2, m3, i;

    VRI_A( inst, regs, v1, i2, m3 );

    ZVECTOR_CHECK( regs );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    if (i2 & 0x8000)
        i2 |= 0xFFFF0000;

    switch (m3)
    {
        case 0:  /* Byte */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_8( (S8) i2 );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B(v1, i) = (S8) i2;
        #endif
            break;

        case 1:  /* Halfword */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_16( (S16) i2 );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H(v1, i) = (S16) i2;
        #endif
            break;

        case 2:  /* Word */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_32( (S32) i2 );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F(v1, i) = (S32) i2;
        #endif
            break;

        case 3:  /* Doubleword */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_64( (S64) i2 );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D(v1, i) = (S64) i2;
        #endif
            break;

        default:
            ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
            break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E746 VGM    - Vector Generate Mask                        [VRI-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_generate_mask )
{
    int     v1, i2, i3, m4, i;
    U64     bitmask;

    VRI_B( inst, regs, v1, i2, i3, m4 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
        i2 &= 7;
        i3 &= 7;
        if (i2 <= i3) {
            if (i2 == 0)
                bitmask = 0u - (1u << (7 - i3));
            else
                bitmask = (1u << (8 - i2)) - (1u << (7 - i3));
        } else {
            if (i2 == 0)
                bitmask = 0xFFu - (1u << (7 - i3));
            else
                bitmask = 0xFFu - (1u << (7 - i3)) + (1u << (8 - i2));
        }

        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_8( bitmask );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B(v1, i) = bitmask;
        #endif
        break;

    case 1:  /* Halfword */
        i2 &= 15;
        i3 &= 15;
        if (i2 <= i3) {
            if (i2 == 0)
                bitmask = 0u - (1u << (15 - i3));
            else
                bitmask = (1u << (16 - i2)) - (1u << (15 - i3));
        } else {
            if (i2 == 0)
                bitmask = 0xFFFFu - (1u << (15 - i3));
            else
                bitmask = 0xFFFFu - (1u << (15 - i3)) + (1u << (16 - i2));
        }

        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_16(bitmask );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H(v1, i) = bitmask;
        #endif
        break;

    case 2:  /* Word */
        i2 &= 31;
        i3 &= 31;
        if (i2 <= i3) {
            if (i2 == 0)
                bitmask = 0u - (1u << (31 - i3));
            else
                bitmask = (1u << (32 - i2)) - (1u << (31 - i3));
        } else {
            if (i2 == 0)
                bitmask = 0xFFFFFFFFu - (1u << (31 - i3));
            else
                bitmask = 0xFFFFFFFFu - (1u << (31 - i3)) + (1u << (32 - i2));
        }

        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_32( bitmask );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F(v1, i) = bitmask;
        #endif
            break;

    case 3:  /* Doubleword */
        i2 &= 63;
        i3 &= 63;
        if (i2 <= i3) {
            if (i2 == 0)
                bitmask = 0ull - (1ull << (63 - i3));
            else
                bitmask = (1ull << (64 - i2)) - (1ull << (63 - i3));
        } else {
            if (i2 == 0)
                bitmask = 0xFFFFFFFFFFFFFFFFull - (1ull << (63 - i3));
            else
                bitmask = 0xFFFFFFFFFFFFFFFFull - (1ull << (63 - i3)) + (1ull << (64 - i2));
        }

        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_64( bitmask );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D(v1, i) = bitmask;
        #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E74D VREP   - Vector Replicate                            [VRI-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_replicate )
{
    int     v1, v3, i2, m4, i;

    VRI_C( inst, regs, v1, v3, i2, m4 );

    ZVECTOR_CHECK( regs );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );

    if (i2 >= (16 >> m4))
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    switch (m4)
    {
    case 0:   /* Byte */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_8( regs->VR_B(v3, i2) );
        #else
            for (i=0; i < 16; i++)
                regs->VR_B(v1, i) = regs->VR_B(v3, i2);
        #endif
        break;

    case 1:  /* Halfword */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_16( regs->VR_H(v3, i2) );
        #else
            for (i=0; i < 8; i++)
                regs->VR_H(v1, i) = regs->VR_H(v3, i2);
        #endif
        break;

    case 2:  /* Word */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_32( regs->VR_F(v3, i2) );
        #else
            for (i=0; i < 4; i++)
                regs->VR_F(v1, i) = regs->VR_F(v3, i2);
        #endif
        break;

    case 3:  /* Doubleword */
        #if defined( FEATURE_V128_SSE )
            regs->VR_Q(v1).v = v128_replicate_64( regs->VR_D(v3, i2) );
        #else
            for (i=0; i < 2; i++)
                regs->VR_D(v1, i) = regs->VR_D(v3, i2);
        #endif

        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E750 VPOPCT - Vector Population Count                     [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_population_count )
{

    static const BYTE OneBitsInByte[256] =
    /*        -0  -1  -2  -3  -4  -5  -6  -7  -8  -9  -A  -B  -C  -D  -E  -F */
    /* 0- */ { 0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
    /* 1- */   1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    /* 2- */   1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    /* 3- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* 4- */   1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    /* 5- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* 6- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* 7- */   3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    /* 8- */   1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    /* 9- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* A- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* B- */   3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    /* C- */   2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    /* D- */   3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    /* E- */   3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    /* F- */   4,  5,  5,  6,  5,  6,  6,  7,  5,  6,  6,  7,  6,  7,  7,  8 };

    int     v1, v2, m3, m4, m5;
    int     i, j, count;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    if ( !FACILITY_ENABLED( 135_ZVECTOR_ENH_1, regs ) )
    {
        if ( m3 > 1 )
            ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
    }

    switch (m3)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            count = OneBitsInByte[regs->VR_B(v2, i)];
            regs->VR_B(v1, i) = count;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            count = 0;
            for (j=i*2; j < (i*2)+2; j++)
            {
                count += OneBitsInByte[regs->VR_B(v2, j)];
            }
            regs->VR_H(v1, i) = count;
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            count = 0;
            for (j=i*4; j < (i*4)+4; j++)
            {
                count += OneBitsInByte[regs->VR_B(v2, j)];
            }
            regs->VR_F(v1, i) = count;
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            count = 0;
            for (j=i*8; j < (i*8)+8; j++)
            {
                count += OneBitsInByte[regs->VR_B(v2, j)];
            }
            regs->VR_D(v1, i) = count;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E752 VCTZ   - Vector Count Trailing Zeros                 [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_count_trailing_zeros )
{

    static const BYTE TrailingZerosInByte[256] =
    /*        -0  -1  -2  -3  -4  -5  -6  -7  -8  -9  -A  -B  -C  -D  -E  -F */
    /* 0- */ { 8,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 1- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 2- */   5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 3- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 4- */   6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 5- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 6- */   5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 7- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 8- */   7,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* 9- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* A- */   5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* B- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* C- */   6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* D- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* E- */   5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
    /* F- */   4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0 };

    int     v1, v2, m3, m4, m5;
    int     i, j, k, count;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            count = TrailingZerosInByte[regs->VR_B(v2, i)];
            regs->VR_B(v1, i) = count;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            count = 0;
            for (j=(i*2)+1; j >= i*2; j--)
            {
                k = TrailingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_H(v1, i) = count;
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            count = 0;
            for (j=(i*4)+3; j >= i*4; j--)
            {
                k = TrailingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_F(v1, i) = count;
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            count = 0;
            for (j=(i*8)+7; j >= i*8; j--)
            {
                k = TrailingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_D(v1, i) = count;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E753 VCLZ   - Vector Count Leading Zeros                  [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_count_leading_zeros )
{

    static const BYTE LeadingZerosInByte[256] =
    /*        -0  -1  -2  -3  -4  -5  -6  -7  -8  -9  -A  -B  -C  -D  -E  -F */
    /* 0- */ { 8,  7,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,
    /* 1- */   3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    /* 2- */   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* 3- */   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* 4- */   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 5- */   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 6- */   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 7- */   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 8- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* 9- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* A- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* B- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* C- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* D- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* E- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /* F- */   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };

    int     v1, v2, m3, m4, m5;
    int     i, j, k, count;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            count = LeadingZerosInByte[regs->VR_B(v2, i)];
            regs->VR_B(v1, i) = count;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            count = 0;
            for (j=i*2; j < (i*2)+2; j++)
            {
                k = LeadingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_H(v1, i) = count;
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            count = 0;
            for (j=i*4; j < (i*4)+4; j++)
            {
                k = LeadingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_F(v1, i) = count;
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            count = 0;
            for (j=i*8; j < (i*8)+8; j++)
            {
                k = LeadingZerosInByte[regs->VR_B(v2, j)];
                count += k;
                if (k != 8) break;
            }
            regs->VR_D(v1, i) = count;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E756 VLR    - Vector Load Vector                          [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_vector )
{
    int     v1, v2, m3, m4, m5;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m3, m4, m5 are not part of this instruction */
    UNREFERENCED( m3 );
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    regs->VR_Q( v1 ) = regs->VR_Q( v2 );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E75C VISTR  - Vector Isolate String                       [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_isolate_string )
{
    int     v1, v2, m3, m4, m5;
    int     i;
    BYTE    newcc = 3;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4 is not part of this instruction */
    UNREFERENCED( m4 );

    ZVECTOR_CHECK( regs );

    // printf("VISTR enter: v1=%d, v2=%d, m3=%d, m5=%d\n", v1, v2, m3,m5);
    // u128_logmsg(" VISTR v2: ", (U128) regs->VR_Q(v2) );

#define M5_CS ((m5 & 0x1) != 0)  // Condition Code Set

    switch (m3)
    {
    case 0:  /* Byte */
        #if defined ( FEATURE_V128_SSE )
            i = v128_find_first_zero_in_vector_8 ( regs->VR_Q(v2).v );
            if ( i == 16 )
            {
                regs->VR_Q(v1).v = regs->VR_Q(v2).v;      /* no zero */
            }
            else
            {
                newcc = 0;
                regs->VR_Q(v1).v = v128_isolate_string_8( regs->VR_Q(v2).v );
            }
        #else
            for (i=0; i < 16; i++)
            {
                if (regs->VR_B(v2, i) != 0)
                {
                    regs->VR_B(v1, i) = regs->VR_B(v2, i);
                }
                else
                {
                    newcc = 0;
                    for (; i < 16; i++)
                    {
                        regs->VR_B(v1, i) = 0;
                    }
                    break;
                }
            }
        #endif
        break;

    case 1:  /* Halfword */
        #if defined ( FEATURE_V128_SSE )
            i = v128_find_first_zero_in_vector_16 ( regs->VR_Q(v2).v );
            if ( i == 8 )
            {
                regs->VR_Q(v1).v = regs->VR_Q(v2).v;    /* no zero */
            }
            else
            {
                newcc = 0;
                regs->VR_Q(v1).v = v128_isolate_string_16( regs->VR_Q(v2).v );
            }
        #else
            for (i=0; i < 8; i++)
            {
                if (regs->VR_H(v2, i) != 0)
                {
                    regs->VR_H(v1, i) = regs->VR_H(v2, i);
                }
                else
                {
                    newcc = 0;
                    for (; i < 8; i++)
                    {
                        regs->VR_H(v1, i) = 0;
                    }
                    break;
                }
            }
        #endif
        break;

    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            if (regs->VR_F(v2, i) != 0)
            {
                regs->VR_F(v1, i) = regs->VR_F(v2, i);
            }
            else
            {
                newcc = 0;
                for (; i < 4; i++)
                {
                    regs->VR_F(v1, i) = 0;
                }
                break;
            }
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    if (M5_CS)               // if M5_CS (Condition Code Set)
        regs->psw.cc = newcc;

#undef M5_CS

    // u128_logmsg(" VISTR v1: ", (U128) regs->VR_Q(v1) );
    // printf("VISTR end: newcc=%d\n",newcc);

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E75F VSEG   - Vector Sign Extend To Doubleword            [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_sign_extend_to_doubleword )
{
    int     v1, v2, m3, m4, m5;
    U64     element;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* remove GCC warnings and MSVC error */
    element = 0;
    UNREFERENCED( element );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sign_extend_8_64( regs->VR_Q(v2).v );
    #else
        element = regs->VR_B(v2, 7);
        if (element & 0x0000000000000080ull)
            element |= 0xFFFFFFFFFFFFFF00ull;
        regs->VR_D(v1, 0) = element;
        element = regs->VR_B(v2, 15);
        if (element & 0x0000000000000080ull)
            element |= 0xFFFFFFFFFFFFFF00ull;
        regs->VR_D(v1, 1) = element;
    #endif
        break;
    case 1:  /* Halfword */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sign_extend_16_64( regs->VR_Q(v2).v );
    #else
        element = regs->VR_H(v2, 3);
        if (element & 0x0000000000008000ull)
            element |= 0xFFFFFFFFFFFF0000ull;
        regs->VR_D(v1, 0) = element;
        element = regs->VR_H(v2, 7);
        if (element & 0x0000000000008000ull)
            element |= 0xFFFFFFFFFFFF0000ull;
        regs->VR_D(v1, 1) = element;
    #endif
        break;
    case 2:  /* Word */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sign_extend_32_64( regs->VR_Q(v2).v );
    #else
        element = regs->VR_F(v2, 1);
        if (element & 0x0000000080000000ull)
            element |= 0xFFFFFFFF00000000ull;
        regs->VR_D(v1, 0) = element;
        element = regs->VR_F(v2, 3);
        if (element & 0x0000000080000000ull)
            element |= 0xFFFFFFFF00000000ull;
        regs->VR_D(v1, 1) = element;
    #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E760 VMRL   - Vector Merge Low                            [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_merge_low )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );
    j = 0;
    UNREFERENCED( j );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_low_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=8; i<16; i+=2, j++ )
        {
            SV_B( temp, i   ) = regs->VR_B( v2, j );
            SV_B( temp, i+1 ) = regs->VR_B( v3, j );
        }
    #endif
        break;
    case 1:  /* Halfword */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_low_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=4; i<8; i+=2, j++ )
        {
            SV_H( temp, i   ) = regs->VR_H( v2, j );
            SV_H( temp, i+1 ) = regs->VR_H( v3, j );
        }
    #endif
        break;
    case 2:  /* Word */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_low_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=2; i<4; i+=2, j++ )
        {
            SV_F( temp, i   ) = regs->VR_F( v2, j );
            SV_F( temp, i+1 ) = regs->VR_F( v3, j );
        }
    #endif
        break;
    case 3:  /* Doubleword */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_low_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 1 );
        SV_D( temp, 1 ) = regs->VR_D( v3, 1 );
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    #if !defined ( FEATURE_V128_SSE )
        regs->VR_D( v1, 0 ) = SV_D( temp, 0 );
        regs->VR_D( v1, 1 ) = SV_D( temp, 1 );
    #endif

    ZVECTOR_END( regs );

}

/*-------------------------------------------------------------------*/
/* E761 VMRH   - Vector Merge High                           [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_merge_high )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );
    j = 0;
    UNREFERENCED( j );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=0; i<16; i+=2, j++ )
        {
            SV_B( temp, i   ) = regs->VR_B( v2, j );
            SV_B( temp, i+1 ) = regs->VR_B( v3, j );
        }
    #endif
        break;
    case 1:  /* Halfword */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=0; i<8; i+=2, j++ )
        {
            SV_H( temp, i   ) = regs->VR_H( v2, j );
            SV_H( temp, i+1 ) = regs->VR_H( v3, j );
        }
    #endif
        break;
    case 2:  /* Word */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i=0, j=0; i<4; i+=2, j++ )
        {
            SV_F( temp, i   ) = regs->VR_F( v2, j );
            SV_F( temp, i+1 ) = regs->VR_F( v3, j );
        }
    #endif
        break;
    case 3:  /* Doubleword */
    #if defined ( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_merge_high_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 1 ) = regs->VR_D( v3, 0 );
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    #if !defined ( FEATURE_V128_SSE )
        regs->VR_D( v1, 0 ) = SV_D( temp, 0 );
        regs->VR_D( v1, 1 ) = SV_D( temp, 1 );
    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E762 VLVGP  - Vector Load VR from GRs Disjoint            [VRR-f] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_vr_from_grs_disjoint )
{
    int     v1, r2, r3;

    VRR_F( inst, regs, v1, r2, r3 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->GR(r2);
    regs->VR_D(v1, 1) = regs->GR(r3);

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E764 VSUM   - Vector Sum Across Word                      [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_sum_across_word )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    U32     sum[4];

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );
    j = 0;
    UNREFERENCED( j );
    sum[0] = 0;
    UNREFERENCED( sum[0] );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sum_across_word_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for (i = 0, j = 0; i < 4; i++, j+=4)
        {
            sum[i] = 0;
            sum[i] += regs->VR_B(v2, j+0);
            sum[i] += regs->VR_B(v2, j+1);
            sum[i] += regs->VR_B(v2, j+2);
            sum[i] += regs->VR_B(v2, j+3);
            sum[i] += regs->VR_B(v3, j+3);
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sum_across_word_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for (i = 0, j = 0; i < 4; i++, j+=2)
        {
            sum[i] = 0;
            sum[i] += regs->VR_H(v2, j+0);
            sum[i] += regs->VR_H(v2, j+1);
            sum[i] += regs->VR_H(v3, j+1);
        }
    #endif
        break;

    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    #if !defined( FEATURE_V128_SSE )
        regs->VR_F(v1, 0) = sum[0];
        regs->VR_F(v1, 1) = sum[1];
        regs->VR_F(v1, 2) = sum[2];
        regs->VR_F(v1, 3) = sum[3];
    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E765 VSUMG  - Vector Sum Across Doubleword                [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_sum_across_doubleword )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    U64     sum[2];

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_sum_across_doubleword_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for (i = 0, j = 0; i < 2; i++, j+=4)
        {
            sum[i] = 0;
            sum[i] += regs->VR_H(v2, j+0);
            sum[i] += regs->VR_H(v2, j+1);
            sum[i] += regs->VR_H(v2, j+2);
            sum[i] += regs->VR_H(v2, j+3);
            sum[i] += regs->VR_H(v3, j+3);
        }

        regs->VR_D(v1, 0) = sum[0];
        regs->VR_D(v1, 1) = sum[1];
    #endif

        break;
    case 2:  /* Word */
        for (i = 0, j = 0; i < 2; i++, j+=2)
        {
            sum[i] = 0;
            sum[i] += regs->VR_F(v2, j+0);
            sum[i] += regs->VR_F(v2, j+1);
            sum[i] += regs->VR_F(v3, j+1);
        }

        regs->VR_D(v1, 0) = sum[0];
        regs->VR_D(v1, 1) = sum[1];
        break;

    default:
        /* remove initialization warning */
        sum[0] = 0;
        sum[1] = 0;

        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E766 VCKSM  - Vector Checksum                             [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_checksum )
{
    int     v1, v2, v3, m4, m5, m6;
    U64     ksum;
    U32     carry[2];
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    ksum = 0;
    carry[0] = carry[1] = 0;

    for (i = 0; i < 4; i++)
    {
        ksum += regs->VR_F(v2, i);
        carry[1] = ksum >> 32;
        if (carry[0] != carry[1])
            ksum += 1;
        carry[0] = carry[1];
    }

    ksum += regs->VR_F(v3, 1);
    carry[1] = ksum >> 32;
    if (carry[0] != carry[1])
        ksum += 1;

    regs->VR_F(v1, 0) = 0;
    regs->VR_F(v1, 1) = ksum;
    regs->VR_D(v1, 1) = 0;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E767 VSUMQ  - Vector Sum Across Quadword                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_sum_across_quadword )
{
    int     v1, v2, v3, m4, m5, m6;
    U64     high, low, add;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    high = low = add = 0;

    switch (m4)
    {
    case 2:  /* Word */
        for (i = 0; i < 4; i++)
        {
            add += regs->VR_F(v2, i);
        }
        add += regs->VR_F(v3, 3);
        break;
    case 3:  /* Doubleword */
        low = regs->VR_D(v2, 0);
        add = low + regs->VR_D(v2, 1);
        if (add < low) high++;
        low = add;
        add = low + regs->VR_D(v3, 1);
        if (add < low) high++;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    regs->VR_D(v1, 0) = high;
    regs->VR_D(v1, 1) = add;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E768 VN     - Vector AND                                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_and )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->VR_D(v2, 0) & regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = regs->VR_D(v2, 1) & regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E769 VNC    - Vector AND with Complement                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_and_with_complement )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->VR_D(v2, 0) & ~regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = regs->VR_D(v2, 1) & ~regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E76A VO     - Vector OR                                   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_or )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->VR_D(v2, 0) | regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = regs->VR_D(v2, 1) | regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E76B VNO    - Vector NOR                                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_nor )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = ~regs->VR_D(v2, 0) & ~regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = ~regs->VR_D(v2, 1) & ~regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}

#if defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 )
/*-------------------------------------------------------------------*/
/* E76C VNX    - Vector Not Exclusive OR                     [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_not_exclusive_or )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = ~(regs->VR_D(v2, 0) ^ regs->VR_D(v3, 0));
    regs->VR_D(v1, 1) = ~(regs->VR_D(v2, 1) ^ regs->VR_D(v3, 1));

    ZVECTOR_END( regs );
}
#endif /* defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 ) */

/*-------------------------------------------------------------------*/
/* E76D VX     - Vector Exclusive OR                         [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_exclusive_or )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->VR_D(v2, 0) ^ regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = regs->VR_D(v2, 1) ^ regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}

#if defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 )
/*-------------------------------------------------------------------*/
/* E76E VNN    - Vector NAND                                 [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_nand )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = ~regs->VR_D(v2, 0) | ~regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = ~regs->VR_D(v2, 1) | ~regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}
#endif /* defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 ) */

#if defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 )
/*-------------------------------------------------------------------*/
/* E76F VOC    - Vector OR with Complement                   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_or_with_complement )
{
    int     v1, v2, v3, m4, m5, m6;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 0) = regs->VR_D(v2, 0) | ~regs->VR_D(v3, 0);
    regs->VR_D(v1, 1) = regs->VR_D(v2, 1) | ~regs->VR_D(v3, 1);

    ZVECTOR_END( regs );
}
#endif /* defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 ) */

/*-------------------------------------------------------------------*/
/* E770 VESLV  - Vector Element Shift Left Vector            [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_left_vector )
{
    int     v1, v2, v3, m4, m5, m6, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:

    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_shift_left_vector_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v  );
    #else
        for (i=0; i < 16; i++)
            regs->VR_B(v1, i) = regs->VR_B(v2, i) << (regs->VR_B(v3, i) % 8);
    #endif
        break;

    case 1:
        for (i=0; i < 8; i++)
            regs->VR_H(v1, i) = regs->VR_H(v2, i) << (regs->VR_H(v3, i) % 16);
        break;

    case 2:
        for (i=0; i < 4; i++)
            regs->VR_F(v1, i) = regs->VR_F(v2, i) << (regs->VR_F(v3, i) % 32);
        break;

    case 3:
        for (i=0; i < 2; i++)
            regs->VR_D(v1, i) = regs->VR_D(v2, i) << (regs->VR_D(v3, i) % 64);
        break
        ;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E772 VERIM  - Vector Element Rotate and Insert Under Mask [VRI-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_rotate_and_insert_under_mask )
{
    int     v1, v2, v3, i4, m5;
    int     sl, sr;
    union   { U64 d[2]; U32 f[2]; U16 h[2]; BYTE b[2]; } temp;
    int     i;

    VRI_D( inst, regs, v1, v2, v3, i4, m5 );

    /* remove GCC warnings and MSVC error */
    i = 0;
    UNREFERENCED( i );
    sl = 0;
    UNREFERENCED( sl );
    sr = 0;
    UNREFERENCED( sr );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_rotate_and_insert_under_mask_8( regs->VR_Q(v1).v, regs->VR_Q(v2).v, regs->VR_Q(v3).v, i4 );
    #else
        for (i=0; i < 16; i++)
        {
            sl = i4 % 8;
            sr = 8 - sl;
            temp.b[1] = ((regs->VR_B( v2, i ) << sl) | (regs->VR_B( v2, i ) >> sr)) & regs->VR_B( v3, i );
            temp.b[0] = regs->VR_B( v1, i ) & (~regs->VR_B( v3, i ));
            regs->VR_B( v1, i ) = temp.b[0] | temp.b[1];
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_rotate_and_insert_under_mask_16( regs->VR_Q(v1).v, regs->VR_Q(v2).v, regs->VR_Q(v3).v, i4 );
    #else
        for (i=0; i < 8; i++)
        {
            sl = i4 % 16;
            sr = 16 - sl;
            temp.h[1] = ((regs->VR_H( v2, i ) << sl) | (regs->VR_H( v2, i ) >> sr)) & regs->VR_H( v3, i );
            temp.h[0] = regs->VR_H( v1, i ) & (~regs->VR_H( v3, i ));
            regs->VR_H( v1, i ) = temp.h[0] | temp.h[1];
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_rotate_and_insert_under_mask_32( regs->VR_Q(v1).v, regs->VR_Q(v2).v, regs->VR_Q(v3).v, i4 );
    #else
        for (i=0; i < 4; i++)
        {
            sl = i4 % 32;
            sr = 32 - sl;
            temp.f[1] = ((regs->VR_F( v2, i ) << sl) | (regs->VR_F( v2, i ) >> sr)) & regs->VR_F( v3, i );
            temp.f[0] = regs->VR_F( v1, i ) & (~regs->VR_F( v3, i ));
            regs->VR_F( v1, i ) = temp.f[0] | temp.f[1];
        }
    #endif
        break;

    case 3:  /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_rotate_and_insert_under_mask_64( regs->VR_Q(v1).v, regs->VR_Q(v2).v, regs->VR_Q(v3).v, i4 );
    #else
        for (i=0; i < 2; i++)
        {
            sl = i4 % 64;
            sr = 64 - sl;
            temp.d[1] = ((regs->VR_D( v2, i ) << sl) | (regs->VR_D( v2, i ) >> sr)) & regs->VR_D( v3, i );
            temp.d[0] = regs->VR_D( v1, i ) & (~regs->VR_D( v3, i ));
            regs->VR_D( v1, i ) = temp.d[0] | temp.d[1];
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E773 VERLLV - Vector Element Rotate Left Logical Vector   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_rotate_left_logical_vector )
{
    int     v1, v2, v3, m4, m5, m6;
    int     sl, sr, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            sl = regs->VR_B( v3, i ) % 8;
            sr = 8 - sl;
            regs->VR_B( v1, i ) = (regs->VR_B( v2, i ) << sl) | (regs->VR_B( v2, i ) >> sr);
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            sl = regs->VR_H( v3, i ) % 16;
            sr = 16 - sl;
            regs->VR_H( v1, i ) = (regs->VR_H( v2, i ) << sl) | (regs->VR_H( v2, i ) >> sr);
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            sl = regs->VR_F( v3, i ) % 32;
            sr = 32 - sl;
            regs->VR_F( v1, i ) = (regs->VR_F( v2, i ) << sl) | (regs->VR_F( v2, i ) >> sr);
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            sl = regs->VR_D( v3, i ) % 64;
            sr = 64 - sl;
            regs->VR_D( v1, i ) = (regs->VR_D( v2, i ) << sl) | (regs->VR_D( v2, i ) >> sr);
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E774 VSL    - Vector Shift Left                           [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_left )
{
    int     v1, v2, v3, m4, m5, m6;
    int     sl, sr, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    for (i = 0 ; ; i++)
    {
        sl = regs->VR_B( v3, i ) & 0x07;
        if (i == 15)
        {
            regs->VR_B( v1, i ) = regs->VR_B( v2, i ) << sl;
            break;
        }
        sr = 8 - sl;
        regs->VR_B( v1, i ) = (regs->VR_B( v2, i ) << sl) | (regs->VR_B( v2, i+1 ) >> sr);
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E775 VSLB   - Vector Shift Left By Byte                   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_left_by_byte )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    j = (regs->VR_B( v3, 7 ) & 0x78) >> 3;

    #if defined ( FEATURE_V128_SSE )
        i = 0;
        UNREFERENCED( i );
        temp.d[0] = 0;
        UNREFERENCED( temp.d[0] );

        regs->VR_Q(v1).v = v128_shift_left_byte( regs->VR_Q(v2).v, j );

    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
        SV_D( temp, 2 ) = 0;
        SV_D( temp, 3 ) = 0;

        for (i = 0; i < 16; i++, j++)
            regs->VR_B(v1, i) = SV_B( temp, j );

    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E777 VSLDB  - Vector Shift Left Double By Byte            [VRI-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_left_double_by_byte )
{
    int     v1, v2, v3, i4, m5;
    int     i, j;
    SV      temp;

    VRI_D( inst, regs, v1, v2, v3, i4, m5 );

    /* m5 is not part of this instruction */
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    #if defined ( FEATURE_V128_SSE )
        i = 0;
        UNREFERENCED( i );
        j = 0;
        UNREFERENCED( j );
        temp.d[0] = 0;
        UNREFERENCED( temp.d[0] );

        regs->VR_Q(v1).v = v128_shift_left_double_byte( regs->VR_Q(v2).v, regs->VR_Q(v3).v, i4 );

    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
        SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
        SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

        for (i = 0, j = i4; i < 16; i++, j++)
            regs->VR_B(v1, i) = SV_B( temp, j );

    #endif

        ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E778 VESRLV - Vector Element Shift Right Logical Vector   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_right_logical_vector )
{
    int     v1, v2, v3, m4, m5, m6;
    int     shift, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            shift = regs->VR_B( v3, i ) % 8;
            regs->VR_B( v1, i ) = regs->VR_B( v2, i ) >> shift;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            shift = regs->VR_H( v3, i ) % 16;
            regs->VR_H( v1, i ) = regs->VR_H( v2, i ) >> shift;
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            shift = regs->VR_F( v3, i ) % 32;
            regs->VR_F( v1, i ) = regs->VR_F( v2, i ) >> shift;
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            shift = regs->VR_D( v3, i ) % 64;
            regs->VR_D( v1, i ) = regs->VR_D( v2, i ) >> shift;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E77A VESRAV - Vector Element Shift Right Arithmetic Vector[VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_shift_right_arithmetic_vector )
{
    int     v1, v2, v3, m4, m5, m6;
    int     shift, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
        for (i=0; i < 16; i++)
        {
            shift = regs->VR_B( v3, i ) % 8;
            regs->VR_B( v1, i ) = (S8) regs->VR_B( v2, i ) >> shift;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i < 8; i++)
        {
            shift = regs->VR_H( v3, i ) % 16;
            regs->VR_H( v1, i ) = (S16) regs->VR_H( v2, i ) >> shift;
        }
        break;
    case 2:  /* Word */
        for (i=0; i < 4; i++)
        {
            shift = regs->VR_F( v3, i ) % 32;
            regs->VR_F( v1, i ) = (S32) regs->VR_F( v2, i ) >> shift;
        }
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            shift = regs->VR_D( v3, i ) % 64;
            regs->VR_D( v1, i ) = (S64) regs->VR_D( v2, i ) >> shift;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E77C VSRL   - Vector Shift Right Logical                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_right_logical )
{
    int     v1, v2, v3, m4, m5, m6;
    int     sr, sl, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    for (i = 15 ; ; i--)
    {
        sr = regs->VR_B( v3, i ) & 0x07;
        if (i == 0)
        {
            regs->VR_B( v1, i ) = regs->VR_B( v2, i ) >> sr;
            break;
        }
        sl = 8 - sr;
        regs->VR_B( v1, i ) = (regs->VR_B( v2, i ) >> sr) | (regs->VR_B( v2, i-1 ) << sl);
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E77D VSRLB  - Vector Shift Right Logical By Byte          [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_right_logical_by_byte )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    #if defined ( FEATURE_V128_SSE )
        i = 0;
        UNREFERENCED( i );
        temp.d[0] = 0;
        UNREFERENCED( temp.d[0] );

        j = (regs->VR_B( v3, 7 ) & 0x78) >> 3;
        regs->VR_Q(v1).v = v128_shift_right_logical_byte( regs->VR_Q(v2).v, j );

    #else
        j = 16 - ((regs->VR_B( v3, 7 ) & 0x78) >> 3);

        SV_D( temp, 0 ) = 0;
        SV_D( temp, 1 ) = 0;
        SV_D( temp, 2 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 3 ) = regs->VR_D( v2, 1 );

        for (i = 0; i < 16; i++, j++)
            regs->VR_B(v1, i) = SV_B( temp, j );

    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E77E VSRA   - Vector Shift Right Arithmetic               [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_right_arithmetic )
{
    int     v1, v2, v3, m4, m5, m6;
    int     sr, sl, i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    for (i = 15 ; ; i--)
    {
        sr = regs->VR_B( v3, i ) & 0x07;
        if (i == 0)
        {
            regs->VR_B( v1, i ) = (S8)regs->VR_B( v2, i ) >> sr;
            break;
        }
        sl = 8 - sr;
        regs->VR_B( v1, i ) = (regs->VR_B( v2, i ) >> sr) | (regs->VR_B( v2, i-1 ) << sl);
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E77F VSRAB  - Vector Shift Right Arithmetic By Byte       [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_right_arithmetic_by_byte )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    j = 16 - ((regs->VR_B( v3, 7 ) & 0x78) >> 3);

    #if defined ( FEATURE_V128_SSE )
        i = 0;
        UNREFERENCED( i );
        temp.d[0] =0;
        UNREFERENCED( temp.d[0] );

        j = ((regs->VR_B( v3, 7 ) & 0x78) >> 3);
        regs->VR_Q(v1).v = v128_shift_right_arithmetic_byte( regs->VR_Q(v2).v, j );

    #else
        j = 16 - ((regs->VR_B( v3, 7 ) & 0x78) >> 3);

        if (regs->VR_B( v2, 0 ) & 0x80)
        {
            SV_D( temp, 0 ) = 0xFFFFFFFFFFFFFFFFull;
            SV_D( temp, 1 ) = 0xFFFFFFFFFFFFFFFFull;
        }
        else
        {
            SV_D( temp, 0 ) = 0;
            SV_D( temp, 1 ) = 0;
        }
        SV_D( temp, 2 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 3 ) = regs->VR_D( v2, 1 );

        for (i = 0; i < 16; i++, j++)
            regs->VR_B(v1, i) = SV_B( temp, j );
    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E780 VFEE   - Vector Find Element Equal                   [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_find_element_equal )
{
    int     v1, v2, v3, m4, m5;
    int     ef, ei, zf, zi, i;
    BYTE    newcc;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    ZVECTOR_CHECK( regs );

#define M5_RE ((m5 & 0xc) != 0) // Reserved
#define M5_ZS ((m5 & 0x2) != 0) // Zero Search
#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    if (m4 > 2 || M5_RE)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    zf = ef = FALSE;
    zi = ei = 16;     // Number of bytes in vector

    switch (m4)
    {
    case 0:  /* Byte */
        #if defined ( FEATURE_V128_SSE )
            ei = v128_find_first_equal_8 ( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
            if (ei != 16) ef = TRUE;
            if (M5_ZS)
            {
                zi = v128_find_first_zero_in_vector_8 ( regs->VR_Q(v2).v );
                if (zi != 16) zf = TRUE;
            }
        #else
            for (i=0; i<16; i++)
            {
                if (regs->VR_B(v2,i) == regs->VR_B(v3,i))
                {
                    ef = TRUE;
                    ei = i;     // Element index in bytes
                    break;
                }
            }
            if (M5_ZS)
            {
                for (i=0; i<16; i++)
                {
                    if (regs->VR_B(v2,i) == 0)
                    {
                        zf = TRUE;
                        zi = i;      // Zero element index in bytes
                        break;
                    }
                }
            }
        #endif
        break;
    case 1:  /* Halfword */
        #if defined ( FEATURE_V128_SSE )
            ei = 2 * v128_find_first_equal_16 ( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
            if (ei != 16) ef = TRUE;
            if (M5_ZS)
            {
                zi = 2 * v128_find_first_zero_in_vector_16 ( regs->VR_Q(v2).v );
                if (zi != 16)  zf = TRUE;
            }
        #else
            for (i=0; i<8; i++)
            {
                if (regs->VR_H(v2,i) == regs->VR_H(v3,i))
                {
                    ef = TRUE;
                    ei = i * 2;  // Element index in bytes
                    break;
                }
            }
            if (M5_ZS)
            {
                for (i=0; i<8; i++)
                {
                    if (regs->VR_H(v2,i) == 0)
                    {
                        zf = TRUE;
                        zi = i * 2;  // Zero element index in bytes
                        break;
                    }
                }
            }
        #endif
        break;
    case 2:  /* Word */
        for (i=0; i<4; i++)
        {
            if (regs->VR_F(v2,i) == regs->VR_F(v3,i))
            {
                ef = TRUE;
                ei = i * 4;  // Element index in bytes
                break;
            }
        }
        if (M5_ZS)
        {
            for (i=0; i<4; i++)
            {
                if (regs->VR_F(v2,i) == 0)
                {
                    zf = TRUE;
                    zi = i * 4;  // Zero element index in bytes
                    break;
                }
            }
        }
        break;
    }

    if (ef == TRUE)
    {
        if (zf == TRUE)
        {
            if (zi <= ei)
            {
                newcc = 0;   // Equal element follows zero element, or equal element is zero element
                ei = zi;     // Element index in bytes
            }
            else
            {
                newcc = 2;   // Equal element before zero element
            }
        }
        else /* zf == FALSE */
        {
            newcc = 1;       // Equal element and, if M5_ZS, no zero element
        }
    }
    else  /* ef == FALSE */
    {
        if (zf == TRUE)
        {
            newcc = 0;       // No equal element and a zero element
            ei = zi;         // Element index in bytes
        }
        else /* zf == FALSE */
        {
            newcc = 3;       // No equal element and, if M5_ZS, no zero element
        }
    }

    regs->VR_D(v1, 0) = ei;
    regs->VR_D(v1, 1) = 0;

    if (M5_CS)
        regs->psw.cc = newcc;

#undef M5_RE
#undef M5_ZS
#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E781 VFENE  - Vector Find Element Not Equal               [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_find_element_not_equal )
{
    int     v1, v2, v3, m4, m5;
    int     nef, nei, zf, zi, i;
    BYTE    newcc;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    ZVECTOR_CHECK( regs );

#define M5_RE ((m5 & 0xc) != 0) // Reserved
#define M5_ZS ((m5 & 0x2) != 0) // Zero Search
#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    if (m4 > 2 || M5_RE)
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);

    zf = nef = FALSE;
    zi = nei = 16;  // Number of bytes in vector
    newcc = 3;  // All equal, no zero

    switch (m4)
    {
    case 0:  /* Byte */
        #if defined ( FEATURE_V128_SSE )
            nei = v128_find_first_not_equal_8 ( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
            if (nei != 16)
            {
                nef = TRUE;
                newcc = (regs->VR_B(v2, nei) < regs->VR_B(v3, nei)) ? 1 : 2;
            }
            if (M5_ZS)
            {
                zi = v128_find_first_zero_in_vector_8 ( regs->VR_Q(v2).v );
                if (zi != 16) zf = TRUE;
            }

            // u128_logmsg(" VFENE byte v2: ", (U128) regs->VR_Q(v2) );
            // u128_logmsg(" VFENE byte v3: ", (U128) regs->VR_Q(v3) );
            // printf(" VFENE byte: nei=%d, nef=%d, zi=%d, zf=%d\n", nei, nef, zi, zf);

        #else
            for (i=0; i<16; i++)
            {
                if (regs->VR_B(v2,i) != regs->VR_B(v3,i))
                {
                    nef = TRUE;
                    nei = i;     // Element index in bytes
                    newcc = (regs->VR_B(v2,i) < regs->VR_B(v3,i)) ? 1 : 2;
                    break;
                }
            }
            if (M5_ZS)
            {
                for (i=0; i<16; i++)
                {
                    if (regs->VR_B(v2,i) == 0)
                    {
                        zf = TRUE;
                        zi = i;      // Zero element index in bytes
                        break;
                    }
                }
            }
        #endif
        break;
    case 1:  /* Halfword */
        #if defined ( FEATURE_V128_SSE )
            nei = v128_find_first_not_equal_16 ( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
            if (nei != 8)
            {
                nef = TRUE;
                newcc = (regs->VR_H(v2, nei) < regs->VR_H(v3, nei)) ? 1 : 2;
            }
            nei = nei * 2;  // Element index in bytes
            if (M5_ZS)
            {
                zi = v128_find_first_zero_in_vector_16 ( regs->VR_Q(v2).v );
                if (zi != 8)
                {
                     zf = TRUE;
                }
                zi = zi * 2;  // Zero element index in bytes
            }

            // u128_logmsg(" VFENE hw v2: ", (U128) regs->VR_Q(v2) );
            // u128_logmsg(" VFENE hw v3: ", (U128) regs->VR_Q(v3) );
            // printf(" VFENE hw: nei=%d, nef=%d, zi=%d, zf=%d\n", nei, nef, zi, zf);

        #else
            for (i=0; i<8; i++)
            {
                if (regs->VR_H(v2,i) != regs->VR_H(v3,i))
                {
                    nef = TRUE;
                    nei = i * 2;  // Element index in bytes
                    newcc = (regs->VR_H(v2,i) < regs->VR_H(v3,i)) ? 1 : 2;
                    break;
                }
            }
            if (M5_ZS)
            {
                for (i=0; i<8; i++)
                {
                    if (regs->VR_H(v2,i) == 0)
                    {
                        zf = TRUE;
                        zi = i * 2;  // Zero element index in bytes
                        break;
                    }
                }
            }
        #endif
        break;
    case 2:  /* Word */
        for (i=0; i<4; i++)
        {
            if (regs->VR_F(v2,i) != regs->VR_F(v3,i))
            {
                nef = TRUE;
                nei = i * 4;  // Element index in bytes
                newcc = (regs->VR_F(v2,i) < regs->VR_F(v3,i)) ? 1 : 2;
                break;
            }
        }
        if (M5_ZS)
        {
            for (i=0; i<4; i++)
            {
                if (regs->VR_F(v2,i) == 0)
                {
                    zf = TRUE;
                    zi = i * 4;  // Zero element index in bytes
                    break;
                }
            }
        }
        break;
    }

    if (nef == TRUE)
    {
        if (zf == TRUE)
        {
            if (zi < nei)
            {
                newcc = 0;   // Not equal element follows zero element
                nei = zi;    // Element index in bytes
            }
            else
            {
             /* newcc = 1 or 2; */     // Not equal element before zero element, or not equal element is zero element
            }
        }
        else /* zf == FALSE */
        {
         /* newcc = 1 or 2; */         // Not equal element and, if M5_ZS, no zero element
        }
    }
    else  /* nef == FALSE */
    {
        if (zf == TRUE)
        {
            newcc = 0;       // No not equal (i.e. all equal) element and a zero element
            nei = zi;        // Element index in bytes
        }
        else /* zf == FALSE */
        {
         /* newcc = 3; */    // No not equal (i.e. all equal) element and, if M5_ZS, no zero element
        }
    }

    regs->VR_D(v1, 0) = nei;
    regs->VR_D(v1, 1) = 0;

    if (M5_CS)
        regs->psw.cc = newcc;

#undef M5_RE
#undef M5_ZS
#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E782 VFAE   - Vector Find Any Element Equal               [VRR-b] */
/*-------------------------------------------------------------------*/
/* In PoP (SA22-7832-13), for VFAE & VFEE we can read:
"Programming Notes:
1. If the RT flag is zero, a byte index is always
stored into the first operand for any element size.
For example, if the specified element size is halfword
and the 2nd indexed halfword compared
equal, a byte index of 4 would be stored."

But I think that 4 would be a 2.
The 2nd Half = 1 (index byte) x 2 (length of H) = 2.

After 35 years, I must say this is the first typo I found in POP.
Sent to IBM, they will reformulate the sentence.

salva - 2023, feb,27.
*/
#if defined ( FEATURE_V128_SSE )
/* ================================================================= */
/* Intrinsic version of                                              */
/* E782 VFAE   - Vector Find Any Element Equal               [VRR-b] */
/* ================================================================= */

DEF_INST( vector_find_any_element_equal )
{

    int     v1, v2, v3, m4, m5;
    int     lxt1, lxt2;                // Lowest indexed true
    int     mxt;                       // Maximum indexed true
    QW      irt1, irt2;                // First and second intermediate results

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    ZVECTOR_CHECK( regs );

#define M5_IN ((m5 & 0x8) != 0) // Invert Result
#define M5_RT ((m5 & 0x4) != 0) // Result Type
#define M5_ZS ((m5 & 0x2) != 0) // Zero Search
#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    irt1.v = v128_zero();
    irt2.v = v128_zero();

    switch (m4)
    {
    case 0:  // Byte
        // Compare the element of the second with the elements of the third operands
        irt1.v = v128_find_any_equal_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        if (M5_IN) irt1.v = v128_not( irt1.v );
        if (M5_ZS) irt2.v = v128_find_equal_8( regs->VR_Q(v2).v, v128_zero() );
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            regs->VR_Q(v1) = irt1;
        }
        else                           // else !M5_RT
        {
            lxt1 = v128_find_first_in_vector_8 ( irt1.v, 0xFF);
            lxt2 = v128_find_first_in_vector_8 ( irt2.v, 0xFF);

            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 1:  // Halfword
        // Compare the element of the second with the elements of the third operands
        irt1.v = v128_find_any_equal_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        if (M5_IN) irt1.v = v128_not( irt1.v );
        if (M5_ZS) irt2.v = v128_find_equal_16( regs->VR_Q(v2).v, v128_zero() );
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            regs->VR_Q(v1) = irt1;
        }
        else                           // else !M5_RT
        {
            lxt1 = 2 * v128_find_first_in_vector_16 ( irt1.v, 0xFFFF);
            lxt2 = 2 * v128_find_first_in_vector_16 ( irt2.v, 0xFFFF);

            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 2:  // Word
        // Compare the element of the second with the elements of the third operands
        irt1.v = v128_find_any_equal_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        if (M5_IN) irt1.v = v128_not( irt1.v );
        if (M5_ZS) irt2.v = v128_find_equal_32( regs->VR_Q(v2).v, v128_zero() );
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            regs->VR_Q(v1) = irt1;
        }
        else                           // else !M5_RT
        {
            lxt1 = 4 * v128_find_first_in_vector_32 ( irt1.v, 0xFFFFFFFF);
            lxt2 = 4 * v128_find_first_in_vector_32 ( irt2.v, 0xFFFFFFFF);

            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    if (M5_CS)                         // if M5_CS (Condition Code Set)
    {
        switch (m4)
        {
        case 0:  // Byte
            lxt2 = mxt = 16;
            lxt1 = v128_find_first_in_vector_8 ( irt1.v, 0xFF);
            if (M5_ZS) lxt2 = v128_find_first_in_vector_8 ( irt2.v, 0xFF);
            break;
        case 1:  // Halfword
            lxt2 = mxt = 8;
            lxt1 = v128_find_first_in_vector_16 ( irt1.v, 0xFFFF);
            if (M5_ZS)  lxt2 = v128_find_first_in_vector_16 ( irt2.v, 0xFFFF);
            break;
        case 2:  // Word
            lxt2 = mxt = 4;
            lxt1 = v128_find_first_in_vector_32 ( irt1.v, 0xFFFFFFFF);
            if (M5_ZS)  lxt2 = v128_find_first_in_vector_32 ( irt2.v, 0xFFFFFFFF);
            break;
        default:  // Prevent erroneous "may be used uninitialized" warnings
            lxt1 = lxt2 = mxt = 0;
            break;
        }

        // cc 1 and 3 are possible when M5_ZS is 0 or 1.
        if (lxt1 == mxt && lxt2 == mxt)
            regs->psw.cc = 3;
        else if (lxt1 < mxt && lxt2 == mxt )
            regs->psw.cc = 1;
        // cc 0 and 2 are only possible when M5_ZS is 1.
        else if (lxt1 < lxt2)
            regs->psw.cc = 2;
        else
            regs->psw.cc = 0;
    }

#undef M5_IN
#undef M5_RT
#undef M5_ZS
#undef M5_CS

    ZVECTOR_END( regs );
}

#else
/* ================================================================= */
/* Portable C of                                                     */
/* E782 VFAE   - Vector Find Any Element Equal               [VRR-b] */
/* ================================================================= */

DEF_INST( vector_find_any_element_equal )
{

    int     v1, v2, v3, m4, m5;
    int     i, j;
    int     lxt1, lxt2;                // Lowest indexed true
    int     mxt;                       // Maximum indexed true
    BYTE    irt1[16], irt2[16];        // First and second intermediate results

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    ZVECTOR_CHECK( regs );

#define M5_IN ((m5 & 0x8) != 0) // Invert Result
#define M5_RT ((m5 & 0x4) != 0) // Result Type
#define M5_ZS ((m5 & 0x2) != 0) // Zero Search
#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    for (i=0; i<16; i++)
    {
        irt1[i] = irt2[i] = FALSE;
    }

    switch (m4)
    {
    case 0:  /* Byte */
        for (i=0; i<16; i++)
        {
            // Compare the element of the second with the elements of the third operands
            for (j=0; j<16; j++)
            {
                if (regs->VR_B(v2,i) == regs->VR_B(v3,j))
                {
                    irt1[i] = TRUE;
                    break;
                }
            }
            // Invert the result if required
            if (M5_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M5_ZS && regs->VR_B(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            for (i=0; i<16; i++)
            {
                regs->VR_B(v1, i) = (irt1[i] == TRUE) ? 0xFF : 0x00;
            }
        }
        else                           // else !M5_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<16; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16)
                    lxt1 = i;
                if (irt2[i] == TRUE && lxt2 == 16)
                    lxt2 = i;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i<8; i++)
        {
            // Compare the element of the second with the elements of the third operands
            for (j=0; j<8; j++)
            {
                if (regs->VR_H(v2,i) == regs->VR_H(v3,j))
                {
                    irt1[i] = TRUE;
                    break;
                }
            }
            // Invert the result if required
            if (M5_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M5_ZS && regs->VR_H(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            for (i=0; i<8; i++)
            {
                regs->VR_H(v1, i) = (irt1[i] == TRUE) ? 0xFFFF : 0x0000;
            }
        }
        else                           // else !M5_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<8; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16)
                    lxt1 = i * 2;
                if (irt2[i] == TRUE && lxt2 == 16)
                    lxt2 = i * 2;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 2:  /* Word */
        for (i=0; i<4; i++)
        {
            // Compare the element of the second with the elements of the third operands
            for (j=0; j<4; j++)
            {
                if (regs->VR_F(v2,i) == regs->VR_F(v3,j))
                {
                    irt1[i] = TRUE;
                    break;
                }
            }
            // Invert the result if required
            if (M5_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M5_ZS && regs->VR_F(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M5_RT)                     // if M5_RT (Result Type)
        {
            for (i=0; i<4; i++)
            {
                regs->VR_F(v1, i) = (irt1[i] == TRUE) ? 0xFFFFFFFF : 0x00000000;
            }
        }
        else                           // else !M5_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<4; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16)
                    lxt1 = i * 4;
                if (irt2[i] == TRUE && lxt2 == 16)
                    lxt2 = i * 4;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    if (M5_CS)                         // if M5_CS (Condition Code Set)
    {
        switch (m4)
        {
        case 0:  /* Byte */
            lxt1 = lxt2 = mxt = 16;
            break;
        case 1:  /* Halfword */
            lxt1 = lxt2 = mxt = 8;
            break;
        case 2:  /* Word */
            lxt1 = lxt2 = mxt = 4;
            break;
        default:  // Prevent erroneous "may be used uninitialized" warnings
            lxt1 = lxt2 = mxt = 0;
            break;
        }

        for (i = 0; i < mxt; i++)
        {
            if (irt1[i] == TRUE && lxt1 == mxt)
                lxt1 = i;
            if (M5_ZS)                 // if M5_ZS (Zero Search)
            {
                if (irt2[i] == TRUE && lxt2 == mxt)
                    lxt2 = i;
            }
        }

        // cc 1 and 3 are possible when M5_ZS is 0 or 1.
        if (lxt1 == mxt && lxt2 == mxt)
            regs->psw.cc = 3;
        else if (lxt1 < mxt && lxt2 == mxt )
            regs->psw.cc = 1;
        // cc 0 and 2 are only possible when M5_ZS is 1.
        else if (lxt1 < lxt2)
            regs->psw.cc = 2;
        else
            regs->psw.cc = 0;
    }

#undef M5_IN
#undef M5_RT
#undef M5_ZS
#undef M5_CS

    ZVECTOR_END( regs );
}
#endif

/*-------------------------------------------------------------------*/
/* E784 VPDI   - Vector Permute Doubleword Immediate         [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_permute_doubleword_immediate )
{
    int     v1, v2, v3, m4, m5, m6;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

#define M4_SO ((m4 & 0x4) != 0)  // Second operand index
#define M4_TO ((m4 & 0x1) != 0)  // Third operand index

    SV_D( temp, 0 ) = regs->VR_D( v2, M4_SO );
    SV_D( temp, 1 ) = regs->VR_D( v3, M4_TO );

    regs->VR_D( v1, 0 ) = SV_D( temp, 0 );
    regs->VR_D( v1, 1 ) = SV_D( temp, 1 );

#undef M4_SO
#undef M4_TO

    ZVECTOR_END( regs );
}

#if defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 )
/*-------------------------------------------------------------------*/
/* E785 VBPERM - Vector Bit Permute                          [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_bit_permute )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i, j, k;
    U16     wanted, result = 0;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m4, m5, m6 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    #if defined ( FEATURE_V128_SSE )
        i = j = k = wanted = 0;
        UNREFERENCED( i & j & k & wanted);
        temp.d[0]  = 0;
        UNREFERENCED( temp.d[0] );

        result = v128_bit_permute( regs->VR_Q(v2).v, regs->VR_Q(v3).v );

    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
        SV_D( temp, 2 ) = 0;
        SV_D( temp, 3 ) = 0;

        // Each of the sixteen 1-byte elements in vector register v3
        // contains the bit number (0 to 255) of a bit in the source
        // vector.
        // 1. Calculate the number of the byte (0 to 31) in the source
        //    vector that contains the wanted bit number.
        // 2. Calculate the number of the bit (0 to 7) in the byte
        //    that is the wanted bit.
        // 3. Calculate the value (0 or 1) of the wanted bit, and
        // 4. If the wanted bit has a value of 1, place the bit in
        //    the result.
        // The bit value of the bit number in the first element of v3
        // becomes result bit 0, the bit value of the bit number in the
        // second element of v3 becomes result bit 1, and so on until
        // the bit value of the bit number in the sixteenth element of
        // v3 becomes result bit 15.
        for (i = 0; i < 16; i++)
        {
            j = regs->VR_B( v3, i ) / 8;
            k = regs->VR_B( v3, i ) % 8;
            wanted = SV_B( temp, j ) & ( 0x80 >> k );
            if (wanted)
            {
                result |= ( 0x0001 << ( 15 - i ) );
            }
        }
#endif

    regs->VR_D( v1, 0 ) = result;
    regs->VR_D( v1, 1 ) = 0;

    ZVECTOR_END( regs );
}
#endif /* defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 ) */

/*-------------------------------------------------------------------*/
/* E786 VSLD   - Vector Shift Left Double By Bit             [VRI-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_left_double_by_bit )
{
    int     v1, v2, v3, i4, m5;
    int     i;
    U64     j, k;
    SV      temp;

    VRI_D( inst, regs, v1, v2, v3, i4, m5 );

    /* m5 is not part of this instruction */
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    if (i4 & 0xF8)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
    SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
    SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
    SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

    if ( i4 > 0 )
        for (i = 0; i < 3; i++) {
            j = SV_D( temp, i ) << i4;
            k = SV_D( temp, i+1 ) >> ( 64 - i4 );
            SV_D( temp, i ) = j | k;
        }

    regs->VR_D( v1, 0 ) = SV_D( temp, 0 );
    regs->VR_D( v1, 1 ) = SV_D( temp, 1 );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E787 VSRD   - Vector Shift Right Double By Bit            [VRI-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_shift_right_double_by_bit )
{
    int     v1, v2, v3, i4, m5;
    int     i;
    U64     j, k;
    SV      temp;

    VRI_D( inst, regs, v1, v2, v3, i4, m5 );

    /* m5 is not part of this instruction */
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    if (i4 & 0xF8)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
    SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
    SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
    SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

    if ( i4 > 0 )
        for (i = 3; i > 0; i--) {
            j = SV_D( temp, i-1 ) << ( 64 - i4 );
            k = SV_D( temp, i ) >> i4;
            SV_D( temp, i ) = j | k;
        }

    regs->VR_D( v1, 0 ) = SV_D( temp, 2 );
    regs->VR_D( v1, 1 ) = SV_D( temp, 3 );

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E78A VSTRC  - Vector String Range Compare                 [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_string_range_compare )
{
    int     v1, v2, v3, v4, m5, m6;
    int     i, j;
    int     lxt1, lxt2;                // Lowest indexed true
    int     mxt;                       // Maximum indexed true
    BYTE    irt1[16], irt2[16];        // First and second intermediate results
    BYTE    erc, orc;                  // Even and Odd range comparison results

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    ZVECTOR_CHECK( regs );

#define M6_IN ((m6 & 0x8) != 0) // Invert Result
#define M6_RT ((m6 & 0x4) != 0) // Result Type
#define M6_ZS ((m6 & 0x2) != 0) // Zero Search
#define M6_CS ((m6 & 0x1) != 0) // Condition Code Set

    for (i=0; i<16; i++)
    {
        irt1[i] = irt2[i] = FALSE;
    }

    switch (m5)
    {
    case 0:  /* Byte */
        for (i=0; i<16; i++)
        {
            // Compare the element of the second operand with the ranges
            for (j=0; j<16; j+=2)
            {
                erc = orc = FALSE;
                // Compare the element of the second operand with the even element of the third operand.
                if ((regs->VR_B(v4, j)   & 0x80) && regs->VR_B(v2, i) == regs->VR_B(v3, j))   erc = TRUE;
                if ((regs->VR_B(v4, j)   & 0x40) && regs->VR_B(v2, i) <  regs->VR_B(v3, j))   erc = TRUE;
                if ((regs->VR_B(v4, j)   & 0x20) && regs->VR_B(v2, i) >  regs->VR_B(v3, j))   erc = TRUE;
                // Compare the element of the second operand with the odd element of the third operand.
                if ((regs->VR_B(v4, j+1) & 0x80) && regs->VR_B(v2, i) == regs->VR_B(v3, j+1)) orc = TRUE;
                if ((regs->VR_B(v4, j+1) & 0x40) && regs->VR_B(v2, i) <  regs->VR_B(v3, j+1)) orc = TRUE;
                if ((regs->VR_B(v4, j+1) & 0x20) && regs->VR_B(v2, i) >  regs->VR_B(v3, j+1)) orc = TRUE;
                // Determine the result of the range comparison
                if (erc == TRUE && orc == TRUE) irt1[i] = TRUE;
            }
            // Invert the ranges result if required
            if (M6_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M6_ZS && regs->VR_B(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M6_RT)                     // if M6_RT (Result Type)
        {
            for (i=0; i<16; i++)
            {
                regs->VR_B(v1, i) = (irt1[i] == TRUE) ? 0xFF : 0x00;
            }
        }
        else                           // else !M6_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<16; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16) lxt1 = i;
                if (irt2[i] == TRUE && lxt2 == 16) lxt2 = i;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 1:  /* Halfword */
        for (i=0; i<8; i++)
        {
            // Compare the element of the second operand with the ranges
            for (j=0; j<8; j+=2)
            {
                erc = orc = FALSE;
                // Compare the element of the second operand with the even element of the third operand.
                if ((regs->VR_H(v4, j)   & 0x8000) && regs->VR_H(v2, i) == regs->VR_H(v3, j))   erc = TRUE;
                if ((regs->VR_H(v4, j)   & 0x4000) && regs->VR_H(v2, i) <  regs->VR_H(v3, j))   erc = TRUE;
                if ((regs->VR_H(v4, j)   & 0x2000) && regs->VR_H(v2, i) >  regs->VR_H(v3, j))   erc = TRUE;
                // Compare the element of the second operand with the odd element of the third operand.
                if ((regs->VR_H(v4, j+1) & 0x8000) && regs->VR_H(v2, i) == regs->VR_H(v3, j+1)) orc = TRUE;
                if ((regs->VR_H(v4, j+1) & 0x4000) && regs->VR_H(v2, i) <  regs->VR_H(v3, j+1)) orc = TRUE;
                if ((regs->VR_H(v4, j+1) & 0x2000) && regs->VR_H(v2, i) >  regs->VR_H(v3, j+1)) orc = TRUE;
                // Determine the result of the range comparison
                if (erc == TRUE && orc == TRUE) irt1[i] = TRUE;
            }
            // Invert the ranges result if required
            if (M6_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M6_ZS && regs->VR_H(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M6_RT)                     // if M6_RT (Result Type)
        {
            for (i=0; i<8; i++)
            {
                regs->VR_H(v1, i) = (irt1[i] == TRUE) ? 0xFFFF : 0x0000;
            }
        }
        else                           // else !M6_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<8; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16) lxt1 = i * 2;
                if (irt2[i] == TRUE && lxt2 == 16) lxt2 = i * 2;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    case 2:  /* Word */
        for (i=0; i<4; i++)
        {
            // Compare the element of the second operand with the ranges
            for (j=0; j<4; j+=2)
            {
                erc = orc = FALSE;
                // Compare the element of the second operand with the even element of the third operand.
                if ((regs->VR_F(v4, j)   & 0x80000000) && regs->VR_F(v2, i) == regs->VR_F(v3, j))   erc = TRUE;
                if ((regs->VR_F(v4, j)   & 0x40000000) && regs->VR_F(v2, i) <  regs->VR_F(v3, j))   erc = TRUE;
                if ((regs->VR_F(v4, j)   & 0x20000000) && regs->VR_F(v2, i) >  regs->VR_F(v3, j))   erc = TRUE;
                // Compare the element of the second operand with the odd element of the third operand.
                if ((regs->VR_F(v4, j+1) & 0x80000000) && regs->VR_F(v2, i) == regs->VR_F(v3, j+1)) orc = TRUE;
                if ((regs->VR_F(v4, j+1) & 0x40000000) && regs->VR_F(v2, i) <  regs->VR_F(v3, j+1)) orc = TRUE;
                if ((regs->VR_F(v4, j+1) & 0x20000000) && regs->VR_F(v2, i) >  regs->VR_F(v3, j+1)) orc = TRUE;
                // Determine the result of the range comparison
                if (erc == TRUE && orc == TRUE) irt1[i] = TRUE;
            }
            // Invert the ranges result if required
            if (M6_IN) irt1[i] ^= TRUE;
            // Compare the element of the second operand with zero
            if (M6_ZS && regs->VR_F(v2,i) == 0) irt2[i] = TRUE;
        }
        if (M6_RT)                     // if M6_RT (Result Type)
        {
            for (i=0; i<4; i++)
            {
                regs->VR_F(v1, i) = (irt1[i] == TRUE) ? 0xFFFFFFFF : 0x00000000;
            }
        }
        else                           // else !M6_RT
        {
            lxt1 = lxt2 = 16;
            for (i=0; i<4; i++)
            {
                if (irt1[i] == TRUE && lxt1 == 16) lxt1 = i * 4;
                if (irt2[i] == TRUE && lxt2 == 16) lxt2 = i * 4;
            }
            regs->VR_D(v1, 0) = min(lxt1, lxt2);
            regs->VR_D(v1, 1) = 0;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    if (M6_CS)                         // if M6_CS (Condition Code Set)
    {
        switch (m5)
        {
        case 0:  /* Byte */
            lxt1 = lxt2 = mxt = 16;
            break;
        case 1:  /* Halfword */
            lxt1 = lxt2 = mxt = 8;
            break;
        case 2:  /* Word */
            lxt1 = lxt2 = mxt = 4;
            break;
        default:  // Prevent erroneous "may be used uninitialized" warnings
            lxt1 = lxt2 = mxt = 0;
            break;
        }

        for (i = 0; i < mxt; i++)
        {
            if (irt1[i] == TRUE && lxt1 == mxt)
                lxt1 = i;
            if (M6_ZS)                 // if M6_ZS (Zero Search)
            {
                if (irt2[i] == TRUE && lxt2 == mxt)
                    lxt2 = i;
            }
        }

        // cc 1 and 3 are possible when M6_ZS is 0 or 1.
        if (lxt1 == mxt && lxt2 == mxt)
            regs->psw.cc = 3;
        else if (lxt1 < mxt && lxt2 == mxt )
            regs->psw.cc = 1;
        // cc 0 and 2 are only possible when M6_ZS is 1.
        else if (lxt1 < lxt2)
            regs->psw.cc = 2;
        else
            regs->psw.cc = 0;
    }

#undef M6_IN
#undef M6_RT
#undef M6_ZS
#undef M6_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E78B VSTRS  - Vector String Search                        [VRR-d] */
/*-------------------------------------------------------------------*/
/*                                                                   */
/* In PoP (SA22-7832-13), for VSTRS we can read:                     */
/*   Byte element seven of the fourth operand specifies              */
/*   the length of the substring in bytes and must be in             */
/*   the range of 0-16. Other values will result in an               */
/*   unpredictable result.                                           */
/*                                                                   */
/* However, empirical evidence suggests that any value larger than   */
/* 16 is treated as 16. This may be model dependant behaviour, but   */
/* this implementation will follow a models (z15) behaviour.         */
/*                                                                   */
#if defined ( FEATURE_V128_SSE )
/*-------------------------------------------------------------------*/
/* Intrinsic version of                                              */
/* E78B VSTRS  - Vector String Search                        [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_string_search )
{
    int     v1, v2, v3, v4, m5, m6;

    int     substr_len, char_size, str_len, eos, i, k;
    int     v2_str_len, v3_str_len;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    ZVECTOR_CHECK( regs );

#define M6_RE ((m6 & 0xD) != 0) // Reserved
#define M6_ZS ((m6 & 0x2) != 0) // Zero Search

    // logmsg("VSTRS  - Vector String Search: m5=%d, zs=%d \n", m5, M6_ZS);
    // u128_logmsg(" vector2       ", (U128) regs->VR_Q(v2) );
    // u128_logmsg(" vector3       ", (U128) regs->VR_Q(v3) );
    // u128_logmsg(" vector4       ", (U128) regs->VR_Q(v4) );

    if (m5 > 2 || M6_RE)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    eos = i = k = 0;
    char_size = 1;
    str_len = 16;

    substr_len = regs->VR_B( v4, 7 );

    switch (m5)
    {
    case 0:
        char_size = 1;
        break;
    case 1:
        char_size = 2;
        break;
    case 2:
        char_size = 4;
        break;
    default:
        break;
    }

    if (M6_ZS)
    {
        switch (m5)
        {
        case 0:     /* byte character strings */
            v2_str_len = v128_find_first_zero_in_vector_8( regs->VR_Q(v2).v );
            if (v2_str_len < str_len )
            {
                str_len = v2_str_len;
                eos = 1;
            }

            v3_str_len = v128_find_first_zero_in_vector_8( regs->VR_Q(v3).v );
            if (v3_str_len < substr_len )
                substr_len = v3_str_len;

            break;

        case 1:     /* halfword chartacter strings */
            v2_str_len = 2 * v128_find_first_zero_in_vector_16( regs->VR_Q(v2).v );
            if (v2_str_len < str_len )
            {
                str_len = v2_str_len;
                eos = 1;
            }

            v3_str_len = 2 * v128_find_first_zero_in_vector_16( regs->VR_Q(v3).v );
            if (v3_str_len < substr_len )
                substr_len = v3_str_len;

            break;

        case 2: /* word character strings */
            v2_str_len = 4 * v128_find_first_zero_in_vector_32( regs->VR_Q(v2).v );
            if (v2_str_len < str_len )
            {
                str_len = v2_str_len;
                eos = 1;
            }

            v3_str_len = 4 * v128_find_first_zero_in_vector_32( regs->VR_Q(v3).v );
            if (v3_str_len < substr_len )
                substr_len = v3_str_len;

            break;

        default:
            break;
        }
    }

    // check substr length
    if ( substr_len == 0 )
        goto vector_string_search_full_match;
    if ( (substr_len % char_size) != 0 )
        goto vector_string_search_mdresult;
    if (substr_len > 16) substr_len = 16;

    // logmsg(" str_len= %d, substr_len=%d\n", str_len, substr_len);

    switch (m5)
    {
    case 0:     /* byte character strings */
        k = v128_find_first_substring_8( regs->VR_Q(v2).v, str_len, regs->VR_Q(v3).v, substr_len );
        break;

    case 1:     /* halfword chartacter strings */
        k = 2 * v128_find_first_substring_16( regs->VR_Q(v2).v, str_len / 2, regs->VR_Q(v3).v, substr_len / 2 );
        break;

    case 2: /* word character strings */
        k = 4 * v128_find_first_substring_32( regs->VR_Q(v2).v, str_len / 4, regs->VR_Q(v3).v, substr_len / 4 );
        break;

    default:
        break;
    }

    if ( M6_ZS )
    {
        if ( k == 16 )
            goto vector_string_search_no_match_zero;

        if ( k + substr_len <= 16 )
            goto vector_string_search_full_match;

        if ( k + substr_len > 16 )
            goto vector_string_search_partial_match;
    }
    else
    {
        if ( k == 16 )
            goto vector_string_search_no_match;

        if ( k + substr_len <= 16 )
            goto vector_string_search_full_match;

        if ( k + substr_len > 16 )
            goto vector_string_search_partial_match;
    }

    UNREACHABLE_CODE( goto vector_string_search_mdresult );

vector_string_search_mdresult:
    regs->VR_D( v1, 0 ) = 16;                    /* Model dependant */
    regs->VR_D( v1, 1 ) = 0;                     /* results are     */
    regs->psw.cc = 0;  /* no match */            /* unpredictable   */
    goto vector_string_search_end;

vector_string_search_no_match:
    regs->VR_D( v1, 0 ) = 16;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 0;  /* no match */
    goto vector_string_search_end;

vector_string_search_no_match_zero:
    regs->VR_D( v1, 0 ) = 16;
    regs->VR_D( v1, 1 ) = 0;
    if ( eos == 1 )
        regs->psw.cc = 1;  /* no match, zero char */
    else
        regs->psw.cc = 0;  /* no match */
    goto vector_string_search_end;

vector_string_search_full_match:
    regs->VR_D( v1, 0 ) = k;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 2;  /* full match */
    goto vector_string_search_end;

vector_string_search_partial_match:
    regs->VR_D( v1, 0 ) = k;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 3;  /* partial match */
    goto vector_string_search_end;

vector_string_search_end:

    // logmsg(" end - cc= %d, k=%2d\n", regs->psw.cc, regs->VR_B( v1, 7 ) );

#undef M6_RE
#undef M6_ZS

    ZVECTOR_END( regs );
}

#else
/*-------------------------------------------------------------------*/
/* portable C version of                                             */
/* E78B VSTRS  - Vector String Search                        [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_string_search )
{
    int     v1, v2, v3, v4, m5, m6;
    char    v2_temp[16], v3_temp[16], nulls[16];
    int     substr_len, char_size, str_len, eos, i, k;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    ZVECTOR_CHECK( regs );

#define M6_RE ((m6 & 0xD) != 0) // Reserved
#define M6_ZS ((m6 & 0x2) != 0) // Zero Search

    if (m5 > 2 || M6_RE)
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );

    /* Get the contents of v2 and v3 as a string of bytes arranged */
    /* as they would be if they were in the guests storage.        */
    for (i = 0; i < 16; i++)
    {
        v2_temp[i] = regs->VR_B( v2, i );
        v3_temp[i] = regs->VR_B( v3, i );
    }

    substr_len = regs->VR_B( v4, 7 );

    switch (m5)
    {
    case 0:
        char_size = 1;
        break;
    case 1:
        char_size = 2;
        break;
    case 2:
        char_size = 4;
        break;
    default:  // Prevent erroneous "may be used uninitialized" warnings
        char_size = 1;
        break;
    }

    str_len = eos = i = k = 0;

    if (M6_ZS)
    {
        memset( nulls, 0, sizeof(nulls) );

        for (i = 0; i < 16; i += char_size)
        {
            if ( memcmp(&v3_temp[i], &nulls, char_size) == 0 )
            {
                break;
            }
        }

        if ( i < substr_len )
        {
            substr_len = i;
        }

        if (substr_len == 0)
        {
            goto vector_string_search_full_match;
        }
        else
        {
            if (substr_len > 16)
            {
                substr_len = 16;
            }

            for ( ; k < 16 ; k += char_size )
            {
                if ( memcmp(&v2_temp[k], &nulls, char_size) == 0 )
                {
                    eos = 1;
                    break;
                }
            }

            str_len = k;
            k = 0;

            if ( (substr_len % char_size) != 0 )
            {
                goto vector_string_search_mdresult;
            }

            for ( ; ; k += char_size )
            {
                if ( k < str_len )
                {
                    if ( eos == 0 || ( k + substr_len ) <= str_len )
                    {
                        if ((k + substr_len) <= str_len)
                        {
                            if ( memcmp(&v2_temp[k], &v3_temp[0], substr_len) == 0 )
                            {
                                goto vector_string_search_full_match;
                            }
                        }
                        else
                        {
                            if ( memcmp(&v2_temp[k], &v3_temp[0], str_len - k ) == 0 )
                            {
                                goto vector_string_search_partial_match;
                            }
                        }
                    }
                    else
                    {
                        goto vector_string_search_no_match_zero;
                    }
                }
                else
                {
                    goto vector_string_search_no_match_zero;
                }
            }
        }
    }
    else
    {
        if ( substr_len == 0 )
        {
            goto vector_string_search_full_match;
        }
        else
        {
            if ( (substr_len % char_size) != 0 )
            {
                goto vector_string_search_mdresult;
            }
            if (substr_len > 16)
            {
                substr_len = 16;
            }
            for ( ; ; k += char_size )
            {
                if (k == 16)
                {
                    goto vector_string_search_no_match;
                }

                if ((k + substr_len) <= 16)
                {
                    if ( memcmp(&v2_temp[k], &v3_temp[0], substr_len) == 0 )
                    {
                        goto vector_string_search_full_match;
                    }
                }
                else
                {
                    if ( memcmp(&v2_temp[k], &v3_temp[0], 16 - k) == 0 )
                    {
                        goto vector_string_search_partial_match;
                    }
                }
            }
        }
    }

    UNREACHABLE_CODE( goto vector_string_search_mdresult );

vector_string_search_mdresult:
    regs->VR_D( v1, 0 ) = 16;                    /* Model dependant */
    regs->VR_D( v1, 1 ) = 0;                     /* results are     */
    regs->psw.cc = 0;  /* no match */            /* unpredictable   */
    goto vector_string_search_end;

vector_string_search_no_match:
    regs->VR_D( v1, 0 ) = 16;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 0;  /* no match */
    goto vector_string_search_end;

vector_string_search_no_match_zero:
    regs->VR_D( v1, 0 ) = 16;
    regs->VR_D( v1, 1 ) = 0;
    if ( eos == 1 )
        regs->psw.cc = 1;  /* no match, zero char */
    else
        regs->psw.cc = 0;  /* no match */
    goto vector_string_search_end;

vector_string_search_full_match:
    regs->VR_D( v1, 0 ) = k;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 2;  /* full match */
    goto vector_string_search_end;

vector_string_search_partial_match:
    regs->VR_D( v1, 0 ) = k;
    regs->VR_D( v1, 1 ) = 0;
    regs->psw.cc = 3;  /* partial match */
    goto vector_string_search_end;

vector_string_search_end:

#undef M6_RE
#undef M6_ZS

    ZVECTOR_END( regs );
}

#endif

/*-------------------------------------------------------------------*/
/* E78C VPERM  - Vector Permute                              [VRR-e] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_permute )
{
    int     v1, v2, v3, v4, m5, m6;
    int     i, j;
    SV      temp;

    VRR_E( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    #if defined ( FEATURE_V128_SSE )
        i = j = 0;
        UNREFERENCED( i );
        UNREFERENCED( j );
        temp.d[0] = 0;
        UNREFERENCED( temp.d[0] );

        regs->VR_Q(v1).v = v128_permute( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v  );

    #else
        SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
        SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
        SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
        SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

        for (i = 0; i < 16; i++) {
            j = regs->VR_B(v4, i) & 0x1f;
            regs->VR_B(v1, i) = SV_B( temp, j );
        }
    #endif

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E78D VSEL   - Vector Select                               [VRR-e] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_select )
{
    int     v1, v2, v3, v4, m5, m6;

    VRR_E( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    regs->VR_D(v1, 1) = (regs->VR_D(v4, 1) & regs->VR_D(v2, 1)) | (~regs->VR_D(v4, 1) & regs->VR_D(v3, 1));
    regs->VR_D(v1, 0) = (regs->VR_D(v4, 0) & regs->VR_D(v2, 0)) | (~regs->VR_D(v4, 0) & regs->VR_D(v3, 0));

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E794 VPK    - Vector Pack                                 [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_pack )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i;
    SV      temp;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
    SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
    SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
    SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

    switch (m4)
    {
    case 1:  /* Halfword: Low-order bytes from halfwords */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_pack_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i = 0; i < 16; i++ )
        {
            regs->VR_B( v1, i ) = SV_B( temp, (i*2)+1 );
        }
    #endif
        break;

    case 2:  /* Word: Low-order halfwords from words */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_pack_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i = 0; i < 8; i++ )
        {
            regs->VR_H( v1, i ) = SV_H( temp, (i*2)+1 );
        }
    #endif
        break;

    case 3:  /* Doubleword: Low-order words from doublewords */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_pack_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v );
    #else
        for ( i = 0; i < 4; i++ )
        {
            regs->VR_F( v1, i ) = SV_F( temp, (i*2)+1 );
        }
    #endif
        break;

    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E795 VPKLS  - Vector Pack Logical Saturate                [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST(vector_pack_logical_saturate)
{
    int     v1, v2, v3, m4, m5;
    int     sat, allsat, i;
    BYTE    newcc;
    SV      temp;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    /* may be unreferenced */
    sat = allsat = i = newcc = 0;
    UNREFERENCED( sat );
    UNREFERENCED( allsat );
    UNREFERENCED( i );
    UNREFERENCED( newcc );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

#define M5_CS ((m5 & 0x1) != 0)  // Condition Code Set

    SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
    SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
    SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
    SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

    sat = allsat = 0;

    switch (m4)
    {
    case 1:  /* Halfword: Low-order bytes from halfwords */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_logical_saturate_cc_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_logical_saturate_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 16; i++ )
        {
            if ( SV_H( temp, i ) <= 0x00FF )
            {
                regs->VR_B( v1, i ) = SV_B( temp, (i*2)+1 );
            }
            else
            {
                regs->VR_B( v1, i ) = 0xFF;
                sat++;
            }
        }
        allsat = 16;
        break;
    #endif

    case 2:  /* Word: Low-order halfwords from words */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_logical_saturate_cc_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_logical_saturate_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 8; i++ )
        {
            if ( SV_F( temp, i ) <= 0x0000FFFF )
            {
                regs->VR_H( v1, i ) = SV_H( temp, (i*2)+1 );
            }
            else
            {
                regs->VR_H( v1, i ) = 0xFFFF;
                sat++;
            }
        }
        allsat = 8;
        break;
    #endif

    case 3:  /* Doubleword: Low-order words from doublewords */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_logical_saturate_cc_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_logical_saturate_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 4; i++ )
        {
            if ( SV_D( temp, i ) <= 0x00000000FFFFFFFFull )
            {
                regs->VR_F( v1, i ) = SV_F( temp, (i*2)+1 );
            }
            else
            {
                regs->VR_F( v1, i ) = 0xFFFFFFFF;
                sat++;
            }
        }
        allsat = 4;
        break;
    #endif

    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    #if !defined ( FEATURE_V128_SSE )
        if (M5_CS)               // if M5_CS (Condition Code Set)
        {
            if ( sat >= allsat )
            {
                newcc = 3;       // Saturation on all elements
            }
            else if ( sat != 0 )
            {
                newcc = 1;       // At least one but not all elements saturated
            }
            else
            {
                newcc = 0;       // No saturation
            }
            regs->psw.cc = newcc;
        }
    #endif

#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E797 VPKS   - Vector Pack Saturate                        [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_pack_saturate )
{
    int     v1, v2, v3, m4, m5;
    int     sat, allsat, i;
    BYTE    newcc;
    SV      temp;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    /* may be unreferenced */
    sat = allsat = i = newcc = 0;
    UNREFERENCED( sat );
    UNREFERENCED( allsat );
    UNREFERENCED( i );
    UNREFERENCED( newcc );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

#define M5_CS ((m5 & 0x1) != 0)  // Condition Code Set

    SV_D( temp, 0 ) = regs->VR_D( v2, 0 );
    SV_D( temp, 1 ) = regs->VR_D( v2, 1 );
    SV_D( temp, 2 ) = regs->VR_D( v3, 0 );
    SV_D( temp, 3 ) = regs->VR_D( v3, 1 );

    sat = allsat = 0;

    switch (m4)
    {
    case 1:  /* Halfword: Low-order bytes from halfwords */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_saturate_cc_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_saturate_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 16; i++ )
        {
            if ( !( SV_H( temp, i ) & 0x8000 ) )
            {
                if ( SV_H( temp, i ) <= 0x007F )
                {
                    regs->VR_B( v1, i ) = SV_B( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_B( v1, i ) = 0x7F;
                    sat++;
                }
            }
            else
            {
                if ( (S16)SV_H( temp, i ) >= (S16)0xFF80 )
                {
                    regs->VR_B( v1, i ) = SV_B( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_B( v1, i ) = 0x80;
                    sat++;
                }
            }
        }
        allsat = 16;
        break;
    #endif

    case 2:  /* Word: Low-order halfwords from words */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_saturate_cc_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_saturate_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 8; i++ )
        {
            if ( !( SV_F( temp, i ) & 0x80000000 ) )
            {
                if ( SV_F( temp, i ) <= 0x00007FFF )
                {
                    regs->VR_H( v1, i ) = SV_H( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_H( v1, i ) = 0x7FFF;
                    sat++;
                }
            }
            else
            {
                if ( (S32)SV_F( temp, i ) >= (S32)0xFFFF8000 )
                {
                    regs->VR_H( v1, i ) = SV_H( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_H( v1, i ) = 0x8000;
                    sat++;
                }
            }
        }
        allsat = 8;
        break;
    #endif

    case 3:  /* Doubleword: Low-order words from doublewords */
    #if defined ( FEATURE_V128_SSE )
        if (M5_CS)  regs->psw.cc = v128_pack_saturate_cc_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        regs->VR_Q(v1).v = v128_pack_saturate_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
        break;
    #else
        for ( i = 0; i < 4; i++ )
        {
            if ( !( SV_D( temp, i ) & 0x8000000000000000ull ) )
            {
                if ( SV_D( temp, i ) <= 0x000000007FFFFFFFull )
                {
                    regs->VR_F( v1, i ) = SV_F( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_F( v1, i ) = 0x7FFFFFFF;
                    sat++;
                }
            }
            else
            {
                if ( (S64)SV_D( temp, i ) >= (S64)0xFFFFFFFF80000000ull )
                {
                    regs->VR_F( v1, i ) = SV_F( temp, (i*2)+1 );
                }
                else
                {
                    regs->VR_F( v1, i ) = 0x80000000;
                    sat++;
                }
            }
        }
        allsat = 4;
        break;
    #endif

    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    #if !defined ( FEATURE_V128_SSE )
        if (M5_CS)               // if M5_CS (Condition Code Set)
        {
            if ( sat >= allsat )
            {
                newcc = 3;       // Saturation on all elements
            }
            else if ( sat != 0 )
            {
                newcc = 1;       // At least one but not all elements saturated
            }
            else
            {
                newcc = 0;       // No saturation
            }
            regs->psw.cc = newcc;
        }
    #endif

#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A1 VMLH   - Vector Multiply Logical High                [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_logical_high )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d = 0;
    UNREFERENCED( temp.d );      /* temp maybe unreferenced */

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            regs->VR_B(v1, i) = temp.h >> 8;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            regs->VR_H(v1, i) = temp.f >> 16;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            regs->VR_F(v1, i) = temp.d >> 32;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A2 VML    - Vector Multiply Low                         [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_low )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_low_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            regs->VR_B(v1, i) = temp.h & 0xFF;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_low_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            regs->VR_H(v1, i) = temp.f & 0xFFFF;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_low_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            regs->VR_F(v1, i) = temp.d & 0xFFFFFFFF;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A3 VMH    - Vector Multiply High                        [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_high )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            regs->VR_B(v1, i) = temp.sh >> 8;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            regs->VR_H(v1, i) = temp.sf >> 16;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            regs->VR_F(v1, i) = temp.sd >> 32;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A4 VMLE   - Vector Multiply Logical Even                [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_logical_even )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i, j;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_even_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 16; i+=2, j++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            regs->VR_H(v1, j) = temp.h;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_even_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 8; i+=2, j++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            regs->VR_F(v1, j) = temp.f;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_even_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 4; i+=2, j++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            regs->VR_D(v1, j) = temp.d;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A5 VMLO   - Vector Multiply Logical Odd                 [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_logical_odd )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i, j;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.d = 0;
    UNREFERENCED( temp.d );      /* temp maybe unreferenced */

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_odd_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 16; i+=2, j++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            regs->VR_H(v1, j) = temp.h;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_odd_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 8; i+=2, j++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            regs->VR_F(v1, j) = temp.f;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_logical_odd_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 4; i+=2, j++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            regs->VR_D(v1, j) = temp.d;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A6 VME    - Vector Multiply Even                        [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_even )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i, j;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_even_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 16; i+=2, j++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            regs->VR_H(v1, j) = temp.sh;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_even_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 8; i+=2, j++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            regs->VR_F(v1, j) = temp.sf;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_even_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0, j=0; i < 4; i+=2, j++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            regs->VR_D(v1, j) = temp.sd;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A7 VMO    - Vector Multiply Odd                         [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_odd )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i, j;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_odd_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 16; i+=2, j++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            regs->VR_H(v1, j) = temp.sh;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_odd_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 8; i+=2, j++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            regs->VR_F(v1, j) = temp.sf;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_odd_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=1, j=0; i < 4; i+=2, j++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            regs->VR_D(v1, j) = temp.sd;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7A9 VMALH  - Vector Multiply and Add Logical High        [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_logical_high )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 16; i++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            temp.h += regs->VR_B(v4, i);
            regs->VR_B(v1, i) = temp.h >> 8;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 8; i++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            temp.f += regs->VR_H(v4, i);
            regs->VR_H(v1, i) = temp.f >> 16;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 4; i++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            temp.d += regs->VR_F(v4, i);
            regs->VR_F(v1, i) = temp.d >> 32;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7AA VMAL   - Vector Multiply and Add Low                 [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST(vector_multiply_and_add_low)
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i;

    VRR_D(inst, regs, v1, v2, v3, v4, m5, m6);

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK(regs);

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_low_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 16; i++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            temp.h += regs->VR_B(v4, i);
            regs->VR_B(v1, i) = temp.h & 0xFF;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_low_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 8; i++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            temp.f += regs->VR_H(v4, i);
            regs->VR_H(v1, i) = temp.f & 0xFFFF;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_low_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 4; i++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            temp.d += regs->VR_F(v4, i);
            regs->VR_F(v1, i) = temp.d & 0xFFFFFFFF;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END(regs);
}

/*-------------------------------------------------------------------*/
/* E7AB VMAH   - Vector Multiply and Add High                [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_high )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 16; i++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            temp.sh += (S8)regs->VR_B(v4, i);
            regs->VR_B(v1, i) = temp.sh >> 8;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 8; i++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            temp.sf += (S16)regs->VR_H(v4, i);
            regs->VR_H(v1, i) = temp.sf >> 16;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0; i < 4; i++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            temp.sd += (S32)regs->VR_F(v4, i);
            regs->VR_F(v1, i) = temp.sd >> 32;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7AC VMALE  - Vector Multiply and Add Logical Even        [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_logical_even )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i, j;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_even_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 16; i+=2, j++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            temp.h += regs->VR_H(v4, j);
            regs->VR_H(v1, j) = temp.h;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_even_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 8; i+=2, j++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            temp.f += regs->VR_F(v4, j);
            regs->VR_F(v1, j) = temp.f;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_even_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 4; i+=2, j++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            temp.d += regs->VR_D(v4, j);
            regs->VR_D(v1, j) = temp.d;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7AD VMALO  - Vector Multiply and Add Logical Odd         [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_logical_odd )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d; U32 f; U16 h; } temp;
    int     i, j;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.d = 0;
    UNREFERENCED( temp.d );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_odd_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 16; i+=2, j++)
        {
            temp.h = regs->VR_B(v2, i);
            temp.h *= regs->VR_B(v3, i);
            temp.h += regs->VR_H(v4, j);
            regs->VR_H(v1, j) = temp.h;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_odd_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 8; i+=2, j++)
        {
            temp.f = regs->VR_H(v2, i);
            temp.f *= regs->VR_H(v3, i);
            temp.f += regs->VR_F(v4, j);
            regs->VR_F(v1, j) = temp.f;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_logical_odd_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 4; i+=2, j++)
        {
            temp.d = regs->VR_F(v2, i);
            temp.d *= regs->VR_F(v3, i);
            temp.d += regs->VR_D(v4, j);
            regs->VR_D(v1, j) = temp.d;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7AE VMAE   - Vector Multiply and Add Even                [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_even )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i, j;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_even_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 16; i+=2, j++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            temp.sh += (S16)regs->VR_H(v4, j);
            regs->VR_H(v1, j) = temp.sh;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_even_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 8; i+=2, j++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            temp.sf += (S32)regs->VR_F(v4, j);
            regs->VR_F(v1, j) = temp.sf;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_even_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=0, j=0; i < 4; i+=2, j++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            temp.sd += (S64)regs->VR_D(v4, j);
            regs->VR_D(v1, j) = temp.sd;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7AF VMAO   - Vector Multiply and Add Odd                 [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_and_add_odd )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { S64 sd; S32 sf; S16 sh; } temp;
    int     i, j;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    /* may be unreferenced */
    i = j = 0;
    UNREFERENCED( i );
    UNREFERENCED( j );
    temp.sd = 0;
    UNREFERENCED( temp.sd );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_odd_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 16; i+=2, j++)
        {
            temp.sh = (S8)regs->VR_B(v2, i);
            temp.sh *= (S8)regs->VR_B(v3, i);
            temp.sh += (S16)regs->VR_H(v4, j);
            regs->VR_H(v1, j) = temp.sh;
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_odd_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 8; i+=2, j++)
        {
            temp.sf = (S16)regs->VR_H(v2, i);
            temp.sf *= (S16)regs->VR_H(v3, i);
            temp.sf += (S32)regs->VR_F(v4, j);
            regs->VR_F(v1, j) = temp.sf;
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_multiply_add_odd_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v, regs->VR_Q(v4).v );
    #else
        for (i=1, j=0; i < 4; i+=2, j++)
        {
            temp.sd = (S32)regs->VR_F(v2, i);
            temp.sd *= (S32)regs->VR_F(v3, i);
            temp.sd += (S64)regs->VR_D(v4, j);
            regs->VR_D(v1, j) = temp.sd;
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7B4 VGFM   - Vector Galois Field Multiply Sum            [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_galois_field_multiply_sum )
{
    int     v1, v2, v3, m4, m5, m6;

    int     i, k;                 /* loop index                      */
    U16     accu16[16];           /* byte accumulator                */
    U32     accu32[8];            /* halfword accumulator            */
    U64     accu64[4];            /* word accumulator                */
    U64     accu128h[2];          /* doublewword accumulator  - high */
    U64     accu128l[2];          /* doublewword accumulator  - low  */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:         /* Byte */
        for (i=0; i < 16; i++)
            accu16[i] = (U16) gf_mul_32( (U32) regs->VR_B(v2, i), (U32) regs->VR_B(v3, i) );

        /* sum even-odd pair */
        for ( i=0, k=0; i < 8; i++, k += 2)
            regs->VR_H( v1, i ) = accu16[k] ^ accu16[k+1];

        break;

    case 1:         /* Halfword */
        for (i=0; i < 8; i++)
            accu32[i] = (U32) gf_mul_32( (U32) regs->VR_H(v2, i), (U32) regs->VR_H(v3, i) );

        /* sum even-odd pair */
        for ( i=0, k=0; i < 4; i++, k += 2)
            regs->VR_F( v1, i ) = accu32[k] ^ accu32[k+1];

        break;

    case 2:         /* Word */
        for (i=0; i < 4; i++)
            accu64[i] = (U64) gf_mul_32( (U32) regs->VR_F(v2, i), (U32) regs->VR_F(v3, i) );

        /* sum even-odd pair */
        for ( i=0, k=0; i < 2; i++, k += 2)
            regs->VR_D( v1, i ) = accu64[k] ^ accu64[k+1];

        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++)
            gf_mul_64( regs->VR_D(v2, i), regs->VR_D(v3, i), &accu128h[i], &accu128l[i]);

        /* sum even-odd pair */
        regs->VR_D( v1, 0 ) = accu128h[0] ^ accu128h[1];
        regs->VR_D( v1, 1 ) = accu128l[0] ^ accu128l[1];

        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

#if defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 )
/*-------------------------------------------------------------------*/
/* E7B8 VMSL   - Vector Multiply Sum Logical                 [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_multiply_sum_logical )
{
    int     v1, v2, v3, v4, m5, m6;
    U128    intere, intero;
#if defined( _MSVC_ )
    U128    copyv4;
#endif

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    ZVECTOR_CHECK( regs );

#define M6_ES ((m6 & 0x8) != 0) // Even Shift Indication
#define M6_OS ((m6 & 0x4) != 0) // Odd Shift Indication

    switch (m5)
    {
    case 3:  /* Doubleword */
        intere = U64_mul( regs->VR_D(v2, 0), regs->VR_D(v3, 0) );
        intero = U64_mul( regs->VR_D(v2, 1), regs->VR_D(v3, 1) );
        if (M6_ES)
            intere = U128_U32_mul( intere, 2 );  // Shift left
        if (M6_OS)
            intero = U128_U32_mul( intero, 2 );  // Shift left
        intere = U128_add( intere, intero );
#if defined( _MSVC_ )
        copyv4.Q = regs->VR_Q(v4);
        intere = U128_add( intere, copyv4 );
#else
        intere = U128_add( intere, (U128)regs->VR_Q(v4) );
#endif
        regs->VR_Q(v1) = intere.Q;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

#undef M6_ES
#undef M6_OS

    ZVECTOR_END( regs );
}
#endif /* defined( FEATURE_135_ZVECTOR_ENH_FACILITY_1 ) */

/*-------------------------------------------------------------------*/
/* E7B9 VACCC  - Vector Add With Carry Compute Carry         [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_add_with_carry_compute_carry )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d[4]; } temp;
    U64     carry;
    int     i;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 4:  /* Quadword */
        carry = regs->VR_D( v4, 1 ) & 0x0000000000000001ull;
        for (i=3; i >= 0; i--)
        {
            temp.d[i] = carry;
            temp.d[i] += regs->VR_F( v3, i );
            temp.d[i] += regs->VR_F( v2, i );
            carry = temp.d[i] >> 32;
        }
        regs->VR_D( v1, 0 ) = 0;
        regs->VR_D( v1, 1 ) = carry;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7BB VAC    - Vector Add With Carry                       [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_add_with_carry )
{
    int     v1, v2, v3, v4, m5, m6;
    U128    inter, rmost;
#if defined( _MSVC_ )
    U128    copyv2, copyv3;
#endif

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 4:  /* Quadword */
#if defined( _MSVC_ )
        copyv2.Q = regs->VR_Q(v2);
        copyv3.Q = regs->VR_Q(v3);
        inter = U128_add( copyv2, copyv3 );
#else
        inter = U128_add( (U128)regs->VR_Q(v2), (U128)regs->VR_Q(v3) );
#endif
        if (regs->VR_D( v4, 1 ) & 0x0000000000000001ull)
        {
            rmost.Q.D.H.D = 0;
            rmost.Q.D.L.D = 1;
            inter = U128_add(inter, rmost);
        }
        regs->VR_Q(v1) = inter.Q;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-----------------------------------------------------------------------*/
/* E7BC VGFMA  - Vector Galois Field Multiply Sum and Accumulate [VRR-d] */
/*-----------------------------------------------------------------------*/
DEF_INST( vector_galois_field_multiply_sum_and_accumulate )
{
    int     v1, v2, v3, v4, m5, m6;

    int     i, k;                     /* loop index                      */
    U16     accu16[16];               /* byte accumulator                */
    U32     accu32[8];                /* halfword accumulator            */
    U64     accu64[4];                /* word accumulator                */
    U64     accu128h[2];              /* doublewword accumulator  - high */
    U64     accu128l[2];              /* doublewword accumulator  - low  */

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 0:         /* Byte */
        for (i=0; i < 16; i++)
            accu16[i] = (U16) gf_mul_32( (U32) regs->VR_B(v2, i), (U32) regs->VR_B(v3, i) );

        /* sum even-odd pair plus vector accumulate */
        for ( i=0, k=0; i < 8; i++, k += 2)
            regs->VR_H( v1, i ) = accu16[k] ^ accu16[k+1] ^ regs->VR_H( v4, i);

        break;

    case 1:         /* Halfword */
        for (i=0; i < 8; i++)
            accu32[i] = (U32) gf_mul_32( (U32) regs->VR_H(v2, i), (U32) regs->VR_H(v3, i) );

        /* sum even-odd pair plus vector accumulate */
        for ( i=0, k=0; i < 4; i++, k += 2)
            regs->VR_F( v1, i ) = accu32[k] ^ accu32[k+1] ^ regs->VR_F( v4, i);

        break;

    case 2:         /* Word */
        for (i=0; i < 4; i++)
            accu64[i] = (U64) gf_mul_32( (U32) regs->VR_F(v2, i), (U32) regs->VR_F(v3, i) );

        /* sum even-odd pair plus vector accumulate */
        for ( i=0, k=0; i < 2; i++, k += 2)
            regs->VR_D( v1, i ) = accu64[k] ^ accu64[k+1] ^ regs->VR_D( v4, i );

        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++)
            gf_mul_64( regs->VR_D(v2, i), regs->VR_D(v3, i), &accu128h[i], &accu128l[i]);

        /* sum even-odd pair plus vector accumulate */
        regs->VR_D( v1, 0 ) = accu128h[0] ^ accu128h[1] ^ regs->VR_D( v4, 0);
        regs->VR_D( v1, 1 ) = accu128l[0] ^ accu128l[1] ^ regs->VR_D( v4, 1);

        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-----------------------------------------------------------------------------*/
/* E7BD VSBCBI - Vector Subtract With Borrow Compute Borrow Indication [VRR-d] */
/*-----------------------------------------------------------------------------*/
DEF_INST( vector_subtract_with_borrow_compute_borrow_indication )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d; } temp;
    int     i;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 4:  /* Quadword */
        temp.d = regs->VR_D( v4, 1 ) & 0x0000000000000001ull;
        for (i=3; i >= 0; i--)
        {
            temp.d += ~regs->VR_F( v3, i );
            temp.d += regs->VR_F( v2, i );
            temp.d >>= 32;
        }
        regs->VR_D( v1, 0 ) = 0;
        regs->VR_D( v1, 1 ) = temp.d;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7BF VSBI   - Vector Subtract With Borrow Indication      [VRR-d] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_subtract_with_borrow_indication )
{
    int     v1, v2, v3, v4, m5, m6;
    union   { U64 d[4]; } temp;
    U64     carry;
    int     i;

    VRR_D( inst, regs, v1, v2, v3, v4, m5, m6 );

    /* m6 is not part of this instruction */
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m5)
    {
    case 4:  /* Quadword */
        carry = regs->VR_D( v4, 1 ) & 0x0000000000000001ull;
        for (i=3; i >= 0; i--)
        {
            temp.d[i] = carry;
            temp.d[i] += ~regs->VR_F( v3, i );
            temp.d[i] += regs->VR_F( v2, i );
            carry = temp.d[i] >> 32;
        }
        for (i=3; i >= 0; i--)
            regs->VR_F( v1, i ) = temp.d[i];
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D4 VUPLL  - Vector Unpack Logical Low                   [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_unpack_logical_low )
{
    int     v1, v2, m3, m4, m5, i;
    union   { U64 d[2]; U32 f[4]; U16 h[8]; } temp;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_low_8( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 8; i++)
            temp.h[i] = (U16) regs->VR_B(v2, i + 8);
        for (i = 0; i < 8; i++)
            regs->VR_H(v1, i) = temp.h[i];
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_low_16( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 4; i++)
            temp.f[i] = (U32) regs->VR_H(v2, i + 4);
        for (i = 0; i < 4; i++)
            regs->VR_F(v1, i) = temp.f[i];
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_low_32( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 2; i++)
            temp.d[i] = (U64) regs->VR_F(v2, i + 2);
        for (i = 0; i < 2; i++)
            regs->VR_D(v1, i) = temp.d[i];
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D5 VUPLH  - Vector Unpack Logical High                  [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_unpack_logical_high )
{
    int     v1, v2, m3, m4, m5, i;
    union   { U64 d[2]; U32 f[4]; U16 h[8]; } temp;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.d[0] = 0;
    UNREFERENCED( temp.d[0] );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_high_8( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 8; i++)
            temp.h[i] = (U16) regs->VR_B(v2, i);
        for (i = 0; i < 8; i++)
            regs->VR_H(v1, i) = temp.h[i];
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_high_16( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 4; i++)
            temp.f[i] = (U32) regs->VR_H(v2, i);
        for (i = 0; i < 4; i++)
            regs->VR_F(v1, i) = temp.f[i];
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_logical_high_32( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 2; i++)
            temp.d[i] = (U64) regs->VR_F(v2, i);
        for (i = 0; i < 2; i++)
            regs->VR_D(v1, i) = temp.d[i];
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D6 VUPL   - Vector Unpack Low                           [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_unpack_low )
{
    int     v1, v2, m3, m4, m5;
    union   { U64 sd[2]; U32 sf[4]; U16 sh[8]; } temp;
    int     i;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.sd[0] = 0;
    UNREFERENCED( temp.sd[0] );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_low_8( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 8; i++)
        {
            temp.sh[i] = regs->VR_B(v2, i + 8);
            if (temp.sh[i] & 0x0080) temp.sh[i] |= 0xFF00;
        }
        for (i = 0; i < 8; i++)
            regs->VR_H(v1, i) = temp.sh[i];
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_low_16( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 4; i++)
        {
            temp.sf[i] = regs->VR_H(v2, i + 4);
            if (temp.sf[i] & 0x00008000) temp.sf[i] |= 0xFFFF0000;
        }
        for (i = 0; i < 4; i++)
            regs->VR_F(v1, i) = temp.sf[i];
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_low_32( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 2; i++)
        {
            temp.sd[i] = regs->VR_F(v2, i + 2);
            if (temp.sd[i] & 0x0000000080000000ull) temp.sd[i] |= 0xFFFFFFFF00000000ull;
        }
        for (i = 0; i < 2; i++)
            regs->VR_D(v1, i) = temp.sd[i];
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D7 VUPH   - Vector Unpack High                          [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_unpack_high )
{
    int     v1, v2, m3, m4, m5;
    union   { U64 sd[2]; U32 sf[4]; U16 sh[8]; } temp;
    int     i;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );
    temp.sd[0] = 0;
    UNREFERENCED( temp.sd[0] );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_high_8( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 8; i++)
        {
            temp.sh[i] = regs->VR_B(v2, i);
            if (temp.sh[i] & 0x0080) temp.sh[i] |= 0xFF00;
        }
        for (i = 0; i < 8; i++)
            regs->VR_H(v1, i) = temp.sh[i];
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_high_16( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 4; i++)
        {
            temp.sf[i] = regs->VR_H(v2, i);
            if (temp.sf[i] & 0x00008000) temp.sf[i] |= 0xFFFF0000;
        }
        for (i = 0; i < 4; i++)
            regs->VR_F(v1, i) = temp.sf[i];
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_unpack_high_32( regs->VR_Q(v2).v );
    #else
        for (i = 0; i < 2; i++)
        {
            temp.sd[i] = regs->VR_F(v2, i);
            if (temp.sd[i] & 0x0000000080000000ull) temp.sd[i] |= 0xFFFFFFFF00000000ull;
        }
        for (i = 0; i < 2; i++)
            regs->VR_D(v1, i) = temp.sd[i];
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D8 VTM    - Vector Test Under Mask                      [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_test_under_mask )
{
    int     v1, v2, m3, m4, m5;
    union   { U64 d[2]; } temp;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m3, m4, m5 are not part of this instruction */
    UNREFERENCED( m3 );
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    //note: V2 is mask
    temp.d[0] = regs->VR_D(v1,0) & regs->VR_D(v2,0);
    temp.d[1] = regs->VR_D(v1,1) & regs->VR_D(v2,1);

    // Selected bits all zeros; or all mask bits zero
    if ( temp.d[0] == 0 && temp.d[1] == 0 )
        regs->psw.cc = 0;

    // Selected bits all ones
    else if ( temp.d[0] == regs->VR_D(v2,0) && temp.d[1] == regs->VR_D(v2,1) )
        regs->psw.cc = 3;

    // Selected bits a mix of zeros and ones
    else
        regs->psw.cc = 1;

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7D9 VECL   - Vector Element Compare Logical              [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_compare_logical )
{
    int     v1, v2, m3, m4, m5;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
        if (regs->VR_B( v1, 7 ) == regs->VR_B( v2, 7 ))
            regs->psw.cc = 0;
        else if (regs->VR_B( v1, 7 ) < regs->VR_B( v2, 7 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 1:  /* Halfword */
        if (regs->VR_H( v1, 3 ) == regs->VR_H( v2, 3 ))
            regs->psw.cc = 0;
        else if (regs->VR_H( v1, 3 ) < regs->VR_H( v2, 3 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 2:  /* Word */
        if (regs->VR_F( v1, 1 ) == regs->VR_F( v2, 1 ))
            regs->psw.cc = 0;
        else if (regs->VR_F( v1, 1 ) < regs->VR_F( v2, 1 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 3:  /* Doubleword */
        if (regs->VR_D( v1, 0 ) == regs->VR_D( v2, 0 ))
            regs->psw.cc = 0;
        else if (regs->VR_D( v1, 0 ) < regs->VR_D( v2, 0 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7DB VEC    - Vector Element Compare                      [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_element_compare )
{
    int     v1, v2, m3, m4, m5;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
        if ((S8)regs->VR_B( v1, 7 ) == (S8)regs->VR_B( v2, 7 ))
            regs->psw.cc = 0;
        else if ((S8)regs->VR_B( v1, 7 ) < (S8)regs->VR_B( v2, 7 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 1:  /* Halfword */
        if ((S16)regs->VR_H( v1, 3 ) == (S16)regs->VR_H( v2, 3 ))
            regs->psw.cc = 0;
        else if ((S16)regs->VR_H( v1, 3 ) < (S16)regs->VR_H( v2, 3 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 2:  /* Word */
        if ((S32)regs->VR_F( v1, 1 ) == (S32)regs->VR_F( v2, 1 ))
            regs->psw.cc = 0;
        else if ((S32)regs->VR_F( v1, 1 ) < (S32)regs->VR_F( v2, 1 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    case 3:  /* Doubleword */
        if ((S64)regs->VR_D( v1, 0 ) == (S64)regs->VR_D( v2, 0 ))
            regs->psw.cc = 0;
        else if ((S64)regs->VR_D( v1, 0 ) < (S64)regs->VR_D( v2, 0 ))
            regs->psw.cc = 1;
        else
            regs->psw.cc = 2;
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7DE VLC    - Vector Load Complement                      [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_complement )
{
    int     v1, v2, m3, m4, m5;
    int     i;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    /* may be unreferenced */
    i = 0;
    UNREFERENCED( i );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_complement_8( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 16; i++)
            regs->VR_B( v1, i ) = ~(S8)regs->VR_B( v2, i ) + 1;
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_complement_16( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 8; i++)
            regs->VR_H( v1, i ) = ~(S16)regs->VR_H( v2, i ) + 1;
    #endif
            break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_complement_32( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 4; i++)
            regs->VR_F( v1, i ) = ~(S32)regs->VR_F( v2, i ) + 1;
    #endif
            break;
    case 3:  /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_complement_64( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 2; i++)
            regs->VR_D( v1, i ) = ~(S64)regs->VR_D( v2, i ) + 1;
    #endif
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7DF VLP    - Vector Load Positive                        [VRR-a] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_load_positive )
{
    int     v1, v2, m3, m4, m5;
    int     i;

    VRR_A( inst, regs, v1, v2, m3, m4, m5 );

    /* m4, m5 are not part of this instruction */
    UNREFERENCED( m4 );
    UNREFERENCED( m5 );

    ZVECTOR_CHECK( regs );

    switch (m3)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_positive_8( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 16; i++)
            regs->VR_B( v1, i ) = (S8)regs->VR_B( v2, i ) < 0 ?
                                        -((S8)regs->VR_B( v2, i )) :
                                        (S8)regs->VR_B( v2, i );
    #endif
        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_positive_16( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 8; i++)
            regs->VR_H( v1, i ) = (S16)regs->VR_H( v2, i ) < 0 ?
                                         -((S16)regs->VR_H( v2, i )) :
                                         (S16)regs->VR_H( v2, i );
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_positive_32( regs->VR_Q(v2).v );
    #else
        for (i=0; i < 4; i++)
            regs->VR_F( v1, i ) = (S32)regs->VR_F( v2, i ) < 0 ?
                                         -((S32)regs->VR_F( v2, i )) :
                                         (S32)regs->VR_F( v2, i );
    #endif
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
            regs->VR_D( v1, i ) = (S64)regs->VR_D( v2, i ) < 0 ?
                                         -((S64)regs->VR_D( v2, i )) :
                                         (S64)regs->VR_D( v2, i );
        break;
    default:
        ARCH_DEP(program_interrupt) (regs, PGM_SPECIFICATION_EXCEPTION);
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F0 VAVGL  - Vector Average Logical                      [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_average_logical )
{
    int     v1, v2, v3, m4, m5, m6;

    int i;                          /* loop index                    */
    U64 lsa, lsb;                   /* least significant 64-bits     */
    U64 msa;                        /* most significant 64-bits      */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    ZVECTOR_CHECK( regs );

    /* m5 and m5 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_average_logical_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = (U8) ( ( (U16) regs->VR_B(v2, i) + (U16) regs->VR_B(v3, i) + 1) >> 1 );
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_average_logical_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = (U16) ( ( (U32) regs->VR_H(v2, i) + (U32) regs->VR_H(v3, i) + 1) >> 1 );
        }
    #endif
        break;

    case 2:         /* Word */
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = (U32) ( ( (U64) regs->VR_F(v2, i) + (U64) regs->VR_F(v3, i) + 1) >> 1 );
        }
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            /* U128 a + U64 b */
            msa = 0;
            lsa = regs->VR_D(v2, i);
            lsb = regs->VR_D(v3, i);

            /* inline U128:  a + b */
            lsa += lsb;
            if ( lsa < lsb ) msa++;

            /* inline U128: a+1 */
            lsa += 1;
            if ( lsa < 1 ) msa++;

            /* shift right: average  */
            regs->VR_D(v1, i) = (lsa >> 1) | ( msa << 63 );
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F1 VACC   - Vector Add Compute Carry                    [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_add_compute_carry )
{
    int     v1, v2, v3, m4, m5, m6;
    union   { U64 d; } temp;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_compute_carry_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++)
        {
            regs->VR_B( v1, i ) = (U8) ( ( (U16)regs->VR_B( v2, i ) + (U16)regs->VR_B( v3, i ) ) >> 8 );
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_compute_carry_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++)
        {
            regs->VR_H( v1, i ) = (U16) ( ( (U32)regs->VR_H( v2, i ) + (U32)regs->VR_H( v3, i ) ) >> 16 );
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_compute_carry_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++)
        {
            regs->VR_F( v1, i ) = (U32) ( ( (U64)regs->VR_F( v2, i ) + (U64)regs->VR_F( v3, i ) ) >> 32 );
        }
    #endif
        break;
    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
        {
            temp.d = 0;
            temp.d += regs->VR_F( v2, (i*2)+1 );
            temp.d += regs->VR_F( v3, (i*2)+1 );
            temp.d >>= 32;
            temp.d += regs->VR_F( v2, i*2 );
            temp.d += regs->VR_F( v3, i*2 );
            regs->VR_D( v1, i ) = temp.d >> 32;
        }
        break;
    case 4:  /* Quadword */
        temp.d = 0;
        for (i=3; i >= 0; i--)
        {
            temp.d += regs->VR_F( v2, i );
            temp.d += regs->VR_F( v3, i );
            temp.d >>= 32;
        }
        regs->VR_D( v1, 0 ) = 0;
        regs->VR_D( v1, 1 ) = temp.d;
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F2 VAVG   - Vector Average                              [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_average )
{
    int     v1, v2, v3, m4, m5, m6;

    int     i;                      /* loop index                    */
    union   { S64 sd; } temp;       /* signed temp                   */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    ZVECTOR_CHECK( regs );

    /* m5 and m5 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_average_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = ( (S16) ( (S8) regs->VR_B(v2, i) + (S8) regs->VR_B(v3, i) ) + 1) >> 1;
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_average_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = ( (S32) ( (S16) regs->VR_H(v2, i) + (S16) regs->VR_H(v3, i) ) + 1) >> 1;
        }
    #endif
        break;

    case 2:         /* Word */
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = ( (S64) ( (S32) regs->VR_F(v2, i) + (S32) regs->VR_F(v3, i) ) + 1) >> 1;
        }
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            if  (
                    ( regs->VR_D(v2, i) & 0x8000000000000000ull )  ==
                    ( regs->VR_D(v3, i) & 0x8000000000000000ull )
                )
            {
                /* same signs: possible overflow */
                if  ( regs->VR_D(v2, i) & 0x8000000000000000ull )
                {
                    /* negative signs: allow overflow, round and force back to negative */
                    temp.sd = (S64) regs->VR_D(v2, i) + (S64) regs->VR_D(v3, i);
                    temp.sd++;
                    regs->VR_D(v1, i) = (U64) ( temp.sd >> 1 ) | 0x8000000000000000ull;
                }
                else
                {
                    /* positive signs: handle as U64 values */
                    regs->VR_D(v1, i) = (U64) ( ( (U64) regs->VR_D(v2, i) + (U64) regs->VR_D(v3, i) + 1) >> 1 );
                }
            }
            else
            {
                /* different signs */
                regs->VR_D(v1, i) = (U64) ( ( (S64) regs->VR_D(v2, i) + (S64) regs->VR_D(v3, i) + 1) >> 1 );
            }
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F3 VA     - Vector Add                                  [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST(vector_add)
{
    int     v1, v2, v3, m4, m5, m6, i;
    U128    temp;
#if defined( _MSVC_ )
    U128    copyv2, copyv3;
#endif

    VRR_C(inst, regs, v1, v2, v3, m4, m5, m6);

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* i maybe unreferenced */
    i = 0;
    UNREFERENCED( i );

    ZVECTOR_CHECK(regs);

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = (S8) regs->VR_B(v2, i) + (S8) regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = (S16) regs->VR_H(v2, i) + (S16) regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = (S32) regs->VR_F(v2, i) + (S32) regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:  /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_add_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = (S64) regs->VR_D(v2, i) + (S64) regs->VR_D(v3, i);
        }
    #endif
        break;

    case 4:  /* Quadword */
#if defined( _MSVC_ )
        copyv2.Q = regs->VR_Q(v2);
        copyv3.Q = regs->VR_Q(v3);
        temp = U128_add( copyv2, copyv3 );
#else
        temp = U128_add( (U128)regs->VR_Q(v2), (U128)regs->VR_Q(v3) );
#endif
        regs->VR_Q(v1) = temp.Q;
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END(regs);
}

/*-------------------------------------------------------------------*/
/* E7F5 VSCBI  - Vector Subtract Compute Borrow Indication   [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_subtract_compute_borrow_indication )
{
    int     v1, v2, v3, m4, m5, m6;
    int     i;

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* maybe unreferenced */
    i = 0;
    UNREFERENCED( i );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_compute_borrow_ind_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++)
            regs->VR_B( v1, i ) = (regs->VR_B( v2, i ) < regs->VR_B( v3, i )) ? 0 : 1;
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_compute_borrow_ind_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++)
            regs->VR_H( v1, i ) = (regs->VR_H( v2, i ) < regs->VR_H( v3, i )) ? 0 : 1;
    #endif
            break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_compute_borrow_ind_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++)
            regs->VR_F( v1, i ) = (regs->VR_F( v2, i ) < regs->VR_F( v3, i )) ? 0 : 1;
    #endif
        break;

    case 3:  /* Doubleword */
        for (i=0; i < 2; i++)
            regs->VR_D( v1, i ) = (regs->VR_D( v2, i ) < regs->VR_D( v3, i )) ? 0 : 1;
        break;

    case 4:  /* Quadword */
        if (regs->VR_D( v2, 0 ) == regs->VR_D( v3, 0 ))
        {
            regs->VR_D( v1, 1 ) = (regs->VR_D( v2, 1 ) < regs->VR_D( v3, 1 )) ? 0 : 1;
            regs->VR_D( v1, 0 ) = 0;
        }
        else
        {
            regs->VR_D( v1, 1 ) = (regs->VR_D( v2, 0 ) < regs->VR_D( v3, 0 )) ? 0 : 1;
            regs->VR_D( v1, 0 ) = 0;
        }
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F7 VS     - Vector Subtract                             [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST(vector_subtract)
{
    int     v1, v2, v3, m4, m5, m6, i;
    U128    temp;
#if defined( _MSVC_ )
    U128    copyv2, copyv3;
#endif

    VRR_C(inst, regs, v1, v2, v3, m4, m5, m6);

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    /* i maybe unreferenced */
    i = 0;
    UNREFERENCED( i );

    ZVECTOR_CHECK(regs);

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = (S8) regs->VR_B(v2, i) - (S8) regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = (S16) regs->VR_H(v2, i) - (S16) regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = (S32) regs->VR_F(v2, i) - (S32) regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:  /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_subtract_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = (S64)regs->VR_D(v2, i) - (S64)regs->VR_D(v3, i);
        }
    #endif
        break;

    case 4:  /* Quadword */
#if defined( _MSVC_ )
        copyv2.Q = regs->VR_Q(v2);
        copyv3.Q = regs->VR_Q(v3);
        temp = U128_sub( copyv2, copyv3 );
#else
        temp = U128_sub( (U128)regs->VR_Q(v2), (U128)regs->VR_Q(v3) );
#endif
        regs->VR_Q(v1) = temp.Q;
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END(regs);
}

/*-------------------------------------------------------------------*/
/* E7F8 VCEQ   - Vector Compare Equal                        [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_compare_equal )
{
    int     v1, v2, v3, m4, m5;
    int     i, el, eq = 0;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    /* may be unreferenced */
    i = el = eq = 0;
    UNREFERENCED(  i );
    UNREFERENCED( el );
    UNREFERENCED( eq );

    ZVECTOR_CHECK( regs );

#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    switch (m4)
    {
    case 0:  /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_equal_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=16, i=0; i < 16; i++) {
            if (regs->VR_B(v2, i) == regs->VR_B(v3, i)) {
                regs->VR_B(v1, i) = 0xff;
                eq++;
            }
            else {
                regs->VR_B(v1, i) = 0x00;
            }
        }
    #endif

        break;
    case 1:  /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_equal_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=8, i=0; i < 8; i++) {
            if (regs->VR_H(v2, i) == regs->VR_H(v3, i)) {
                regs->VR_H(v1, i) = 0xffff;
                eq++;
            }
            else {
                regs->VR_H(v1, i) = 0x0000;
            }
        }
    #endif
        break;
    case 2:  /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_equal_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=4, i=0; i < 4; i++) {
            if (regs->VR_F(v2, i) == regs->VR_F(v3, i)) {
                regs->VR_F(v1, i) = 0xFFFFFFFF;
                eq++;
            }
            else {
                regs->VR_F(v1, i) = 0x00000000;
            }
        }
    #endif
        break;
    case 3:  /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_equal_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=2, i=0; i < 2; i++) {
            if (regs->VR_D(v2, i) == regs->VR_D(v3, i)) {
                regs->VR_D(v1, i) = 0xFFFFFFFFFFFFFFFFull;
                eq++;
            }
            else {
                regs->VR_D(v1, i) = 0x0000000000000000ull;
            }
        }
    #endif
        break;
    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }
    #if defined( FEATURE_V128_SSE )
        if (M5_CS)
            regs->psw.cc = v128_compare_equal_cc( regs->VR_Q(v1).v );
    #else
        if (M5_CS) {
            if (eq == el)
                regs->psw.cc = 0;
            else if (eq != 0)
                regs->psw.cc = 1;
            else
                regs->psw.cc = 3;
        }
    #endif

#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7F9 VCHL   - Vector Compare High Logical                 [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_compare_high_logical )
{
    int     v1, v2, v3, m4, m5;
    int     i, el, hi = 0;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    /* may be unreferenced */
    i = el = hi = 0;
    UNREFERENCED(  i );
    UNREFERENCED( el );
    UNREFERENCED( hi );

    ZVECTOR_CHECK( regs );

#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_logical_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=16, i=0; i < 16; i++) {
            if (regs->VR_B(v2, i) > regs->VR_B(v3, i)) {
                regs->VR_B(v1, i) = 0xff;
                hi++;
            }
            else {
                regs->VR_B(v1, i) = 0x00;
            }
        }
    #endif
        break;

    case 1:        /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_logical_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=8, i=0; i < 8; i++) {
            if (regs->VR_H(v2, i) > regs->VR_H(v3, i)) {
                regs->VR_H(v1, i) = 0xffff;
                hi++;
            }
            else {
                regs->VR_H(v1, i) = 0x0000;
            }
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_logical_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=4, i=0; i < 4; i++) {
            if (regs->VR_F(v2, i) > regs->VR_F(v3, i)) {
                regs->VR_F(v1, i) = 0xFFFFFFFF;
                hi++;
            }
            else {
                regs->VR_F(v1, i) = 0x00000000;
            }
        }
    #endif
        break;

    case 3:        /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_logical_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=2, i=0; i < 2; i++) {
            if (regs->VR_D(v2, i) > regs->VR_D(v3, i)) {
                regs->VR_D(v1, i) = 0xFFFFFFFFFFFFFFFFull;
                hi++;
            }
            else {
                regs->VR_D(v1, i) = 0x0000000000000000ull;
            }
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    #if defined( FEATURE_V128_SSE )
        if (M5_CS)
            regs->psw.cc = v128_compare_high_cc( regs->VR_Q(v1).v );
    #else
        if (M5_CS) {
            if (hi == el)
                regs->psw.cc = 0;
            else if (hi != 0)
                regs->psw.cc = 1;
            else
                regs->psw.cc = 3;
        }
    #endif

#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7FB VCH    - Vector Compare High                         [VRR-b] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_compare_high )
{
    int     v1, v2, v3, m4, m5;
    int     i, el, hi = 0;

    VRR_B( inst, regs, v1, v2, v3, m4, m5 );

    /* may be unreferenced */
    i = el = hi = 0;
    UNREFERENCED(  i );
    UNREFERENCED( el );
    UNREFERENCED( hi );

    ZVECTOR_CHECK( regs );

#define M5_CS ((m5 & 0x1) != 0) // Condition Code Set

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=16, i=0; i < 16; i++) {
            if ( (S8) regs->VR_B(v2, i) > (S8) regs->VR_B(v3, i) ) {
                regs->VR_B(v1, i) = 0xff;
                hi++;
            }
            else {
                regs->VR_B(v1, i) = 0x00;
            }
        }
    #endif
        break;

    case 1:        /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=8, i=0; i < 8; i++) {
            if ( (S16) regs->VR_H(v2, i) > (S16) regs->VR_H(v3, i) ) {
                regs->VR_H(v1, i) = 0xffff;
                hi++;
            }
            else {
                regs->VR_H(v1, i) = 0x0000;
            }
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=4, i=0; i < 4; i++) {
            if ( (S32) regs->VR_F(v2, i) > (S32) regs->VR_F(v3, i) ) {
                regs->VR_F(v1, i) = 0xFFFFFFFF;
                hi++;
            }
            else {
                regs->VR_F(v1, i) = 0x00000000;
            }
        }
    #endif
        break;

    case 3:        /* Doubleword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_compare_high_64( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (el=2, i=0; i < 2; i++) {
            if ( (S64) regs->VR_D(v2, i) > (S64) regs->VR_D(v3, i) ) {
                regs->VR_D(v1, i) = 0xFFFFFFFFFFFFFFFFull;
                hi++;
            }
            else {
                regs->VR_D(v1, i) = 0x0000000000000000ull;
            }
        }
    #endif
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    #if defined( FEATURE_V128_SSE )
        if (M5_CS)
            regs->psw.cc = v128_compare_high_cc( regs->VR_Q(v1).v );
    #else
        if (M5_CS) {
            if (hi == el)
                regs->psw.cc = 0;
            else if (hi != 0)
                regs->psw.cc = 1;
            else
                regs->psw.cc = 3;
        }
    #endif

#undef M5_CS

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7FC VMNL   - Vector Minimum Logical                      [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_minimum_logical )
{
    int     v1, v2, v3, m4, m5, m6;

    int i;                          /* loop index                    */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_logical_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) =  regs->VR_B(v2, i) <= regs->VR_B(v3, i) ? regs->VR_B(v2, i) : regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_logical_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = regs->VR_H(v2, i) <= regs->VR_H(v3, i) ? regs->VR_H(v2, i) : regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_logical_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = regs->VR_F(v2, i) <= regs->VR_F(v3, i) ? regs->VR_F(v2, i) : regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = regs->VR_D(v2, i) <= regs->VR_D(v3, i) ? regs->VR_D(v2, i) : regs->VR_D(v3, i);
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7FD VMXL   - Vector Maximum Logical                      [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_maximum_logical )
{
    int     v1, v2, v3, m4, m5, m6;

    int i;                          /* loop index                    */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_logical_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) =  regs->VR_B(v2, i) >= regs->VR_B(v3, i) ? regs->VR_B(v2, i) : regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_logical_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = regs->VR_H(v2, i) >= regs->VR_H(v3, i) ? regs->VR_H(v2, i) : regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_logical_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = regs->VR_F(v2, i) >= regs->VR_F(v3, i) ? regs->VR_F(v2, i) : regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = regs->VR_D(v2, i) >= regs->VR_D(v3, i) ? regs->VR_D(v2, i) : regs->VR_D(v3, i);
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7FE VMN    - Vector Minimum                              [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_minimum )
{
    int     v1, v2, v3, m4, m5, m6;

    int i;                          /* loop index                    */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = (S8) regs->VR_B(v2, i) <= (S8) regs->VR_B(v3, i) ? regs->VR_B(v2, i) : regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = (S16) regs->VR_H(v2, i) <= (S16) regs->VR_H(v3, i) ? regs->VR_H(v2, i) : regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_minimum_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = (S32) regs->VR_F(v2, i) <= (S32) regs->VR_F(v3, i) ? regs->VR_F(v2, i) : regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = (S64) regs->VR_D(v2, i) <= (S64) regs->VR_D(v3, i) ? regs->VR_D(v2, i) : regs->VR_D(v3, i);
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }
    ZVECTOR_END( regs );
}

/*-------------------------------------------------------------------*/
/* E7FF VMX    - Vector Maximum                              [VRR-c] */
/*-------------------------------------------------------------------*/
DEF_INST( vector_maximum )
{
    int     v1, v2, v3, m4, m5, m6;

    int i;                          /* loop index                    */

    VRR_C( inst, regs, v1, v2, v3, m4, m5, m6 );

    /* m5, m6 are not part of this instruction */
    UNREFERENCED( m5 );
    UNREFERENCED( m6 );

    ZVECTOR_CHECK( regs );

    switch (m4)
    {
    case 0:         /* Byte */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_8( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 16; i++) {
            regs->VR_B(v1, i) = (S8) regs->VR_B(v2, i) >= (S8) regs->VR_B(v3, i) ? regs->VR_B(v2, i) : regs->VR_B(v3, i);
        }
    #endif
        break;

    case 1:         /* Halfword */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_16( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 8; i++) {
            regs->VR_H(v1, i) = (S16) regs->VR_H(v2, i) >= (S16) regs->VR_H(v3, i) ? regs->VR_H(v2, i) : regs->VR_H(v3, i);
        }
    #endif
        break;

    case 2:         /* Word */
    #if defined( FEATURE_V128_SSE )
        regs->VR_Q(v1).v = v128_maximum_32( regs->VR_Q(v2).v, regs->VR_Q(v3).v);
    #else
        for (i=0; i < 4; i++) {
            regs->VR_F(v1, i) = (S32) regs->VR_F(v2, i) >= (S32) regs->VR_F(v3, i) ? regs->VR_F(v2, i) : regs->VR_F(v3, i);
        }
    #endif
        break;

    case 3:         /* Doubleword */
        for (i=0; i < 2; i++) {
            regs->VR_D(v1, i) = (S64) regs->VR_D(v2, i) >= (S64) regs->VR_D(v3, i) ? regs->VR_D(v2, i) : regs->VR_D(v3, i);
        }
        break;

    default:
        ARCH_DEP( program_interrupt )( regs, PGM_SPECIFICATION_EXCEPTION );
        break;
    }

    ZVECTOR_END( regs );
}

#endif /* defined( FEATURE_129_ZVECTOR_FACILITY ) */

#if !defined( _GEN_ARCH )

  #if defined(              _ARCH_NUM_1 )
    #define   _GEN_ARCH     _ARCH_NUM_1
    #include "zvector.c"
  #endif

  #if defined(              _ARCH_NUM_2 )
    #undef    _GEN_ARCH
    #define   _GEN_ARCH     _ARCH_NUM_2
    #include "zvector.c"
  #endif

#endif /*!defined(_GEN_ARCH)*/
