#include <loop_fusion/loop_fusion.hpp>

#include <vector>

int main(int /*unused*/, char** /*unused*/)
{
    using namespace loop_fusion::main_range;

    auto vec = std::vector<int> {};
    std::size_t sum = 0;

    auto fill = [&](std::size_t) { vec.push_back(0); };
    auto add = [&](std::size_t i) { vec.at(i) += 10; };
    auto sumvec = [&](std::size_t i) { sum += vec.at(i); };

    (loop_from_to(0, 1000) | fill | add | loop_to(0, sumvec) | loop(range { 0, 11 }, sumvec)).run();

    return 0;
}
