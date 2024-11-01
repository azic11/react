#include <random>
#include <iostream>
#include <filesystem>

#include "Network.hpp"
#include "ParadigmFile.hpp"
#include "logging.hpp"
#include "stimulation.hpp"

struct Args
{
	std::filesystem::path protocol_file;
	std::filesystem::path output_dir;
};

Args parse_command_line_arguments(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <protocol file> <output directory>" << std::endl;
		std::exit(1);
	}

	return Args({argv[1], argv[2]});
}

int main(int argc, char* argv[])
{
	constexpr std::size_t N = 20;
	constexpr double dt = 1.; // seconds

	Network<N> network(0, 16);

	const Args args = parse_command_line_arguments(argc, argv);
	ParadigmFile paradigm_file(args.protocol_file);

	logging::Logger logger = paradigm_file.create_logger(args.output_dir);

	std::random_device rd;
	std::mt19937 rng(rd());
	stimulation::Stimulator<N> stimulator = paradigm_file.create_stimulator<N,std::mt19937>(rng);

	const double simulation_time = 2. * 60 * 60;
	std::size_t step = 0;
	for (double time = 0.; time < simulation_time; time += dt)
	{
		std::cout << "time=" << time / 3600. << "\r" << std::flush;
		nvec<N> stimulus_currents = stimulator.get_stimulus_currents(time);
		network.update(time, dt, stimulus_currents);
		logger.handle_logging(time, step, network);
		step++;
	}
	std::cout << std::endl;

	logger.log(network);

	return 0;
}

