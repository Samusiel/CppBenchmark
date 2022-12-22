//#include <cstdint>
#include <config/ConfigRegistry.hpp>
// #include <concepts>
// #include <array>
// #include <string_view>

// template <class T>
// concept CItem = requires(T item) {
//     { item.update() } -> std::same_as<void>;
// };

// template <CItem ... Types>
// class Container {
// public:
//     using TypedVectorContainer = std::tuple<std::vector<Types>...>;

// public:
//     void update() {
//         const auto perTypeContainerUpdate = [this] (auto& perTypeContainer) {
//             for (auto& item : perTypeContainer) {
//                 item->update();
//             }
//         };
//         std::apply(
//             [this, &perTypeContainerUpdate](auto& ... perTypeContainer) {
//                 (perTypeContainerUpdate(perTypeContainer), ...);
//             },
//             _items
//         );
//     }

//     template <CItem T>
//     bool push_back(T&& item) {
//         auto& perTypeContainer = std::get<std::vector<T>>(_items);
//         perTypeContainer.push_back(std::move(item));
//     }

// private:
//     TypedVectorContainer _items;
// };

// class MyItem {
// public:
//     void update() {

//     }
// };

// template <class Char, int Size>
// struct BasicStaticString {
// public:
// private:
//     std::array<Char, Size> _characters;
// };
// template <int Size>
// using StaticString = BasicStaticString<char, Size>;

int main() {
    ConfigLibrary::ConfigRegistry registry;
    //auto id = registry.registerConfigVariable("Hello World", 42);

    return 0;
}