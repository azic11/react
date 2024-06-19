#ifndef STIMULATION_HPP
#define STIMULATION_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "global_defs.hpp"


namespace stimulation
{
	struct Stimulation
	{
		double time;
		char mode; // TODO: change to enum
		range_t range; // change name or to two separate indices
		double current;
		std::size_t update_interval;
	};

	template <std::size_t N>
	nvec<N> generate_stimulus_currents(std::size_t from_idx, std::size_t to_idx, double current)
	{
		nvec<N> currents;
		std::fill(currents.begin(), currents.end(), 0.);
		std::fill(currents.begin() + from_idx, currents.begin() + to_idx, current);
		return currents;
	}

	template <std::size_t N, class rng_t>
	nvec<N> generate_random_currents(std::size_t from_idx, std::size_t num_active, double current, rng_t& rng)
	{
		nvec<N> currents;
		std::fill(currents.begin(), currents.end(), 0.);
		std::uniform_int_distribution<std::size_t> distr(from_idx, N);
		std::vector<std::size_t> indices(num_active);
		std::generate_n(indices.begin(), num_active, [&rng, &distr](){return distr(rng);});
		for (std::size_t i : indices)
			currents[i] = current;
		return currents;
	}

	template <std::size_t N, class rng_t>
	class Paradigm
	{
	private:
		std::vector<Stimulation> stimulations;
		nvec<N> current_stimulation_currents;
		rng_t& rng;
	public:
		Paradigm(std::filesystem::path path, rng_t& rng);
		std::size_t get_logging_interval() const; // TODO: implement separate log paradigm mechanism
		nvec<N> get_currents(double time);
	};
}


template <std::size_t N, class rng_t>
stimulation::Paradigm<N,rng_t>::Paradigm(std::filesystem::path path, rng_t& rng) : rng(rng)
{
	Stimulation tmp;
	std::ifstream infile(path);
	for( std::string line; getline( infile, line ); )
	{
		std::istringstream iss(line);
		iss >> tmp.time >> tmp.mode >> tmp.range.first >> tmp.range.second >> tmp.current >> tmp.update_interval;
		tmp.time *= 60 * 60;

		if ((tmp.mode=='S') and (tmp.range.second <= tmp.range.first))
			printf("Indices in stim-file are unordered. Skipping line.\n");
		else if (stimulations.size() and (tmp.time <= stimulations.back().time))
			printf("Times in stim-file are unordered. Skipping line.\n");
		else
			stimulations.push_back(tmp);
	}
	infile.close();
}

template <std::size_t N, class rng_t>
nvec<N> stimulation::Paradigm<N,rng_t>::get_currents(double time)
{
	if ((stimulations.size() > 1) and (time >= stimulations[1].time))
	{
		stimulations.erase(stimulations.begin());
		switch (stimulations[0].mode)
		{
			case 'S':
				current_stimulation_currents = generate_stimulus_currents<N>(stimulations[0].range.first, stimulations[0].range.second, stimulations[0].current);
				break;
			case 'R':
				current_stimulation_currents = generate_random_currents<N>(stimulations[0].range.first, stimulations[0].range.second, stimulations[0].current, rng);
				break;
		}
	}
	return current_stimulation_currents;
}

#endif

