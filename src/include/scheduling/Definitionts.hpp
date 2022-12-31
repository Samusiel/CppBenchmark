#pragma once

#include <concepts>
#include <queue>
#include <functional>

namespace Scheduling {

using TaskFunction = std::function<void()>;

template <typename Task>
concept SchedulerTask = std::convertible_to<Task, TaskFunction>;

using TaskQueue = std::queue<TaskFunction>;

} // namespace Scheduling