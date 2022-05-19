ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

CLEANFILES = *~ .*~

X_FILES = \
	hotstuff/xdr/types.x \
	hotstuff/xdr/hotstuff.x

XH_FILES = $(X_FILES:.x=.h)

# main srcs

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
	hotstuff/protocol/hotstuff_server.cc \
	hotstuff/vm/counting_vm.cc

HOTSTUFF_TEST_SRCS = \
	hotstuff/vm/tests/test_speculative_exec_gadget.cc \
	hotstuff/vm/tests/test_counting_vm.cc

LMDB_SRCS = \
	lmdb/lmdb_types.cc \
	lmdb/lmdb_wrapper.cc

UTILS_SRCS = \
	utils/cleanup.cc

SRCS = \
	$(HOTSTUFF_SRCS) \
	$(LMDB_SRCS) \
	$(UTILS_SRCS)

MAIN_CCS = \
	main/test.cc

TEST_SRCS = \
	$(HOTSTUFF_TEST_SRCS)

AM_CPPFLAGS = $(xdrpp_CFLAGS) $(tbb_CFLAGS) $(libsodium_CFLAGS) $(lmdb_CFLAGS)
LDADD = $(xdrpp_LIBS) $(tbb_LIBS) $(libsodium_LIBS)  $(lmdb_LIBS)

$(SRCS:.cc=.o) : $(XH_FILES)
$(MAIN_CCS:.cc=.o) : $(XH_FILES)
$(TEST_SRCS:.cc=.o) : CXXFLAGS += $(Catch2_CFLAGS)
$(TEST_SRCS:.cc=.o) : $(XH_FILES)
main/test.o : CXXFLAGS += $(Catch2_CFLAGS)

bin_PROGRAMS = \
	test

test_SOURCES = $(SRCS) $(TEST_SRCS) main/test.cc

test_LDFLAGS = $(Catch2_LIBS)

#building x files
$(XH_FILES) : $(XDRC)

SUFFIXES = .x .h

.x.h:
	$(XDRC)  -hh --hhpy -o $@ $<

clean-local:
	$(RM) $(XH_FILES)

README: README.md

.PHONY: doc
doc:
	doxygen Doxyfile




