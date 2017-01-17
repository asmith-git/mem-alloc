#ifndef ASMITH_MEMORY_ALLOCATOR_HPP
#define ASMITH_MEMORY_ALLOCATOR_HPP

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

#include <cstddef>

namespace as {
	/*!
		\brief A simple memory allocation class.
		\date 17th January 2017
		\author Adam Smith
	*/
	class memory_allocator {
	public:
		/*!
			\brief Destroy the allocator and free any currently allocated memory.
		*/
		virtual ~memory_allocator() throw() {}

		/*!
			\brief Allocate memory.
			\param aBytes The number of bytes to allocate.
			\return The starting address of the memory or nullptr if allocation failed.
		*/
		virtual void* allocate(size_t) throw() = 0;

		/*!
			\brief Free memory that was allocated from this allocator.
			\param aPtr The starting address of the memory.
			\return False if the memory failed to be deallocated.
		*/
		virtual bool free(void*) throw() = 0;
	};
}

#endif