#include "Network.hpp"
#include "logging.hpp"


int main()
{
	constexpr std::size_t N = 10;
	Network<N> network;

	nvec<N> stimulus_current;
	stimulus_current.fill(0.0);

	std::ofstream potentials_log("potentials_log.txt");
	std::ofstream synapse_count_log("synapse_count_log.txt");
	std::ofstream accumulated_weights_log("accumulated_weights_log.txt");

	neural::vec<N> potentials;
	synaptic::mat<N> weights;
	ncountmat<N> synapse_counts;
	nmat<N> accumulated_weights;
	for (int i = 0; i < 10; ++i)
	{
		network.update(0.1, stimulus_current);
		potentials = network.get_potentials();
		weights = network.get_weights();
		synapse_counts = synaptic::count(weights);
		accumulated_weights = synaptic::calculate_accumulated_weights(weights);

		logging::log_iterable(potentials_log, potentials);
		logging::log_double_iterable(synapse_count_log, synapse_counts);
		logging::log_double_iterable(accumulated_weights_log, accumulated_weights);
	}

	potentials_log.close();
	synapse_count_log.close();
	accumulated_weights_log.close();

	return 0;
}

