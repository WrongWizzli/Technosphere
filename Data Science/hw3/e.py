import itertools


def chain_loop(iterables):

    for i in range(len(iterables)):
        if not hasattr(iterables[i], '__next__'):
            iterables[i] = iter(iterables[i])
    exhausted_sum = 0
    exhausted = [0] * len(iterables)
    while exhausted_sum != len(iterables):
        for i, iterable in enumerate(iterables):
            if exhausted[i] == 0:
                try:
                    yield next(iterable)
                except StopIteration as s:
                    exhausted[i] = 1
                    exhausted_sum += 1
