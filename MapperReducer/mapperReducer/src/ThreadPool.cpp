
#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(size_t maxThreads) {
  std::cout << "ThreadPool: Starting " << maxThreads << " threads.\n";
  // create all worker jthreads
  for (size_t i = 0; i < maxThreads; ++i) {
    workers.emplace_back([this](std::stop_token stoken) { worker(stoken); });
  }
}

ThreadPool::~ThreadPool() {
  stop = true;
  stopSource.request_stop(); // notify jthreads
  condition.notify_all();    // wake up threads

  for (auto &worker : workers) {
    if (worker.joinable())
      worker.join();
  }
}

void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard lock(queueMutex);
    tasks.push(std::move(task));
    ++tasksRemaining;
  }
  condition.notify_one();
}

void ThreadPool::waitForCompletion() {
  std::unique_lock lock(queueMutex);
  condition.wait(lock, [this] { return tasksRemaining == 0 && tasks.empty(); });
}

void ThreadPool::worker(std::stop_token stoken) {
  while (!stoken.stop_requested()) {
    std::function<void()> task;
    {
      std::unique_lock lock(queueMutex);

      condition.wait(lock, stoken,
                     [this] { return !tasks.empty() || stop.load(); });

      if (stop && tasks.empty())
        return;

      task = std::move(tasks.front());
      tasks.pop();
    }

    task(); // run task
    --tasksRemaining;

    condition.notify_all();
  }
}
