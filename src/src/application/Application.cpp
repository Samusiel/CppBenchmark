#include "ApplicationWorker.hpp"
#include <application/Application.hpp>
#include <profiling/Profiler.hpp>
#include <scheduling/ScheduleManager.hpp>

namespace Application {

Application* Application::_instance = nullptr;

Application& Application::instance() {
    assert(_instance != nullptr && "The variable should be initilized once in main.");
    return *_instance;
}

// ToDo: currently we specify those settings by hand, but at this point we could use a generalised way of loading them.
Application::Application(const ApplicationSettings& settings)
    : _profiler(std::make_unique<Profiling::Profiler>(Profiling::ProfileSettings{}))
    , _scheduleManager(std::make_unique<Scheduling::ScheduleManager>(Scheduling::ScheduleSettings{4}))
    , _appScheduler(_scheduleManager->create<ApplicationWorker>()) {
    _instance = this;
}

Application::~Application() {
    _instance = nullptr;
}

} // namespace Application