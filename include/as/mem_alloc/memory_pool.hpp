#ifndef ASMITH_ADV_MEMORY_ALLOCATOR_WRAPPER_HPP
#define ASMITH_ADV_MEMORY_ALLOCATOR_WRAPPER_HPP

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

#include <vector>
#include "advanced_memory_allocator.hpp"

namespace as {
	/*!
		\brief A memory_allocator that caches memory when freed for later re-use.
		\detail If 'ASMITH_MEMORY_POOL_NO_SPLITS' is defined then block splitting will be disabled. Which will result in faster allocation and freeing of memory, but more memory may be allocated.
		\date 17th January 2017
		\author Adam Smith
	*/
	class memory_pool : public advanced_memory_allocator {
	private:
		typedef std::pair<void*, size_t> block_t;	//!< A block of memory
		std::vector<block_t> mRealBlocks;			//!< Memory allocated from the heap
		std::vector<block_t> mOpenBlocks;			//!< Blocks which are currently free
		std::vector<block_t> mClosedBlocks;			//!< Blocks that are currently in use
	public:
		~memory_pool() throw();

		// Inherited from advanced_memory_allocator
		size_t size_of(const void* aPtr) const throw() override;
		size_t allocated_bytes() const throw() override;
		size_t max_allocation_size() const throw() override;

		// Inherited from memory_allocator
		void* allocate(size_t aBytes) throw() override;
		bool free(void* aPtr) throw() override;
	};
}

#endif