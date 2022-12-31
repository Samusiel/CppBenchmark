#include <scheduling/WorkerBase.hpp>
#include <scheduling/Scheduler.hpp>

namespace Scheduling {

thread_local WorkerBase::PerThreadData WorkerBase::_current;

WorkerBase::~WorkerBase() {
    // For all threads we send the message to shut down
    // Note, that according to our grabing mechanism, simultaneously only one thread can grab a task to perform
    // If a task was completed, we wait for another one, but in our case we won't grab anymore because of the end of the cycle.
    std::for_each(_workers.begin(), _workers.end(), [this](const auto& _) {
        _scheduler->schedule([] {
            _current.isAlive = false;
        });
    });
    
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