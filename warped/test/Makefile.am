# Process this file with automake to produce Makefile.in

# See copyright notice in file Copyright in the root directory of this archive.

include $(top_srcdir)/Makefile.am.common
# Ping pong stuff
include pingPong/Makefile.am.stub
# Unit test stuff
include warped/Makefile.am.stub
# Adaptive test stuff
include adaptTest/Makefile.am.stub
# PHOLD Stuff
include phold/Makefile.am.stub
# Raid stuff
include raid/Makefile.am.stub
# Random stuff
include rnd/Makefile.am.stub
# SMMP Stuff
include smmp/Makefile.am.stub

noinst_PROGRAMS = $(PING_PONG_PROGRAM) $(UNIT_TEST_PROGRAM) $(ADAPT_TEST_PROGRAM) $(PHOLD_PROGRAM) $(RAID_PROGRAM) $(SMMP_PROGRAM)
