#include <config/ConfigRegistry.hpp>
#include <experimental/coroutine>
#include <functional>
#include <iostream>
#include <math/Hash.hpp>
#include <profiling/ProfileBackend.hpp>
#include <profiling/ProfileGuard.hpp>
#include <profiling/ProfileMark.hpp>
#include <profiling/ProfileMarks.hpp>
#include <reflection/AggregateTypeVisitor.hpp>
#include <resources/ResourceManager.hpp>
#include <scheduling/Scheduler.hpp>
#include <thread>

template <typename Factory, typename Resource>
concept ResourceFactory = requires(Factory factory) {
                              { factory() } -> std::same_as<Resource>;
                          };

template <typename Resource, ResourceFactory<Resource> Loader>
class ResourceLoader {
public:
    ResourceLoader(Loader&& loader)
        : _loader(std::move(loader)) { }

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::experimental::coroutine_handle<> h) {
        // auto resourceFuture = _loader();
        h.resume();
    }

    Resource&& await_resume() noexcept { return std::move(_resource); }

    operator Resource() { return _resource; }

private:
    Loader _loader;
    Resource _resource;
};

template <typename Resource>
struct Task {
    struct promise_type {
        Task<Resource> get_return_object() { return {}; }
        std::experimental::suspend_never initial_suspend() { return {}; }
        std::experimental::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() { }
        void return_void() { }
    };
};

struct File { };

struct Mesh { };

struct Texture { };

Task<File> loadFile(std::string name, Resources::ResourceManager& resourceManager) {
    auto resourceLoader = [name = std::move(name)] {
        return File{};
    };
    [[maybe_unused]] File file = co_await ResourceLoader<File, decltype(resourceLoader)>(std::move(resourceLoader));
}

// Task loadMesh(std::string name) {
//     auto resourceLoader = [name = std::move(name)] { return Mesh{}; };
//     co_yield ResourceLoader<Mesh,
//     decltype(resourceLoader)>(std::move(resourceLoader));
// }

// Task loadTexture(std::string name) {
//     auto resourceLoader = [name = std::move(name)] { return Texture{}; };
//     co_yield ResourceLoader<Texture,
//     decltype(resourceLoader)>(std::move(resourceLoader));
// }

int main() {
    // loadFile("file.txt");
    Profiling::ProfileScopeGuard guard{
        Profiling::ProfileBackendFactory::createCPUBackend(Profiling::ProfileMark::create("Hello World"))};

    // const auto call = [guard = std::move(guard)] {

    // };

    // std::cout << "Aand done" << std::endl;
    //  h.destroy();
    return 0;
}