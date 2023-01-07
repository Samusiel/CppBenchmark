#pragma once

#include <concepts>
#include <cstdint>
#include <limits>
#include <optional>
#include <scheduling/Definitionts.hpp>
#include <scheduling/ThreadCommunication.hpp>
#include <semaphore>
#include <thread>
#include <vector>

namespace Scheduling {

class Scheduler;

template <typename T>
concept LoopObject = requires(T loopObject) {
                         { loopObject(std::stop_token{}) } -> std::convertible_to<void>;
                     };

class WorkerBase {
protected:
    friend class Scheduler;

private:
    struct PerThreadData {
        Scheduler* scheduler = nullptr;
        std::thread::id currentThreadId;
    };

public:
    virtual ~WorkerBase();

protected:
    template <LoopObject Loop>
    WorkerBase(Scheduler* scheduler, uint8_t threads, Loop&& loop)
        : _scheduler(scheduler) {
        for (uint8_t thread = 0; thread < threads; ++thread) {
            auto mainLoop = [this, loop = std::move(loop), stopToken = _stopSource.get_token()] {
                _current = PerThreadData{.scheduler = _scheduler, .currentThreadId = std::this_thread::get_id()};

                loop(stopToken);

                _current = PerThreadData{};
            };
            _workers.push_back(std::thread{mainLoop});
        }
    }

    virtual void notify() = 0;

protected:
    [[nodiscard]] auto grabTask() -> std::optional<TaskFunction>;
    [[nodiscard]] auto grabTasks() -> TaskQueue;

protected:
    Scheduler* _scheduler = nullptr;
    std::vector<std::thread> _workers;
    std::stop_source _stopSource;
    static thread_local PerThreadData _current;
};

} // namespace Scheduling