#pragma once

#include <profiling/IProfileOutput.hpp>

namespace Profiling {

class StreamOutput : public IProfileOutput {
public:
    struct Settings {
        using Output = StreamOutput;
    };

public:
    StreamOutput(Settings& settings);
    ~StreamOutput() override;

    void emit(Timer timer) override;
    void emit(Counter counter) override;

    auto settings() const { return _settings; }

private:
    Settings& _settings;
};

} // Profiling