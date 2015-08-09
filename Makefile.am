ACLOCAL_AMFLAGS           = -I m4

#EXTRA_DIST                = compile DecodeIR.html INSTALL pom.xml README
#EXTRA_DIST                += Makefile.Linux Makefile.MacOSX StdAfx.h

#include_HEADERS           = DecodeIRCaller.h DecodeIR.h

#lib_LTLIBRARIES           = libDecodeIR.la
#libDecodeIR_la_SOURCES    = DecodeIR.cpp
                          # Use -avoid-version to disable symlinks and SONAME.
#libDecodeIR_la_LDFLAGS    = -version-info 0:0:0

#javadir                   = src/main/java/com/hifiremote/decodeir

#dist-hook:
#	mkdir -p $(distdir)/$(javadir)
#	install -t $(distdir)/$(javadir) $(srcdir)/$(javadir)/*.java

SUBDIRS                 = src/main/native
