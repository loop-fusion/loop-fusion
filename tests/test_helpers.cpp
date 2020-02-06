#include "test_helpers.hpp"

loop_fusion::common::range const& RandomRangeGenerator::get() const
{
    return current_range;
}

Catch::Generators::GeneratorWrapper<loop_fusion::common::range> random_range(std::size_t low, std::size_t high)
{
    return Catch::Generators::GeneratorWrapper<loop_fusion::common::range>(
        std::unique_ptr<Catch::Generators::IGenerator<loop_fusion::common::range>>(
            new RandomRangeGenerator(low, high)));
}

TEST_CASE("Generate random ranges", "[range][generator]")
{
    auto r = GENERATE(take(50, random_range(100, 1000)));
    REQUIRE(r.start <= r.end);
    REQUIRE(r.start >= 100);
    REQUIRE(r.end <= 1000);
}
