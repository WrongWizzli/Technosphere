#ifndef PARSE_READ_HPP
#define PARSE_READ_HPP

#include "searchtools.hpp"
#include <string>
#include <map>
#include <vector>
#include <ios>
#include <iostream>

namespace My_Space {
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
    return s.size() >= 10 && s[0] == 'h' && s[1] == 't' && s[2] == 't' && s[3] == 'p';
}

bool read_word(std::ifstream& rd, int& word_len, int& pos, const int& max_pos, std::string& word) {
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


void parse_file(std::ifstream& rd, std::vector<std::string>& links, std::map<std::string, VarByte>& word_links) {
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
                    word += *it;
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


std::set<int> get_urls(Operation_Tree &tree, std::vector<VarByte>& v) {
    //Get urls indices based on user requeststd::set<int>
    if (v.size()) {
        tree.build_order_tree();
        return tree.get_result(v);
    }
    return std::set<int>();
}


//true false true true false
void read_request(std::vector<std::string>& links, std::map<std::string, VarByte>& word_links) {
    //Read search request and process it
    std::string s;
    std::cout << "Write request:" << std::endl;
    while (std::getline(std::cin, s)) {

        std::system("clear");
        std::vector<VarByte> v;
        auto it = s.begin();
        Operation_Tree tree;

        while (it < s.end()) {
            std::string word = "";
            int wl = 0;

            read_word_request(it, s.end(), wl, word);
            if (wl) {
                wl -= cut_word(word);
            }
            it++;
            //We try to exclude <= 2letter words because they are almost always not informative at all
            if (wl > 0) {
                mytolower(word);
                auto ite = word_links.find(word);
                if (ite != word_links.end()) {
                    v.push_back(ite->second);
                    tree.existing_word();
                } else {
                    tree.vanish_word();
                }  
            } else if (word.size() == 1) {
                tree.add_sign(word[0]);
            }
        }
        std::set<int> urls = get_urls(tree, v);
        auto p = urls.begin();

        std::cout << "Request: " << s << std::endl;
        std::cout << "Urls found for your request: " << urls.size() << std::endl;
        while (p != urls.end()) {
            std::cout << links[*p] << std::endl;
            p++;
        }
        std::cout << "===========End of current request===========" << std::endl;
        std::cout << "Write next request:" << std::endl;
    }
}
}

#endif