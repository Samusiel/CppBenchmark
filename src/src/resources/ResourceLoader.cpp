#include <resources/Image.hpp>
#include <resources/ResourceLoader.hpp>
#include <std_helpers/Visitor.hpp>
#include <storage/FileSystemStorage.hpp>
#include <storage/WebStorage.hpp>
#include <tuple>

namespace Resources {

class ResourceLoader::Impl {
public:
    template <typename Resource>
    void load(const ResourceId& id);

private:
    template <typename Storage, typename Factory, typename ResId>
    void load(Factory&& factory, const ResId& id);

private:
    std::tuple<Storage::FileSystemStorage, Storage::WebStorage> _storages;
};

ResourceLoader::ResourceLoader()
    : _impl(std::make_unique<Impl>()) { }
ResourceLoader::~ResourceLoader() { }

template <typename Resource>
void ResourceLoader::load(const ResourceId& id) {
    _impl->load<Resource>(id);
}
template <>
void ResourceLoader::load<Image>(const ResourceId& id);

template <typename Resource>
void ResourceLoader::Impl::load(const ResourceId& id) {
    StdHelpers::match(
        id,
        [this](const FileSystemPath& path) {
            load<Storage::FileSystemStorage>(typename Resource::ResourceFactory{}, path);
        },
        [this](const WebPath& path) {
            load<Storage::WebStorage>(typename Resource::ResourceFactory{}, path);
        });
}

template <typename Storage, typename Factory, typename ResId>
void ResourceLoader::Impl::load(Factory&& factory, const ResId& id) {
    auto& storage = std::get<Storage>(_storages);
    storage.load(std::move(factory), id);
}

} // namespace Resources