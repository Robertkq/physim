#include "common.h"
#include "physim.h"

int main(int argc, char** argv)
{
    std::cout << "Sunt " << argc << " argumente date!\n";
    for(int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << '\n';
    }
    kq::physim simulator;

    simulator.run();

    return 0;
}