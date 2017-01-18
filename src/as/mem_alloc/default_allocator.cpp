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

#include "as/mem_alloc/default_allocator.hpp"
#include "as/mem_alloc/c_memory_allocator.hpp"
#include "as/mem_alloc/advanced_upgrade_wrapper.hpp"

namespace as {

	memory_allocator* ALLOCATOR = nullptr;
	advanced_memory_allocator* ADV_ALLOCATOR = nullptr;

	class allocator_wrapper : public memory_allocator {
	public:
		// Inherited from memory_allocator
		void* allocate(size_t aBytes) throw() override {
			return ALLOCATOR->allocate(aBytes);
		}

		bool free(void* aPtr) throw() override {
			return ALLOCATOR->free(aPtr);
		}
	};

	advanced_upgrade_wrapper<allocator_wrapper> WRAPPER;

	void set_default_allocator(memory_allocator& aAllocator) {
		ALLOCATOR = &aAllocator;
		ADV_ALLOCATOR = &WRAPPER;
	}

	void set_default_allocator(advanced_memory_allocator& aAllocator) {
		ADV_ALLOCATOR = &aAllocator;
	}

	advanced_memory_allocator& get_default_allocator() {
		static bool ONCE = true;
		static c_memory_allocator C_ALLOCATOR;
		if(ONCE) {
			ONCE = false; 
			set_default_allocator(C_ALLOCATOR);
		}
		return *ADV_ALLOCATOR;
	}


}