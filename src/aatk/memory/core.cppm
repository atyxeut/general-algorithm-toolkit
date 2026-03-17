// Copyright (C) 2026 atyxeut
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library.  If not, see <https://www.gnu.org/licenses/>.

export module aatk.memory.core;

import aatk.math.integer.core;

export namespace aatk {

// represent std::allocator<T>
struct std_allocator_tag
{
};

// represent std::pmr::polymorphic_allocator<T>
struct std_pmr_allocator_tag
{
};

template <meta::for_size_integral T>
constexpr T dynamic_capacity = static_cast<T>(-1);

enum class resource_location
{
  inplace,
  heap
};

enum class exception_safety
{
  basic,
  strong
};

} // namespace aatk
