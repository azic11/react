#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

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
		Logger(std::vector<LoggerState> paradigm, std::filesystem::path output_dir);
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

logging::Logger::Logger(std::vector<LoggerState> paradigm, std::filesystem::path output_dir)
{
	if (paradigm.size() < 1)
		throw std::runtime_error("No log paradigm entries given.");

	for (std::size_t i = 0; i < paradigm.size(); i++)
		if (paradigm[i].log_interval == 0)
			throw std::runtime_error(std::string("Cannot set log interval to 0 (entry ") + std::to_string(i) + std::string(")."));

	for (std::size_t i = 1; i < paradigm.size(); i++)
		if (paradigm[i].start_time < paradigm[i-1].start_time)
			throw std::runtime_error(std::string("Paradigm times are unordered (entry " + std::to_string(i) + std::string(").")));

	this->paradigm = paradigm;

	if (!std::filesystem::exists(output_dir))
		throw std::runtime_error("Output directory does not exist.");

	potentials_log.open(output_dir / "potentials_log.txt");
	if (!potentials_log.is_open())
		throw std::runtime_error("Could not open potentials log.");
	synapse_count_log.open(output_dir / "synapse_count_log.txt");
	if (!synapse_count_log.is_open())
		throw std::runtime_error("Could not open synapse count log.");
	accumulated_weights_log.open(output_dir / "accumulated_weights_log.txt");
	if (!accumulated_weights_log.is_open())
		throw std::runtime_error("Could not open accumulated weights log.");
}

logging::Logger::~Logger()
{
	if (potentials_log.fail())
		std::cerr << "Error writing potentials log." << std::endl;
	if (synapse_count_log.fail())
		std::cerr << "Error writing synapse count log." << std::endl;
	if (accumulated_weights_log.fail())
		std::cerr << "Error writing accumulated weights log." << std::endl;

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

