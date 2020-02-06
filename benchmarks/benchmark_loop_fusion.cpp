#include "loop_fusion/main_range/loop_fusion.hpp"

#include <Catch2/catch.hpp>

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

using namespace loop_fusion::main_range;

TEST_CASE("Loop fusion benchmark")
{
    constexpr std::size_t limit = 1'000'000;

    std::vector<int> a(limit);
    std::vector<int> b(limit);
    std::random_device rd {};
    std::mt19937 gen { rd() };
    std::uniform_int_distribution dist { 1, 10'000 };
    auto generator = [&dist, &gen]() { return dist(gen); };
    std::generate(a.begin(), a.end(), generator);
    std::generate(b.begin(), b.end(), generator);

    std::vector<int> c0(limit);
    std::vector<int> d0(limit);
    std::vector<int> c(limit);
    std::vector<int> d(limit);

    BENCHMARK("Plain loop (unfused)")
    {
        for (std::size_t i = 0; i != limit; ++i) {
            c0[i] = a[i] + b[i];
        }
        for (std::size_t i = 0; i != limit; ++i) {
            d0[i] = c0[i] * a[i];
        }
        for (std::size_t i = 0; i != limit; ++i) {
            c0[i] = d0[i] - a[i];
        }
    };

    BENCHMARK("Plain loop (fused)")
    {
        for (std::size_t i = 0; i != limit; ++i) {
            c[i] = a[i] + b[i];
            d[i] = c[i] * a[i];
            c[i] = d[i] - a[i];
        }
    };

    CHECK(std::equal(c0.begin(), c0.end(), c.begin(), c.end()));
    CHECK(std::equal(d0.begin(), d0.end(), d.begin(), d.end()));

    BENCHMARK("Looper (unfused)")
    {
        auto op1 = [&](std::size_t i) { c[i] = a[i] + b[i]; };
        auto op2 = [&](std::size_t i) { d[i] = c[i] * a[i]; };
        auto op3 = [&](std::size_t i) { c[i] = d[i] - a[i]; };
        loop_to(limit, op1).run();
        loop_to(limit, op2).run();
        loop_to(limit, op3).run();
    };

    CHECK(std::equal(c0.begin(), c0.end(), c.begin(), c.end()));
    CHECK(std::equal(d0.begin(), d0.end(), d.begin(), d.end()));

    BENCHMARK("Looper (fused)")
    {
        auto op1 = [&](std::size_t i) { c[i] = a[i] + b[i]; };
        auto op2 = [&](std::size_t i) { d[i] = c[i] * a[i]; };
        auto op3 = [&](std::size_t i) { c[i] = d[i] - a[i]; };
        loop_to(limit, op1, op2, op3).run();
    };

    CHECK(std::equal(c0.begin(), c0.end(), c.begin(), c.end()));
    CHECK(std::equal(d0.begin(), d0.end(), d.begin(), d.end()));
}
