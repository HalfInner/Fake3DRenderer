#! /bin/bash

cmake \
    -DGLFW_BUILD_DOCS=OFF \
    -G 'MSYS Makefiles' \
    -S . \
    -B out/build