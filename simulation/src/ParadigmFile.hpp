#ifndef PARADIGM_FILE_HPP
#define PARADIGM_FILE_HPP

//reads, checks integrity and stores everything in the file somehow, doesn't matter how
//functions to create Logger and stimulation::Paradigm

#include <filesystem>
#include <fstream>

#include "logging.hpp"
#include "stimulation.hpp"

struct ParadigmFileEntry
{
	double start_time;
	char mode; // TODO: change to enum
	std::size_t idx_param1, idx_param2;
	double current;
	std::size_t log_interval;
};

class ParadigmFile
{
private:
	std::vector<ParadigmFileEntry> entries;
public:
	ParadigmFile(std::filesystem::path paradigm_file_path);
	logging::Logger create_logger() const;
	template <std::size_t N, class rng_t>
	stimulation::Stimulator<N> create_stimulator(rng_t& rng) const;
};

ParadigmFile::ParadigmFile(std::filesystem::path paradigm_file_path)
{
	std::ifstream paradigm_file(paradigm_file_path);
	if (!paradigm_file.is_open())
		throw std::runtime_error("Could not open paradigm file.");

	std::string line;
	for(std::size_t line_num; getline(paradigm_file, line); line_num++)
	{
		ParadigmFileEntry tmp;
		std::istringstream iss(line);
		iss >> tmp.start_time >> tmp.mode >> tmp.idx_param1 >> tmp.idx_param2 >> tmp.current >> tmp.log_interval;
		tmp.start_time *= 60 * 60;

		if ((tmp.mode=='S') and (tmp.idx_param2 <= tmp.idx_param1))
			printf("Indices in stim-file are unordered. Skipping line.\n");
		else if ((entries.size() > 1) and (tmp.start_time<= entries.back().start_time))
			printf("Times in stim-file are unordered. Skipping line.\n");
		else
			entries.push_back(tmp);
	}
	paradigm_file.close();
}

logging::Logger ParadigmFile::create_logger() const
{
	std::vector<logging::LoggerState> paradigm(entries.size());
	std::transform(entries.begin(), entries.end(), paradigm.begin(), [](ParadigmFileEntry entry)
			{
				logging::LoggerState state;
				state.start_time = entry.start_time;
				state.log_interval = entry.log_interval;
				return state;
			});
	return logging::Logger(paradigm);
}

template<std::size_t N, class rng_t>
stimulation::Stimulator<N> ParadigmFile::create_stimulator(rng_t& rng) const
{
	std::vector<stimulation::Stimulus<N>> stimuli;
	for(ParadigmFileEntry entry : entries)
	{
		stimulation::Stimulus<N> stimulus;
		stimulus.start_time = entry.start_time;
		switch (entry.mode)
		{
			case 'S':
				stimulus.currents = stimulation::generate_well_defined_currents<N>(entry.idx_param1, entry.idx_param2, entry.current);
				break;
			case 'R':
				stimulus.currents = stimulation::generate_random_currents<N>(entry.idx_param1, entry.idx_param2, entry.current, rng);
				break;
		}
		stimuli.push_back(stimulus);
	}
	return stimulation::Stimulator<N>(stimuli);
}

#endif

