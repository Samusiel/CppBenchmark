#include <config/ConfigRegistry.hpp>
#include <math/Hash.hpp>
#include <iostream>

int main() {
    //ConfigLibrary::ConfigRegistry registry;
    constexpr size_t hashValue = Math::compileTimeHash("Hello World");
    static_assert(hashValue == 13366473710097474195U);
    std::cout << "Hello World hash = " << hashValue << std::endl;
    return 0;
}