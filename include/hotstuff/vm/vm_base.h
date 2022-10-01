#pragma once

#include "hotstuff/vm/vm_block_id.h"

namespace hotstuff
{

class VMBlock
{

public:
    virtual VMBlockID get_id() const = 0;
    virtual xdr::opaque_vec<> serialize() const = 0;

    virtual ~VMBlock() = default;
};

class LogAccessWrapper;

class VMBase
{

    [[noreturn]] void unimplemented() const
    {
        throw std::runtime_error("unimplemented");
    }

public:
    static VMBlockID nonempty_block_id(VMBlock const& blk)
    {
        return blk.get_id(); // return VMBlockID{blk};
    }
    static VMBlockID empty_block_id() { return {}; }



    /**
     * Try to parse body into a vm object (for speedex, this is (header, tx list)).
     * body.size() == 0 is considered to be no vm block (which is valid for speedex.  Just ignored.)
     * parse failures are considered an invalid vm block. (but valid for hotstuff, which doesn't care about
     * parsing).
     */
    virtual std::unique_ptr<VMBlock> try_parse(xdr::opaque_vec<> const& body)
        = 0;

    virtual void init_clean() = 0;

    virtual void init_from_disk(LogAccessWrapper const& lmdb) = 0;

    virtual std::unique_ptr<VMBlock> propose() = 0;

    /**
     * In speculative vm:
     *      non-proposer has exec_block called by update() immediately followed by log_commitment.
     *      proposer skips exec_block call.
     * In nonspeculative vm:
     *      everyone calls exec_block (log commitment is not called).
     */
    virtual void exec_block(const VMBlock& blk) = 0;

    virtual void log_commitment(const VMBlockID& id) { unimplemented(); }

    virtual void rewind_to_last_commit() = 0;

    virtual ~VMBase() = default;
};

} // namespace hotstuff
