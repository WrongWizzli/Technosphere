#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <future>
#include <mutex>
#include <atomic>
#include <functional>
#include <string>

namespace My_Tools {

    class EndOfThreadSession_Error: std::runtime_error {

        std::string message;

        public:
        EndOfThreadSession_Error(const std::string &s): std::runtime_error("EndOfThreadSession_Error"), message(s) {}
        const char* what() const noexcept override {
            return message.c_str();
        }
        virtual ~EndOfThreadSession_Error() noexcept {};
    };

    class ThreadPool {

        private:
        size_t poolSize;
        std::vector<std::thread> threads;
        std::mutex wait_mutex;
        std::condition_variable cond;
        std::deque <std::function<void()>> tasks;
        bool end = false;

        void wait_for_task() {
            while (true) {
                std::function<void()> current_task;
                {
                    std::unique_lock<std::mutex> lock(wait_mutex);
                    while (!end && tasks.empty()) {
                        cond.wait(lock);
                    }
                    if (end && tasks.empty()) {
                        return;
                    }
                    current_task = tasks.front();
                    tasks.pop_front();
                }
                current_task();
            }
        }


        public:
        explicit ThreadPool(size_t poolSize = std::thread::hardware_concurrency()): poolSize(poolSize) {
            for (int i = 0; i < poolSize; ++i) {
                threads.push_back(std::thread([this]{this->wait_for_task();}));
            }
        }


        template<class Func, class... Args>
        auto exec(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
            if (!end) {
                using return_type = typename std::result_of<Func(Args...)>::type;
                auto task = std::make_shared<std::packaged_task<return_type()>>
                        (std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
                std::future<return_type> res = task->get_future();
                {
                    std::unique_lock<std::mutex> lock(wait_mutex);
                    tasks.push_back([task](){ (*task)(); });
                }
                cond.notify_one();
                return res;
            }
            throw EndOfThreadSession_Error("You can't ask for more execs of threadpool. End status is active!");
        }

        void close_pool() noexcept {
            {
                std::lock_guard<std::mutex> lock(wait_mutex);
                end = true;
            }
        }


        ~ThreadPool() {
            {
                std::lock_guard<std::mutex> lock(wait_mutex);
                end = true;
            }
            cond.notify_all();
            for (int i = 0; i < poolSize; ++i) {
                threads[i].join();
            }
        }
    };
}