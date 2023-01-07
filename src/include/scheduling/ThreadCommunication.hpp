#pragma once

/**
 * @brief This file is temporary until clang will implement standard classes
 * std::stop_token and std::stop_source
 *
 */

#ifdef __clang__

/// @brief It's a bad practice to call you namespace std, but the code is temporary
/// Copy-paste and simplification of the GCC implementation.

// <stop_token> -*- C++ -*-

// Copyright (C) 2019-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/stop_token
 *  This is a Standard C++ Library header.
 */

#include <atomic>
#include <semaphore>
#include <thread>

namespace std {

/// Tag type indicating a stop_source should have no shared-stop-state.
struct nostopstate_t {
    explicit nostopstate_t() = default;
};
inline constexpr nostopstate_t nostopstate{};

class stop_source;

/// Allow testing whether a stop request has been made on a `stop_source`.
class stop_token {
public:
    stop_token() noexcept = default;

    stop_token(const stop_token&) noexcept = default;
    stop_token(stop_token&&) noexcept = default;

    ~stop_token() = default;

    stop_token& operator=(const stop_token&) noexcept = default;

    stop_token& operator=(stop_token&&) noexcept = default;

    [[nodiscard]] bool stop_possible() const noexcept {
        return static_cast<bool>(_M_state) && _M_state->_M_stop_possible();
    }

    [[nodiscard]] bool stop_requested() const noexcept {
        return static_cast<bool>(_M_state) && _M_state->_M_stop_requested();
    }

    void swap(stop_token& __rhs) noexcept { _M_state.swap(__rhs._M_state); }

    [[nodiscard]] friend bool operator==(const stop_token& __a, const stop_token& __b) {
        return __a._M_state == __b._M_state;
    }

    friend void swap(stop_token& __lhs, stop_token& __rhs) noexcept { __lhs.swap(__rhs); }

private:
    friend class stop_source;

    static void _S_yield() noexcept { std::this_thread::yield(); }

    struct _Stop_state_t {
        using value_type = uint32_t;
        static constexpr value_type _S_stop_requested_bit = 1;
        static constexpr value_type _S_locked_bit = 2;
        static constexpr value_type _S_ssrc_counter_inc = 4;

        std::atomic<value_type> _M_owners{1};
        std::atomic<value_type> _M_value{_S_ssrc_counter_inc};
        std::thread::id _M_requester;

        _Stop_state_t() = default;

        bool _M_stop_possible() noexcept {
            // true if a stop request has already been made or there are still
            // stop_source objects that would allow one to be made.
            return _M_value.load(std::memory_order_acquire) & ~_S_locked_bit;
        }

        bool _M_stop_requested() noexcept { return _M_value.load(std::memory_order_acquire) & _S_stop_requested_bit; }

        void _M_add_owner() noexcept { _M_owners.fetch_add(1, std::memory_order_relaxed); }

        void _M_release_ownership() noexcept {
            if (_M_owners.fetch_sub(1, std::memory_order_acq_rel) == 1)
                delete this;
        }

        void _M_add_ssrc() noexcept { _M_value.fetch_add(_S_ssrc_counter_inc, std::memory_order_relaxed); }

        void _M_sub_ssrc() noexcept { _M_value.fetch_sub(_S_ssrc_counter_inc, std::memory_order_release); }

        // Obtain lock.
        void _M_lock() noexcept {
            // Can use relaxed loads to get the current value.
            // The successful call to _M_try_lock is an acquire operation.
            auto __old = _M_value.load(std::memory_order_relaxed);
            while (!_M_try_lock(__old, std::memory_order_relaxed)) { }
        }

        // Precondition: calling thread holds the lock.
        void _M_unlock() noexcept { _M_value.fetch_sub(_S_locked_bit, std::memory_order_release); }

        bool _M_request_stop() noexcept {
            // obtain lock and set stop_requested bit
            auto __old = _M_value.load(std::memory_order_acquire);
            do {
                if (__old & _S_stop_requested_bit) // stop request already made
                    return false;
            } while (!_M_try_lock_and_stop(__old));

            _M_requester = std::this_thread::get_id();
            _M_unlock();
            return true;
        }

        // Try to obtain the lock.
        // Returns true if the lock is acquired (with memory order acquire).
        // Otherwise, sets __curval = _M_value.load(__failure) and returns false.
        // Might fail spuriously, so must be called in a loop.
        bool _M_try_lock(value_type& __curval, std::memory_order __failure = std::memory_order_acquire) noexcept {
            return _M_do_try_lock(__curval, 0, std::memory_order_acquire, __failure);
        }

