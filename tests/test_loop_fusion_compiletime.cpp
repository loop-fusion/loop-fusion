#include "test_helpers.hpp"

#include "loop_fusion/compiletime/loop_fusion.hpp"

#include <array>
#include <numeric>
#include <vector>

using namespace loop_fusion::compiletime;

TEST_CASE("types")
{
    using seq_4_5 = std::integer_sequence<std::size_t, 4, 5>;
    using seq_6_7 = std::integer_sequence<std::size_t, 6, 7>;
    using seq_4_5_6_7 = std::integer_sequence<std::size_t, 4, 5, 6, 7>;

    static_assert(std::is_same_v<seq_4_5_6_7, types::index_sequence_from_to<4, 8>>);
    static_assert(std::is_same_v<seq_4_5_6_7, types::index_sequence_cat<seq_4_5, seq_6_7>>);

    using nested = std::tuple<int, std::tuple<int, char>, char>;
    using flattened = std::tuple<int, int, char, char>;
    static_assert(std::is_same_v<std::tuple<char>, types::as_tuple_t<char>>);
    static_assert(std::is_same_v<std::tuple<char>, types::as_tuple_t<std::tuple<char>>>);
    static_assert(std::is_same_v<flattened, types::flatten_tuple_t<nested>>);
    CHECK(true);
}

TEST_CASE("basic_loop with different type", "[basic]")
{
    std::vector<int> vec;
    auto fill = [&vec](int i) { vec.push_back(i); };
    auto l = basic_looper<int, -100, 101, decltype(fill)>(std::make_tuple(fill));
    l.run();
    REQUIRE(vec.size() == 201);
    CHECK(vec.at(0) == -100);
    CHECK(vec.at(100) == 0);
    CHECK(vec.at(200) == 100);
}

TEST_CASE("Function Sequence", "[basic]")
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

    auto merged = loop<0, vecsize>(set_one) | loop<0, vecsize>(set_two) //
        | loop<0, vecsize>(set_three) | loop<0, 2>(set_four);
    merged.run();

    const int sum = std::accumulate(vec.cbegin(), vec.cend(), 0);
    CHECK(sum == vec.size() * 3 + 2);
}

TEST_CASE("loops are executed", "[basic]")
{
    std::vector<size_t> vec;
    auto fill = [&vec](size_t i) { vec.push_back(i); };
    auto l = loop_to<10>(fill) | fill; // 2x
    l.run();
    CHECK(vec.size() == 20);
}

TEST_CASE("loop union", "[union]")
{
    // Example from MergingLoops.md
    std::array<std::vector<size_t>, 5> vec;
    const auto fill_0 = [&vec](size_t /*unused*/) { vec[0].push_back(0); };
    const auto fill_1 = [&vec](size_t /*unused*/) { vec[1].push_back(1); };
    const auto fill_2 = [&vec](size_t /*unused*/) { vec[2].push_back(2); };
    const auto fill_3 = [&vec](size_t /*unused*/) { vec[3].push_back(3); };
    const auto fill_4 = [&vec](size_t /*unused*/) { vec[4].push_back(4); };

    // Note that the second integer is exclusive.
    using loop_fill_0 = basic_looper_union_range<std::size_t, 0, 2, std::index_sequence<1, 2, 4>>;
    using loop_fill_1 = basic_looper_union_range<std::size_t, 2, 100, std::index_sequence<1, 2, 3, 4>>;
    using loop_fill_2 = basic_looper_union_range<std::size_t, 100, 201, std::index_sequence<0, 1, 2, 3, 4>>;
    using loop_fill_3 = basic_looper_union_range<std::size_t, 201, 202, std::index_sequence<1, 2, 4>>;
    using loop_fill_4 = basic_looper_union_range<std::size_t, 202, 206, std::index_sequence<1, 4>>;

    using loop_tuple = std::tuple<loop_fill_0, loop_fill_1, loop_fill_2, loop_fill_3, loop_fill_4>;
    using function_tuple
        = std::tuple<decltype(fill_0), decltype(fill_1), decltype(fill_2), decltype(fill_3), decltype(fill_4)>;
    // pair for required types
    using pair = std::pair<loop_tuple, function_tuple>;
    const auto loops = std::make_tuple(fill_0, fill_1, fill_2, fill_3, fill_4);
    basic_looper_union<std::size_t, pair> union_loop { loops };

    union_loop.run();

    CHECK(vec[0].size() == 101);
    CHECK(vec[1].size() == 206);
    CHECK(vec[2].size() == 202);
    CHECK(vec[3].size() == 199);
    CHECK(vec[4].size() == 206);
}

TEST_CASE("loop merge", "[merge]")
{
    // Example from MergingLoops.md
    std::array<std::vector<size_t>, 4> vec;
    const auto fill_0 = [&vec](size_t /*unused*/) { vec[0].push_back(0); };
    const auto fill_1 = [&vec](size_t /*unused*/) { vec[1].push_back(1); };
    const auto fill_2 = [&vec](size_t /*unused*/) { vec[2].push_back(2); };
    const auto fill_3 = [&vec](size_t /*unused*/) { vec[3].push_back(3); };

    auto merged = loop<10, 20>(fill_0) | loop<2, 20>(fill_1);
    auto merged_2 = merged | loop<30, 60>(fill_2);
    auto merged_3 = merged_2 | loop<10, 40>(fill_3);
    merged_3.run();

    // For Debugging purposes:
    // typedef decltype(merged_3)::something_made_up X;

    CHECK(vec[0].size() == 10);
    CHECK(vec[1].size() == 18);
    CHECK(vec[2].size() == 30);
    CHECK(vec[3].size() == 30);
}
