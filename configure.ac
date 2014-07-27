AC_INIT([moxiebox], [0.0.2],
        [http://github.com/jgarzik/moxiebox/])

AC_PREREQ(2.52)
AC_CONFIG_SRCDIR([sandbox.cc])
AM_INIT_AUTOMAKE([gnu])
AC_CONFIG_HEADERS([moxiebox-config.h])


AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_OBJC
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_SED
AM_PROG_AS
AC_PATH_TOOL(AR, ar)
AC_PATH_TOOL(RANLIB, ranlib)
AC_PATH_TOOL(STRIP, strip)
AC_CHECK_PROG(MOX_AS, moxie-elf-as, moxie-elf-as)
AC_CHECK_PROG(MOX_AR, moxie-elf-ar, moxie-elf-ar)
AC_CHECK_PROG(MOX_GCC, moxie-elf-gcc, moxie-elf-gcc)
PKG_PROG_PKG_CONFIG

AC_LANG_PUSH([C++])

AC_CHECK_LIB(elf, elf_getphdrnum, ELF_LIBS=-lelf,AC_MSG_ERROR([Libelf is required to build this package!]))
AC_CHECK_LIB(crypto, SHA256_Update, CRYPTO_LIBS=-lcrypto,AC_MSG_ERROR([Libcrypto is required to build this package!]))

AC_SUBST(ELF_LIBS)
AC_SUBST(CRYPTO_LIBS)
AC_SUBST(MOX_AS)
AC_SUBST(MOX_AR)
AC_SUBST(MOX_GCC)

AC_CONFIG_FILES([Makefile runtime/Makefile])
AC_OUTPUT

