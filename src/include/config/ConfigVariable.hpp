#include <cstdint>
#include <string_view>

namespace ConfigLibrary {

template <class T>
concept AtomicType = std::atomic<T>::is_always_lock_free;

template <class T>
concept IsAccessor = requires(T item) {
    { item.getValue() } -> std::same_as<typename T::ValueType>;
    { item.setValue(typename T::ValueType{}) } -> std::same_as<void>;
};

using ConfigVariableId = size_t;

template <AtomicType T>
class ConfigVariable {
private:
    template <AtomicType ... Ts>
    friend class ConfigRegistryBase;
    using AccessorGet = T(void* accessorPtr);
    using AccessorSet = void(void* accessorPtr, const T&);
    
public:

    auto getValue() const -> const T& {
        return _getter(_accessor);
    }

    auto setValue(T value) {
        _setter(value);
    }

private:
    template <IsAccessor Accessor>
    ConfigVariable(Accessor& accessor): 
        _accessor{&accessor},
        _getter([](void* accessorPtr) {
            const auto& accessor = *static_cast<Accessor*>(accessorPtr);
            return accessor.getValue();
        }),
        _setter([](void* accessorPtr, const T& value) {
            auto& accessor = *static_cast<Accessor*>(accessorPtr);
            accessor.setValue(value);
        }) {}

private:
    void* _accessor = nullptr;
    AccessorGet* _getter = nullptr;
    AccessorSet* _setter = nullptr;
};

} // namespace ConfigLibrary