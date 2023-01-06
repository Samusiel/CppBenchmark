#include <scheduling/Scheduler.hpp>
#include <scheduling/Worker.hpp>

namespace Scheduling {

Worker::Worker(Scheduler* scheduler, uint8_t threads)
    : WorkerBase(scheduler, threads, [this](std::stop_token stopToken) {
        run(std::move(stopToken));
    }) { }

void Worker::notify() {
    _pushedTasks.release();
}

void Worker::run(std::stop_token&& stopToken) {
    constexpr std::chrono::microseconds period{32};
    while (!stopToken.stop_requested()) {
        // Wait until a new task has been submitted
        // https://stackoverflow.com/questions/71893279/why-does-stdcounting-semaphoreacquire-suffer-deadlock-in-this-case
        if (_pushedTasks.try_acquire_for(period)) {
            if (auto task = grabTask()) {
                (*task)();
            }
        }
    }
}

} // namespace Scheduling