#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper
#* of GNU readline
#*
#* Copyright (c) 1987-1998 Free Software Foundation, Inc.
#* Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
#*
#* This program is free software; you can redistribute it and/or modify
#* it under the terms of the GNU Library General Public License as published
#* by  the Free Software Foundation; either version 2 of the License or
#* (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful, but
#* WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU Library General Public License for more details.
#*
#* You should have received a copy of the GNU Library General Public License
#* along with this program; see the file COPYING.LIB.  If not, write to
#* the Free Software Foundation Inc., 59 Temple Place - Suite 330,
#* Boston, MA  02111-1307 USA
#***************************************************************************
#
# Toplevel Makefile for Java-Readline
#
# $Author: Bablokb $
# $Revision: 1.11 $
#

TARGET    = java_readline
README    = README README.1st
CHANGELOG = ChangeLog
LICENSE   = COPYING.LIB
TODO      = TODO
NAME      = The Java-Readline Library
HOMEPAGE  = http://www.bablokb.de/java/readline.html
COPYRIGHT = Released under the LGPL, (c) Bernhard Bablok 1998-2001
WTITLE    = "$(NAME)"
DTITLE    = "$(NAME), Version $(VERSION)"
DBOTTOM   = "$(COPYRIGHT)<br>Homepage: <a href="$(HOMEPAGE)">$(HOMEPAGE)</a>"
DHEADER   = "<strong>$(NAME), Version $(VERSION)</strong>"
DFOOTER   = "<strong>$(NAME), Version $(VERSION)</strong>"
BIN_ADD   = $(README) $(TODO) $(CHANGELOG) $(LICENSE) \
             $(JAR) *.so $(APIDIR)
SRC_ADD   = $(README) $(TODO) $(CHANGELOG) $(LICENSE) \
             Makefile VERSION $(SUBDIRS) contrib src etc
MF_STUB   = etc/manifest.stub

# this might be /usr/src/RPM or /usr/src/packages, depending on the distrib
RPM_BASE  = /usr/src/redhat

# libraries to build
T_LIBS    = JavaReadline JavaEditline

# Operating system dependent
JAVAINCLUDE       = $(JAVA_HOME)/include
JAVANATINC        = $(JAVA_HOME)/include/linux

## normal javac
JAVAC = javac
JC_FLAGS = 

## with jikes
#JAVAC = jikes
#JC_FLAGS = -O +E

VERSION         = `cat VERSION`
JAR             = $(TARGET).jar
APIDIR          = ./api
BUILDDIR        = ./build

world : jar build-native

jar: build-java
	cd $(BUILDDIR) ; jar -cvmf ../$(MF_STUB) ../$(JAR) *

build-java: $(BUILDDIR)
	cd src ; $(MAKE) JAVAC="$(JAVAC)" JC_FLAGS="$(JC_FLAGS)" java

build-native: 
	cd src; $(MAKE) T_LIBS="$(T_LIBS)" JAVAINCLUDE="$(JAVAINCLUDE)" \
		        JAVANATINC="$(JAVANATINC)" native

apidoc: $(APIDIR)
	javadoc -sourcepath src -d $(APIDIR) -windowtitle $(WTITLE) \
                -doctitle $(DTITLE) -footer $(DFOOTER) -header $(DHEADER) \
                -bottom $(DBOTTOM) \
                -version -author `find src -name "*.java"` 

bin-dist: jar build-native apidoc
	mkdir -p "$(TARGET)-$(VERSION)"
	-cp -r $(BIN_ADD) "$(TARGET)-$(VERSION)"
	tar -czf $(TARGET)-$(VERSION)-bin.tar.gz --exclude "CVS" "$(TARGET)-$(VERSION)"
	rm -rf "$(TARGET)-$(VERSION)"

src-dist: clean
	mkdir -p "$(TARGET)-$(VERSION)"
	-cp -r $(SRC_ADD) "$(TARGET)-$(VERSION)"
	tar -czf $(TARGET)-$(VERSION)-src.tar.gz --exclude "CVS" "$(TARGET)-$(VERSION)"
	rm -rf "$(TARGET)-$(VERSION)"

$(APIDIR):
	mkdir $(APIDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(METADIR): 
	mkdir $(METADIR)

rpm: src-dist
	cp etc/libreadline-java.spec $(RPM_BASE)/SPECS
	cp $(TARGET)-$(VERSION)-src.tar.gz $(RPM_BASE)/SOURCES
	rpm -ba $(RPM_BASE)/SPECS/libreadline-java.spec

test: jar build-native
	LD_LIBRARY_PATH=. java -jar $(JAR) src/test/tinputrc

clean:
	-rm -fr `find . -name "*.o" -o -name "*.h" -o -name "*~"` \
		$(JAR) $(TARGET)-*.tar.*z* $(APIDIR) \
		$(BUILDDIR) *.so .rltest_history
