cmake_minimum_required(VERSION 3.24)
project(Closs_InT)

set(CMAKE_CXX_STANDARD 14)
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++")
    message(STATUS "Linux is NOT well-tested")
endif ()

# SDL path config
set(SDL2_PATH C:/Include/SDL2-mingw/SDL2-2.26.3/x86_64-w64-mingw32)
set(SDL2_INCLUDE ${SDL2_PATH}/include/SDL2)
set(SDL2_LIB ${SDL2_PATH}/lib)

set(SDL2_IMAGE_PATH C:/Include/SDL2-mingw/SDL2_image-2.6.3/x86_64-w64-mingw32)
set(SDL2_IMAGE_INCLUDE ${SDL2_IMAGE_PATH}/include/SDL2)
set(SDL2_IMAGE_LIB ${SDL2_IMAGE_PATH}/lib)

set(SDL2_TTF_PATH C:/Include/SDL2-mingw/SDL2_ttf-2.20.1/x86_64-w64-mingw32)
set(SDL2_TTF_INCLUDE ${SDL2_TTF_PATH}/include/SDL2)
set(SDL2_TTF_LIB ${SDL2_TTF_PATH}/lib)

set(SDL2_MIXER_PATH C:/Include/SDL2-mingw/SDL2_mixer-2.6.3/x86_64-w64-mingw32)
set(SDL2_MIXER_INCLUDE ${SDL2_MIXER_PATH}/include/SDL2)
set(SDL2_MIXER_LIB ${SDL2_MIXER_PATH}/lib)

# nlohmann json path config
set(nlohmann_INCLUDE C:/Include/nlohmann)

include_directories(
        ${SDL2_INCLUDE}
        ${SDL2_IMAGE_INCLUDE}
        ${SDL2_TTF_INCLUDE}
        ${SDL2_MIXER_INCLUDE}
        ${nlohmann_INCLUDE}
)

link_directories(
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
        ${SDL2_MIXER_LIB}
)

add_executable(Closs_InT
        main.cpp
        src/display.cpp src/display.h src/const.cpp src/const.h src/closs.cpp src/closs.h src/loader.cpp src/loader.h src/img.cpp src/img.h src/page.cpp src/page.h src/mixer.cpp src/mixer.h)

target_link_libraries(Closs_InT
        SDL2
        SDL2main
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        )