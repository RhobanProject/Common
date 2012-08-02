BASE=../..
include Makefile.sources
include $(BASE)/Makefile.param

SRCPP = $(COMMON_COMM_SRC)

ifeq ($(PLATFORM), win32)
LIBS +=	-lws2_32
endif

include $(BASE)/Makefile.generic
