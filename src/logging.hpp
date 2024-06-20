#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <vector>
#include <filesystem>
#include <fstream>

#include "io.hpp"
#include "Network.hpp"


namespace logging
{
	struct LoggerState
	{
		double start_time;
		std::size_t log_interval;
	};

	class Logger
	{
	private:
		std::vector<LoggerState> paradigm;

		std::ofstream potentials_log;
		std::ofstream synapse_count_log;
		std::ofstream accumulated_weights_log;

		LoggerState current_state() const;
		LoggerState next_state() const;
		bool next_state_available() const;
		bool time_to_log(std::size_t step) const;
		bool time_for_next_state(double time) const;
		void move_to_next_state();
	public:
		Logger(std::filesystem::path paradigm_file_path);
		~Logger();
		template <std::size_t N>
		void log(const Network<N>& net);
		template <std::size_t N>
		void handle_logging(double time, std::size_t step, const Network<N>& net);
	};
}

logging::LoggerState logging::Logger::current_state() const
{
	return paradigm[0];
}

logging::LoggerState logging::Logger::next_state() const
{
	return paradigm[1];
}

bool logging::Logger::next_state_available() const
{
	return paradigm.size() > 1;
}

bool logging::Logger::time_to_log(std::size_t step) const
{
	return (step % current_state().log_interval) == 0;
}

bool logging::Logger::time_for_next_state(double time) const
{
	return (time >= next_state().start_time);
}

void logging::Logger::move_to_next_state()
{
	paradigm.erase(paradigm.begin());
}

logging::Logger::Logger(std::filesystem::path paradigm_file_path)
{
	std::ifstream paradigm_file(paradigm_file_path);
	if (!paradigm_file.is_open())
		throw std::runtime_error("Could not open paradigm file.");

	LoggerState tmp;
	std::size_t line_num = 0;
	for( std::string line; getline( paradigm_file, line ); line_num++)
	{
		std::istringstream iss(line);
		iss >> tmp.start_time;
		for (std::size_t i = 0; i < 5; i++)
			iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
		iss >> tmp.log_interval;
		if (tmp.log_interval == 0)
			throw std::runtime_error(std::string("Cannot set log interval to 0 (line ") + std::to_string(line_num) + std::string(")!"));
		paradigm.push_back(tmp);
	}
	paradigm_file.close();

	if (paradigm.size() < 1)
		throw std::runtime_error("No log paradigm entries found!");

	potentials_log.open("potentials_log.txt");
	synapse_count_log.open("synapse_count_log.txt");
	accumulated_weights_log.open("accumulated_weights_log.txt");
}

logging::Logger::~Logger()
{
	potentials_log.close();
	synapse_count_log.close();
	accumulated_weights_log.close();
}

template <std::size_t N>
void logging::Logger::log(const Network<N>& net)
{
	neural::vec<N> potentials = net.get_potentials();
	synaptic::mat<N> weights = net.get_weights();

	ncountmat<N> synapse_counts = synaptic::count(weights);
	nmat<N> accumulated_weights = synaptic::calculate_accumulated_weights(weights);

	io::write::iterable(potentials_log, potentials);
	io::write::double_iterable(synapse_count_log, synapse_counts);
	io::write::double_iterable(accumulated_weights_log, accumulated_weights);
}

template <std::size_t N>
void logging::Logger::handle_logging(double time, std::size_t step, const Network<N>& net)
{
	if (next_state_available() and time_for_next_state(time))
		move_to_next_state();

	if (time_to_log(step))
		log(net);
}

#endif

