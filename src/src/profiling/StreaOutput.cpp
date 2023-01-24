#include <profiling/StreamOutput.hpp>

namespace Profiling {

StreamOutput::StreamOutput(Settings& settings)
    : _settings(settings) { }

StreamOutput::~StreamOutput() { }

void StreamOutput::emit(Timer timer) { }

void StreamOutput::emit(Counter counter) { }

} // namespace Profiling