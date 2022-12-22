#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <concepts>
#include <atomic>
#include <string_view>

namespace ConfigLibrary {

template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...> {};

template <class T>
concept AtomicType = std::atomic<T>::is_always_lock_free;

template <class T, class ... Ts>
concept IsConfigAllowedType = AtomicType<T> && is_any<T, Ts...>::value;

template <AtomicType ... Ts>
class ConfigRegistryBase {
protected:
    using ConfigVariableId = size_t;
    template <IsConfigAllowedType<Ts...> T>
    class ConfigVariable {        
    public:
        ConfigVariable(ConfigRegistryBase& registry_, ConfigVariableId id_): registry{registry_}, id{id_} {}
        ConfigVariable(const ConfigVariable&) = delete;

        auto getValue() const -> T { 
            const auto& container = std::get<PerTypeConfigContainer<T>>(registry._configVariables);
            return container[id].value.load();
        }

        auto setValue(T value) -> void { 
            auto& container = std::get<PerTypeConfigContainer<T>>(registry._configVariables);
            container[id].value.store(value);
        }

        ConfigRegistryBase& registry;
        ConfigVariableId id;
    };

private:
    template <IsConfigAllowedType<Ts...> T>
    struct ConfigVariableValueContainer {
        ConfigVariableValueContainer(T value_): value{value_} {}
        ConfigVariableValueContainer(ConfigVariableValueContainer&& other): value{other.value.load()} {}

        std::atomic<T> value;
    };

    template <IsConfigAllowedType<Ts...> T>
    using PerTypeConfigContainer = std::vector<ConfigVariableValueContainer<T>>;

public:
    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto getConfigVariableValueByName(std::string_view name) const -> T {
        const auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        //container.emplace_back(std::move(defaultValue));
        return 0;
    }

protected:
    ConfigRegistryBase() {}

    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]]
    auto registerConfigVariable(std::string_view name, T defaultValue = {}) -> ConfigVariable<T> {
        auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        container.emplace_back(container.size());
        return ConfigVariable<T>{*this, container.size() - 1};
    }

private:
    std::tuple<PerTypeConfigContainer<Ts>...> _configVariables;
};

class ConfigRegistry : public ConfigRegistryBase<int, float, double> {
public:
    ConfigRegistry();

    void serialize();

    ConfigVariable<int> exampleVariable;
};

} // namespace ConfigLibrary