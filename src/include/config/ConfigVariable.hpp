#include <cstdint>
#include <string_view>

namespace ConfigLibrary {

template <class T>
concept AtomicType = std::atomic<T>::is_always_lock_free;

using ConfigVariableId = size_t;

template <AtomicType T>
class ConfigVariable {
private:
    friend class ConfigRegistry;

    struct Accessor {
        Getter getValue;
        Setter setValue;
    };
    
public:
    auto getValue() const -> const T& {
        return _accessor.getValue();
    }

    auto setValue(T value) {
        _accessor.setValue(value);
    }

private:
    template <>
    ConfigVariable(ConfigVariableId id): {

    }

private:
    ConfigRegistry& registry;
    Accessor _accessor;
};

}