#include <sstream>
#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <ios>
#include <map>
#include <vector>
#include <set>
#include <chrono>
#include <ctime>

using namespace std;

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

void mytolower(std::string &s) {
    //Super mega костыльный tolower function to translate russian capital letters to non-capital
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == -48 && s[i] == -127) {
            if (i > 0)
                s[i - 1] = -47;
            s[i] = -79;
        }
        if (i > 0 && s[i - 1] == -48 && s[i] <= -81) {
            int temp = int(s[i] + 32);
            if (temp > -65) {
                if (i > 0)
                    s[i - 1] = -47;
                s[i] = -128 + 64 + temp;
            } else {
                s[i] += 32;
            }
        }
        if (s[i] > 32) {
            s[i] = std::tolower(s[i]);
        }
    }
}


char russian_letter_converter(char v1, char v2) {
    //Converts Pairs of chars of Russian letters in single char from -33 to -1 (so lower and upper case are equivalent)
    int x1 = (v1 + 48) * 64;
    int x2 = 128 + v2;
    if ((!x1 && x2 == 1) || (x1 && x2 == 17)) {
        return -1;
    }
    if (!x1 && x2 <= 47) {
        x2 += 32;
    }
    return x1 + x2 - 81;
}


std::set<int> skippable_chars{-33, -28, -1, -25, -24, -19, -14, -7, -6, -4, -3, -2};
bool russian_letter_skip(char v1, char v2) {
    //Auxiliary function to cut_word() to detect Russian vowels
    int x = russian_letter_converter(v1, v2);
    return skippable_chars.find(x) != skippable_chars.end();
}


int cut_word(std::string &word) {
    //Cut the end of word if it contains punctuation||Russian vowel||anything which is not either number or letter
    int cut_size = 0;
    int cut_sym = 0;
    int i = word.size() - 1;
    while (i >= 0) {
        if (word[i] >= 0 && (word[i] < '0' || word[i] > '9') && (word[i] < 'a' || word[i] > 'z') && (word[i] < 'A' || word[i] > 'Z')) {
            cut_size++;
            cut_sym++;
        } else if (word[i] < 0) {
            if (russian_letter_skip(word[i - 1], word[i])) {
                cut_size += 2;
                i--;
            }
            break;
        } else {
            break;
        }
        i--;
    }
    word.resize(word.size() - cut_size);
    return cut_sym;
}


bool check_link(const std::string &s) {
    //Auxiliary function to read_word(). Return true, if first four letters of read word are "http"
    return s.size() >= 4 && s[0] == 'h' && s[1] == 't' && s[2] == 't' && s[3] == 'p';
}

bool read_word(ifstream& rd, int& word_len, int& pos, const int& max_pos, std::string& word) {
    //Read word for parse_file(). Reads words
    char c;
    bool is_cyrillic = false;
    bool have_letter = false;
    const std::string http = "http";
    while (pos < max_pos) {

        rd.read((char *) &c, sizeof(c));
        pos++;

        if (abs(c) <= 32) {
            return check_link(word);
        }
        if (is_cyrillic) {
            is_cyrillic = false;
            word += c;
            ++word_len;
        } else if (c > 0) {
            if (have_letter) {
                word += c;
                ++word_len;
            } else {
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    have_letter = true;
                    word += c;
                    ++word_len;
                } else {
                    return check_link(word);
                }
            }
        } else if (c == -48 || c == -47) {
            is_cyrillic = true;
            word += c;
            have_letter = true;
        } else {
            word += c;
            ++word_len;
        }
    }
    return check_link(word);
}


void parse_file(ifstream& rd, std::vector<std::string>& links, std::map<std::string, VarByte>& word_links) {
    //Parse binary input file to links and words from these links
    rd.seekg(0, rd.end);
    int l = rd.tellg();
    rd.seekg(0, rd.beg);

    u_char c;
    std::string cur_link = "";
    std::string word = "";
    int i = 0, wl = 0;
    int cnt_links = 0;
    while (i < l) {
        word = "";
        wl = 0;
        bool is_link = read_word(rd, wl, i, l, word);
        if (is_link) {
            cur_link = word;
            cnt_links++;
            links.push_back(word);
        } else if (wl >= 3) {
            wl -= cut_word(word);
            if (wl > 0) {
                mytolower(word);
                auto it = word_links.find(word);
                if (it == word_links.end()) {
                    word_links[word] = VarByte(100, 100);
                }
                word_links[word].add_interval(links.size() - 1);
            }
        }
    }
}


