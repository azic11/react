#ifndef IO_HPP
#define IO_HPP

#include <fstream>


namespace io
{
	namespace write
	{
		template <class Iterable>
		void iterable(std::ofstream& file, const Iterable& iterable)
		{
			for (const auto& item : iterable)
				file << item << "\t";
			file << std::endl;
		}

		template <class DoubleIterable>
		void double_iterable(std::ofstream& file, const DoubleIterable& iterable)
		{
			for (const auto& item : iterable)
			{
				for (const auto& subitem : item)
					file << subitem << "\t";
				file << '\n';
			}
			file << std::flush;
		}
	}
}

#endif

