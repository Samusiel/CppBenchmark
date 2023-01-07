#include <scheduling/Scheduler.hpp>
#include <scheduling/WorkerBase.hpp>

namespace Scheduling {

thread_local WorkerBase::PerThreadData WorkerBase::_current = WorkerBase::PerThreadData{};

WorkerBase::~WorkerBase() {
    _stopSource.request_stop();

    std::for_each(_workers.begin(), _workers.end(), [](auto& worker) {
        if (worker.joinable()) {
            worker.join();
        }
    });
    _workers.clear();
}

auto WorkerBase::grabTask() -> std::optional<TaskFunction> {
    assert((_current.currentThreadId == std::this_thread::get_id())
        && "Grab should perform only from a thread, created as a worker.");
    return _scheduler->grabTask();
}

auto WorkerBase::grabTasks() -> TaskQueue {
    assert((_current.currentThreadId == std::this_thread::get_id())
        && "Grab should perform only from a thread, created as a worker.");
    return _scheduler->grabTasks();
}

} // namespace Scheduling