#include <random>

#include "Network.hpp"
#include "logging.hpp"
#include "stimulation.hpp"


int main()
{
	constexpr std::size_t N = 10;
	constexpr double dt = 1.; // seconds
	const std::filesystem::path paradigm_file_path("../../stimulation_long_term.txt");

	Network<N> network;

	logging::Logger logger(paradigm_file_path);

	std::random_device rd;
	std::mt19937 rng(rd());
	stimulation::Paradigm<N,std::mt19937> paradigm(paradigm_file_path, rng);

	const double simulation_time = 1. * 60 * 60;
	std::size_t step = 0;
	for (double time = 0.; time < simulation_time; time += dt)
	{
		nvec<N> stimulus_current = paradigm.get_currents(time);
		network.update(0.1, stimulus_current);
		logger.handle_logging(time, step, network);
		step++;
	}
	logger.log(network);

	return 0;
}

