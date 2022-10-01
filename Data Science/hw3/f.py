def try_to_build_seq(seq, i, k):
    free_closing = 1
    for j in range(k):
        if (i & 1) == 1:
            free_closing -= 1
            seq[-2 - j] = '('
        else:
            free_closing += 1
            seq[-2 - j] = ')'
        if free_closing < 0:
            return False
        i >>= 1
    if free_closing == 1:
        return True
    return False


def brackets(n):
    if n == 1:
        yield '()'
    elif n != 0:
        k = 2 * n - 2
        half = n - 1
        seq = ['(' for i in range(2 * n)]
        seq[-1] = ')'
        for i in range(2 ** k - 1, 0, -1):
            n_ones = bin(i).count('1')
            if n_ones == half and try_to_build_seq(seq, i, k):
                yield ''.join(seq)


if __name__ == '__main__':
    for i in brackets(int(input())):
        print(i)
