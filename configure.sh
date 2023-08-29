#! /bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DGLFW_BUILD_DOCS=OFF \
    -S . \
    -B out/build
