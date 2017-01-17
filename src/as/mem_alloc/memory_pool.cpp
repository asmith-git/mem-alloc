// Copyright 2017 Adam Smith
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdlib.h>
#include "as/mem_alloc/memory_pool.hpp"

namespace as {
	// memory_pool

	memory_pool::~memory_pool() throw() {
		mOpenBlocks.clear();
		mClosedBlocks.clear();
		for(const block_t& i : mRealBlocks) ::free(i.first);
		mRealBlocks.clear();
	}

	size_t memory_pool::size_of(const void* aPtr) const throw() {
		for(const block_t& i : mRealBlocks) if(i.first == aPtr) return i.second;
		return 0;
	}
	
	size_t memory_pool::allocated_bytes() const throw() {
		size_t sum = 0;
		for(const block_t& i : mClosedBlocks) sum += i.second;
		return sum;
	}
	
	size_t memory_pool::max_allocation_size() const throw() {
		return UINT32_MAX;
	}
	
	void* memory_pool::allocate(size_t aBytes) throw() {

		// Check for an open block that is large enough
		const auto end = mOpenBlocks.end();
		for(auto i = mOpenBlocks.begin(); i != end; ++i) {
#ifdef ASMITH_MEMORY_POOL_NO_SPLITS
			if(i->second >= aBytes) {
#else
			if(i->second == aBytes) {
#endif
				const block_t tmp = *i;
				mOpenBlocks.erase(i);
				mClosedBlocks.push_back(tmp);
				return tmp.first;
#ifndef ASMITH_MEMORY_POOL_NO_SPLITS
			}else if(i->second > aBytes) {
				// Split the block into two
				block_t tmp = *i;
				i->second -= aBytes;
				tmp.second = aBytes;
				mClosedBlocks.push_back(tmp);
				return tmp.first;
#endif
			}
		}

		// Create a new block
		const block_t tmp(malloc(aBytes), aBytes);
		if(! tmp.first) return nullptr;
		mRealBlocks.push_back(tmp);
		mClosedBlocks.push_back(tmp);
		return tmp.first;
	}
	
	bool memory_pool::free(void* aPtr) throw() {
		// Check if the address is the start of a closed block
		const auto end = mClosedBlocks.end();
		for(auto i = mClosedBlocks.begin(); i != end; ++i) {
			if(i->first == aPtr) {
#ifdef ASMITH_MEMORY_POOL_NO_SPLITS
				mOpenBlocks.push_back(*i);
#else
				// Try to merge with adjacent open blocks
				bool merged = false;
				for(block_t& j : mOpenBlocks) {
					if(static_cast<uint8_t*>(j.first) + j.second == i->first) {
						j.second += i->second;
						merged = true;
						break;
					}else if (static_cast<uint8_t*>(i->first) + i->second == j.first) {
						j.first = i->first;
						j.second += i->second;
						merged = true;
						break;
					}
				}
				//! \todo If block was merged then try to merge it again
				if(! merged) mOpenBlocks.push_back(*i);
#endif
				mClosedBlocks.erase(i);
				return true;
			}
		}
		return false;
	}

}