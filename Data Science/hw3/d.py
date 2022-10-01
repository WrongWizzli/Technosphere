import functools


def counter(func):
    func.rdepth = 0
    ncalls = 0

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        nonlocal ncalls
        if ncalls == 0:
            wrapper.rdepth = 0
            wrapper.ncalls = 0
        func.rdepth += 1
        wrapper.ncalls += 1
        ncalls += 1
        res = func(*args, **kwargs)
        ncalls -= 1
        if func.rdepth > wrapper.rdepth:
            wrapper.rdepth = func.rdepth
        func.rdepth -= 1
        return res
    return wrapper
