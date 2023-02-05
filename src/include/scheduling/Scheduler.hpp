#pragma once

#include <cassert>
#include <concepts>
#include <mutex>
#include <optional>
#include <scheduling/Definitionts.hpp>
#include <scheduling/WorkerBase.hpp>

namespace Scheduling {

template <typename WorkerType>
concept WorkerConcept = std::derived_from<WorkerType, WorkerBase>;

class Scheduler {
private:
    friend class WorkerBase;

public:
    template <typename Factory>
    Scheduler(Factory&& factory, uint8_t threads = 1)
        : _worker{factory(this, threads)} { }
    Scheduler(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    ~Scheduler() = default;

    template <SchedulerTask Task>
    void schedule(Task&& task) {
        // Submit the task
        {
            std::lock_guard lock(_queueAccess);
            // Observation: according to the benchmark, std::function creation is very expensive procedure: under the
            // hood we create a new object on the heap
            _tasks.emplace(std::move(task));
        }
        _worker->notify();
    }

    static Scheduler& current() noexcept {
        assert((WorkerBase::_current.current != nullptr)
            && "This method was called not from a thread, created by a scheduler");
        return *WorkerBase::_current.current;
    }

private:
    [[nodiscard]] auto grabTask() {
        std::lock_guard lock(_queueAccess);
        if (_tasks.size() > 0) {
            auto task = _tasks.front();
            _tasks.pop();
            return std::optional<TaskFunction>{std::move(task)};
        }
        return std::optional<TaskFunction>{};
    }

    [[nodiscard]] auto grabTasks() {
        TaskQueue tasks;
        std::lock_guard lock(_queueAccess);
        tasks.swap(_tasks);
        return tasks;
    }

private:
    // ToDo: use a production ready lock free queue
    std::mutex _queueAccess;
    TaskQueue _tasks;
    std::unique_ptr<WorkerBase> _worker;
};

} // namespace Scheduling