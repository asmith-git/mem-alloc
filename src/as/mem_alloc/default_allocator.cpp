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
#include <mutex>
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

	class thread_wrapper : public advanced_memory_allocator {
	private:
		mutable std::mutex mLock;
	public:
		// Inherited from advanced_memory_allocator

		size_t size_of(const void* aPtr) const throw() override {
			mLock.lock();
			const size_t tmp = ADV_ALLOCATOR->size_of(aPtr);
			mLock.unlock();
			return tmp;
		}

		size_t allocated_bytes() const throw() override {
			mLock.lock();
			const size_t tmp = ADV_ALLOCATOR->allocated_bytes();
			mLock.unlock();
			return tmp;
		}

		size_t max_allocation_size() const throw() override {
			mLock.lock();
			const size_t tmp = ADV_ALLOCATOR->max_allocation_size();
			mLock.unlock();
			return tmp;
		}

		// Inherited from memory_allocator

		void* allocate(size_t aBytes) throw() override {
			mLock.lock();
			void* const tmp = ADV_ALLOCATOR->allocate(aBytes);
			mLock.unlock();
			return tmp;
		}

		bool free(void* aPtr) throw() override {
			mLock.lock();
			const bool tmp = ADV_ALLOCATOR->free(aPtr);
			mLock.unlock();
			return tmp;
		}
	};

	thread_wrapper THREAD_WRAPPER;

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
#ifdef ASMITH_DEFAULT_ALLOCATOR_NO_THREAD_PROTECTION
		return *ADV_ALLOCATOR;
#else
		return THREAD_WRAPPER;
#endif
	}


}