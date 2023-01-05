#include <scheduling/WorkerBase.hpp>
#include <scheduling/Scheduler.hpp>

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
    return _scheduler->grabTask();
}

auto WorkerBase::grabTasks() -> TaskQueue {
    return _scheduler->grabTasks();
}

} // namespace Scheduling