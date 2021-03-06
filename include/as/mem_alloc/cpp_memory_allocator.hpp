#ifndef ASMITH_CPP_MEMORY_ALLOCATOR_HPP
#define ASMITH_CPP_MEMORY_ALLOCATOR_HPP

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

#include "memory_allocator.hpp"

namespace as {
	/*!
		\brief A memory_allocator that uses the C++ functions operator new and operator delete.
		\date 17th January 2017
		\author Adam Smith
	*/
	class cpp_memory_allocator : public memory_allocator {
	public:
		// Inherited from memory_allocator
		void* allocate(size_t aBytes) throw() override;
		bool free(void* aPtr) throw() override;
	};
}

#endif