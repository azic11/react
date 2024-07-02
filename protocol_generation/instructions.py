from dataclasses import dataclass

from group import Group


@dataclass
class Instruction:
    time: float
    log_interval: float

    def __str__(self):
        raise NotImplementedError

@dataclass
class Rest(Instruction):
    n_total: int

    def __str__(self):
        return (f'{self.time:.3f}\t'
                 'S\t'
                f'{0}\t'
                f'{self.n_total - 1}\t'
                f'{0}\t'
                f'{self.log_interval}')


@dataclass
class Learn(Instruction):
    group: Group
    stimulus_current: float

    def __str__(self):
        return (f'{self.time:.3f}\t'
                 'S\t'
                f'{self.group.start_idx}\t'
                f'{self.group.end_idx - 1}\t'
                f'{self.stimulus_current}\t'
                f'{self.log_interval}')


@dataclass
class Sense(Instruction):
    group: Group
    n_firings: int
    stimulus_current: float

    def __post_init__(self):
        if self.n_firings < 0:
            raise ValueError('n_firings must be non-negative')
        if self.n_firings > self.group.size:
            raise ValueError('n_firings must be less than or equal to group size')

    def __str__(self):
        return (f'{self.time:.3f}\t'
                 'R\t'
                f'{self.group.start_idx}\t'
                f'{self.n_firings}\t'
                f'{self.stimulus_current}\t'
                f'{self.log_interval}')

