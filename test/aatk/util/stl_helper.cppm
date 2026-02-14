// Copyright 2026 atyxeut
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

module;

#include <cassert>

export module test.aatk.util.stl_helper;

import std;

import aatk.memory;
import aatk.util.stl_helper;
import aatk.util.tmp;

namespace test::aatk {

using namespace ::aatk;

export consteval void is_vector_nested_std_vector() noexcept
{
  using T1 = std::vector<double>;
  using T1_ = vector<double>;
  static_assert(std::same_as<T1, T1_>);

  using T2 = std::pmr::vector<double>;
  using T2_ = vector<double, 1, memory::std_pmr_allocator_tag>;
  static_assert(std::same_as<T2, T2_>);

  using T3 = std::pmr::vector<std::pmr::vector<std::pmr::vector<std::pmr::vector<std::pmr::vector<double>>>>>;
  using T3_ = vector<double, 5, memory::std_pmr_allocator_tag, memory::std_pmr_allocator_tag, memory::std_pmr_allocator_tag, memory::std_pmr_allocator_tag, memory::std_pmr_allocator_tag>;
  static_assert(std::same_as<T3, T3_>);

  using T4 = std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>>;
  using T4_ = vector<double, 5>;
  static_assert(std::same_as<T4, T4_>);
}

export consteval void do_vector_empty_allocator_template_arguments_default_to_std_allocator() noexcept
{
  using T1 = std::pmr::vector<double>;
  using T1_ = vector<double, 1, std::pmr::polymorphic_allocator<double>>;
  static_assert(std::same_as<T1, T1_>);

  using T2 = std::vector<std::vector<std::pmr::vector<int>>>;
  using T2_ = vector<int, 3, std::pmr::polymorphic_allocator<int>>;
  using T2__ = vector<int, 3, std::pmr::polymorphic_allocator<int>, std::allocator<std::vector<int, std::pmr::polymorphic_allocator<int>>>>;
  static_assert(std::same_as<T2, T2_> && std::same_as<T2_, T2__>);
}

export void does_make_vector_work()
{
  auto get_v1 = [] consteval noexcept
  {
    return std::vector<double>(3, 3.14);
  };
  auto get_v1_ = [] consteval noexcept
  {
    return make_vector<double>(3, 3.14);
  };
  static_assert(get_v1() == get_v1_());

  // std::pmr::polymorphic_allocator cannot be used in constexpr context
  auto v2 = std::vector<std::pmr::vector<std::vector<double>>>(3, std::pmr::vector<std::vector<double>>(4, std::vector<double>(5, 3.14)));
  auto v2_ = make_vector<double, std::allocator<double>, std::pmr::polymorphic_allocator<std::vector<double>>>(3, 4, 5, 3.14);
  auto v2__ = make_vector<double, memory::std_allocator_tag, memory::std_pmr_allocator_tag>(3, 4, 5, 3.14);
  assert(v2 == v2_ && v2_ == v2__);
}

} // namespace test::aatk
