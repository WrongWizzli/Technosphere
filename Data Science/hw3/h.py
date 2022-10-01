from collections import deque


nums = [chr(x + ord('0')) for x in range(10)]
alphabet = [chr(x + ord('A')) for x in range(26)] + [chr(x + ord('a')) for x in range(26)]
starters = nums + alphabet


class Node:
    def __init__(self, sym):
        self.sym = sym
        self.subtrees = {}
        self.word_end = False
        self.words = 0

    def add_subtree(self, symbol):
        if symbol not in self.subtrees:
            self.subtrees[symbol] = Node(symbol)

    def __contains__(self, symbol):
        return (symbol in self.subtrees)

    def get_subtree(self, symbol):
        return self.subtrees[symbol]

    def clear(self):
        self.subtrees.clear()

    def empty(self):
        return len(self.subtrees)

    def keys(self):
        return self.subtrees.keys()


class TrieIterator:

    call_stack = deque()
    depth = 0
    prefix = ""

    def __init__(self, node, prefix=""):
        if node is not None:
            self.root = node
            self.prefix = prefix
            self.call_stack.append("")

    def __iter__(self):
        return self

    def __next__(self):
        while(self.call_stack):
            node = self.root
            cur_word = self.call_stack.popleft()
            for sym in cur_word:
                if sym in node:
                    node = node.get_subtree(sym)
            for sym in starters:
                if sym in node:
                    self.call_stack.append(cur_word + sym)
            if node.word_end:
                return self.prefix + cur_word
        raise StopIteration


class Trie:

    length = 0

    def __init__(self):
        self.root = Node('\0')

    def __len__(self):
        return self.length

    def __contains__(self, word):
        node = self.root
        for sym in word:
            try:
                node = node.get_subtree(sym)
            except KeyError:
                return False
        if not node.word_end:
            return False
        return True

    def __iter__(self):
        return TrieIterator(self.root)

    def starts_with(self, prefix):
        node = self.root
        for sym in prefix:
            if sym not in node:
                return iter(Trie())
            node = node.get_subtree(sym)
        return TrieIterator(node, prefix)

    def add(self, word):
        node = self.root
        for sym in word:
            if sym not in node:
                node.add_subtree(sym)
            node.words += 1
            node = node.get_subtree(sym)
        if not node.word_end:
            node.word_end = True
            self.length += 1

    def check_word(self, word):
        node = self.root
        for sym in word:
            if sym not in node:
                raise KeyError(word)
            node = node.get_subtree(sym)
        if not node.word_end:
            raise KeyError(word)

    def pop(self, word):

        last_ind = len(word) - 1
        self.check_word(word)
        node = self.root

        for i, sym in enumerate(word):
            if node.words == 1 and not node.empty():
                node.clear()
                node.words = 0
                break
            else:
                node = node.get_subtree(sym)
                node.words -= 1

        self.length -= 1
        node.word_end = False
        return word
