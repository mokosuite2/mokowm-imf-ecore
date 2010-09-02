#!/bin/bash
# use this script to compile a shr/OE application.
# THIS SCRIPT WILL DELETE "build" directory, whatever is inside it
# what must be customized:
# 1) those env variables below (they are self-explainatory)
# 2) header include paths in function compile_file
# 3) any code-generation tool like dbus-binding-tool (if any)
# 4) call compile_file <source.c> build/<source.o> for every source file
# 5) libraries to be linked, can be found in the final gcc line
#
# if the script is called with "nostrip", it doesn't execute strip on the final executable


set -e

BASE=/root/buildhost/shr-unstable/tmp
STAGE=$BASE/sysroots/armv4t-oe-linux-gnueabi
STAGE_NATIVE=$BASE/sysroots/i686-linux
EXENAME=mokowm-imf-module.so
#EXTRA_CFLAGS="-DQVGA"

rm -fR build
mkdir -p build

function compile_file() {
$STAGE_NATIVE/usr/armv4t/bin/arm-oe-linux-gnueabi-gcc -march=armv4t -mtune=arm920t -mthumb-interwork -mthumb \
-DPACKAGE_NAME=\"mokosuite\" -DPACKAGE_TARNAME=\"mokosuite\" -DPACKAGE_VERSION=\"0.1\" -DPACKAGE_STRING=\"mokosuite\ 0.1\" \
-DPACKAGE_BUGREPORT=\"daniele.athome@gmail.com\" -DPACKAGE_URL=\"\" -DPACKAGE=\"mokosuite\" -DCONTACTSDB_SQLITE $3 \
-DGETTEXT_PACKAGE=\"mokosuite\" -DHAVE_LC_MESSAGES=1 -DHAVE_BIND_TEXTDOMAIN_CODESET=1 -DHAVE_GETTEXT=1 -DHAVE_DCGETTEXT=1 \
-DENABLE_NLS=1 -DVERSION=\"0.1\" -DLOCALEDIR=\"/usr/share/locale\" -DSYSCONFDIR=\"/etc\" -DDATADIR=\"/usr/share\" -I. -I.. -Ibuild -Wall $EXTRA_CFLAGS \
-I$STAGE/usr/include \
-I$STAGE/usr/include/eet-1 \
-I$STAGE/usr/include/ecore-1 \
-I$STAGE/usr/include/eina-1 \
-I$STAGE/usr/include/eina-1/eina \
-I$STAGE/usr/include/glib-2.0 \
-I$STAGE/usr/lib/glib-2.0/include \
-I$STAGE/usr/include/freetype2 \
-I$STAGE/usr/include/lua5.1 \
-fexpensive-optimizations -fomit-frame-pointer -frename-registers -g -Os -c -o $2 $1
}

compile_file ilike_imf_context.c build/ilike_imf_context.o

#-fexpensive-optimizations -fomit-frame-pointer -frename-registers -Os 
$STAGE_NATIVE/usr/armv4t/bin/arm-oe-linux-gnueabi-gcc -g -march=armv4t -mtune=arm920t -mthumb-interwork -mthumb \
-fexpensive-optimizations -fomit-frame-pointer -frename-registers \
-o build/$EXENAME build/*.o \
-Wl,-soname -Wl,mokowm-imf-module.so -shared -Wl,-rpath-link,$STAGE/usr/lib -Wl,-O1 -Wl,--hash-style=gnu \
-L$STAGE/usr/lib \
-L$STAGE/lib \
-leina -lecore -lecore_imf


if [ "$1" != "nostrip" ]; then
$STAGE_NATIVE/usr/armv4t/bin/arm-oe-linux-gnueabi-strip build/$EXENAME
fi

echo "Compiled."
