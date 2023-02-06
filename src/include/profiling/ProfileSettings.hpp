#pragma once

#include <profiling/StreamOutput.hpp>
#include <vector>

namespace Profiling {

template <typename Output>
concept OutputConcept = requires {
                            typename Output::Settings;
                            typename Output::Settings::Output;
                        };

template <OutputConcept... Outputs>
using BaseOutputSettings = std::variant<typename Outputs::Settings...>;

struct ProfileSettings {
    using OutputSettings = BaseOutputSettings<StreamOutput>;

    std::vector<OutputSettings> outputs;
};

} // namespace Profiling