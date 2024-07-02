class Group:
    def __init__(self, n_total: int, start_idx: int, end_idx: int):
        if start_idx >= end_idx:
            raise ValueError('start_idx must be less than end_idx')
        if end_idx > n_total:
            raise ValueError('end_idx must be less than or equal to n_total')

        self.n_total = n_total
        self.start_idx = start_idx
        self.end_idx = end_idx

    @property
    def size(self):
        return self.end_idx - self.start_idx
