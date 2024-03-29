ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

abs_top_srcdir=@r_abs_top_srcdir@
abs_top_builddir=@r_abs_top_builddir@

X_FILES = \
	include/hotstuff/xdr/types.x \
	hotstuff/xdr/hotstuff.x

XH_FILES = $(X_FILES:.x=.h)

BUILT_SOURCES = $(XH_FILES)

# main srcs

lib_LIBRARIES = libhotstuff.a

pkginclude_HEADERS = \
	include/hotstuff/config/replica_config.h \
	include/hotstuff/hotstuff_app.h \
	include/hotstuff/hotstuff_configs.h \
	include/hotstuff/liveness.h \
	include/hotstuff/log_access_wrapper.h \
	include/hotstuff/manage_data_dirs.h \
	include/hotstuff/vm/vm_base.h \
	include/hotstuff/vm/vm_block_id.h \
	include/hotstuff/xdr/types.h 

EXTRA_DIST = \
	.gitignore \
	autogen.sh \
	include/hotstuff/xdr/types.x

CLEANFILES = *~ */*~ */*/*~ .gitignore~ include/hotstuff/xdr/types.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = hotstuff.pc

HOTSTUFF_SRCS = \
	hotstuff/block.cc \
	hotstuff/block_storage/io_utils.cc \
	hotstuff/block_storage/block_fetch_server.cc \
	hotstuff/block_storage/block_fetch_manager.cc \
	hotstuff/block_storage/block_fetch_worker.cc \
	hotstuff/block_storage/block_store.cc \
	hotstuff/block_storage/garbage_collector.cc \
	hotstuff/config/replica_config.cc \
	hotstuff/consensus.cc \
	hotstuff/crypto/crypto_utils.cc \
	hotstuff/crypto/certs.cc \
	hotstuff/event.cc \
	hotstuff/event_queue.cc \
	hotstuff/hotstuff.cc \
	hotstuff/hotstuff_app.cc \
	hotstuff/log_access_wrapper.cc \
	hotstuff/lmdb.cc \
	hotstuff/manage_data_dirs.cc \
	hotstuff/network_event.cc \
	hotstuff/network_event_queue.cc \
	hotstuff/protocol/hotstuff_protocol_client.cc \
	hotstuff/protocol/hotstuff_protocol_manager.cc \
	hotstuff/protocol/hotstuff_server.cc \
	hotstuff/vm/hotstuff_vm_bridge.cc \
	hotstuff/vm/nonspeculative_vm_bridge.cc \
	hotstuff/vm/vm_control_interface.cc

EXAMPLES_SRCS = \
	examples/vm/counting_vm.cc 

HOTSTUFF_TEST_SRCS = \
	hotstuff/vm/tests/test_speculative_exec_gadget.cc \
	hotstuff/vm/tests/test_counting_vm.cc \
	test_utils/keygen.cc

libhotstuff_a_SOURCES = $(HOTSTUFF_SRCS)

MAIN_CCS = \
	main/test.cc

SRCS = \
	$(HOTSTUFF_SRCS)

TEST_SRCS = \
	$(HOTSTUFF_TEST_SRCS)

EXTRA_SRCS = \
	$(EXAMPLES_SRCS) \
	$(TEST_SRCS)

$(SRCS:.cc=.o): CXXFLAGS += -I./include
$(EXTRA_SRCS:.cc=.o) : CXXFLAGS += -I./include
$(MAIN_CCS:.cc=.o) : CXXFLAGS += -I./include

AM_CPPFLAGS = $(xdrpp_CFLAGS) $(libsodium_CFLAGS) $(lmdb_CFLAGS) $(utility_CFLAGS)
LDADD = $(xdrpp_LIBS) $(libsodium_LIBS)  $(lmdb_LIBS) $(utility_LIBS)

$(SRCS:.cc=.o) : $(XH_FILES)
$(EXTRA_SRCS:.cc=.o) : $(XH_FILES)
$(MAIN_CCS:.cc=.o) : $(XH_FILES)
$(TEST_SRCS:.cc=.o) : CXXFLAGS += $(Catch2_CFLAGS)

main/test.o : CXXFLAGS += $(Catch2_CFLAGS)

test_SOURCES = $(SRCS) $(EXTRA_SRCS) main/test.cc

test_LDADD = $(LDADD) $(Catch2_LIBS)

check_PROGRAMS = test
TESTS = test

#building x files
$(XH_FILES) : $(XDRC)

SUFFIXES = .x .h

.x.h:
	$(XDRC)  -hh -o $@ $<

clean-local:
	$(RM) $(XH_FILES)

README: README.md

.PHONY: doc
doc:
	doxygen Doxyfile




