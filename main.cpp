#include <config/ConfigRegistry.hpp>
#include <experimental/coroutine>
#include <iostream>
#include <math/Hash.hpp>
#include <reflection/AggregateTypeVisitor.hpp>
#include <resources/ResourceManager.hpp>
#include <scheduling/Scheduler.hpp>
#include <std_helpers/Visitor.hpp>
#include <thread>

// template <typename Factory, typename Resource>
// concept ResourceFactory = requires(Factory factory) {
//                               { factory() } -> std::same_as<Resource>;
//                           };

// template <typename Resource, ResourceFactory<Resource> Loader>
// class ResourceLoader {
// public:
//     ResourceLoader(Loader&& loader)
//         : _loader(std::move(loader)) { }

//     bool await_ready() const noexcept { return false; }

//     void await_suspend(std::experimental::coroutine_handle<> h) {
//         // auto resourceFuture = _loader();
//         h.resume();
//     }

//     Resource&& await_resume() noexcept { return std::move(_resource); }

//     operator Resource() { return _resource; }

// private:
//     Loader _loader;
//     Resource _resource;
// };

// template <typename Resource>
// struct Task {
//     struct promise_type {
//         Task<Resource> get_return_object() { return {}; }
//         std::experimental::suspend_never initial_suspend() { return {}; }
//         std::experimental::suspend_never final_suspend() noexcept { return {}; }
//         void unhandled_exception() { }
//         void return_void() { }
//     };
// };

// struct File { };

// struct Mesh { };

// struct Texture { };

// void loadFile(std::string name, Resources::ResourceManager& resourceManager) {
//     auto resourceLoader = [name = std::move(name)] {
//         return File{};
//     };
//     [[maybe_unused]] File file = co_await ResourceLoader<File, decltype(resourceLoader)>(std::move(resourceLoader));
// }

// // Task loadMesh(std::string name) {
// //     auto resourceLoader = [name = std::move(name)] { return Mesh{}; };
// //     co_yield ResourceLoader<Mesh,
// //     decltype(resourceLoader)>(std::move(resourceLoader));
// // }

// // Task loadTexture(std::string name) {
// //     auto resourceLoader = [name = std::move(name)] { return Texture{}; };
// //     co_yield ResourceLoader<Texture,
// //     decltype(resourceLoader)>(std::move(resourceLoader));
// // }

int main() {

    std::variant<int, double> v{5.0};
    StdHelpers::match(
        v,
        [](const double& value) {
            std::cout << "Double: " << value << std::endl;
        },
        [](const int& value) {
            std::cout << "Double: " << value << std::endl;
        });
}