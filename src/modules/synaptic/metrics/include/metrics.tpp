#include "metrics.hpp"

template <std::size_t N>
ncountmat<N> synaptic::count(mat<N> weights)
{
	ncountmat<N> count;
	std::transform(weights.begin(), weights.end(), count.begin(), [](synaptic::vec<N> all_weights_one_neuron) -> ncountvec<N>
	{
		ncountvec<N> counts_one_neuron;
		std::transform(all_weights_one_neuron.begin(), all_weights_one_neuron.end(), counts_one_neuron.begin(), [](std::vector<double> weights_between_neuron_pair) -> std::size_t
		{
			return weights_between_neuron_pair.size();
		});
		return counts_one_neuron;
	});
	return count;
}

template<std::size_t N>
nmat<N> synaptic::calculate_accumulated_weights(synaptic::mat<N> weights)
{
	nmat<N> accumulated_weights;
	std::transform(weights.begin(), weights.end(), accumulated_weights.begin(), [](synaptic::vec<N> all_weights_one_neuron) -> nvec<N>
	{
		nvec<N> accumulated_weights_one_neuron;
		std::transform(all_weights_one_neuron.begin(), all_weights_one_neuron.end(), accumulated_weights_one_neuron.begin(), [](std::vector<double> weights_between_neuron_pair) -> double
		{
			return std::reduce(weights_between_neuron_pair.begin(), weights_between_neuron_pair.end());
		});
		return accumulated_weights_one_neuron;
	});
	return accumulated_weights;
}


