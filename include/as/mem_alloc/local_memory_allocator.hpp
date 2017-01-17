#ifndef ASMITH_LOCAL_MEMORY_ALLOCATOR_WRAPPER_HPP
#define ASMITH_LOCAL_MEMORY_ALLOCATOR_WRAPPER_HPP

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

#include "advanced_memory_allocator.hpp"

namespace as {
	/*!
		\brief A fixed size memory_allocator that does not allocate from the heap.
		\tparam SIZE The size of the memory block in bytes.
		\date 17th January 2017
		\author Adam Smith
	*/
	template<const size_t SIZE>
	class local_memory_allocator : public advanced_memory_allocator {
	private:
		uint8_t mBlock[SIZE];
		bool mAllocated;
	public:
		memory_pool() :
			mAllocated(false)
		{}
		
		~memory_pool() throw() {
			mAllocated = false;
		}

		// Inherited from advanced_memory_allocator
		
		size_t size_of(const void* aPtr) const throw() override {
			return aPtr == mBlock && mAllocated ? SIZE : 0;
		}
		
		size_t allocated_bytes() const throw() override {
			return mAllocated ? SIZE : 0;
		}
		
		size_t max_allocation_size() const throw() override {
			return SIZE;
		}
		

		// Inherited from memory_allocator
		
		void* allocate(size_t aBytes) throw() override {
			if(mAllocated || aBytes > SIZE) return false;
			mAllocated = true;
			return mBlock;
		}
		
		bool free(void* aPtr) throw() override {
			if(aPtr != mAllocated || ! mAllocated) return false;
			mAllocated = false;
			return true;
		}
		
	};
}

#endif
#endif