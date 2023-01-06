#pragma once

#include <atomic>
#include <concepts>
#include <cstdint>
#include <math/Hash.hpp>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace ConfigLibrary {

template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...> { };

template <class T>
concept AtomicType = std::atomic<T>::is_always_lock_free;

template <class T, class... Ts>
concept IsConfigAllowedType = AtomicType<T> && is_any<T, Ts...>::value;

template <AtomicType... Ts>
class ConfigRegistryBase {
protected:
    using ConfigVariableId = size_t;
    template <IsConfigAllowedType<Ts...> T>
    class ConfigVariable {
    public:
        ConfigVariable(ConfigRegistryBase& registry_, ConfigVariableId id_)
            : registry{registry_}
            , id{id_} { }
        ConfigVariable(const ConfigVariable&) = delete;
        ConfigVariable(ConfigVariable&&) = delete;

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
        ConfigVariableValueContainer(T value_)
            : value{value_} { }
        ConfigVariableValueContainer(ConfigVariableValueContainer&& other)
            : value{other.value.load()} { }

        std::atomic<T> value;
    };

    template <IsConfigAllowedType<Ts...> T>
    using PerTypeConfigContainer = std::vector<ConfigVariableValueContainer<T>>;
    using AddressTable = std::unordered_map<std::string_view, ConfigVariableId>;

public:
    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]] auto getConfigVariableValueByName(std::string_view name) const -> T {
        const auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        if (auto it = _addressTable.find(name); it != _addressTable.end()) {
            return container[it->second].value.load();
        }
        // No values related to the name
        assert(false);
        return T{};
    }

protected:
    ConfigRegistryBase() { }

    template <IsConfigAllowedType<Ts...> T>
    [[nodiscard]] auto registerConfigVariable(std::string_view name, T defaultValue = {}) -> ConfigVariable<T> {
        auto& container = std::get<PerTypeConfigContainer<T>>(_configVariables);
        // We check that the name is not presented
        assert(_addressTable.find(name) == _addressTable.end());
        container.emplace_back(container.size());
        size_t idx = container.size() - 1;
        _addressTable.insert({name, idx});
        return ConfigVariable<T>{*this, idx};
    }

private:
    std::tuple<PerTypeConfigContainer<Ts>...> _configVariables;
    AddressTable _addressTable;
};

class ConfigRegistry : public ConfigRegistryBase<int, float, double> {
public:
    ConfigRegistry();

    void serialize();

    ConfigVariable<int> exampleVariable;
};

} // namespace ConfigLibrary