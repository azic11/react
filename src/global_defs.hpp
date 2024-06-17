#ifndef GLOBAL_DEFS_HPP
#define GLOBAL_DEFS_HPP

#include <array>

template <std::size_t N>
using nvec = std::array<double,N>;

template <std::size_t N>
using nmat = std::array<nvec<N>,N>;

template <std::size_t N>
using ncountvec = std::array<std::size_t,N>;

template <std::size_t N>
using ncountmat = std::array<ncountvec<N>,N>;

#endif
