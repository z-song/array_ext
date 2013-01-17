dnl $Id$
dnl config.m4 for extension song

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(array_ext, for array_ext support,
dnl Make sure that the comment is aligned:
dnl [  --with-array_ext      Include array_ext support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(array_ext, whether to enable array_ext support,
Make sure that the comment is aligned:
[  --enable-array_ext           Enable array_ext support])

if test "$PHP_ARRAY_EXT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-array_ext -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/array_ext.h"  # you most likely want to change this
  dnl if test -r $PHP_ARRAY_EXT/$SEARCH_FOR; then # path given as parameter
  dnl   SONG_DIR=$PHP_ARRAY_EXT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for array_ext files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SONG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ARRAY_EXT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the array_ext distribution])
  dnl fi

  dnl # --with-array_ext -> add include path
  dnl PHP_ADD_INCLUDE($ARRAY_EXT_DIR/include)

  dnl # --with-array_ext -> check for lib and symbol presence
  dnl LIBNAME=array_ext # you may want to change this
  dnl LIBSYMBOL=array_ext # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ARRAY_EXT_DIR/lib, ARRAY_EXT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ARRAY_EXTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong array_ext lib version or lib not found])
  dnl ],[
  dnl   -L$ARRAY_EXT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ARRAY_EXT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(array_ext, array_ext.c, $ext_shared)
fi
