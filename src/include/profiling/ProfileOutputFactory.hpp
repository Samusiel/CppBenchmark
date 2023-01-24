#pragma once

#include <memory>
#include <profiling/IProfileOutput.hpp>

namespace Profiling {

template <typename Settings>
concept SettingsConcept = requires {
                              typename Settings::Output; // In Settings we should specify the output class
                              std::derived_from<typename Settings::Output,
                                  IProfileOutput>; // This output class should be derived from IProfileOutput
                              requires requires(Settings & settings) {
                                           typename Settings::Output(settings);
                                       }; // We ensure that this class has a constructor with Settings as a parameter
                          };

class ProfileOutputFactory {
public:
    template <SettingsConcept Settings>
    static auto create(Settings& settings) -> std::unique_ptr<IProfileOutput> {
        return std::make_unique<typename Settings::Output>(settings);
    }
};

} // namespace Profiling