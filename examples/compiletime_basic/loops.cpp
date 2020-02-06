#include <loop_fusion/loop_fusion.hpp>

#include <vector>

int main(int /*unused*/, char** /*unused*/)
{
    using namespace loop_fusion::compiletime;

    auto vec = std::vector<int> {};
    std::size_t sum = 0;

    auto fill = [&](std::size_t) { vec.push_back(0); };
    auto add = [&](std::size_t i) { vec.at(i) += 10; };
    auto sumvec = [&](std::size_t i) { sum += vec.at(i); };

    auto l = loop<0, 1000>(fill) | add | loop<0, 10>(sumvec) | loop<0, 11>(sumvec);
    l.run();

    return 0;
}
