#include <random>
#include <iostream>
#include <filesystem>

#include "Network.hpp"
#include "ParadigmFile.hpp"
#include "logging.hpp"
#include "stimulation.hpp"

std::filesystem::path parse_command_line_arguments(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <path to protocol file>" << std::endl;
		std::exit(1);
	}
	return std::filesystem::path(argv[1]);
}

int main(int argc, char* argv[])
{
	constexpr std::size_t N = 20;
	constexpr double dt = 1.; // seconds

	Network<N> network(0, 16);

	const std::filesystem::path paradigm_file_path = parse_command_line_arguments(argc, argv);
	ParadigmFile paradigm_file(paradigm_file_path);

	logging::Logger logger = paradigm_file.create_logger();

	std::random_device rd;
	std::mt19937 rng(rd());
	stimulation::Stimulator<N> stimulator = paradigm_file.create_stimulator<N,std::mt19937>(rng);

	const double simulation_time = 0.0025 * 60 * 60;
	std::size_t step = 0;
	for (double time = 0.; time < simulation_time; time += dt)
	{
		std::cout << "time=" << time / 3600. << std::endl;
		nvec<N> stimulus_currents = stimulator.get_stimulus_currents(time);
		network.update(time, dt, stimulus_currents);
		logger.handle_logging(time, step, network);
		step++;
	}
	logger.log(network);

	return 0;
}

