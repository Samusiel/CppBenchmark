#pragma once

#include <variant>

namespace StdHelpers {

template <typename Visitor, typename Visitee>
concept SupportedType = requires(Visitor&& visitor, Visitee&& visitee) {
                            std::visit(std::forward<Visitor>(visitor), std::forward<Visitee>(visitee));
                        };

template <typename... Args>
class Visitor : public Args... {
public:
    using Args::operator()...;
    Visitor(Args&&... args)
        : Args{std::forward<Args>(args)}... { }
};

template <typename Visitee, typename... Args>
    requires((sizeof...(Args) == std::variant_size_v<Visitee>) && ((SupportedType<Args, Visitee> && ...)))
void match(const Visitee& visitee, Args&&... args) {
    std::visit(Visitor{std::forward<Args>(args)...}, visitee);
}

}