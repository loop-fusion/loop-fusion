#include "test_helpers.hpp"

#include "loop_fusion/main_range/loop_fusion.hpp"
#include "loop_fusion/main_range/looper_union.hpp"

#include <array>
#include <numeric>

using namespace loop_fusion;
using namespace loop_fusion::main_range;

TEST_CASE("Function Sequence main_range", "[basic]")
{
    std::vector<int> vec { 0, 0, 0, 0, 0, 0, 0 };
    const int vecsize { 7 };
    const auto set_one = [&vec](size_t i) {
        if (vec[i] == 0) {
            vec[i] = 1;
        } else {
            vec[i] = 100;
        }
    };
    const auto set_two = [&vec](size_t i) {
        if (vec[i] == 1) {
            vec[i] = 2;
        } else {
            vec[i] = 100;
        }
    };
    const auto set_three = [&vec](size_t i) {
        if (vec[i] == 2) {
            vec[i] = 3;
        } else {
            vec[i] = 100;
        }
    };
    const auto set_four = [&vec](size_t i) {
        if (vec[i] == 3) {
            vec[i] = 4;
        } else {
            vec[i] = 100;
        }
    };

    auto merged = loop_to(vecsize, set_one) | loop_to(vecsize, set_two) //
        | loop_to(vecsize, set_three) | loop_to(2, set_four);
    merged.run();

    const int sum = std::accumulate(vec.cbegin(), vec.cend(), 0);
    CHECK(sum == vec.size() * 3 + 2);
}

TEST_CASE("main_range merger", "[merge][main_range]")
{
    SECTION("basic loop merge")
    {
        std::size_t sum = 0;

        auto addOne = [&sum](std::size_t /*unused*/) { sum += 1; };

        auto loop1 = loop(range { 100, 1000 }, addOne); // + 900
        auto loop2 = loop_to(2000, addOne, addOne); // + 4000
        auto loop3 = loop_from_to(500, 1500) | addOne; // + 1000

        merge_and_run(loop1, loop2, loop3);

        CHECK(sum == 5900);
    }

    SECTION("offset of 1")
    {
        constexpr std::size_t size = 1'000;
        std::vector<int> a(size, 0);
        std::vector<int> b(size, 0);

        // Init array a [0, size)
        auto fct1 = [&](size_t i) { a[i] = i; };
        // Set array b [0, size) based on array a with offset 1
        auto fct2 = [&](size_t i) { b[i - 1] = a[i - 1] + a[i]; };

        (loop_to(size, fct1) | loop(range { 1, size }, fct2)).run();

        CHECK(a[3] == 3);
        CHECK(b[3] == 7);
    }

    SECTION("random ranges")
    {
        std::size_t sum = 0;
        auto addOne = [&sum](std::size_t /*unused*/) { sum += 1; };

        const auto r1 = GENERATE(take(50, random_range(0, 1000)));
        const auto r2 = GENERATE(take(50, random_range(0, 1000)));

        auto l1 = loop(r1, addOne);
        auto l2 = loop(r2, addOne);
        (l1 | l2).run();

        CHECK(sum == (r1.span() + r2.span()));
    }

    SECTION("integer ranges")
    {
        int sum = 0;
        auto add = [&sum](int i) { sum += (i < 0) ? -1 : 1; };

        auto l1 = loop(common::basic_range<int> { -10, 20 }, add);
        auto l2 = loop(common::basic_range<int> { -20, 30 }, add);
        (l1 | l2).run();

        CHECK(sum == 20);
    }

    SECTION("merger: union merging")
    {
        size_t sum = 0;
        auto add = [&sum](size_t /*unused*/) { ++sum; };

        auto l1 = loop_to(10, add);
        auto l2 = loop_to(20, add);
        ((l1 | l2) | (l1 | l2)).run();

        CHECK(sum == 60);
    }
}
