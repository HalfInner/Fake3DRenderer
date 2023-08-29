//
// Created by kajbr on 23.02.2020.
//
#include "Fake3DEngine.hh"
#include "PlatformUtils.hh"

int main(int argc, char *argv[]) {
    platform::utils::initialize(argc, argv);
    BasicFake3DEngine sw;
    sw.initialize();
    sw.run();
}
