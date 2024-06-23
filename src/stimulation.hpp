#ifndef STIMULATION_HPP
#define STIMULATION_HPP

#include <vector>
#include <algorithm>

#include "global_types.hpp"


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
		double start_time;
		nvec<N> currents;
	};

	template <std::size_t N>
	class Stimulator
	{
	private:
		std::vector<Stimulus<N>> stimuli;

		bool next_stimulus_available() const;
		bool time_for_next_stimulus(double time) const;
		void move_to_next_stimulus();
	public:
		Stimulator(std::vector<Stimulus<N>> stimuli);
		nvec<N> get_stimulus_currents(double time);
	};
}

template <std::size_t N>
bool stimulation::Stimulator<N>::next_stimulus_available() const
{
	return (stimuli.size() > 1);
}

template <std::size_t N>
bool stimulation::Stimulator<N>::time_for_next_stimulus(double time) const
{
	return (time >= stimuli[1].start_time);
}

template <std::size_t N>
void stimulation::Stimulator<N>::move_to_next_stimulus()
{
	stimuli.erase(stimuli.begin());
}

template <std::size_t N>
stimulation::Stimulator<N>::Stimulator(std::vector<Stimulus<N>> stimuli)
{
	if (stimuli.size() < 1)
		throw std::runtime_error("No stimuli given.");

	for (std::size_t i = 1; i < stimuli.size(); i++)
		if (stimuli[i].start_time < stimuli[i-1].start_time)
			throw std::runtime_error(std::string("Paradigm times are unordered (entry " + std::to_string(i) + std::string(").")));

	this->stimuli = stimuli;
}

template <std::size_t N>
nvec<N> stimulation::Stimulator<N>::get_stimulus_currents(double time)
{
	if (next_stimulus_available() and time_for_next_stimulus(time))
		move_to_next_stimulus();

	return stimuli[0].currents;
}

#endif

