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

export module aatk.data_structure.graph;

import std;

import aatk.math;

namespace aatk::graph {

template <typename>
struct weight;

template <>
struct weight<void>
{
};

template <meta::arithmetic T>
struct weight<T>
{
  T w;
};

template <std::integral TVertex, typename TWeight = void>
struct edge_from : weight<TWeight>
{
  TVertex u;
};

template <std::integral TVertex, typename TWeight = void>
struct edge_to : weight<TWeight>
{
  TVertex v;
};

template <std::integral TVertex, typename TWeight = void>
struct edge : weight<TWeight>
{
  TVertex u;
  TVertex v;
};

export template <std::integral TVertex, typename TWeight = void>
class edge_list
{
  std::vector<edge<TVertex, TWeight>> edges_;

public:
  [[nodiscard]] constexpr const auto& operator [](usize idx) const { return edges_[idx]; }
  [[nodiscard]] auto& operator [](usize idx) { return edges_[idx]; }

  constexpr auto begin() const noexcept { return edges_.begin(); }
  constexpr auto begin() noexcept { return edges_.begin(); }

  constexpr auto end() const noexcept { return edges_.end(); }
  constexpr auto end() noexcept { return edges_.end(); }

  [[nodiscard]] constexpr auto size() const noexcept { return edges_.size(); }
  constexpr void reserve(usize capacity) { edges_.reserve(capacity); }

  template <typename... TArgs>
  constexpr auto& add_edge(TArgs... args)
  {
    return edges_.emplace_back(std::forward<TArgs>(args)...);
  }
};

} // namespace aatk::graph
