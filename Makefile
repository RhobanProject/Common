###############################################################################
BASE=..
include Makefile.sources

SUBDIRS = $(COMMON_SUBDIRS)

include $(BASE)/Makefile.param
include $(BASE)/Makefile.generic
###############################################################################

