#pragma once

#ifdef __clang__
#include <experimental/coroutine>

// Not the best practise, but will be removed when clang will move coroutines from experimental
namespace std {

using suspend_always = experimental::suspend_always;
using suspend_never = experimental::suspend_never;
template <typename T>
using coroutine_handle = experimental::coroutine_handle<T>;

} // namespace std

#elif __GNUC__

#include <coroutine>

#endif

#include <type_traits>
#include <utility>

namespace Coroutines {

template <typename T>
class generator;

namespace Detail {

template <typename T>
class generator_promise {
    pointer_type m_value;

public:
    using value_type = std::remove_reference_t<T>;
    using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
    using pointer_type = value_type*;

    generator_promise() = default;

    generator<T> get_return_object() noexcept;

    constexpr std::suspend_always initial_suspend() const { return {}; }
    constexpr std::suspend_always final_suspend() const { return {}; }

    template <typename U, typename = std::enable_if_t<std::is_same<U, T>::value>>
    std::suspend_always yield_value(U& value) noexcept {
        m_value = std::addressof(value);
        return {};
    }

    std::suspend_always yield_value(T&& value) noexcept {
        m_value = std::addressof(value);
        return {};
    }

    void unhandled_exception() { std::rethrow_exception(std::current_exception()); }

    void return_void() { }

    reference_type value() const noexcept { return *m_value; }

    // Don't allow any use of 'co_await' inside the generator coroutine.
    template <typename U>
    std::suspend_never await_transform(U&& value) = delete;
};

template <typename T>
class generator_iterator {
    using coroutine_handle = std::coroutine_handle<generator_promise<T>>;
    coroutine_handle m_coroutine;

public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::size_t;
    using value_type = std::remove_reference_t<T>;
    using reference = value_type&;
    using pointer = value_type*;

    explicit generator_iterator(std::nullptr_t) noexcept
        : m_coroutine(nullptr) { }

    explicit generator_iterator(coroutine_handle coroutine) noexcept
        : m_coroutine(coroutine) { }

    bool operator==(const generator_iterator& other) const noexcept { return m_coroutine == other.m_coroutine; }

    bool operator!=(const generator_iterator& other) const noexcept { return !(*this == other); }

    generator_iterator& operator++() {
        m_coroutine.resume();
        if (m_coroutine.done()) {
            m_coroutine = nullptr;
        }

        return *this;
    }

    // Don't support post-increment as that would require taking a
    // copy of the old value into the returned iterator as there
    // are no guarantees it's still going to be valid after the
    // increment is executed.
    generator_iterator operator++(int) = delete;

    reference operator*() const noexcept { return m_coroutine.promise().value(); }

    pointer operator->() const noexcept { return std::addressof(operator*()); }
};

} // namespace Detail

template <typename T>
class generator {
public:
    using promise_type = Detail::generator_promise<T>;
    using iterator = Detail::generator_iterator<T>;

    generator() noexcept
        : m_coroutine(nullptr) { }

    generator(generator&& other) noexcept
        : m_coroutine(other.m_coroutine) {
        other.m_coroutine = nullptr;
    }

    generator(const generator& other) = delete;

    ~generator() {
        if (m_coroutine) {
            m_coroutine.destroy();
        }
    }

    auto operator=(generator other) noexcept -> generator& {
        swap(other);
        return *this;
    }

    auto begin() -> iterator {
        if (m_coroutine) {
            m_coroutine.resume();
            if (!m_coroutine.done()) {
                return iterator{m_coroutine};
            }
        }

        return iterator{nullptr};
    }

    auto end() noexcept -> iterator { return iterator{nullptr}; }

    void swap(generator& other) noexcept { std::swap(m_coroutine, other.m_coroutine); }

private:
    friend class detail::generator_promise<T>;

    explicit generator(std::coroutine_handle<promise_type> coroutine) noexcept
        : m_coroutine(coroutine) { }

    std::coroutine_handle<promise_type> m_coroutine;
};

template <typename T>
void swap(generator<T>& a, generator<T>& b) {
    a.swap(b);
}

template <typename T>
auto generator_promise<T>::get_return_object() noexcept {
    using coroutine_handle = std::coroutine_handle<generator_promise<T>>;
    return generator<T>{coroutine_handle::from_promise(*this)};
}

} // Coroutines