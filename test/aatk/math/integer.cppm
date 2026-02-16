// Copyright 2026 atyxeut
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
// s
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

export module test.aatk.math.integer;

import std;

import aatk.math.integer;

namespace test::aatk::math {

consteval void does_meta_is_boolean_work() noexcept
{
  static_assert(::aatk::meta::is_boolean_v<int> == false);
  static_assert(::aatk::meta::is_boolean_v<bool> == true);
  static_assert(::aatk::meta::is_boolean_v<const bool> == true);
  static_assert(::aatk::meta::is_boolean_v<volatile bool> == true);
  static_assert(::aatk::meta::is_boolean_v<const volatile bool> == true);

  static_assert(::aatk::meta::boolean<int> == false);
  static_assert(::aatk::meta::boolean<bool> == true);
  static_assert(::aatk::meta::boolean<const bool> == true);
  static_assert(::aatk::meta::boolean<volatile bool> == true);
  static_assert(::aatk::meta::boolean<const volatile bool> == true);
}

consteval void does_meta_make_signed_unsigned_work() noexcept
{
  static_assert(std::same_as<::aatk::meta::make_signed_t<int>, int> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<const int>, const int> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<const volatile unsigned int>, const volatile int> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<const u64>, const i64> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<i128>, i128> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<u128>, i128> == true);
  static_assert(std::same_as<::aatk::meta::make_signed_t<::aatk::fixed_width_integer::u<1024>>, ::aatk::fixed_width_integer::i<1024>> == true);

  static_assert(std::same_as<::aatk::meta::make_unsigned_t<int>, unsigned int> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<const int>, const unsigned int> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<const volatile unsigned int>, const volatile unsigned int> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<const u64>, const u64> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<i128>, u128> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<u128>, u128> == true);
  static_assert(std::same_as<::aatk::meta::make_unsigned_t<::aatk::fixed_width_integer::i<1024>>, ::aatk::fixed_width_integer::u<1024>> == true);
}

consteval void does_meta_make_larger_width_work() noexcept
{
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<int>, i64> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<const int>, const i64> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<const volatile unsigned int>, const volatile u64> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<const u64>, const u128> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<i128>, ::aatk::fixed_width_integer::i<256>> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<u128>, ::aatk::fixed_width_integer::u<256>> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<::aatk::fixed_width_integer::u<1024>>, ::aatk::fixed_width_integer::u<2048>> == true);
  static_assert(std::same_as<::aatk::meta::make_larger_width_t<::aatk::fixed_width_integer::i<256>>, ::aatk::fixed_width_integer::i<512>> == true);
}

} // namespace test::aatk::math
