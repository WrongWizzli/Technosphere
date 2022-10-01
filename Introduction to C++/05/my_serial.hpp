#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <string>


namespace My_Tools {

    class Wrong_Deserializer_Type : virtual public std::exception {
        std::string message;
        public:
        Wrong_Deserializer_Type() = delete;
        Wrong_Deserializer_Type(const std::string &message) throw(): std::exception(), message(message) {}
        const char *what() const noexcept override {
            return message.c_str();
        }
    };


    struct Type {
        uint64_t num;
        bool is_bool;
    };


    struct Data {

        std::vector<Type> data{};

        private:
        Type get_element(bool val) {
            return Type{val, true};
        }

        template <typename T>
        Type get_element(T val) {
            return Type{uint64_t(val), false};
        }


        public:
        template<typename T>
        void read_data(T&& val) {
            data.push_back(get_element(val));
        }

        template<typename T, typename... Args>
        void read_data(T&& val, Args&&... args) {
            data.push_back(get_element(val));
            read_data(std::forward<Args>(args)...);
        }

        template<typename... Args>
        Data(Args&&... args) {
            read_data(std::forward<Args>(args)...);
        }

        Data(){}
        
        Type operator[](const int32_t i) {
            return data[i];
        }

        size_t size() const {
            return data.size();
        }
    };


    template<class T>
    class Serializer {
        private:
        T* stream;
        struct Data data;

        public:
        Serializer(T &c) {
            stream = &c;
        }

        void save_data() {
            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i].is_bool) {
                    if (data[i].num) {
                        *stream << "true ";
                    } else {
                        *stream << "false ";
                    }
                } else {
                    *stream << data[i].num << " ";
                }
            }
            stream->flush();
        }

        template <typename... Args>
        void save(Args&&... args) {
            data = Data(args...);
            save_data();
        }
    };

    template <class T>
    class Deserializer {
        private:
        T *stream;

        public:
        Deserializer(T &c) {
            stream = &c;
        }

        template <typename... Args>
        void read_next(bool &x, Args&... args) {
            std::string reader;
            if (*stream >> reader) {
                if (!reader.empty()) {
                    if (reader != "true" && reader != "false") {
                        throw Wrong_Deserializer_Type("Error occured during deserealizing bool variable");
                    }
                    x = reader == "true";
                }
                read_next(args...);
            }
        }

        template <typename... Args>
        void read_next(uint64_t &x, Args&... args) {
            std::string reader;
            if (*stream >> reader) {
                if (!reader.empty()) {
                    try {
                        uint64_t num = std::stoull(reader);
                        x = num;
                    } catch(std::invalid_argument &a) {
                        throw Wrong_Deserializer_Type("Error occured during deserealizng uint64_t variable");
                    }
                }
                read_next(args...);
            }
        }

        void read_next(bool &x) {
            std::string reader;
            if (*stream >> reader) {
                if (!reader.empty()) {
                    if (reader != "true" && reader != "false") {
                        throw Wrong_Deserializer_Type("Error occured during deserealizing bool variable");
                    }
                    x = reader == "true";
                }
            }
        }

        void read_next(uint64_t &x) {
            std::string reader;
            if (*stream >> reader) {
                if (!reader.empty()) {
                    try {
                        uint64_t num = std::stoull(reader);
                        x = num;
                    } catch(std::invalid_argument &a) {
                        throw Wrong_Deserializer_Type("Error occured during deserealizng uint64_t variable");
                    }
                }
            }
        }

        template<typename... Args>
        void load(Args&... args) {
            read_next(args...);
        }
    };

}