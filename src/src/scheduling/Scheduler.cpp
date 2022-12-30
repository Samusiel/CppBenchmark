#include <scheduling/Scheduler.hpp>

namespace Scheduling {

thread_local Scheduler::PerThreadData Scheduler::_current {
    .current = nullptr,
    .isAlive = false
};

Scheduler::Scheduler(bool start_, uint8_t threads_) {
    if (start_) {
        start(threads_);
    }
}

Scheduler::Scheduler(uint8_t threads_) {
    start(threads_);
}

Scheduler::~Scheduler() {
    // For all threads we send the message to shut down
    // Note, that according to our grabing mechanism, simultaneously only one thread can grab a task to perform
    // If a task was completed, we wait for another one, but in our case we won't grab anymore because of the end of the cycle.
    std::for_each(_workers.begin(), _workers.end(), [this](const auto& _) {
        schedule([] {
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

void Scheduler::start(uint8_t threads) {
    assert(_workers.size() == 0);
    for (uint8_t thread = 0; thread < threads; ++thread) {
        [[maybe_unused]]
        auto mainLoop = [this] {
            _current = PerThreadData {
                .current = this,
                .isAlive = true
            };
            
            // Start the inner loop 
            run();

            // Clear the data
            _current = PerThreadData{};
        };
        _workers.push_back(std::thread{mainLoop});
    }
}

void Scheduler::run() {
    constexpr std::chrono::microseconds period{64};
    auto grabTask = [this] {
        std::lock_guard lock(_queueAccess);
        auto task = std::move(_tasks.front());
        _tasks.pop();
        return task;
    };
    while (_current.isAlive) {
        // Wait until a new task has been submitted
        if (_pushedTasks.try_acquire_for(period)) {
            // Grab a task and immediate call
            grabTask()();
        }
    }
}

} // namespace Scheduling