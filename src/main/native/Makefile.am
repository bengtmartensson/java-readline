# ACLOCAL_AMFLAGS             = -I m4

#DISTCLEANFILES              = input_map.inc lirc/input_map.inc
#CLEANFILES                  = driver_api.dox lirc_client.dox
#EXTRA_DIST                  = input_map.inc input_map.sh \
#                              driver_api.dox  lirc_client.dox  mainpage.dox \
#                              driver_api.doxhead  lirc_client.doxhead

BUILT_SOURCES               = org_gnu_readline_Readline.h

AM_CPPFLAGS                 = -I$(top_srcdir) -DJavaReadline

lib_LTLIBRARIES             = libJavaReadline.la

libJavaReadline_la_LDFLAGS  =
libJavaReadline_la_SOURCES  = org_gnu_readline_Readline.c


org_gnu_readline_Readline.h: $(top_srcdir)/target/classes/org/gnu/readline/Readline.class
	$(JAVAH) -classpath $(top_srcdir)/target/classes -jni org.gnu.readline.Readline
#	touch org_gnu_readline_Readline.h
