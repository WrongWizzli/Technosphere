def solution1(s):
    return [4 * c for c in s]


def solution2(s):
    return [(i + 1) * c for i, c in enumerate(s)]


def solution3(rng):
    return [i for i in rng if (i % 3 == 0 or i % 5 == 0)]


def solution4(lst):
    return [x for m in lst for x in m]


def solution5(n):
    return [(j, i, k) for i in range(1, n + 1) for j in range(1, i) for k in range(i, n + 1) if i * i + j * j == k * k]


def solution6(t):
    return [[elem_l + elem_r for elem_r in t[1]] for elem_l in t[0]]


def solution7(mat):
    return [[mat[j][i] for j in range(len(mat))] for i in range(len(mat[0]))]


def solution8(lst):
    return [[int(elem) for elem in s.split(" ")] for s in lst]


def solution9(rng):
    return {chr(ord('a') + i): i * i for i in rng}


def solution10(lst):
    return {name[0].upper() + name[1:].lower() for name in lst if len(name) > 3}


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
