#include <scheduling/Worker.hpp>
#include <scheduling/Scheduler.hpp>

namespace Scheduling {

Worker::Worker(Scheduler* scheduler, uint8_t threads): 
    WorkerBase(scheduler, threads, [this] { run(); }) {}

void Worker::notify() {
    _pushedTasks.release();
}

void Worker::run() {
    constexpr std::chrono::microseconds period{64};
    while (_current.isAlive) {
        // Wait until a new task has been submitted
        if (_pushedTasks.try_acquire_for(period)) {
            if (auto task = grabTask()) {
                (*task)();
            }
        }
    }
}

} // namespace Scheduling