void read_word_request(std::string::iterator &it, const std::string::iterator &it_end, int& word_len, std::string& word) {
    //Read next word in user request
    bool is_cyrillic = false;
    bool have_letter = false;

    while (it != it_end) {
        if (abs(*it) <= 32) {
            return;
        }
        if (is_cyrillic) {
            is_cyrillic = false;
            word += *it;
            ++word_len;
        } else if (*it > 0) {
            if (have_letter) {
                word += *it;
                ++word_len;
            } else {
                if ((*it >= '0' && *it <= '9') || (*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z')) {
                    have_letter = true;
                    word += *it;
                    ++word_len;
                } else {
                    return;
                }
            }
        } else if (*it == -48 || *it == -47) {
            is_cyrillic = true;
            word += *it;
            have_letter = true;
        } else {
            word += *it;
            ++word_len;
        }
        ++it;
    }
}


template <typename T>
std::set<int> intersect_sets(T p00, T p01, T p10, T p11) {
    //Intersection of pages appropriate for each single word of user search request
    std::set<int> a;
    while (p00 != p01 && p10 != p11) {
        if (*p00 < * p10) {
            ++p00;
        } else {
            if (!(*p10 < *p00)) {
                a.insert(*p00++);
            }
            ++p10;
        }
    }
    return a;
}


void merge_sets(std::set<int> &s1, const std::set<int> &s2) {
    //Merge of two sets
    auto it = s2.begin();
    while (it != s2.end()) {
        s1.insert(*it++);
    }
}

void print_set(std::set<int> &u) {
    auto it = u.begin();
    while (it != u.end()) {
        std::cout << *it++ << " ";
    }
    std::cout << std::endl;
}

std::set<int> get_urls(std::vector<std::string>& words, std::map<std::string, VarByte>& word_links) {
    //Get urls indices based on user requeststd::set<int>
    std::set<int> url_indices;
    for (int i = 0; i < words.size(); ++i) {
        auto p = word_links.find(words[i]);
        if (p != word_links.end()) {
            if (url_indices.size() == 0) {
                int id = 0;
                url_indices = p->second.get_urls();
                //print_set(url_indices);
            } else {
                //We try to get at least something for request and ignore rest words if they make our set empty. Better to have anything +-ok than nothing!
                std::set<int> url_indices2 = p->second.get_urls();
                url_indices2 = intersect_sets(url_indices.begin(), url_indices.end(), url_indices2.begin(), url_indices2.end());
                if (!url_indices2.empty()) {
                    url_indices = url_indices2;
                } else if (url_indices.size() < 100) {
                    //Merge only if we don't have too much pages already. So we get priority for the first words in request
                    int m;
                    //merge_sets(url_indices, p->second);
                }
            }
        }
    }
    return url_indices;
}


void read_request(std::vector<std::string>& links, std::map<std::string, VarByte>& word_links) {
    //Read search request and process it
    std::string s;
    while (std::getline(std::cin, s)) {

        std::system("clear");

        std::vector<std::string> words;
        std::vector<std::string> bad_words;
        auto it = s.begin();

        while (it < s.end()) {
            std::string word = "";
            int wl = 0;

            read_word_request(it, s.end(), wl, word);
            wl -= cut_word(word);
            it++;

            //We try to exclude <= 2letter words because they are almost always not informative at all
            if (wl >= 3) {
                mytolower(word);
                words.push_back(word);
            } else if (wl > 0) {
                mytolower(word);
                bad_words.push_back(word);
            }
        }
        
        auto urls = get_urls(words, word_links);
        //If we didn't find urls for good words, we search for bad ones
        if (urls.size() == 0) {
            urls = get_urls(bad_words, word_links);
        }
        auto p = urls.begin();

        std::cout << "Request: " << s << std::endl;
        std::cout << "Urls found for your request: " << urls.size() << std::endl;
        while (p != urls.end()) {
            std::cout << links[*p] << endl;
            p++;
        }
        std::cout << "===========End of current request===========" << std::endl;
    }
}


int main(void) {

    std::vector<std::string> links;
    links.push_back("empty");
    std::map<std::string, VarByte> word_links;
    std::vector<std::string> files = {"1", "2", "3", "4", "5", "6", "7", "8"};
    std::vector<std::string> dots = {".", "..", "..."};
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < files.size(); ++i) {
        std::system("clear");
        cout << "Reading #" << i << " file from #" << files.size() << dots[i % dots.size()] << std::endl;

        std::ifstream rd(files[i], ios::in | ios::binary);
        parse_file(rd, links, word_links);
    }
    auto end = std::chrono::system_clock::now();

    std::system("clear");
    std::cout << "===========Write your requests now!===========" << std::endl;

    read_request(links, word_links);
        std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}