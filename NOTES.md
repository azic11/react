# Notes

## Possible starting points for master thesis topic
- fluctuations inherent to synapses, not only on structural level? see
  abschnitt "Synaptic weight fluctuations"
    - in this case, research the nature of these fluctuations and attempt to
      model and integrate them in the model
- all-to-all connectivity assumption only valid in small networks simulating
  localised regions
- artificial hard bounds
    - e.g. max 16 synapses (but this could be sensible, like maybe biologically
      this is due to resources or whatever)

## Potential flaws in original code
- for initial synapses, each neuron has own (and same seeded??) random
  generator
- neural time-scale exactly the same as global inhibition time-scale
- even when a synapse is supposed to be created according the distribution, it
  might not be because there is no free place, so it is not totally according
  to the distribution. probably not a problem and might even be necessary for
  there not to be an arbitrary number of creations happening
- deletion prob distribution stated in paper was not accurately implemented???
  lifecounter approach not suitable since weights continually change? the
  current lifecounter value depends on the last decrement step which in turn
  depends on the weight during the last step. the deletion prob distr stated in
  the paper only depends on the weight at a given timestep, not the timesteps
  before. so how is this the same?
- IMPORTANT!!!! when random stimulation is switched on, is the previous
  stimulation completely switched off????? it seems like only those neurons
  after the supplied index1 are. yes, but it appears that they are set
  "manually" to 0 in the read in stimulation file
  
## Implementational differences which should not produce different functionality
- stimulus paradigm application (insbesondere random stimulus)
- we apply maximally one stimulus at a time, whereas in the original code this
  is only implicitly enforced by requiring the read-in stimulus times to be
  monotonically increasing
  
## TODO
### Simulation
#### musts
- [ ] cross check implemented ODEs with those of original code
- [X] implement stimulation
- [ ] implement structural plasticity
- [O] implement logging
  - [X] general mechanism with variable logging rates
  - [O] specific logs / measurements
    - [X] firing rates
    - [X] synapse weights
    - [X] synapse counts
    - [ ] binary representations of connections

#### maybes
- [ ] fast-logging
- [ ] switches- / up-down-logging
- [ ] benchmarking and optimisation where applicable
- [ ] unit tests where applicable
- [ ] option to read in initial weights and potentials
- [ ] switch public and private sections in classes
- [ ] introduce .cpp files where applicable
- [ ] clean up stimulation namespace (preferably just generate all currents in
      advance and keep them stored, then follow design just like
      logging::Logger)
- [ ] acute_log mechanism
- [ ] optimise Logger by reserving memory as members for to-log stuff if this
      is not already optimised away by compiler via reference passing or sth
- [ ] customised error classes where applicable (mostly io stuff)
- [ ] rename stimulation::Paradigm to stimulation::Stimulator?

### evaluation
- [ ] histograms
