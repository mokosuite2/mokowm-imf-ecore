MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = -Wall $(EFL_CFLAGS)

immoduledir = $(libdir)/ecore/immodules
immodule_LTLIBRARIES = mokowm-imf-module.la

mokowm_imf_module_la_SOURCES = mokowm_imf_context.c

mokowm_imf_module_la_LDFLAGS = -avoid-version -module
mokowm_imf_module_la_LIBADD = $(EFL_LIBS)
