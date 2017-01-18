#ifndef ASMITH_DEFAULT_ALLOCATOR_HPP
#define ASMITH_DEFAULT_ALLOCATOR_HPP

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
		\brief Set the default allocator.
		\detail The allocator must be kept within scope until another allocator is set or the program terminates.
		\param aAllocator The allocator to set as default
		\see get_default_allocator
	*/
	void set_default_allocator(memory_allocator&);

	/*!
		\brief Set the default allocator.
		\detail The allocator must be kept within scope until another allocator is set or the program terminates.
		\param aAllocator The allocator to set as default
		\see get_default_allocator
	*/
	void set_default_allocator(advanced_memory_allocator&);

	/*!
		\brief Return the default allocator
		\detail If ASMITH_DEFAULT_ALLOCATOR_NO_THREAD_PROTECTION is defined then multi-threading protection for the allocator will be removed, but will improve performance.
		If set_default_allocator has been set then a c_memory_allocator will be used.
		\return The default allocator
		\see set_default_allocator
		\see c_memory_allocator
	*/
	advanced_memory_allocator& get_default_allocator();
}

#endif