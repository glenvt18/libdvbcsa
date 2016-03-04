AC_INIT(libdvbcsa, 1.1.0)
AC_PREREQ(2.50)

AC_CANONICAL_HOST

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug]), enable_debug=$enableval, enable_debug=no)

if test "$enable_debug" = "yes" ; then
    GCC_CFLAGS="$CFLAGS -g -DDVBCSA_DEBUG -D_XOPEN_SOURCE=600"
else
    GCC_CFLAGS="$CFLAGS -O3 -funroll-loops -fomit-frame-pointer -D_XOPEN_SOURCE=600"
fi

AC_ARG_ENABLE(uint32, AC_HELP_STRING(--enable-uint32, [Use native 32 bits integers for bitslice]), enable_uint32=$enableval, enable_uint32=no)
AC_ARG_ENABLE(uint64, AC_HELP_STRING(--enable-uint64, [Use native 64 bits integers for bitslice]), enable_uint64=$enableval, enable_uint64=no)
AC_ARG_ENABLE(mmx, AC_HELP_STRING(--enable-mmx, [Use MMX for bitslice]), mmx_debug=$enableval, enable_mmx=no)
AC_ARG_ENABLE(sse2, AC_HELP_STRING(--enable-sse2, [Use SSE2 for bitslice]), sse2_debug=$enableval, enable_sse2=no)
AC_ARG_ENABLE(ssse3, AC_HELP_STRING(--enable-ssse3, [Use SSSE3 for bitslice]), ssse3_debug=$enableval, enable_ssse3=no)
AC_ARG_ENABLE(altivec, AC_HELP_STRING(--enable-altivec, [Use AltiVec for bitslice]), altivec_debug=$enableval, enable_altivec=no)
AC_ARG_ENABLE(neon, AC_HELP_STRING(--enable-neon, [Use NEON for bitslice]), neon_debug=$enableval, enable_neon=no)

AM_INIT_AUTOMAKE(libdvbcsa, 1.1.0)
AC_CONFIG_HEADERS(config.h)

AC_PROG_CC

AM_PROG_LIBTOOL 

AC_LANG(C)

AC_STDC_HEADERS
AC_CHECK_HEADERS(assert.h)

AC_C_CONST
AC_C_INLINE
AC_CHECK_SIZEOF(long)

AC_C_BIGENDIAN(
 AC_DEFINE(DVBCSA_ENDIAN_BIG, 1, [Target is big-endian]),
 AC_DEFINE(DVBCSA_ENDIAN_LITTLE, 1, [Target is little-endian]),
 AC_MSG_ERROR(unknown endianess),
 AC_MSG_ERROR(universial endianess not supported)
)

if test "$enable_mmx" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_MMX, 1, Using MMX bitslice.)
     GCC_CFLAGS="$GCC_CFLAGS -mmmx"

elif test "$enable_sse2" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_SSE, 1, Using SSE2 bitslice.)
     GCC_CFLAGS="$GCC_CFLAGS -msse -msse2"

elif test "$enable_ssse3" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_SSSE3, 1, Using SSSE3 bitslice.)
     GCC_CFLAGS="$GCC_CFLAGS -mssse3"

elif test "$enable_altivec" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_ALTIVEC, 1, Using AltiVec bitslice.)
     GCC_CFLAGS="$GCC_CFLAGS -maltivec -mabi=altivec"

elif test "$enable_neon" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_NEON, 1, Using NEON bitslice.)
     case $host_cpu in
         arm64*|aarch64*) ;;
         arm*) GCC_CFLAGS="$GCC_CFLAGS -mfpu=neon" ;;
     esac

elif test "$enable_uint32" = "yes" ; then
     transpose_32=yes
     AC_DEFINE(DVBCSA_USE_UINT32, 1, Using 32 bits integer bitslice.)

elif test "$enable_uint64" = "yes" ; then
     AC_DEFINE(DVBCSA_USE_UINT64, 1, Using 64 bits integer bitslice.)

else
     case $ac_cv_sizeof_long in
     	  8)
	       AC_DEFINE(DVBCSA_USE_UINT64, 1, Using 64 bits integer bitslice.)
	  ;;
	  *)
	       transpose_32=yes
	       AC_DEFINE(DVBCSA_USE_UINT32, 1, Using 32 bits integer bitslice.)
     	  ;;
     esac
fi

AM_CONDITIONAL(TRANSPOSE_32, test "$transpose_32" = "yes")

if test "$GCC" = "yes" ; then
    CFLAGS="-Wall $GCC_CFLAGS"
fi

AC_CHECK_FUNC(posix_memalign, AC_DEFINE(HAVE_POSIX_MEMALIGN, 1, posix_memalign is available))

AC_MSG_CHECKING([_mm_malloc availability])
AC_TRY_LINK([#include <mm_malloc.h>], [_mm_malloc(8,8);],
    [
	AC_DEFINE(HAVE_MM_MALLOC, 1, _mm_malloc is available)
	AC_MSG_RESULT([found])
    ],[
	AC_MSG_RESULT([no])
    ])

AC_OUTPUT([
  Makefile
  src/Makefile
  src/dvbcsa/Makefile
  test/Makefile
])

