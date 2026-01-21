#pragma once

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

class ThreadPool {
public:
  ThreadPool(size_t maxThreads); // automatic number of threads
  ~ThreadPool();                 // stop via stop_source

  void enqueue(std::function<void()> task);
  bool waitForCompletion(std::chrono::seconds timeout);

private:
  void worker(std::stop_token stoken);

  std::vector<std::jthread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex queueMutex;
  std::condition_variable_any condition; // avoid busy waiting

  // atomic boolena: so it is not affected by other threads
  std::atomic<bool> stop{false};

  // atomic counter of remaining tasks: so it gives the exact number
  std::atomic<size_t> tasksRemaining{0};

  std::stop_source stopSource;
};
