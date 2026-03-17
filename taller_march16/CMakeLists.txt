cmake_minimum_required(VERSION 3.10)
project(taller_filtros)

set(CMAKE_CXX_STANDARD 14)

# Solo los modulos que realmente usamos: core, highgui (ventanas), videoio (camara), imgproc (normalize/threshold)
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgproc)

include_directories(${OpenCV_INCLUDE_DIRS} include)

set(SOURCES
    src/main.cpp
    src/log_filter.cpp
    src/zero_crossing.cpp
    src/scharr.cpp
    src/laplacian.cpp
    src/sobel.cpp
    src/sobel_magnitudes.cpp
    src/canny.cpp
)

add_executable(taller_filtros ${SOURCES})

target_link_libraries(taller_filtros ${OpenCV_LIBS})
