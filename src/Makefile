#
# OMNeT++/OMNEST Makefile for $(LIB_PREFIX)detnetmod
#
# This file was generated with the command:
#  opp_makemake --make-so -f --deep -O out -KINET_PROJ=../../inet -KNESTING_PROJ=../../nesting -DINET_IMPORT -I$$\(NESTING_PROJ\)/src -I$$\(INET_PROJ\)/src -L$$\(INET_PROJ\)/src -L$$\(NESTING_PROJ\)/src -lINET$$\(D\) -lnesting$$\(D\)
#

# Name of target to be created (-o option)
TARGET = $(LIB_PREFIX)detnetmod$(D)$(SHARED_LIB_SUFFIX)
TARGET_DIR = .

# C++ include paths (with -I)
INCLUDE_PATH = -I$(NESTING_PROJ)/src -I$(INET_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET_PROJ)/src $(LDFLAG_LIBPATH)$(NESTING_PROJ)/src  -lINET$(D) -lnesting$(D)

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/detnetmod/applications/SetKind.o \
    $O/detnetmod/applications/SetKindNetPerf.o \
    $O/detnetmod/applications/UdpBasicAppRandomScheduled.o \
    $O/detnetmod/applications/UdpBasicAppTest.o \
    $O/detnetmod/applications/UdpSink.o \
    $O/detnetmod/applications/VlanEtherTrafGenFullLoadDetnet.o \
    $O/detnetmod/applications/VlanEtherTrafGenSchedDetnet.o \
    $O/detnetmod/dropper/mfd.o \
    $O/detnetmod/dropper/red.o \
    $O/detnetmod/ieee8021q/relay/macrelay.o \
    $O/detnetmod/ieee8021q/tunnel8021q/tunnel8021q.o \
    $O/detnetmod/linklayer/ethernet/VLAN8021qEncap.o \
    $O/detnetmod/networklayer/diffserv/DummyTC.o \
    $O/detnetmod/networklayer/diffserv/FrameForward.o \
    $O/detnetmod/networklayer/diffserv/PcpMarker.o \
    $O/detnetmod/networklayer/diffserv/TokenBucketClass.o \
    $O/detnetmod/queuing/DetnetQueuingFrames.o \
    $O/detnetmod/queuing/PifoDetnet.o \
    $O/detnetmod/queuing/QueueDetnet.o \
    $O/detnetmod/linklayer/common/VLANTag_m.o

# Message files
MSGFILES = \
    detnetmod/linklayer/common/VLANTag.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET_PROJ=../../inet
NESTING_PROJ=../../nesting

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(TOOLCHAIN_NAME),clangc2)
LIBS += -Wl,-rpath,$(abspath $(INET_PROJ)/src) -Wl,-rpath,$(abspath $(NESTING_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)