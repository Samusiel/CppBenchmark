#include <cstdint>
#include <vector>
#include <tuple>
#include <concepts>
#include <atomic>
#include <string_view>
#include <config/ConfigVariable.hpp>

namespace ConfigLibrary {

template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...> {};

template <class T, class ... Ts>
concept IsConfigAllowedType = AtomicType<T> && is_any<T, Ts...>::value;

template <AtomicType T>
using ConfigVariableValue = std::atomic<T>;

template <AtomicType ... Ts>
class ConfigRegistryBase {
public:
    template <IsConfigAllowedType<Ts...> T>
    using PerTypeConfigContainer = std::vector<ConfigVariableValue<T>>;

public:
    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto registerConfigVariable(std::string_view name, T defaultValue = {}) -> ConfigVariable<T> {
        auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        //container.emplace_back(std::move(defaultValue));
        return 0;
    }

    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto getConfigVariableValueByName(std::string_view name) -> T {
        auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        //container.emplace_back(std::move(defaultValue));
        return 0;
    }

private:
    std::tuple<PerTypeConfigContainer<Ts>...> _configVariables;
};
using ConfigRegistry = ConfigRegistryBase<int, float, double>;

} // namespace ConfigLibrary