#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(size_t maxThreads) {
  std::cout << "ThreadPool: Starting " << maxThreads << " threads.\n";
  // create all worker jthreads
  for (size_t i = 0; i < maxThreads; ++i) {
    workers.emplace_back([this](std::stop_token stoken) { worker(stoken); });
  }
}

ThreadPool::~ThreadPool() {
  for (auto &w : workers) {
    w.request_stop();
  }
  condition.notify_all();

  for (auto &w : workers) {
    if (w.joinable()) {
      w.join();
    }
  }
}
void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard lock(queueMutex);
    tasks.push(std::move(task));
    ++tasksRemaining;
  } // lock is automatically released (RAII)
  condition.notify_one();
}

bool ThreadPool::waitForCompletion(std::chrono::seconds timeout) {
  std::unique_lock lock(queueMutex);
  return condition.wait_for(
      lock, timeout, [this] { return tasksRemaining == 0 && tasks.empty(); });
}

void ThreadPool::worker(std::stop_token stoken) {
  while (!stoken.stop_requested()) {
    std::function<void()> task;
    {
      std::unique_lock lock(queueMutex);
      condition.wait(lock, stoken, [this] { return !tasks.empty(); });
      if (tasks.empty()) {
        if (stoken.stop_requested()) {
          return;
        }
        continue;
      }
      task = std::move(tasks.front());
      tasks.pop();
    }
    task(); // run task
    {
      std::lock_guard lock(queueMutex);
      --tasksRemaining;
    }
    condition.notify_all();
  }
}