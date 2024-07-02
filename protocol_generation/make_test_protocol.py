from protocol import Protocol, RestFactory, SenseFactory, LearnFactory
from group import Group


n_total: int = 20
file_path = '../test_protocol.txt'

rest_fac = RestFactory(n_total,
                       mean_duration = 2.,
                       log_interval  = 3600.)

sense_fac = SenseFactory(group = Group(n_total, 10, 20),
                         n_firings        = 3,
                         mean_duration    = 4.,
                         log_interval     = 3600.,
                         stimulus_current = 50.)

learn_fac = LearnFactory(groups = [Group(n_total, 0, 5),
                                   Group(n_total, 5, 10)],
                         learn_duration   = .005,
                         rest_duration    = .01 ,
                         log_interval     = 900.,
                         stimulus_current = 200.)

protocol = Protocol(learn_fac,
                    sense_fac,
                    rest_fac,
                    learn_phase_duration      = 1.,
                    post_learn_phase_duration = 48.)

protocol.to_file(file_path)

