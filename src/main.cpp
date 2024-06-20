#include <random>

#include "Network.hpp"
#include "io.hpp"
#include "stimulation.hpp"


int main()
{
	constexpr std::size_t N = 10;
	constexpr double dt = 1.; // seconds
	const std::filesystem::path stimulus_file_path("../../stimulation_long_term.txt");

	Network<N> network;

	std::random_device rd;
	std::mt19937 rng(rd());
	stimulation::Paradigm<N,std::mt19937> paradigm(stimulus_file_path, rng);

	std::ofstream potentials_log("potentials_log.txt");
	std::ofstream synapse_count_log("synapse_count_log.txt");
	std::ofstream accumulated_weights_log("accumulated_weights_log.txt");

	neural::vec<N> potentials;
	synaptic::mat<N> weights;
	ncountmat<N> synapse_counts;
	nmat<N> accumulated_weights;

	const double simulation_time = 1. * 60 * 60;
	for (double time = 0.; time < simulation_time; time += dt)
	{
		nvec<N> stimulus_current = paradigm.get_currents(time);
		network.update(0.1, stimulus_current);
		potentials = network.get_potentials();
		weights = network.get_weights();
		synapse_counts = synaptic::count(weights);
		accumulated_weights = synaptic::calculate_accumulated_weights(weights);

		io::write::iterable(potentials_log, potentials);
		io::write::double_iterable(synapse_count_log, synapse_counts);
		io::write::double_iterable(accumulated_weights_log, accumulated_weights);
	}

	potentials_log.close();
	synapse_count_log.close();
	accumulated_weights_log.close();

	return 0;
}

