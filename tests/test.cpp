#define CATCH_CONFIG_RUNNER
#include <Catch2/catch.hpp>

#include <vector>

int main(int argc, char** argv)
{
    Catch::Session session;
    const int res = session.run(argc, argv);
    return res;
}
