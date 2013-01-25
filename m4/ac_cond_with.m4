dnl @synopsis AC_COND_WITH(PACKAGE [,DEFAULT])
dnl
dnl obsoleted - do not use this macro, use AX_SUBST_WITH or AM_CONDITIONAL
dnl
dnl actually used after an AC_ARG_WITH(PKG,...) option-directive,
dnl where AC_ARG_WITH is a part of the standard autoconf to define a
dnl `configure` --with-PKG option.
dnl
dnl the AC_COND_WITH(PKG) will use the $with_PKG var to define WITH_PKG and
dnl WITHOUT_PKG substitutions (AC_SUBST), that are either '' or '#' -
dnl depending whether the var was "no" or not (probably 'yes', or a value);
dnl it will also declare WITHVAL_PKG for use when someone wanted to set a val
dnl other than just "yes". And there is a WITHDEF_PKG that expands to a
dnl C-precompiler definition of the form -DWITH_PKG or  -DWITH_PKG=\"value\"
dnl (n.b.: the PKG *is*  uppercased if in lowercase and "-" translit to "_")
dnl
dnl this macro is most handily in making Makefile.in/Makefile.am that have
dnl a set of if-with declarations that can be defined as follows:
dnl
dnl  CFLAGS = -Wall @WITHOUT_FLOAT@ -msoft-float # --without-float
dnl  @WITH_FLOAT@ LIBS += -lm              # --with-float
dnl  DEFS += -DNDEBUG @WITHDEF_MY_PKG@     # --with-my-pkg="/usr/lib"
dnl  DEFS += @WITHVAL_DEFS@                # --with-defs="-DLOGLEVEL=6"
dnl
dnl Example configure.in:
dnl
dnl  AC_ARG_WITH(float,
dnl  [ --with-float,       with float words support])
dnl  AC_COND_WITH(float,no)
dnl
dnl Extened notes:
dnl
dnl 1. the idea comes from AM_CONDITIONAL but it is much easier to use,
dnl and unlike automake's ifcond, the Makefile.am will work as a
dnl normal $(MAKE) -f Makefile.am makefile.
dnl
dnl 2. the @VALS@ are parsed over by automake so automake will see all
dnl the filenames and definitions that follow @WITH_FLOAT@, so that
dnl the AC_COND_WITH user can see additional message if they apply.
dnl
dnl 3. in this m4-part, there's a AC_ARG_COND_WITH with the synopsis of
dnl AC_ARG_WITH and an implicit following AC_COND_WITH   =:-)
dnl
dnl 4. and there is an AC_ARG_COND_WITH_DEFINE that will emit an
dnl implicit AC_DEFINE that is actually seen by autoheader, even
dnl generated with the correct name and comment, for config.h.in
dnl
dnl some non-autoconf coders tend to create "editable" Makefile where
dnl they have out-commented lines with an example (additional) definition.
dnl Each of these can be replaced with a three-liner in configure.in as
dnl shown above. Starting to use AC_COND_WITH will soon lead you to
dnl provide a dozen --with-option rules for the `configure` user. Do it!
dnl
dnl @version $Id: ac_cond_with.m4,v 1.3 2003/06/05 00:56:52 guidod Exp $
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AC_COND_WITH],
[dnl the names to be defined...
pushdef([WITH_VAR],    patsubst([with_$1], -, _))dnl
pushdef([VAR_WITH],    patsubst(translit([with_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHOUT], patsubst(translit([without_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHVAL], patsubst(translit([withval_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHDEF], patsubst(translit([withdef_$1], [a-z], [A-Z]), -, _))dnl
AC_SUBST(VAR_WITH)
AC_SUBST(VAR_WITHOUT)
AC_SUBST(VAR_WITHVAL)
AC_SUBST(VAR_WITHDEF)
if test -z "$WITH_VAR" ; then WITH_VAR=`echo ifelse([$2], , no, [$2])` ; fi
if test "$WITH_VAR" != "no"; then
  VAR_WITH=    ; VAR_WITHOUT='#'
  case "$WITH_VAR" in
    [yes)]    VAR_WITHVAL=""
              VAR_WITHDEF="-D""VAR_WITH" ;;
    [*)]      VAR_WITHVAL=WITH_VAR
              VAR_WITHDEF="-D""VAR_WITH="'"'$WITH_VAR'"' ;;
  esac
else
  VAR_WITH='#' ;  VAR_WITHOUT=
  VAR_WITHVAL= ;  VAR_WITHDEF=
fi
popdef([VAR_WITH])dnl
popdef([VAR_WITHOUT])dnl
popdef([VAR_WITHVAL])dnl
popdef([VAR_WITHDEF])dnl
popdef([WITH_VAR])dnl
])

AC_DEFUN([AC_ARG_COND_WITH],
[dnl
AC_ARG_WITH([$1],[$2],[$3],[$4],[$5])
# done with AC_ARG_WITH, now do AC_COND_WITH (rather than AM_CONDITIONAL)
AC_COND_WITH([$1])
])

dnl and the same version as AC_COND_WITH but including the
dnl AC_DEFINE for WITH_PACKAGE

AC_DEFUN([AC_COND_WITH_DEFINE],
[dnl the names to be defined...
pushdef([WITH_VAR],    patsubst([with_$1], -, _))dnl
pushdef([VAR_WITH],    patsubst(translit([with_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHOUT], patsubst(translit([without_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHVAL], patsubst(translit([withval_$1], [a-z], [A-Z]), -, _))dnl
pushdef([VAR_WITHDEF], patsubst(translit([withdef_$1], [a-z], [A-Z]), -, _))dnl
AC_SUBST(VAR_WITH)
AC_SUBST(VAR_WITHOUT)
AC_SUBST(VAR_WITHVAL)
AC_SUBST(VAR_WITHDEF)
if test -z "$WITH_VAR" ; then WITH_VAR=`echo ifelse([$2], , no, [$2])` ; fi
if test "$WITH_VAR" != "no"; then
  VAR_WITH=    ; VAR_WITHOUT='#'
  case "$WITH_VAR" in
    [yes)]    VAR_WITHVAL=""
              VAR_WITHDEF="-D""VAR_WITH" ;;
    [*)]      VAR_WITHVAL=WITH_VAR
              VAR_WITHDEF="-D""VAR_WITH="'"'$WITH_VAR'"' ;;
  esac
else
  VAR_WITH='#' ;  VAR_WITHOUT=
  VAR_WITHVAL= ;  VAR_WITHDEF=
fi
if test "_$WITH_VAR" != "_no" ; then
      AC_DEFINE_UNQUOTED(VAR_WITH, "$WITH_VAR", "--with-$1")
fi dnl
popdef([VAR_WITH])dnl
popdef([VAR_WITHOUT])dnl
popdef([VAR_WITHVAL])dnl
popdef([VAR_WITHDEF])dnl
popdef([WITH_VAR])dnl
])

