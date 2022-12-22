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

private:
    using ConfigVariableId = size_t;
    template <IsConfigAllowedType<Ts...> T>
    class Accessor {
    public:
        using ValueType = T;

    public:
        Accessor(ConfigRegistryBase& registry, ConfigVariableId id): _registry(registry),  _id(id) {}
        auto getValue() const -> T { 
            const auto& container = std::get<PerTypeConfigContainer<T>>(_registry._configVariables);
            return container[_id];
        }

        auto setValue(T value) -> void { 
            auto& container = std::get<PerTypeConfigContainer<T>>(_registry._configVariables);
            container[_id] = value;
        }

    private:
        ConfigRegistryBase& _registry;
        ConfigVariableId _id;
    };

public:
    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto registerConfigVariable(std::string_view name, T defaultValue = {}) -> ConfigVariable<T> {
        auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        //container.emplace_back(std::move(defaultValue));
        auto accessor = Accessor<T>{*this, 0};
        return ConfigVariable<T>{accessor};
    }

    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto getConfigVariableValueByName(std::string_view name) const -> T {
        const auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        //container.emplace_back(std::move(defaultValue));
        return 0;
    }

private:
    std::tuple<PerTypeConfigContainer<Ts>...> _configVariables;
};
using ConfigRegistry = ConfigRegistryBase<int, float, double>;

} // namespace ConfigLibrary