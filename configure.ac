AC_PREREQ(2.53)
AC_INIT([gogo_lemmatizer], [1.0], [jdkrot@gmail.com])
AM_INIT_AUTOMAKE

LT_INIT
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])
m4_include([m4.incl/ax_path_lib_pcre.m4])

AX_PATH_LIB_PCRE([], AC_MSG_ERROR([Could not find libpcre]))

AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([localtime_r])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strspn])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([iconv_open])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/timeb.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([iconv.h])


PREFIX=$prefix
test "x${PREFIX}" = xNONE && PREFIX=${ac_default_prefix}

RMLROOT="$PREFIX/lemmatizer"
AC_SUBST(RMLROOT)

COMPAT_DIR="/usr/lemmatizer"
AC_SUBST(COMPAT_DIR)

AC_OUTPUT( Source/AgramtabLib/Makefile \
           Source/GraphanLib/Makefile \
           Source/Interfaces/cpp/Makefile \
           Source/Interfaces/c/Makefile
           Source/LemmatizerLib/Makefile \
           Source/MorphGen/Makefile \
           Source/MorphWizardLib/Makefile \
           Source/StructDictLib/Makefile \
           Source/RegressionTest/Makefile \
           Makefile \
           pkgconfig/lemmatizer.pc \
         )

AC_MSG_NOTICE([ ])
AC_MSG_NOTICE([ "Lemmatizer will be installed to : ${RMLROOT}" ])
