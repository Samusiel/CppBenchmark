#pragma once

#include <scheduling/Scheduler.hpp>
#include <scheduling/WorkerBase.hpp>

namespace Application {

class ApplicationWorker : public Scheduling::WorkerBase {
public:
    ApplicationWorker(Scheduling::Scheduler* scheduler, uint8_t threads);
    ~ApplicationWorker();

    void notify() override { }

private:
    void run(std::stop_token stopToken);
};

} // namespace Application