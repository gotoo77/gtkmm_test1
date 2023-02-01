//
// Created by gotoo77 on 21/10/2022.
//
#include <algorithm>
#include <array>
#include <cstring>
#include <functional>
#include <random>
#include <string>
#include "string_gen.hpp"


std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;
    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

template <typename T = std::mt19937>
auto random_generator() -> T {
    auto constexpr seed_bytes = sizeof(typename T::result_type) * T::state_size;
    auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
    auto seed = std::array<std::seed_seq::result_type, seed_len>();
    std::random_device dev;
//    auto dev= std::random_device();
    std::generate_n(begin(seed), seed_len, std::ref(dev));
//    auto seed_seq = std::seed_seq(begin(seed), end(seed));
    std::seed_seq seed_seq{1,2,3,4,5};
    std::vector<std::uint32_t> seeds(10);
    seed_seq.generate(begin(seed), end(seed));

    return T{seed_seq};
}

auto generate_random_alphanumeric_string(std::size_t len) -> std::string {
    static constexpr auto chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    thread_local auto rng = random_generator<>();

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    (std::mt19937(rd())); //Standard mersenne_twister_engine seeded with rd()
    std::size_t char_len = std::strlen(chars) - 1;
//    std::uniform_int_distribution<> dist(0, static_cast<int>(char_len));
    std::uniform_int_distribution<> dist(0, 1);
//    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
    return result;
}