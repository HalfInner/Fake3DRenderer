@echo off

cmake ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
    -DGLFW_BUILD_DOCS=OFF ^
    -S . ^
    -B out/build_win
