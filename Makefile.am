ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

CLEANFILES = *~ .*~

X_FILES = \
	xdr/types.x \
	xdr/hotstuff.x

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
	hotstuff/consensus.cc \
	hotstuff/crypto.cc \
	hotstuff/event.cc \
	hotstuff/event_queue.cc \
	hotstuff/hotstuff.cc \
	hotstuff/lmdb.cc \
	hotstuff/network_event.cc \
	hotstuff/network_event_queue.cc \
	hotstuff/protocol/hotstuff_protocol_client.cc \
	hotstuff/protocol/hotstuff_protocol_manager.cc \
	hotstuff/protocol/hotstuff_server.cc \
	hotstuff/vm/counting_vm.cc

HOTSTUFF_TEST_SRCS = \
	hotstuff/vm/tests/test_speculative_exec_gadget.h

LMDB_SRCS = \
	lmdb/lmdb_types.cc \
	lmdb/lmdb_wrapper.cc

UTILS_SRCS = \
	utils/cleanup.cc \
	utils/header_persistence.cc \
	utils/manage_data_dirs.cc

SRCS = \
	$(HOTSTUFF_SRCS) \
	$(UTILS_SRCS)

MAIN_CCS = 

AM_CPPFLAGS = $(libcrypto_CFLAGS) $(gsl_CFLAGS) $(xdrpp_CFLAGS) $(libsodium_CFLAGS) $(LIBFYAML_CFLAGS) $(lmdb_CFLAGS) $(lemon_CFLAGS)
LDADD = $(libcrypto_LIBS) $(gsl_LIBS) $(xdrpp_LIBS) -ltbb -lglpk $(libsodium_LIBS) $(LIBFYAML_LIBS) $(lmdb_LIBS) $(lemon_LIBS)

$(SRCS:.cc=.o) : $(XH_FILES)
$(MAIN_CCS:.cc=.o) : $(XH_FILES)

TEST_SRCS = \
	$(HOTSTUFF_TEST_SRCS)

test_runner.cc : $(TEST_SRCS) $(XHFILES)
	cxxtestgen --error-printer -o test_runner.cc $(TEST_SRCS)

bin_PROGRAMS = \
	test

test_SOURCES = $(SRCS) test_runner.cc

#building x files
$(XH_FILES) : $(XDRC)

SUFFIXES = .x .h

.x.h:
	$(XDRC)  -hh --hhpy -o $@ $<

clean-local:
	$(RM) $(XH_FILES) test_runner.cc

README: README.md

.PHONY: doc
doc:
	doxygen Doxyfile




