from functools import reduce
from operator import setitem, add


def solution1(lst):
    return list(map(lambda x: int("".join(filter(lambda y: y >= '0' and y <= '9', x[::-1]))), lst))


def solution2(zp):
    return list(map(lambda p: p[0] * p[1], zp))


def solution3(rng):
    return list(filter(lambda x: x % 6 != 1 and x % 6 != 3 and x % 6 != 4, rng))


def solution4(lst):
    return list(filter(lambda x: x not in [[], None, "", 0.0], lst))


def solution5(lst):
    return list(map(lambda x: setitem(x, 'square', x['width'] * x['length']), lst))


def solution6(lst):
    return list(map(lambda x: {**x, 'square': x['width'] * x['length']}, lst))


def solution7(lst):
    return reduce(lambda x, y: x & y, lst)


def solution8(lst):
    return dict(map(lambda x: (x, lst.count(x)), lst))


def solution9(lst):
    return list(map(lambda x: x['name'], filter(lambda x: x['gpa'] > 4.5, lst)))


def solution10(lst):
    return list(filter(lambda x: reduce(add, map(int, x[::2])) == reduce(add, map(int, x[1::2])), lst))


solutions = {
    'solution1': solution1,
    'solution2': solution2,
    'solution3': solution3,
    'solution4': solution4,
    'solution5': solution5,
    'solution6': solution6,
    'solution7': solution7,
    'solution8': solution8,
    'solution9': solution9,
    'solution10': solution10,
}
