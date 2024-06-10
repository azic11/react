#include "Network.hpp"

int main()
{
	Network<10> network;
	nvec<10> stimulus_current;
	stimulus_current.fill(0.0);
	for (int i = 0; i < 10; ++i)
	{
		network.update(0.1, stimulus_current);
	}

	return 0;
}

