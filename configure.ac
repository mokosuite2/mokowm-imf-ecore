AC_INIT([mokowm-imf-ecore], [1.0.99], [daniele.athome@gmail.com], [mokowm-imf-ecore])
AC_CONFIG_SRCDIR(Makefile.am)
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")
AC_PROG_CC

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

PKG_CHECK_MODULES(EFL,
    eina
    ecore
    ecore-imf
    ecore-x
    ecore-evas
    evas
)
AC_SUBST(EFL_CFLAGS)
AC_SUBST(EFL_LIBS)


AC_OUTPUT([
Makefile
src/Makefile
])
