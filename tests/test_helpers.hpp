#pragma once

#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <Catch2/catch.hpp>

#include "loop_fusion/common/range.hpp"

#include <random>

// This class is a simple generator for Catch2 tests that can generate
// ranges for loop_fusion. It is based upon the example generator:
// https://github.com/catchorg/Catch2/blob/master/examples/300-Gen-OwnGenerator.cpp
class RandomRangeGenerator final : public Catch::Generators::IGenerator<loop_fusion::common::range> {
    std::minstd_rand m_rand;
    std::uniform_int_distribution<> m_dist;
    loop_fusion::common::range current_range;

public:
    RandomRangeGenerator(std::size_t low, std::size_t high)
        : m_rand(std::random_device {}())
        , m_dist(low, high)
    {
        static_cast<void>(next());
    }

    loop_fusion::common::range const& get() const override;
    bool next() override
    {
        using namespace loop_fusion::common;
        const std::size_t a = static_cast<std::size_t>(m_dist(m_rand));
        const std::size_t b = static_cast<std::size_t>(m_dist(m_rand));
        current_range = (a < b) ? range { a, b } : range { b, a };
        return true;
    }
};

// This helper function provides a nicer UX when instantiating the generator
// Notice that it returns an instance of GeneratorWrapper<loop_fusion::common::range>, which
// is a value-wrapper around std::unique_ptr<IGenerator<loop_fusion::common::range>>.
Catch::Generators::GeneratorWrapper<loop_fusion::common::range> random_range(std::size_t low, std::size_t high);
