#include "test_helpers.hpp"

#include "loop_fusion/runtime/loop_fusion.hpp"

#include <array>
#include <numeric>

using namespace loop_fusion::runtime;

TEST_CASE("runtime union: no duplicate functions")
{
    std::array<int, 41> a {};
    std::vector<int> b;

    auto fill = [&a](std::size_t i) { a[i] = static_cast<int>(i); };
    auto loop1 = loop_to(a.size(), fill);
    auto fibonacci = [&a](std::size_t i) { a[i] = a[i - 2] + a[i - 1]; };
    auto loop2 = loop_from_to(2, a.size(), fibonacci);
    auto addOne = [&a, &b](std::size_t i) { b.push_back(a[i] + 1); };
    auto loop3 = loop(range { a.size() - 16, a.size() }) | addOne;

    (loop1 | loop2 | loop3).run();

    CHECK(a[0] == 0);
    CHECK(a[1] == 1);
    CHECK(a[2] == 1);
    CHECK(a[20] == 6'765);
    CHECK(a[40] == 102'334'155);
    REQUIRE(b.size() == 16);
    CHECK(b[0] == 75'025 + 1);
    CHECK(b[15] == 102'334'155 + 1);
}

TEST_CASE("Function Sequence runtime", "[basic]")
{
    std::vector<int> vec { 0, 0, 0, 0, 0 };
    const int vecsize { 5 };
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

TEST_CASE("runtime union: duplicate functions")
{
    std::size_t sum = 0;

    auto addOne = [&sum](std::size_t /*unused*/) { sum += 1; };

    auto loop1 = loop(range { 100, 1000 }, addOne); // + 900
    auto loop2 = loop_to(2000, addOne, addOne); // + 4000
    auto loop3 = loop_from_to(500, 1500) | addOne; // + 1000

    auto looper_union = (loop1 | loop2 | loop3);
    looper_union.run();

    CHECK(sum == 5900);
}

TEST_CASE("runtime merger: random ranges", "[merge]")
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

/**
 * A simple unit test for multiple unions merged together.
 * Unfortunately, the implementation is missing a final part, see
 * runtime/looper_union.hpp for more information.
 */
// TEST_CASE("multiple runtime unions merged together") {
//     std::size_t sum = 0;
//     auto op1 = [&sum](std::size_t /*unused*/) { sum += 1; };
//     auto op2 = [&sum](std::size_t /*unused*/) { sum += 2; };
//     auto op3 = [&sum](std::size_t /*unused*/) { sum += 3; };
//
//     auto l1 = loop({ 100, 1000 }, op1);
//     auto l2 = loop_to(2000, op2);
//     auto l3 = loop_from_to(500, 1500, op3);
//     auto u1 = l1 | l2;
//     auto u2 = l2 | l3;
//     (u1 | u2).run();
//
//     CHECK(sum == (900 + 4 * 2000 + 3 * 1000));
// }
