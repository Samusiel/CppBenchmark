#pragma once

#include <resources/Resource.hpp>
#include <resources/ResourceId.hpp>

namespace Resources {

struct ResourceLoadRequest {
    ResourceId id;
    IResource::Type type;
};

} // namespace Resources