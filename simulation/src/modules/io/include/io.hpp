#ifndef IO_HPP
#define IO_HPP

#include <fstream>


namespace io
{
	namespace write
	{
		template <class Iterable>
		void iterable(std::ofstream& file, const Iterable& iterable);

		template <class DoubleIterable>
		void double_iterable(std::ofstream& file, const DoubleIterable& iterable);
	}
}

#include "io.tpp"

#endif

