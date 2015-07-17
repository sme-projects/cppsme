libincludedir = $(includedir)/libsme

lib_LTLIBRARIES = libsme.la

pkgconfigdir = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = libsme.pc

libinclude_HEADERS = sme.h cqueue.h bqueue.h threading.h runner.h queue_common.h

libsme_la_SOURCES = sme.cc sme.h threading.cc threading.h runner.cc runner.h cqueue.cc cqueue.h bqueue.cc bqueue.h queue_common.h
# frogger.cc simplecpu.cc adder.cc

CLEANFILES = libsme.pc.in libsme.pc libsme-uninstalled.pc libsme-uninstalled.sh
