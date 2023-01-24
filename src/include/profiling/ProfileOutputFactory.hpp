#pragma once

#include <memory>
#include <profiling/IProfileOutput.hpp>

namespace Profiling {

template <typename Settings>
concept SettingsConcept = requires {
                              typename Settings::Output;
                              std::derived_from<typename Settings::Output, IProfileOutput>;
                              requires requires(Settings & settings) { typename Settings::Output(settings); };
                          };

class ProfileOutputFactory {
public:
    template <SettingsConcept Settings>
    static auto create(Settings& settings) -> std::unique_ptr<IProfileOutput> {
        return std::make_unique<typename Settings::Output>(settings);
    }
};

} // namespace Profiling