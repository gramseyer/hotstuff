#pragma once

namespace hotstuff
{

struct HotstuffConfigs
{
	// default configs for speedex
	size_t proposal_buffer_target = 3;
	bool immediately_refill_proposal_buffer = true;
};

}