cmake_minimum_required(VERSION 2.6)
find_library(HAVE_LIBEVENT NAMES event)
if(HAVE_LIBEVENT)
	set(ExtraLibs ${ExtraLibs} ${HAVE_LIBEVENT})
	set(MODULE_EXTRA_LIBRARY ${ExtraLibs})
	set(MODULE_SOURCE_CODE Http.c)
	add_konoha_module(Http)
endif(HAVE_LIBEVENT)
