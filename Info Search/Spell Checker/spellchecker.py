import numpy as np
from tqdm import tqdm
from Levenshtein import distance as levenshtein_distance, editops
import sys
from catboost import CatBoostClassifier
import pickle

def get_weighted_index(n):
    tmp = np.arange(n)
    return np.random.choice(tmp, size=None, p=(n - tmp) / (tmp.sum() + n))
def gbs(cresult, orig, n_features=30):
    l = []
    for i in range(n_features):
        s = ""
        for elem in cresult:
            s += elem[get_weighted_index(len(elem))][0]
        l.append(Err_model.predict(orig, s))
    return np.asarray(l)

def get_best_fix(fixes):
    s = ""
    for word in fixes:
        s += word[0][0] + ' '
    return s


def get_letter(mistakes):
    keys = list(mistakes.keys())
    vals = np.asarray(list(mistakes.values()))
    return np.random.choice(keys, 1, p=vals / vals.sum())[0]
def gen_incorrect_queries(cq):
    wrong_queries = []
    c_queries = []
    for i, q in enumerate(tqdm(cq)):
        a = []
        for j in range(np.random.randint(1, 3)):
            a.append(np.random.randint(len(q)))
            while q[a[-1]] == ' ':
                a[-1] = np.random.randint(len(q))
        s = ""
        for j in range(len(q)):
            if j in a:
                if q[j] in Err_model.unigrams:
                    s += get_letter(Err_model.unigrams[q[j]][0])
                else:
                    tl = list(Err_model.unigrams.keys())
                    s += tl[np.random.randint(len(tl))]
            else:
                s += q[j]
        if s != q:
            wrong_queries.append(s)
            c_queries.append(q)
    return wrong_queries, c_queries


class Language_Model:
    def __init__(self):
        self.p = {}
        self.pair_p = {}
        self.total_w = 0
        self.total_pair = 0
        
    def fit(self, cq):
        for query in tqdm(cq):
            words = query.split()
            l = len(words) - 1
            self.total_w += l + 1
            for i, word in enumerate(words):
                if word in self.p:
                    self.p[word] += 1
                else:
                    self.p[word] = 1
                if i != l:
                    self.total_pair += 1
                    if words[i + 1] in self.pair_p:
                        self.pair_p[words[i + 1]][1] += 1
                        if word in self.pair_p[words[i + 1]][0]:
                            self.pair_p[words[i + 1]][0][word] += 1
                        else:
                            self.pair_p[words[i + 1]][0][word] = 1
                    else:
                        self.pair_p[words[i + 1]] = [{}, 1]
                        self.pair_p[words[i + 1]][0][word] = 1
                        
    def get_value(self, w1, w2, is_pair):
        if is_pair:
            if w2 in self.pair_p:
                if w1 in self.pair_p[w2][0]:
                    return self.pair_p[w2][0][w1] / self.pair_p[w2][1]
                else:
                    return 1 / self.total_pair
            else:
                return 1 / self.total_pair
        else:
            if w1 in self.p:
                return self.p[w1] / self.total_w
            else:
                return 1 / self.total_w
    
    def predict(self, query):
        words = query.strip().split()
        l = len(words) - 1
        probability = 1
        for i, word in enumerate(words):
            if i != l:
                probability *= self.get_value(word, words[i + 1], True)
            else:
                probability *= self.get_value(word, None, False)
        return probability

with open('lang.bin', 'rb') as f:
    LangModel = pickle.load(f)

