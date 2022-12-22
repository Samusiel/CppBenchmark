#include <config/ConfigRegistry.hpp>

namespace ConfigLibrary {

ConfigRegistry::ConfigRegistry():
    exampleVariable(registerConfigVariable("Hello World", 42)) {}

void ConfigRegistry::serialize() {
    // ToDo: Store/Load from file
}

} // namespace ConfigLibrary