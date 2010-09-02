MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = -Wall @EDEPS_CFLAGS@

immoduledir = $(libdir)/ecore/immodules
immodule_LTLIBRARIES = mokowm-imf-module.la

mokowm_imf_module_la_SOURCES = ilike_imf_context.c

mokowm_imf_module_la_LDFLAGS = -avoid-version -module

mokowm_imf_module_la_LIBADD = @EDEPS_LIBS@
