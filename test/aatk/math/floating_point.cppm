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

export module test.aatk.math.floating_point;

import std;

import aatk.math.floating_point;

namespace test::aatk::math {

consteval void does_make_higher_precision_work()
{
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<f32>, f64> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<const f32>, const f64> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<volatile f32>, volatile f64> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<const volatile f32>, const volatile f64> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<f64>, f80> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<f80>, f128> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<f128>, ::aatk::ieee754_float::f<256>> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<::aatk::ieee754_float::f<256>>, ::aatk::ieee754_float::f<512>> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<::aatk::ieee754_float::f<1024>>, ::aatk::ieee754_float::f<2048>> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<::aatk::ieee754_float::d<256>>, ::aatk::ieee754_float::d<512>> == true);
  static_assert(std::same_as<::aatk::meta::make_higher_precision_t<::aatk::ieee754_float::d<1024>>, ::aatk::ieee754_float::d<2048>> == true);
}

} // namespace test::aatk::math
