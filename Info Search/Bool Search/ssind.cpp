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


class BigInt {
    uint32_t mlim = 4e9;
    std::vector<uint32_t> num;
    public:
    
    BigInt() {}

    BigInt(int x) {
        num.push_back(x);
    }

    BigInt get_next() const {
        BigInt m;
        int overflow = 1;
        for (int i = 0; i < size(); ++i) {
            m.num.push_back(num[i] + overflow);
            if (m.num[i] == mlim) {
                m.num[i] = 0;
                overflow = 1;
            } else {
                overflow = 0;
            }
        }
        if (overflow) {
            m.num.push_back(1);
        }
        return m;
    }

    int size() const {
        return num.size();
    }

    bool operator==(const BigInt &c) const {
        for (int i = size() - 1; i >= 0; --i)
            std::cout << num[i];
        std::cout << std::endl;
        for (int i = c.size() - 1; i >= 0; --i)
            std::cout << c.num[i];
        std::cout << std::endl;
        int i = 0;
        while (i < c.size() && i < num.size()) {
            if (num[i] != c.num[i]) {
                return false;
            }   
            i++;
        }
        return c.size() == num.size();
    }

    bool operator<(const BigInt &c) const {
        if (c.size() < size()) {
            return false;
        } else if (c.size() > size()) {
            return true;
        }
        for (int i = size(); i >= 0; --i) {
            if (c.num[i] > num[i]) {
                return true;
            } else if (c.num[i] < num[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator>(const BigInt &c) const {
        if (c.size() > size()) {
            return false;
        } else if (c.size() < size()) {
            return true;
        }
        for (int i = size(); i >= 0; --i) {
            if (c.num[i] < num[i]) {
                return true;
            } else if (c.num[i] > num[i]) {
                return false;
            }
        }
        return false;
    }

};
BigInt INDEX(0);

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


void parse_file(ifstream& rd, std::map<BigInt, std::string>& links, std::map<std::string, std::set<BigInt>>& word_links) {
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
            INDEX = INDEX.get_next();
            links[INDEX] = word;
        } else if (wl >= 3) {
            wl -= cut_word(word);
            if (wl > 0) {
                mytolower(word);
                auto it = word_links.find(word);
                if (it == word_links.end()) {
                    word_links[word] = std::set<BigInt>();
                }
                word_links[word].insert(INDEX);
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
std::set<BigInt> intersect_sets(T p00, T p01, T p10, T p11) {
    //Intersection of pages appropriate for each single word of user search request
    std::set<BigInt> a;
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


void merge_sets(std::set<BigInt> &s1, const std::set<BigInt> &s2) {
    //Merge of two sets
    auto it = s2.begin();
    while (it != s2.end()) {
        s1.insert(*it++);
    }
}


std::set<BigInt> get_urls(std::vector<std::string>& words, std::map<std::string, std::set<BigInt>>& word_links) {
    //Get urls indices based on user request
    std::set<BigInt> url_indices;
    for (int i = 0; i < words.size(); ++i) {
        auto p = word_links.find(words[i]);
        if (p != word_links.end()) {
            if (url_indices.size() == 0) {
                auto set_it = p->second.begin();
                auto it_end = p->second.end();
                while (set_it != it_end) {
                    url_indices.insert(*set_it);
                    set_it++;
                }
            } else {
                //We try to get at least something for request and ignore rest words if they make our set empty. Better to have anything +-ok than nothing!
                auto url_indices2 = intersect_sets(url_indices.begin(), url_indices.end(), p->second.begin(), p->second.end());
                if (!url_indices2.empty()) {
                    url_indices = url_indices2;
                } else if (url_indices.size() < 100) {
                    //Merge only if we don't have too much pages already. So we get priority for the first words in request
                    merge_sets(url_indices, p->second);
                }
            }
        }
    }
    return url_indices;
}


void read_request(std::map<BigInt, std::string>& links, std::map<std::string, std::set<BigInt>>& word_links) {
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

    std::map<BigInt, std::string> links;
    std::map<std::string, std::set<BigInt>> word_links;
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