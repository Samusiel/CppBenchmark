#pragma once

#include <memory>
#include <resources/ResourceId.hpp>

namespace Resources {

class ResourceLoader {
public:
    ResourceLoader();
    ~ResourceLoader();

    template <typename Resource>
    void load(const ResourceId& id);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace Resources