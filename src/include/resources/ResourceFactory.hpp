#pragma once

#include <span>

namespace Resources {

template <typename Resource>
class ResourceFactory {
public:
    Resource convert(std::span<uint8_t> memory);
};

} // namespace Resources