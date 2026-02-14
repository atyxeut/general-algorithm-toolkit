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

export module aty.gatk.util.stl_helper;

import std;

import aty.gatk.memory;
import aty.gatk.util.tmp;

namespace aty::gatk {

template <typename, std::size_t...>
struct array_impl;

template <typename T, std::size_t Dim>
struct array_impl<T, Dim>
{
  using type = std::array<T, Dim>;
};

template <typename T, std::size_t Dim, std::size_t... Dims>
struct array_impl<T, Dim, Dims...>
{
  using type = std::array<typename array_impl<T, Dims...>::type, Dim>;
};

// aty::gatk::array<int, 3, 5, 2> arr3d {};
//   same as: std::array<std::array<std::array<int, 2>, 5>, 3> arr3d {};
export template <typename T, std::size_t... Dims>
using array = array_impl<T, Dims...>::type;

export namespace tmp {

template <typename>
struct is_no_cv_std_array : std::false_type
{
};

template <typename T, std::size_t N>
struct is_no_cv_std_array<std::array<T, N>> : std::true_type
{
};

template <typename T>
constexpr bool is_no_cv_std_array_v = is_no_cv_std_array<T>::value;

template <typename T>
using is_std_array = is_no_cv_std_array<std::remove_cv_t<T>>;

template <typename T>
constexpr bool is_std_array_v = is_std_array<T>::value;

} // namespace tmp

// aty::gatk::array<int, 3, 5, 2, 10> arr4d;
// int val = -1;
// aty::gatk::fill_array(arr4d, val);
//   set every element of a aty::gatk::array to val
export template <typename TElem, std::size_t Dim, typename T>
constexpr void fill_array(std::array<TElem, Dim>& arr, const T& val)
{
  if constexpr (tmp::is_no_cv_std_array_v<TElem>)
    for (auto& inner_arr : arr)
      fill_array(inner_arr, val);
  else
    arr.fill(static_cast<TElem>(val));
}

// auto arr4d = aty::gatk::make_array<int, 5, 8, 3, 2>(val);
//   combines aty::gatk::array<int, 5, 8, 3, 2> arr4d;
//        and aty::gatk::fill_array(arr4d, val);
export template <typename TElem, std::size_t... Dims, typename T>
[[nodiscard]] constexpr auto make_array(const T& val)
{
  array<TElem, Dims...> arr;
  fill(arr, static_cast<TElem>(val));
  return arr;
}

template <typename TCurAllocatorList, std::size_t DimCnt>
struct adjust_allocator_type_list;

template <typename... TCurAllocators, std::size_t DimCnt>
struct adjust_allocator_type_list<std::tuple<TCurAllocators...>, DimCnt>
{
  using type = tmp::concat_t<std::tuple<TCurAllocators...>, std::conditional_t<sizeof...(TCurAllocators) < DimCnt, std::tuple<memory::std_allocator_tag>, std::tuple<>>>;
};

template <typename TCurAllocatorList, std::size_t DimCnt>
using adjust_allocator_type_list_t = adjust_allocator_type_list<TCurAllocatorList, DimCnt>::type;

template <typename TCurElement, typename TAllocatorList>
class cur_dim_allocator;

template <typename TCurElement, typename... TAllocators>
class cur_dim_allocator<TCurElement, std::tuple<TAllocators...>>
{
  using last_allocator_type_ = tmp::back_t<std::tuple<TAllocators...>>;

public:
  using type = std::conditional_t<std::same_as<last_allocator_type_, memory::std_allocator_tag>, std::allocator<TCurElement>, last_allocator_type_>;
};

template <typename TCurElement, typename TAllocatorList>
using cur_dim_allocator_t = cur_dim_allocator<TCurElement, TAllocatorList>::type;

template <typename T, std::size_t DimCnt, typename TAllocatorList>
  requires (DimCnt > 0)
class vector_impl;

template <typename T, std::size_t DimCnt, typename TInnermostDimAllocator, typename... TAllocators>
class vector_impl<T, DimCnt, std::tuple<TInnermostDimAllocator, TAllocators...>>
{
  using adjusted_allocator_type_list_ = adjust_allocator_type_list_t<std::tuple<TInnermostDimAllocator, TAllocators...>, DimCnt>;
  using element_type_ = vector_impl<T, DimCnt - 1, tmp::pop_back_t<adjusted_allocator_type_list_>>::type;

public:
  using type = std::vector<element_type_, cur_dim_allocator_t<element_type_, adjusted_allocator_type_list_>>;
};

template <typename T, typename TAllocator>
class vector_impl<T, 1, std::tuple<TAllocator>>
{
public:
  using type = std::vector<T, cur_dim_allocator_t<T, std::tuple<TAllocator>>>;
};

template <typename T, std::size_t DimCnt = 1, typename TAllocatorList = std::tuple<std::allocator<T>>>
  requires (std::tuple_size_v<TAllocatorList> <= DimCnt)
using vector_impl_t = vector_impl<T, DimCnt, TAllocatorList>::type;

// aty::gatk::vector<int> vec1d;
//   same as: std::vector<int> vec1d;
// aty::gatk::vector<int, 4> vec4d;
//   same as: std::vector<std::vector<std::vector<std::vector<int>>>> vec4d;
export template <typename T, std::size_t DimCnt = 1, typename TInnermostDimAllocator = std::allocator<T>, typename... TAllocators>
  requires (sizeof...(TAllocators) < DimCnt)
using vector = vector_impl<T, DimCnt, std::tuple<TInnermostDimAllocator, TAllocators...>>::type;

template <typename TElem, typename TAllocatorList, std::integral TDim, typename... Ts>
  requires (sizeof(TDim) <= sizeof(std::size_t) && sizeof...(Ts) > 0 && std::tuple_size_v<TAllocatorList> <= sizeof...(Ts))
[[nodiscard]] constexpr auto make_vector_impl(TDim first_dim_size, Ts&&... args)
{
  using adjusted_allocator_type_list = adjust_allocator_type_list_t<TAllocatorList, sizeof...(Ts)>;
  if constexpr (sizeof...(Ts) == 1) {
    using cur_dim_allocator_type = cur_dim_allocator_t<TElem, adjusted_allocator_type_list>;
    return vector<TElem, 1, cur_dim_allocator_type>(static_cast<std::size_t>(first_dim_size), static_cast<TElem>(args)...);
  }
  else {
    using inner_allocator_type_list = tmp::pop_back_t<adjusted_allocator_type_list>;
    using inner_element_type = vector_impl_t<TElem, sizeof...(Ts) - 1, inner_allocator_type_list>;
    using cur_dim_allocator_type = cur_dim_allocator_t<inner_element_type, adjusted_allocator_type_list>;
    return vector<inner_element_type, 1, cur_dim_allocator_type>(static_cast<std::size_t>(first_dim_size), make_vector_impl<TElem, inner_allocator_type_list>(std::forward<Ts>(args)...));
  }
}

// auto vec3d = aty::gatk::make_vector<int>(x, y, z, 1);
//   same as: auto vec3d = std::vector<std::vector<std::vector<int>>>(
//                           x,
//                           std::vector<std::vector<int>>(y, std::vector<int>(z, 1))
//                         );
// constexpr usage example (error if aty::gatk::make_vector is not constexpr):
//   std::cout << [](const std::vector<int>& vec, int sum = 0) consteval {
//     std::ranges::for_each(vec, [&sum](int elem) { return sum += elem; });
//     return sum;
//   }(aty::gatk::make_vector<int>(10, -1)) << "\n";
export template <typename TElem, typename TInnermostDimAllocator = memory::std_allocator_tag, typename... TAllocators, std::integral TDim, typename... Ts>
  requires (sizeof(TDim) <= sizeof(std::size_t) && sizeof...(Ts) > 0 && sizeof...(TAllocators) < sizeof...(Ts))
[[nodiscard]] constexpr auto make_vector(TDim first_dim_size, Ts&&... args)
{
  return make_vector_impl<TElem, std::tuple<TInnermostDimAllocator, TAllocators...>>(first_dim_size, std::forward<Ts>(args)...);
}

export namespace tmp {

template <typename T>
struct is_no_cv_std_ratio : std::false_type
{
};

template <std::intmax_t Numerator, std::intmax_t Denominator>
struct is_no_cv_std_ratio<std::ratio<Numerator, Denominator>> : std::true_type
{
};

template <typename T>
constexpr bool is_no_cv_std_ratio_v = is_no_cv_std_ratio<T>::value;

template <typename T>
using is_std_ratio = is_no_cv_std_ratio<std::remove_cv_t<T>>;

template <typename T>
constexpr bool is_std_ratio_v = is_std_ratio<T>::value;

template <typename T>
struct is_no_cv_std_duration : std::false_type
{
};

template <typename TRep, typename TPeriod>
struct is_no_cv_std_duration<std::chrono::duration<TRep, TPeriod>> : std::true_type
{
};

template <typename T>
constexpr bool is_no_cv_std_duration_v = is_no_cv_std_duration<T>::value;

template <typename T>
using is_std_duration = is_no_cv_std_duration<std::remove_cv_t<T>>;

template <typename T>
constexpr bool is_std_duration_v = is_std_duration<T>::value;

} // namespace tmp

} // namespace aty::gatk
