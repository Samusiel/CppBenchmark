#pragma once

#include <concepts>
#include <thread>
#include <semaphore>
#include <vector>
#include <limits>
#include <cstdint>
#include <scheduling/Definitionts.hpp>
#include <optional>
#include <scheduling/ThreadCommunication.hpp>

namespace Scheduling {

class Scheduler;

template <typename T>
concept LoopObject = requires (T loopObject) {
    { loopObject(std::stop_token{}) } -> std::convertible_to<void>;
};

class WorkerBase {
protected:
    friend class Scheduler;

private:
    struct PerThreadData {
        Scheduler* current = nullptr;
    };

public:
    virtual ~WorkerBase();

protected:
    template <LoopObject Loop>
    WorkerBase(Scheduler* scheduler, uint8_t threads, Loop&& loop): _scheduler(scheduler) {
        for (uint8_t thread = 0; thread < threads; ++thread) {
            auto mainLoop = [this, loop = std::move(loop), stopToken = _stopSource.get_token()] {
                _current = PerThreadData {
                    .current = _scheduler
                };

                loop(stopToken);

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
    std::stop_source _stopSource;
    static thread_local PerThreadData _current;
};

} // namespace Scheduling