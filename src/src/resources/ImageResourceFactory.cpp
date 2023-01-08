#include <resources/Image.hpp>
#include <resources/ResourceFactory.hpp>

namespace Resources {

template <>
auto ResourceFactory<Image>::convert(std::span<uint8_t> memory) -> Image {
    return Image{};
}

} // namespace Resources