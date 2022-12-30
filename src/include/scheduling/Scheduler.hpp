#pragma once

#include <concepts>
#include <atomic>
#include <functional>
#include <semaphore>
#include <queue>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

namespace Scheduling {

using TaskFunction = std::function<void()>;

template <typename Task>
concept SchedulerTask = std::convertible_to<Task, TaskFunction>;

class Scheduler {
private:
    struct PerThreadData {
        Scheduler* current = nullptr;
        bool isAlive = false;
    };

public:
    static Scheduler& current() {
        assert((_current.current != nullptr) && "Calling the method from a thread, that is not a Scheduler.");
        return *_current.current;
    }

    Scheduler(uint8_t threads = 1);
    Scheduler(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    ~Scheduler();

    template <SchedulerTask Task>
    void schedule(Task&& task) {
        // Submit the task
        {
            std::lock_guard lock(_queueAccess);
            // Observation: according to the benchmark, std::function creation is very expensive procedure: under the hood
            // we create a new object on the heap
            _tasks.emplace(std::move(task));
        }
        _pushedTasks.release();
    }

protected:
    Scheduler(bool start, uint8_t threads = 1);
    void start(uint8_t threads = 1);
    void run();

private:
    std::vector<std::thread> _workers;
    // ToDo: use production ready lock free queue
    std::mutex _queueAccess;
    std::queue<TaskFunction> _tasks;
    std::counting_semaphore<std::numeric_limits<uint32_t>::max()> _pushedTasks{ 0 };
    static thread_local PerThreadData _current;
};

} // Scheduling