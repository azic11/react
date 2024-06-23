#include "io.hpp"

template <class Iterable>
void io::write::iterable(std::ofstream& file, const Iterable& iterable)
{
	for (const auto& item : iterable)
		file << item << "\t";
	file << std::endl;
}

template <class DoubleIterable>
void io::write::double_iterable(std::ofstream& file, const DoubleIterable& iterable)
{
	for (const auto& item : iterable)
	{
		for (const auto& subitem : item)
			file << subitem << "\t";
		file << '\n';
	}
	file << std::flush;
}

