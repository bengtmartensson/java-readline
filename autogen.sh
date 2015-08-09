#!/bin/sh

rm -rf aclocal.m4 config.guess config.h.in* config.sub configure depcomp install-sh install.sh
rm -rf ltmain.sh missing autom4te.cache

autoreconf -fi
