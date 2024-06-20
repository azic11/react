#ifndef STIMULATION_HPP
#define STIMULATION_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "global_defs.hpp"


namespace stimulation
{
	template <std::size_t N>
	nvec<N> generate_well_defined_currents(std::size_t from_idx, std::size_t to_idx, double current)
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
		std::uniform_int_distribution<std::size_t> distr(from_idx, N-1);
		std::vector<std::size_t> indices(num_active);
		std::generate_n(indices.begin(), num_active, [&rng, &distr](){return distr(rng);});
		for (std::size_t i : indices)
			currents[i] = current;
		return currents;
	}

	template <std::size_t N>
	struct Stimulus
	{
		double time;
		nvec<N> currents;
	};

	template <std::size_t N, class rng_t>
	class Stimulator
	{
	private:
		std::vector<Stimulus<N>> stimuli;
		rng_t& rng;

		bool next_stimulus_available() const;
		bool time_for_next_stimulus(double time) const;
		void move_to_next_stimulus();
	public:
		Stimulator(std::filesystem::path path, rng_t& rng);
		nvec<N> get_stimulus_currents(double time);
	};
}

template <std::size_t N, class rng_t>
bool stimulation::Stimulator<N,rng_t>::next_stimulus_available() const
{
	return (stimuli.size() > 1);
}

template <std::size_t N, class rng_t>
bool stimulation::Stimulator<N,rng_t>::time_for_next_stimulus(double time) const
{
	return (time >= stimuli[1].time);
}

template <std::size_t N, class rng_t>
void stimulation::Stimulator<N,rng_t>::move_to_next_stimulus()
{
	stimuli.erase(stimuli.begin());
}

template <std::size_t N, class rng_t>
stimulation::Stimulator<N,rng_t>::Stimulator(std::filesystem::path path, rng_t& rng) : rng(rng)
{
	double time;
	char mode;
	std::size_t idx_param1, idx_param2;
	double current;
	std::size_t update_interval;
	nvec<N> stimulus_current;
	std::ifstream infile(path);
	for( std::string line; getline( infile, line ); )
	{
		std::istringstream iss(line);
		iss >> time >> mode >> idx_param1 >> idx_param2 >> current >> update_interval;
		time *= 60 * 60;

		switch (mode)
		{
			case 'S':
				stimulus_current = generate_well_defined_currents<N>(idx_param1, idx_param2, current);
				break;
			case 'R':
				stimulus_current = generate_random_currents<N>(idx_param1, idx_param2, current, rng);
				break;
		}
		stimuli.push_back({time, stimulus_current});
	}
	infile.close();

	if (stimuli.size() < 1)
		throw std::runtime_error("No stimuli in file.");
}

template <std::size_t N, class rng_t>
nvec<N> stimulation::Stimulator<N,rng_t>::get_stimulus_currents(double time)
{
	if (next_stimulus_available() and time_for_next_stimulus(time))
		move_to_next_stimulus();

	return stimuli[0].currents;
}

#endif

