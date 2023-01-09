#pragma once

#include <span>
#include <tl/expected.hpp>

namespace Resources {

template <typename Resource>
class ResourceFactory {
public:
    tl::expected<Resource> convert(std::span<uint8_t> memory);
};

} // namespace Resources