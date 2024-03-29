# Standalone Hotstuff Implementation

Based on the Hotstuff paper by Maofan Yin, Dahlia Malkhi, Michael K Reiter, Guy Nolan Gueta,
and Ittai Abraham.

The implementation is closely based on the implementation at github.com/hot-stuff/libhotstuff.

This implementation focuses on a clean separation between hotstuff logic
and the logic of the state machine being replicated.  

The integration is designed around allowing the proposer's state machine to speculatively
run ahead of the other machines.  The state machine receives appropriate notifications
of when speculatively executed blocks commit. 

## Design Motivation

 The motivation is that for some state machines
(i.e. speedex, github.com/scslab/speedex), the act of generating a proposal is computationally
expensive and involves updating many (expensive) internal data structures.  In the normal case,
leader rotation happens infrequently, so allowing the leader to run a few steps ahead of the
validators allows the state machine to pipeline the proposal step and consensus (instead of,
for example, waiting for consensus commitment after every individual proposal). 

This implementation has a thread internal to the hotstuff module that (when in
proposal-generation mode, i.e. in the leader) perpetually
"asks" the state machine for new proposals.  These go into a buffer, and are given to
consensus whenever something (i.e. a pacemaker) calls do_propose().  The reason for this
is that this design gives Hotstuff full control over operation of the state machine (in as much
as consensus is concerned), which makes management of speculative state/rolling back
speculation much simpler (Hotstuff manages all of this).  One could implement a "send command"
function on the state machine, which could just forward those input commands to Hotstuff's
buffer on demand.

The design is planned around deployment on a machine with many CPU cores and large but infrequent consensus blocks
(i.e. in the context of SPEEDEX).  Hence the choice of dedicated threads managing event queues (which is very simple to set up)
over an asynchronous callback system.

Contributions of any kind are very welcome.

# Usage

`make doc` for doxygen

`make test` for tests

Requires a C++2a-capable compiler.

## API Usage

ReplicaConfig manages a map of which replicas are located at which addresses.  Dynamic reconfiguration not supported.

HotstuffApp takes configuration data and a pointer to a state machine instance for replication.
Use by calling `do_propose` on the app.

Note: `do_propose` will only propose empty blocks (i.e. without consulting the state machine)
until the leader is stable -- that is, the leader proposes empty blocks until one of the empty
blocks commits.  The rationale here is that this makes management of speculatively executing 
state machines much easier (in particular, this minimizes the number of times that these state machines
have to rollback).  A continuously rotating leader could be implemented naively by sequences of 4 calls to
`do_propose`, although if such behavior is desired, more direct interaction with the speculation on the state machine is likely
desirable.

`examples/vm/tests/test_counting_vm.cc` gives a simple usage example with one replica.

# Integration

The hotstuff implementation handles internally its own passing of protocol messages
and forwarding proposed blocks.  To replicate a state machine, the state machine
needs to be able to propose commands when requested from hotstuff (empty commands allowed),
apply blocks that are committed from hotstuff, and rollback any speculative effects
accrued during proposal.

Hotstuff maintains a log of all blocks, and a cache mapping block height -> block hash (via LMDB)
for crash recovery).  Upon recovery, the state machine gets access to that LMDB instance
and knowledge of which blocks committed, so it can access all the information it needs
to recover.  

Recovery after disk loss not implemented.

# TODO

This implementation
should not be used in production or for a safety-critical system, at least 
not until the implementation can be more thoroughly audited.

# History

I wrote this for my SPEEDEX implementation, and pulled this code out of there
so as to have a standalone implementation.

Now should work with autotools as either an installed package (`./autogen.sh && ./configure && make && make install`)
or as a git submodule.  

To use as a git submodule with xdrpp defined in a separately included git submodule,
the main configure script needs the following:
```
ABS_TOP_DIR=$ac_pwd
export ABS_TOP_DIR

export xdrpp_INTERNAL

global_xdrpp_CFLAGS=$xdrpp_CFLAGS
global_xdrpp_LIBS=$xdrpp_LIBS
global_XDRC=$XDRC

export global_xdrpp_CFLAGS
export global_xdrpp_LIBS
export global_XDRC
```

You should also be able to just install xdrpp on the system.

This library depends on xdrpp, libsodium, and lmdb.

