cmake_minimum_required(VERSION 3.24)
project(Closs_InT)

set(CMAKE_CXX_STANDARD 14)

# SDL path config
set(SDL2_PATH C:/Include/SDL2-mingw/SDL2-2.26.2/x86_64-w64-mingw32)
set(SDL2_INCLUDE ${SDL2_PATH}/include/SDL2)
set(SDL2_LIB ${SDL2_PATH}/lib)

set(SDL2_IMAGE_PATH C:/Include/SDL2-mingw/SDL2_image-2.6.2/x86_64-w64-mingw32)
set(SDL2_IMAGE_INCLUDE ${SDL2_IMAGE_PATH}/include/SDL2)
set(SDL2_IMAGE_LIB ${SDL2_IMAGE_PATH}/lib)

set(SDL2_TTF_PATH C:/Include/SDL2-mingw/SDL2_ttf-2.20.1/x86_64-w64-mingw32)
set(SDL2_TTF_INCLUDE ${SDL2_TTF_PATH}/include/SDL2)
set(SDL2_TTF_LIB ${SDL2_TTF_PATH}/lib)

set(nlohmann_INCLUDE C:/Include/nlohmann)

include_directories(
        ${SDL2_INCLUDE}
        ${SDL2_IMAGE_INCLUDE}
        ${SDL2_TTF_INCLUDE}
        ${nlohmann_INCLUDE}
)

link_directories(
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
)

add_executable(Closs_InT
        main.cpp
        src/display.cpp src/display.h src/const.cpp src/const.h src/closs.cpp src/closs.h src/loader.cpp src/loader.h src/img.cpp src/img.h)

target_link_libraries(Closs_InT
        SDL2
        SDL2main
        SDL2_image
        SDL2_ttf
        )