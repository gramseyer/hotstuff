#pragma once

#include "hotstuff/vm/vm_block_id.h"

namespace hotstuff
{

class VMBlock
{

public:
    virtual VMBlockID get_id() const = 0;
    virtual xdr::opaque_vec<> serialize() const = 0;
};

class LogAccessWrapper;

class VMBase
{

public:
    static VMBlockID nonempty_block_id(VMBlock const& blk)
    {
        return blk.get_id(); // return VMBlockID{blk};
    }
    static VMBlockID empty_block_id() { return {}; }

    virtual std::unique_ptr<VMBlock> try_parse(xdr::opaque_vec<> const& body)
        = 0;

    virtual void init_clean() = 0;

    virtual void init_from_disk(LogAccessWrapper const& lmdb) = 0;

    virtual std::unique_ptr<VMBlock> propose() = 0;

    // Main workflow for non-proposer is exec_block (called indirectly
    // by update) immediately followed by log_commitment
    // Proposer skips the exec_block call.
    virtual void exec_block(const VMBlock& blk) = 0;

    virtual void log_commitment(const VMBlockID& id) = 0;

    virtual void rewind_to_last_commit() = 0;

    virtual ~VMBase() {}
};

} // namespace hotstuff
