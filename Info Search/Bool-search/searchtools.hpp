#ifndef SEARCHTOOLS_HPP
#define SEARCHTOOLS_HPP

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

namespace My_Space {
class VarByte {
    private:
    u_char *s;
    int pos_add = 0;
    int pos_read = 0;
    int maxsize;
    int expand_c;
    int current_value = 0;

    public:
    VarByte(int size=100, int e_size=100): maxsize(size), expand_c(e_size) {
        s = new u_char[size];
    }

    VarByte(VarByte &&c) {
        s = c.s;
        pos_add = c.pos_add;
        pos_read = c.pos_read;
        maxsize = c.maxsize;
        expand_c = c.expand_c;
        current_value = c.current_value;
        c.s = nullptr;
    }

    VarByte(const VarByte &c) {
        s = new u_char[c.maxsize];
        for (int i = 0; i < c.maxsize; ++i) {
            s[i] = c.s[i];
        }
        pos_add = c.pos_add;
        pos_read = c.pos_read;
        maxsize = c.maxsize;
        expand_c = c.expand_c;
        current_value = c.current_value;
    }

    VarByte& operator=(const VarByte &c) {
        if (this == &c) {
            return *this;
        }
        s = new u_char[c.maxsize];
        for (int i = 0; i < c.maxsize; ++i) {
            s[i] = c.s[i];
        }
        pos_add = c.pos_add;
        pos_read = c.pos_read;
        maxsize = c.maxsize;
        expand_c = c.expand_c;
        current_value = c.current_value;
        return *this;
    }

    VarByte& operator=(VarByte &&c) {
        s = c.s;
        pos_add = c.pos_add;
        pos_read = c.pos_read;
        maxsize = c.maxsize;
        expand_c = c.expand_c;
        current_value = c.current_value;
        c.s = nullptr;
        return *this;
    }

    void expand_size() {
        u_char *tmp = new u_char[maxsize + expand_c];
        for (int i = 0; i < pos_add; ++i) {
            tmp[i] = s[i];
        }
        maxsize += expand_c;
        delete[] s;
        s = tmp;
    }

    int get_len128(int x) {
        int len = 0;
        do {
            x >>= 7;
            ++len;
        } while(x);
        return len;
    }

    void add_interval(uint32_t x) {
        int interval = x - current_value;
        if (!interval) {
            return;
        }
        current_value = x;
        int l = get_len128(interval);
        while (pos_add + l >= maxsize) {
            expand_size();
        }
        for (int i = pos_add + l - 1; i >= pos_add; --i) {
            s[i] = (interval & 0x7F);
            if (i == pos_add + l - 1)
                s[i] += 0x80;
            interval >>= 7;
        }
        pos_add += l;
    }

    uint32_t read_next() {
        int tmp = 0;
        while (pos_read < pos_add) {
            if (s[pos_read] & u_char(0x80)) {
                tmp = (tmp << 7) + s[pos_read++] - u_char(0x80);
                break;
            } else {
                tmp = (tmp << 7) + s[pos_read++];
            }
        }
        return tmp;
    }

    void zero_pos() {pos_read = 0;}

    std::set<int> get_urls() {
        std::set<int> urls;
        int id = 0, tmp;
        while (tmp = read_next()) {
            id += tmp;
            urls.insert(id);
        }
        zero_pos();
        return urls;
    }

    ~VarByte() {
        if (s != nullptr)
            delete[] s; 
    }
};


class Operation_Tree {
    private:
    int ppriority = 0;
    std::vector<std::pair<int, int>> p;
    std::vector<bool> empty_op;

    public:

    Operation_Tree() {}

    void add_sign(u_char c) {
        if (c == '(') {
            ppriority += 8;
        } else if (c == ')') {
            ppriority -= 8;
        } else if (c == '&') {
            p.push_back(std::pair<int, int>(ppriority + 4, p.size()));
        } else if (c == '|') {
            p.push_back(std::pair<int, int>(ppriority + 2, p.size()));
        }
    }

    void vanish_word() {
        empty_op.push_back(true);
    }

    void existing_word() {
        empty_op.push_back(false);
    }

    void build_order_tree() {
        std::vector<std::pair<int, int>> tmp;
        std::vector<int> banned(p.size(), false);
        bool pop_last = false;
        for (int i = 0; i < empty_op.size(); ++i) {
            if (empty_op[i]) {
                if (i > 0 && i < p.size()) {
                    if (p[i - 1].first > p[i].first && !banned[i - 1]) {
                        banned[i - 1] = true;
                    } else {
                        banned[i] = true;
                    }
                } else if (!i) {
                    banned[i] = true;
                } else if (!banned[i - 1]) {
                    banned[i - 1] = true;
                } else {
                    pop_last = true;
                }
                if (!banned[i - 1]) {
                    tmp.push_back(p[i - 1]);
                }
            } else if (i > 0) {
                if (!banned[i - 1]) {
                    tmp.push_back(p[i - 1]);
                }
            }
        }
        if (pop_last) {
            tmp.pop_back();
        }
        for (int i = 0; i < tmp.size(); ++i) {
            tmp[i].second = i;
        }
        p = tmp;
        std::sort(p.begin(), p.end(), 
        [](auto &left, auto &right) {return left.first >= right.first;});
    }

    bool process_tree(std::vector<std::pair<bool, bool>> &d) {
        for (int i = 0; i < p.size(); ++i) {
            int ind = p[i].second;
            if (!(p[i].first % 4)) { 
                d[ind].first = (d[ind].first & d[ind + 1].first);
            } else {
                d[ind].first = (d[ind].first | d[ind + 1].first);
            }
            d[ind + 1].first = d[ind].first;
            d[ind].second = d[ind + 1].second = true;
            int ind0 = ind - 1;
            while (ind0 >= 0) {
                if (d[ind0].second) {
                    d[ind0].first = d[ind].first;
                    ind0--;
                } else {
                    break;
                }
            }
            ind0 = ind + 2;
            while (ind0 < d.size()) {
                if (d[ind0].second) {
                    d[ind0].first = d[ind].first;
                    ind0++;
                } else {
                    break;
                }
            }
        }
        return d[0].first;
    }
    
    std::set<int> get_result(std::vector<VarByte> &v) {
        int ind_min = 0;
        bool not_finished = true;
        std::vector<std::pair<bool, bool>> does_exist(v.size(), std::pair<bool, bool>(0, 0));
        std::vector<bool> is_active(v.size(), true);
        std::vector<int> values(v.size(), -1);
        for (int i = 0; i < v.size(); ++i) {
            values[i] = v[i].read_next();
            if (values[ind_min] > values[i]) {
                ind_min = i;
            }
        }
        int cnt = 0;
        std::set<int> urls;
        while (not_finished) {
            int next_min_ind = -1, min_val = values[ind_min];
            not_finished = false;
            for (int i = 0; i < v.size(); ++i) {
                if (is_active[i] && values[i] == min_val) {
                    int tmp = v[i].read_next();
                    if (tmp) {
                        not_finished = true;
                        values[i] += tmp;
                    } else {
                        is_active[i] = false;
                    }
                    does_exist[i].first = true;
                } else if (is_active[i]) {
                    not_finished = true;
                    does_exist[i].first = false;
                } else {
                    does_exist[i].first = false;
                }
                if (is_active[i] && (next_min_ind == -1 || values[i] < values[next_min_ind])) {
                    next_min_ind = i;
                }
                does_exist[i].second = false;
            }
            if (process_tree(does_exist)) {
                urls.insert(min_val);
            }
            ind_min = next_min_ind;
        }
        return urls;
    }
};
}

#endif