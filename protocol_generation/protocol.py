from dataclasses import dataclass
from itertools import cycle
from pathlib import Path
from typing import List, Tuple

import numpy as np

from instructions import Rest, Sense, Learn, Instruction
from group import Group


@dataclass
class LearnProtocol:
    duration: float
    learn_groups: List[Group]
    learn_duration: float   = .005 # hrs
    rest_duration: float    = .01 # hrs
    stimulus_current: float = 200.
    log_interval: float     = 900.

    def __post_init__(self):
        if not all(g.n_total == self.learn_groups[0].n_total for g in self.learn_groups):
            raise ValueError("Learn groups belong to networks of different total sizes")

    @property
    def n_total(self):
        return self.learn_groups[0].n_total

    def __group_permutation(self):
        yield from cycle(self.learn_groups)

    def generate_instructions(self, t_start: float) -> List[Instruction]:
        t_end: float = t_start + self.duration
        t: float = t_start
        instructs = []
        while t < t_end:
            group = next(self.__group_permutation())
            instructs.append(Learn(t, self.log_interval, group, self.stimulus_current))
            t += self.learn_duration
            instructs.append(Rest(t, self.log_interval, self.n_total))
            t += self.rest_duration
        return instructs


@dataclass
class PostLearnProtocol:
    duration: float
    sensory_group: Group
    n_sensory_firings: int
    sensory_current: float             = 50.
    sensory_image_duration: float      = 1.    # sec
    mean_sensory_phase_duration: float = 4.    # hrs
    mean_rest_phase_duration: float    = 2.    # hrs
    log_interval: float                = 3600.

    def generate_sensory_phase_instructions(self, t_start: float, t_duration: float) -> List[Instruction]:
        t_end: float = t_start + t_duration
        t: float = t_start
        instructs = []
        while t < t_end:
            instructs.append(Sense(t, self.log_interval, self.sensory_group, self.n_sensory_firings, self.sensory_current))
            t += self.sensory_image_duration / 3600.
        return instructs

    def generate_instructions(self, t_start: float) -> List[Instruction]:
        t_end: float = t_start + self.duration
        t: float = t_start
        instructs = []
        while t < t_end:
            sensory_phase_duration = np.random.exponential(self.mean_sensory_phase_duration)
            instructs.extend(self.generate_sensory_phase_instructions(t, sensory_phase_duration))
            t += sensory_phase_duration
            instructs.append(Rest(t, self.log_interval, self.sensory_group.n_total))
            t += np.random.exponential(self.mean_rest_phase_duration)
        return instructs


@dataclass
class TotalProtocol:
    learn_protocol: LearnProtocol
    post_learn_protocol: PostLearnProtocol

    def generate_instructions(self, t_start: float = 0.) -> List[Instruction]:
        return self.learn_protocol.generate_instructions(t_start) + \
               self.post_learn_protocol.generate_instructions(t_start + self.learn_protocol.duration)

    def __str__(self) -> str:
        return '\n'.join(map(str, self.generate_instructions()))

    def to_file(self, file_path: Path) -> None:
        file_path = Path(file_path)
        with file_path.open('w') as file:
            file.write(str(self))

