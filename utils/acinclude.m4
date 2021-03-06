dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl
dnl Check for pccts programs (antlr and dlg) and header files needed
dnl by files generated by these tools
dnl
dnl Usage: CL_PROG_PCCTS
dnl
AC_DEFUN([CL_PROG_PCCTS],
[
AC_SUBST(ANTLR)
AC_SUBST(DLG)
AC_SUBST(PCCTSROOT)

AC_PATH_PROG(ANTLR, antlr, no)
if test $ANTLR = no; then
AC_MSG_ERROR( Configure could not locate the program \"antlr\" in your
path.  This program is needed to build the warped system.  It should
be installed as part of the PCCTS system which is available for free
via anonymous ftp from ftp.parr-research.com in the directory
/pub/pccts.  If you have installed PCCTS please verify that \"antlr\"
is in your path which is currently:
$PATH)
fi

AC_PATH_PROG(DLG, dlg, no)
if test $DLG = no; then
AC_MSG_ERROR( Configure could not locate the program \"dlg\" in your
path.  This program is needed to build the warped system.  It should
be installed as part of the PCCTS system which is available for free
via anonymous ftp from ftp.parr-research.com in the directory
/pub/pccts.  If you have installed PCCTS please verify that \"dlg\"
is in your path which is currently:
$PATH)
fi

AC_CACHE_CHECK([for PCCTS includes], [cl_cv_path_pcctsheaders],
  [AC_ARG_WITH(pccts-headers,
    AC_HELP_STRING([--with-pccts-headers],
      [specify location of PCCTS headers (default is /usr/include/pccts)]),
    [
      case "${withval}" in
        yes)  cl_cv_path_pcctsheaders=/usr/include/pccts ;;
        no)   cl_cv_path_pcctsheaders=no ;;
        *)    cl_cv_path_pcctsheaders=$withval ;;
      esac
    ],
    [cl_cv_path_pcctsheaders=/usr/include/pccts]
  )]
)

if test $cl_cv_path_pcctsheaders = no; then
AC_MSG_ERROR(PCCTS includes required.  Please specify the location with
the --with-pccts-headers option.)
fi
AC_MSG_CHECKING(for PCCTS headers)
if test -f $cl_cv_path_pcctsheaders/AParser.cpp ; then
AC_MSG_RESULT(yes)
PCCTSROOT=$cl_cv_path_pcctsheaders
else
AC_MSG_RESULT(no)
AC_MSG_ERROR(Could not find PCCTS headers in $cl_cv_path_pcctsheaders.
Please specify the correct location with the --with-pccts-headers option.)
fi

]) dnl end CL_PROG_PCCTS
