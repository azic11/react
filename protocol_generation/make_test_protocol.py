from protocol import TotalProtocol, LearnProtocol, PostLearnProtocol
from group import Group


n_total: int = 20
file_path = '../test_protocol.txt'

protocol = TotalProtocol(learn_protocol = LearnProtocol(duration          = 1.,
                                                        learn_groups     = [Group(n_total, 0, 5),
                                                                            Group(n_total, 5, 10)],
                                                        learn_duration   = .005,
                                                        rest_duration    = .01 ,
                                                        stimulus_current = 200.,
                                                        log_interval     = 900.),
                         post_learn_protocol = PostLearnProtocol(duration = 48.,
                                                                 sensory_group               = Group(n_total, 10, 20),
                                                                 n_sensory_firings           = 3,
                                                                 sensory_current             = 50.,
                                                                 sensory_image_duration      = 1.,
                                                                 mean_sensory_phase_duration = 4.,
                                                                 mean_rest_phase_duration    = 2.,
                                                                 log_interval                = 3600.))

protocol.to_file(file_path)

