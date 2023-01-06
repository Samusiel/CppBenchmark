#pragma once

namespace Resources {

class ResourceManager {
public:
    struct Handle { };

public:
    ResourceManager();

    template <typename Resource>
    void registerResource(const std::string& name, Resource&& resource) { }
};

} // namespace Resources