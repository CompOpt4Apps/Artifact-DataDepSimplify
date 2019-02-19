cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -m64 -DVERIFY -DMETIS")

set(METIS_INC "ENV{METISROOT}/include")
set(METIS_LIB "ENV{METISROOT}/lib")

if(APPLE)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp")
endif()
if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()


if (APPLE) # OpenMP support in Mac
    include_directories(/usr/local/opt/libomp/include  /usr/local/opt/libomp/lib )
    link_directories(/usr/local/opt/libomp/lib/)
endif ()





project(fs_csc_performance)
include_directories(src/ ${METIS_INC})
link_directories(${METIS_LIB})
set(SOURCE_FILES test/fs_csc_performance.cpp)
add_executable(fs_csc_performance ${SOURCE_FILES})
target_link_libraries(fs_csc_performance
        gomp
        pthread
        m
        dl
        metis
        )


project(ic0_csc_performance)
include_directories(src/ ${METIS_INC})
link_directories(${METIS_LIB})
set(SOURCE_FILES test/ic0_csc_performance.cpp)
add_executable(ic0_csc_performance ${SOURCE_FILES})
target_link_libraries(ic0_csc_performance
        gomp
        pthread
        m
        dl
        metis
        )


project(fs_csr_performance)
include_directories(src/ ${METIS_INC})
link_directories(${METIS_LIB})
set(SOURCE_FILES test/fs_csr_performance.cpp)
add_executable(fs_csr_performance ${SOURCE_FILES})
target_link_libraries(fs_csr_performance
        gomp
        pthread
        m
        dl
        metis
        )


project(gs_csr_performance)
include_directories(src/ ${METIS_INC})
link_directories(${METIS_LIB})
set(SOURCE_FILES test/gs_csr_performance.cpp)
add_executable(gs_csr_performance ${SOURCE_FILES})
target_link_libraries(gs_csr_performance
        gomp
        pthread
        m
        dl
        metis
        )


project(lchol_csc_performance)
include_directories(src/ ${METIS_INC})
link_directories(${METIS_LIB})
set(SOURCE_FILES test/lchol_csc_performance.cpp)
add_executable(lchol_csc_performance ${SOURCE_FILES})
target_link_libraries(lchol_csc_performance
        gomp
        pthread
        m
        dl
        metis
        )


