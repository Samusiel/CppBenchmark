#include <resources/ResourceLoader.hpp>
#include <std_helpers/Visitor.hpp>
#include <storage/FileSystemStorage.hpp>
#include <storage/WebStorage.hpp>
#include <tuple>

namespace Resources {

class ResourceLoader::Impl {
public:
    void load(const ResourceId& id);

private:
    template <typename Storage, typename ResId>
    void load(const ResId& id) {
        auto& storage = std::get<Storage>(_storages);
    }

private:
    std::tuple<Storage::FileSystemStorage, Storage::WebStorage> _storages;
};

ResourceLoader::ResourceLoader()
    : _impl(std::make_unique<Impl>()) { }
ResourceLoader::~ResourceLoader() { }

void ResourceLoader::load(const ResourceId& id) {
    _impl->load(id);
}

void ResourceLoader::Impl::load(const ResourceId& id) {
    StdHelpers::match(
        id,
        [this](const FileSystemPath& path) {
            load<Storage::FileSystemStorage>(path);
        },
        [this](const WebPath& path) {
            load<Storage::WebStorage>(path);
        });
}

}