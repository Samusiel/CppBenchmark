#include "ApplicationWorker.hpp"

namespace Application {

ApplicationWorker::ApplicationWorker(Scheduling::Scheduler* scheduler, uint8_t threads)
    : WorkerBase(scheduler, threads, [this](std::stop_token stopToken) {
        run(std::move(stopToken));
    }) { }

ApplicationWorker::~ApplicationWorker() { }

void ApplicationWorker::run(std::stop_token stopToken) {
    while (!stopToken.stop_requested()) { }
}

} // namespace Application