class Error_Model:
    def __init__(self):
        self.bigrams = {}
        self.unigrams = {}
        self.unitransforms = 0
        self.bitransforms = 0
    
    def fill_unigram(self, key1, key2):
        if key1 in self.unigrams:
            if key2 in self.unigrams[key1][0]:
                self.unigrams[key1][0][key2] += 1
            else:
                self.unigrams[key1][0][key2] = 1
            self.unigrams[key1][1] += 1
        else:
            self.unigrams[key1] = [{}, 1]
            self.unigrams[key1][0][key2] = 1
    
    def fill_bigram(self, key1, key2):
        if key1 in self.bigrams:
            if key2 in self.bigrams[key1][0]:
                self.bigrams[key1][0][key2] += 1
            else:
                self.bigrams[key1][0][key2] = 1
            self.bigrams[key1][1] += 1
        else:
            self.bigrams[key1] = [{}, 1]
            self.bigrams[key1][0][key2] = 1
    
    def fit(self, cqueries, wqueries):
        i = 0
        for cq, wq in zip(tqdm(cqueries), wqueries):
            editions = editops(wq, cq)
            l = len(wq) - 1
            for i, ed in enumerate(editions):
                self.unitransforms += 1
                if ed[0][0] == 'i':
                    self.fill_unigram('', cq[ed[2]])
                    self.bitransforms += 1
                    if ed[1] != 0:
                        self.fill_bigram(wq[ed[1] - 1], wq[ed[1] - 1] + cq[ed[2]])
                    if l != ed[1] - 1:
                        self.fill_bigram(wq[ed[1]], cq[ed[2]] + wq[ed[1]])
                elif ed[0][0] == 'd':
                    self.fill_unigram(wq[ed[1]], '')
                    if ed[1] != 0:
                        self.bitransforms += 1
                        self.fill_bigram(wq[ed[1]-1 : ed[1]+1], wq[ed[1]-1])
                    if ed[1] != l:
                        self.bitransforms += 1
                        self.fill_bigram(wq[ed[1] : ed[1]+2], wq[ed[1]+1])
                else:
                    self.fill_unigram(wq[ed[1]], cq[ed[2]])
                    if ed[1] != 0:
                        self.bitransforms += 1
                        self.fill_bigram(wq[ed[1]-1 : ed[1]+1], wq[ed[1]-1] + cq[ed[2]])
                    if ed[1] != l:
                        self.bitransforms += 1
                        self.fill_bigram(wq[ed[1] : ed[1]+2], cq[ed[2]] + wq[ed[1]+1])
    
    def try_unigram(self, key1, key2):
        if key1 in self.unigrams:
            if key2 in self.unigrams[key1][0]:
                return self.unigrams[key1][0][key2] / self.unigrams[key1][1]
            else:
                return 1 / self.unigrams[key1][1]
        return 1 / self.unitransforms
    
    def try_bigram(self, key1, key2):
        if key1 in self.bigrams:
            if key2 in self.bigrams[key1][0]:
                return self.bigrams[key1][0][key2] / self.bigrams[key1][1]
            else:
                return 1 / self.bigrams[key1][1]
        return 1 / self.bitransforms
    
    def predict(self, orig, fix):
        orig = orig.lower()
        fix = fix.lower()
        editions = editops(orig, fix)
        l = len(orig) - 1
        probability = 1 # 1/5, 2/5, 2/5
        for i, ed in enumerate(editions):
            probs = []
            if ed[0][0] == 'i':
                probs.append(self.try_unigram('', fix[ed[2]]))
                if ed[1] != 0:
                    probs.append(self.try_bigram(orig[ed[1] - 1], orig[ed[1] - 1] + fix[ed[2]]))
                if l != ed[1] - 1:
                    probs.append(self.try_bigram(orig[ed[1]], fix[ed[2]] + orig[ed[1]]))
            elif ed[0][0] == 'd':
                probs.append(self.try_unigram(orig[ed[1]], ''))
                if ed[1] != 0:
                    probs.append(self.try_bigram(orig[ed[1]-1:ed[1]+1], orig[ed[1]-1]))
                elif ed[1] != l:
                    probs.append(self.try_bigram(orig[ed[1] : ed[1]+2], orig[ed[1]+1]))
            else:
                probs.append(self.try_unigram(orig[ed[1]], fix[ed[2]]))
                if ed[1] != 0:
                    probs.append(self.try_bigram(orig[ed[1]-1 : ed[1]+1], orig[ed[1]-1] + fix[ed[2]]))
                if ed[1] != l:
                    probs.append(self.try_bigram(orig[ed[1] : ed[1]+2], fix[ed[2]] + orig[ed[1]+1]))
            if len(probs) == 1:
                probability *= probs[0]
            elif len(probs) == 2:
                probability *= (probs[0] * 1 / 3 + probs[1] * 2 / 3)
            else:
                probability *= (probs[0] * 1 / 5 + probs[1] * 2 / 5 + probs[2] * 2 / 5)
        return probability

with open('error.bin', 'rb') as f:
    Err_model = pickle.load(f)

