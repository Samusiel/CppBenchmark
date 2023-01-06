#pragma once

#include <scheduling/WorkerBase.hpp>

namespace Scheduling {

class Worker : public WorkerBase {
public:
    Worker(Scheduler* scheduler, uint8_t threads);
    Worker(const Worker&) = delete;
    Worker(Worker&&) = delete;
    ~Worker() = default;

    void notify();

private:
    void run(std::stop_token&& stopToken);

private:
    std::counting_semaphore<std::numeric_limits<uint16_t>::max()> _pushedTasks{0};
};

} // namespace Scheduling