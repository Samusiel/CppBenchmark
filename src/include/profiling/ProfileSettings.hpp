#pragma once

#include <profiling/StreamOutput.hpp>
#include <vector>

namespace Profiling {

template <typename... Outputs>
using BaseOutputSettings = std::variant<typename Outputs::Settings...>;

struct ProfileSettings {
    using OutputSettings = BaseOutputSettings<StreamOutput>;

    std::vector<OutputSettings> outputs;
};

} // namespace Profiling