class CorrectWordGenerator:
    
    def __init__(self, errmodel, langmodel, thresh=1e-2):
        self.pwords = []
        self.em = errmodel
        self.lm = langmodel
        self.thresh = thresh
        self.best_p = thresh
    
    def get_value_fix(self, p, fix, word):
        lv = self.lm.get_value(fix, None, False)
        return np.log2(p) + np.log2(lv) * (1 + abs(len(fix) - len(word)) / len(word)) ** 2
    
    def recursive_word_fix(self, word, i, cur_p, fix):
        if i == len(word):
            val = self.get_value_fix(cur_p, fix, word)
            if val > self.best_p:
                self.best_p = val
            yield [fix, cur_p]
            return
        if self.em.predict(word[:len(fix)], fix) < self.thresh:
            return
        if word[i] in self.em.unigrams:
            for key in self.em.unigrams[word[i]][0]:
                tcur_p = cur_p * self.em.unigrams[word[i]][0][key] / self.em.unigrams[word[i]][1]
                if tcur_p > self.thresh:
                    yield from self.recursive_word_fix(word, i + 1, tcur_p, fix + key)
        if word[i] in self.em.bigrams:
            for key in self.em.bigrams[word[i]][0]:
                tcur_p = cur_p * self.em.bigrams[word[i]][0][key] / self.em.bigrams[word[i]][1]
                if tcur_p > self.thresh:
                    yield from self.recursive_word_fix(word, i + 1, tcur_p, fix + key)               
        if i != 0:
            tmp = word[i-1:i+1]
            if tmp in self.em.bigrams:
                for key in self.em.bigrams[tmp][0]:
                    tcur_p = cur_p * self.em.bigrams[tmp][0][key] / self.em.bigrams[tmp][1]
                    if tcur_p > self.thresh:
                        yield from self.recursive_word_fix(word, i + 1, tcur_p, fix + key)
        if i != len(word) - 1:
            tmp = word[i:i+2]
            if tmp in self.em.bigrams:
                for key in self.em.bigrams[tmp][0]:
                    tcur_p = cur_p * self.em.bigrams[tmp][0][key] / self.em.bigrams[tmp][1]
                    if tcur_p > self.thresh:
                        yield from self.recursive_word_fix(word, i + 1, tcur_p, fix + key)
        yield from self.recursive_word_fix(word, i + 1, cur_p, fix + word[i])
    
    def clean_duplicates(self, pcorr):
        clean_pcorr = []
        for i in range(len(pcorr)):
                if i == 0:
                    last_word = pcorr[i][0]
                    clean_pcorr.append(pcorr[i])
                else:
                    if pcorr[i][0] != last_word:
                        clean_pcorr.append(pcorr[i])
                        last_word = pcorr[i][0]    
        return clean_pcorr
    
    def get_possible_fixes(self, query):
        words = query.lower().strip().split()
        pcorrections = []
        for i, word in enumerate(words):
            pcorrections.append(list(self.recursive_word_fix(word, 0, 1, '')))
            for j in range(len(pcorrections[i])):
                pcorrections[i][j].append(self.lm.get_value(pcorrections[i][j][0], None, False))
                pcorrections[i][j].append(np.log2(pcorrections[i][j][1]) + np.log2(pcorrections[i][j][2]) * (1 + abs(len(pcorrections[i][j][0]) - len(word)) / len(word)) ** 2)
        for i in range(len(pcorrections)):
            pcorrections[i] = sorted(self.clean_duplicates(sorted(sorted(pcorrections[i], key= lambda x: x[-1], reverse=True), key= lambda x: x[0])), key= lambda x: x[-1], reverse=True)
        return pcorrections

with open('corr.bin', 'rb') as f:
    CorrModel = pickle.load(f)

class FixNonePredictor:
    
    def __init__(self, it=200):
        self.model = CatBoostClassifier(iterations=it)
        
    def get_features(self, orig, fix):
        return np.array([len(orig.split()), LangModel.predict(orig), LangModel.predict(fix), Err_model.predict(orig, fix)])
    
    def fit(self, wrong_queries, correct_queries):
        self.X_train = np.zeros((len(wrong_queries) * 2, 4))
        self.y_train = np.zeros((len(wrong_queries) * 2, ))
        i = 0
        for wquery, cquery in zip(tqdm(wrong_queries), correct_queries):
            self.X_train[2 * i] = self.get_features(wquery, cquery)
            self.X_train[2 * i + 1] = self.get_features(cquery, wquery)
            self.y_train[2 * i] = 1
            self.y_train[2 * i + 1] = 0
            i += 1
        self.model.fit(self.X_train, self.y_train)
    
    def predict(self, orig_queries):
        self.orig_queries = orig_queries
        self.fix_queries = []
        for query in orig_queries:
            self.fix_queries.append(get_best_fix(CorrModel.get_possible_fixes(query.strip())).strip())
        X_test = np.zeros((len(orig_queries), 4))
        i = 0
        for orig, fix in zip(orig_queries, self.fix_queries):
            X_test[i] = self.get_features(orig, fix)
            i += 1
        return self.model.predict(X_test)


with open('model.bin', 'rb') as f:
    fixclf = pickle.load(f)

for q in sys.stdin:
    q1 = q.strip().lower()
    if not fixclf.predict([q1])[0]:
        print(q.strip())
    else:
        print(fixclf.fix_queries[0].strip())
    