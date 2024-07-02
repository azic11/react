from dataclasses import dataclass
from itertools import cycle
from pathlib import Path
from typing import List, Tuple

import numpy as np

from instructions import Rest, Sense, Learn, Instruction
from group import Group


@dataclass
class RestFactory:
    n_total: int
    mean_duration: float = 2.   # hrs
    log_interval: float  = 3600.

    def make(self, t: float) -> Tuple[float, Rest]:
        instr = Rest(t, self.log_interval, self.n_total)
        t_new = t + np.random.exponential(self.mean_duration)
        return t_new, instr


@dataclass
class SenseFactory:
    group: Group
    n_firings: int
    mean_duration: float    = 4.    # hrs
    log_interval: float     = 3600.
    stimulus_current: float = 50.

    @property
    def n_total(self):
        return self.group.n_total

    def make(self, t: float) -> Tuple[float, Sense]:
        instr = Sense(t, self.log_interval, self.group, self.n_firings, self.stimulus_current)
        t_new = t + np.random.exponential(self.mean_duration)
        return t_new, instr


@dataclass
class LearnFactory:
    groups: List[Group]
    learn_duration: float   = .005 # hrs
    rest_duration: float    = .01  # hrs
    log_interval: float     = 900.
    stimulus_current: float = 200.

    def __post_init__(self):
        if not all(g.n_total == self.groups[0].n_total for g in self.groups):
            raise ValueError("Groups belong to networks of different total sizes")

    @property
    def n_total(self):
        return self.groups[0].n_total

    def __group_permutation(self):
        yield from cycle(self.groups)

    def make_learn(self, t: float) -> Tuple[float, Learn]:
        group = next(self.__group_permutation())
        instr = Learn(t, self.log_interval, group, self.stimulus_current)
        t_new = t + self.learn_duration
        return t_new, instr

    def make_rest(self, t: float) -> Tuple[float, Rest]:
        instr = Rest(t, self.log_interval, self.n_total)
        t_new = t + self.rest_duration
        return t_new, instr


@dataclass
class Protocol:
    learn_fac: LearnFactory
    sense_fac: SenseFactory
    rest_fac: RestFactory
    learn_phase_duration: float
    post_learn_phase_duration :float

    def __post_init__(self):
        if not all(f.n_total == self.rest_fac.n_total for f in self.factories):
            raise ValueError("Factories adress networks of different total sizes")

    @property
    def factories(self):
        return (self.learn_fac, self.sense_fac, self.rest_fac)

    @property
    def instructions(self) -> List[Instruction]:
        t: float = 0
        instructs = []
        while t < self.learn_phase_duration:
            t, instr = self.learn_fac.make_learn(t)
            instructs.append(instr)
            t, instr = self.learn_fac.make_rest(t)
            instructs.append(instr)
        t_stop: float = self.learn_phase_duration + self.post_learn_phase_duration
        while t < t_stop:
            t, instr = self.sense_fac.make(t)
            instructs.append(instr)
            t, instr = self.rest_fac.make(t)
            instructs.append(instr)
        return instructs

    def __str__(self) -> str:
        return '\n'.join(map(str, self.instructions))

    def to_file(self, file_path: Path) -> None:
        file_path = Path(file_path)
        with file_path.open('w') as file:
            file.write(str(self))

