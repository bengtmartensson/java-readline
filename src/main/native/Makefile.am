# For the time being, just do not care for getline.
# (Does not work anyhow...)

DISTCLEANFILES              = $(BUILT_SOURCES)

BUILT_SOURCES               = org_gnu_readline_Readline.h

AM_CPPFLAGS                 = -I$(top_srcdir) -DJavaReadline

lib_LTLIBRARIES             = libJavaReadline.la

libJavaReadline_la_LDFLAGS  = -version-info 0:0:0
libJavaReadline_la_SOURCES  = org_gnu_readline_Readline.c


org_gnu_readline_Readline.h: $(top_srcdir)/target/classes/org/gnu/readline/Readline.class
	$(JAVAH) -classpath $(top_srcdir)/target/classes -jni org.gnu.readline.Readline
