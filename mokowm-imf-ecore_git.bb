DESCRIPTION = "Ecore input method for the Mokosuite window manager"
HOMEPAGE = "http://gitorious.org/mokosuite2"
AUTHOR = "Daniele Ricci"
LICENSE = "GPLv3"
DEPENDS = "ecore eina"
SECTION = "misc/utils"

PV = "1.0.99+gitr${SRCPV}"
PR = "r1"
#SRCREV = "eaa525d2b2d570b117d8f4831ce69743e6e9eb69"

SRC_URI = "git://gitorious.org/mokosuite2/mokowm-imf-ecore.git;protocol=git"
S = "${WORKDIR}/mokowm-imf-ecore"

FILES_${PN}-dbg += "${libdir}/ecore/immodules/.debug"
FILES_${PN} += "${libdir}/ecore/immodules"

inherit autotools
