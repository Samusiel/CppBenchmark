#pragma once

#include <application/ApplicationSettings.hpp>
#include <memory>
#include <scheduling/Scheduler.hpp>

namespace Profiling {
class Profiler;
} // namespace Profiling

namespace Scheduling {
class ScheduleManager;
} // namespace Scheduling

namespace Application {
/**
 * @brief
 *
 */
class Application {
public:
    static Application& instance();

    Application(const ApplicationSettings& settings);
    ~Application();

private:
    static Application* _instance;

    std::unique_ptr<Profiling::Profiler> _profiler;
    std::unique_ptr<Scheduling::ScheduleManager> _scheduleManager;
    Scheduling::Scheduler _appScheduler;
};

} // namespace Application