#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <fstream>


namespace logging
{
	template <class Iterable>
	void log_iterable(std::ofstream& file, const Iterable& iterable)
	{
		for (const auto& item : iterable)
			file << item << "\t";
		file << std::endl;
	}

	template <class DoubleIterable>
	void log_double_iterable(std::ofstream& file, const DoubleIterable& iterable)
	{
		for (const auto& item : iterable)
		{
			for (const auto& subitem : item)
				file << subitem << "\t";
			file << std::endl;
		}
	}
}

#endif

