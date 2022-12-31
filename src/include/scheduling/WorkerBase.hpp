#pragma once

#include <concepts>
#include <thread>
#include <semaphore>
#include <vector>
#include <limits>
#include <cstdint>
#include <scheduling/Definitionts.hpp>
#include <optional>

namespace Scheduling {

class Scheduler;

class WorkerBase {
protected:
    friend class Scheduler;

private:
    struct PerThreadData {
        Scheduler* current = nullptr;
        bool isAlive = false;
    };

public:
    virtual ~WorkerBase();

protected:
    template <typename Loop>
    WorkerBase(Scheduler* scheduler, uint8_t threads, Loop&& loop): _scheduler(scheduler) {
        for (uint8_t thread = 0; thread < threads; ++thread) {
            auto mainLoop = [this, loop = std::move(loop)] {
                _current = PerThreadData {
                    .current = _scheduler,
                    .isAlive = true
                };

                loop();

                _current = PerThreadData {};
            };
            _workers.push_back(std::thread{mainLoop});
        }
    }

    virtual void notify() = 0;
    
protected:
    auto grabTask() -> std::optional<TaskFunction>;
    auto grabTasks() -> TaskQueue;

protected:
    Scheduler* _scheduler = nullptr;
    std::vector<std::thread> _workers;
    static thread_local PerThreadData _current;
};

} // namespace Scheduling