        // Try to obtain the lock to make a stop request.
        // Returns true if the lock is acquired and the _S_stop_requested_bit is
        // set (with memory order acq_rel so that other threads see the request).
        // Otherwise, sets __curval = _M_value.load(std::memory_order_acquire) and
        // returns false.
        // Might fail spuriously, so must be called in a loop.
        bool _M_try_lock_and_stop(value_type& __curval) noexcept {
            return _M_do_try_lock(
                __curval, _S_stop_requested_bit, std::memory_order_acq_rel, std::memory_order_acquire);
        }

        bool _M_do_try_lock(value_type& __curval, value_type __newbits, std::memory_order __success,
            std::memory_order __failure) noexcept {
            if (__curval & _S_locked_bit) {
                _S_yield();
                __curval = _M_value.load(__failure);
                return false;
            }
            __newbits |= _S_locked_bit;
            return _M_value.compare_exchange_weak(__curval, __curval | __newbits, __success, __failure);
        }
    };

    struct _Stop_state_ref {
        _Stop_state_ref() = default;

        explicit _Stop_state_ref(const stop_source&)
            : _M_ptr(new _Stop_state_t()) { }

        _Stop_state_ref(const _Stop_state_ref& __other) noexcept
            : _M_ptr(__other._M_ptr) {
            if (_M_ptr)
                _M_ptr->_M_add_owner();
        }

        _Stop_state_ref(_Stop_state_ref&& __other) noexcept
            : _M_ptr(__other._M_ptr) {
            __other._M_ptr = nullptr;
        }

        _Stop_state_ref& operator=(const _Stop_state_ref& __other) noexcept {
            if (auto __ptr = __other._M_ptr; __ptr != _M_ptr) {
                if (__ptr)
                    __ptr->_M_add_owner();
                if (_M_ptr)
                    _M_ptr->_M_release_ownership();
                _M_ptr = __ptr;
            }
            return *this;
        }

        _Stop_state_ref& operator=(_Stop_state_ref&& __other) noexcept {
            _Stop_state_ref(std::move(__other)).swap(*this);
            return *this;
        }

        ~_Stop_state_ref() {
            if (_M_ptr)
                _M_ptr->_M_release_ownership();
        }

        void swap(_Stop_state_ref& __other) noexcept { std::swap(_M_ptr, __other._M_ptr); }

        explicit operator bool() const noexcept { return _M_ptr != nullptr; }

        _Stop_state_t* operator->() const noexcept { return _M_ptr; }

        friend bool operator==(const _Stop_state_ref& __lhs, const _Stop_state_ref& __rhs) noexcept {
            return __lhs._M_ptr == __rhs._M_ptr;
        }

        friend bool operator!=(const _Stop_state_ref& __lhs, const _Stop_state_ref& __rhs) noexcept {
            return __lhs._M_ptr != __rhs._M_ptr;
        }

    private:
        _Stop_state_t* _M_ptr = nullptr;
    };

    _Stop_state_ref _M_state;

    explicit stop_token(const _Stop_state_ref& __state) noexcept
        : _M_state{__state} { }
};

/// A type that allows a stop request to be made.
class stop_source {
public:
    stop_source()
        : _M_state(*this) { }

    explicit stop_source(nostopstate_t) noexcept { }

    stop_source(const stop_source& __other) noexcept
        : _M_state(__other._M_state) {
        if (_M_state)
            _M_state->_M_add_ssrc();
    }

    stop_source(stop_source&&) noexcept = default;

    stop_source& operator=(const stop_source& __other) noexcept {
        if (_M_state != __other._M_state) {
            stop_source __sink(std::move(*this));
            _M_state = __other._M_state;
            if (_M_state)
                _M_state->_M_add_ssrc();
        }
        return *this;
    }

    stop_source& operator=(stop_source&&) noexcept = default;

    ~stop_source() {
        if (_M_state)
            _M_state->_M_sub_ssrc();
    }

    [[nodiscard]] //
    bool
    stop_possible() const noexcept {
        return static_cast<bool>(_M_state);
    }

    [[nodiscard]] //
    bool
    stop_requested() const noexcept {
        return static_cast<bool>(_M_state) && _M_state->_M_stop_requested();
    }

    bool request_stop() const noexcept {
        if (stop_possible())
            return _M_state->_M_request_stop();
        return false;
    }

    [[nodiscard]] //
    stop_token
    get_token() const noexcept {
        return stop_token{_M_state};
    }

    void swap(stop_source& __other) noexcept { _M_state.swap(__other._M_state); }

    [[nodiscard]] //
    friend bool
    operator==(const stop_source& __a, const stop_source& __b) noexcept {
        return __a._M_state == __b._M_state;
    }

    friend void swap(stop_source& __lhs, stop_source& __rhs) noexcept { __lhs.swap(__rhs); }

private:
    stop_token::_Stop_state_ref _M_state;
};

} // namespace std

#elif __GNUC__

#include <stop_token>

#endif