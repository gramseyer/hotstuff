ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

CLEANFILES = *~ .*~

abs_top_srcdir=@r_abs_top_srcdir@
abs_top_builddir=@r_abs_top_builddir@
top_srcdir=@r_top_srcdir@
top_builddir=@r_top_builddir@

X_FILES = \
	hotstuff/xdr/types.x \
	hotstuff/xdr/hotstuff.x

XH_FILES = $(X_FILES:.x=.h)

BUILT_SOURCES = $(XH_FILES)

# main srcs

lib_LIBRARIES = libhotstuff.a

libhotstuff_a_SOURCES = $(HOTSTUFF_SRCS) $(LMDB_SRCS)

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
	hotstuff/lmdb.cc \
	hotstuff/manage_data_dirs.cc \
	hotstuff/network_event.cc \
	hotstuff/network_event_queue.cc \
	hotstuff/protocol/hotstuff_protocol_client.cc \
	hotstuff/protocol/hotstuff_protocol_manager.cc \
	hotstuff/protocol/hotstuff_server.cc

EXAMPLES_SRCS = \
	examples/vm/counting_vm.cc 

HOTSTUFF_TEST_SRCS = \
	hotstuff/vm/tests/test_speculative_exec_gadget.cc \
	hotstuff/vm/tests/test_counting_vm.cc

LMDB_SRCS = \
	lmdb/lmdb_types.cc \
	lmdb/lmdb_wrapper.cc

UTILS_SRCS = \
	utils/cleanup.cc

MAIN_CCS = \
	main/test.cc

SRCS = \
	$(HOTSTUFF_SRCS) \
	$(LMDB_SRCS) \
	$(UTILS_SRCS)

TEST_SRCS = \
	$(HOTSTUFF_TEST_SRCS)

EXTRA_SRCS = \
	$(EXAMPLES_SRCS) \
	$(TEST_SRCS)

$(SRCS:.cc=.o): CXXFLAGS += -I./include
$(EXTRA_SRCS:.cc=.o) : CXXFLAGS += -I./include
$(MAIN_CCS:.cc=.o) : CXXFLAGS += -I./include

AM_CPPFLAGS = $(xdrpp_CFLAGS) $(libsodium_CFLAGS) $(lmdb_CFLAGS)
LDADD = $(xdrpp_LIBS) $(libsodium_LIBS)  $(lmdb_LIBS)

$(SRCS:.cc=.o) : $(XH_FILES)
$(EXTRA_SRCS:.cc=.o) : $(XH_FILES)
$(MAIN_CCS:.cc=.o) : $(XH_FILES)
$(TEST_SRCS:.cc=.o) : CXXFLAGS += $(Catch2_CFLAGS)

main/test.o : CXXFLAGS += $(Catch2_CFLAGS)

bin_PROGRAMS = \
	test

test_SOURCES = $(SRCS) $(EXTRA_SRCS) main/test.cc

test_LDFLAGS = $(Catch2_LIBS)

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




