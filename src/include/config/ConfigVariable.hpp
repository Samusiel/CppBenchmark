#include <cstdint>
#include <string_view>

namespace ConfigLibrary {

template <class T>
concept AtomicType = std::atomic<T>::is_always_lock_free;

template <class T>
concept IsAccessor = requires(T item) {
    { item.get() } -> std::same_as<typename T::ValueType>;
    { item.set(typename T::ValueType{}) } -> std::same_as<void>;
};

using ConfigVariableId = size_t;

template <AtomicType T>
class ConfigVariable {
private:
    template <AtomicType ... Ts>
    class ConfigRegistryBase;
    using AccessorGet = T();
    using AccessorSet = void(const T&);
    
public:
    auto getValue() const -> const T& {
        return _getter(_accessor);
    }

    auto setValue(T value) {
        _setter(value);
    }

private:
    template <IsAccessor Accessor>
    ConfigVariable(const Accessor& accessor): 
        _accessor{&accessor},
        _getter([](const void* accessorPtr) {
            const auto& accessor = *static_cast<const Accessor*>(accessorPtr);
            return accessor.get();
        }),
        _setter([](const void* accessorPtr, T value) {
            const auto& accessor = *static_cast<const Accessor*>(accessorPtr);
            accessor.set(value);
        }) {}

private:
    const void* _accessor = nullptr;
    AccessorGet* _getter = nullptr;
    AccessorSet* _setter = nullptr;
};

} // namespace ConfigLibrary