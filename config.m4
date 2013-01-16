dnl $Id$
dnl config.m4 for extension song

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(song, for song support,
dnl Make sure that the comment is aligned:
dnl [  --with-song             Include song support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(song, whether to enable song support,
Make sure that the comment is aligned:
[  --enable-song           Enable song support])

if test "$PHP_SONG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-song -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/song.h"  # you most likely want to change this
  dnl if test -r $PHP_SONG/$SEARCH_FOR; then # path given as parameter
  dnl   SONG_DIR=$PHP_SONG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for song files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SONG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SONG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the song distribution])
  dnl fi

  dnl # --with-song -> add include path
  dnl PHP_ADD_INCLUDE($SONG_DIR/include)

  dnl # --with-song -> check for lib and symbol presence
  dnl LIBNAME=song # you may want to change this
  dnl LIBSYMBOL=song # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SONG_DIR/lib, SONG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SONGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong song lib version or lib not found])
  dnl ],[
  dnl   -L$SONG_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SONG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(song, song.c, $ext_shared)
fi
