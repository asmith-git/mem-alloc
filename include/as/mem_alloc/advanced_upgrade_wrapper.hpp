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

#include <cstdint>
#include <map>
#include "advanced_upgrade_wrapper.hpp"

namespace as {
	template<class ALLOCATOR, const size_t MAX_ALLOC = UINT32_MAX>
	class advanced_upgrade_wrapper : public advanced_memory_allocator {
	private:
		ALLOCATOR mAllocator;
		std::map<const void*, size_t> mAllocations;
	public:
		~advanced_upgrade_wrapper() throw() {
			mAllocations.clear();
		}

		// Inherited from advanced_memory_allocator

		size_t size_of(const void* aPtr) const throw() override {
			const auto i = mAllocations.find(aPtr);
			return i == mAllocations.end() ? 0 : i->second;
		}

		size_t allocated_bytes() const throw() override {
			size_t sum = 0;
			for(const auto& i : mAllocations) sum += i.second;
			return sum;
		}

		size_t max_allocation_size() const throw() override {
			return MAX_ALLOC;
		}

		// Inherited from memory_allocator
		
		void* allocate(size_t aBytes) throw() override {
			void* const tmp = mAllocator.allocate(aBytes);
			if(! tmp) return nullptr;
			mAllocations.emplace(tmp, aBytes);
			return tmp;
		}

		bool free(void* aPtr) throw() override {
			const auto i = mAllocations.find(aPtr);
			if(i == mAllocations.end() || ! mAllocations.free(aPtr)) return false;
			mAllocations.erase(i);
			return true;
		}
	};
}

#endif