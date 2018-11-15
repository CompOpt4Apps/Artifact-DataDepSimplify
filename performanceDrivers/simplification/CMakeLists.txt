cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -m64 -DVERIFY -DMETIS")


if(APPLE)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp")
endif()
if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()
#comment following line if you have defined MKLROOT variable otherwise set it to right address
set(MKLROOT "/opt/intel/mkl/")
#set(MKLROOT "/home/kazem/programs/intel/compilers_and_libraries_2017.4.196/linux/mkl/")
message(STATUS "${MKLROOT}")
set(MKL_INC "${MKLROOT}/include")
set(MKL_LIB "${MKLROOT}/lib/intel64")


#set(METISROOT "/home/kazem/programs/metis-5.1.0/")
#set(METIS_INC "${METISROOT}/include")
#set(METIS_LIB "${METISROOT}/build/Linux-x86_64/libmetis")
#set(SUITEROT "/home/kazem/programs/SuiteSparse/")
#set(SUITE_INC "${SUITEROOT}/include")
#set(SUITE_LIB "${SUITEROOT}/lib/intel64")

#set(METISROOT "/home/kazem/programs/metis-5.1.0/")
#set(METIS_INC "${METISROOT}/include")
#set(METIS_LIB "${METISROOT}/build/Linux-x86_64/libmetis")

if (APPLE) # OpenMP support in Mac
    include_directories(/usr/local/opt/libomp/include  /usr/local/opt/libomp/lib )
    link_directories(/usr/local/opt/libomp/lib/)
endif ()
project(simplification)
message(" ===== ${MKL_INC}")
include_directories(${MKL_INC})
link_directories(${MKL_LIB} ${MMM})
set(SOURCE_FILES test/simplification_ichol.cpp src/iChol.h src/levelSet.h src/util.h)
add_executable(simplification ${SOURCE_FILES})
target_link_libraries(simplification
        mkl_intel_lp64
        mkl_core
        pthread
        mkl_intel_thread
        iomp5
        m
        dl
        )



project(triangular)

set(SOURCE_FILES  test/triangular.cpp )
add_executable(triangular ${SOURCE_FILES})
target_link_libraries(triangular

        gomp
        m
        dl
        )


project(triangular_wavefront)

set(SOURCE_FILES  test/fs.cpp )
add_executable(triangular_wavefront ${SOURCE_FILES})
target_link_libraries(triangular_wavefront

        gomp
        m
        dl
        )


project(triangular_HLevel)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/triangular_ordered.cpp)
add_executable(triangular_HLevel ${SOURCE_FILES})
target_link_libraries(triangular_HLevel

        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread

        m
        dl
        metis
        )


project(ichol_wavefront)

set(SOURCE_FILES  test/ic0.cpp)
add_executable(ichol_wavefront ${SOURCE_FILES})
target_link_libraries(ichol_wavefront

        gomp
        m
        dl
        )


project(ichol_HLevel)

include_directories(${MKL_INC} ${METIS_INC} src/)
link_directories(${MKL_LIB} ${MMM}  ${METIS_LIB})
set(SOURCE_FILES test/ic0_ordered.cpp)
add_executable(ichol_HLevel ${SOURCE_FILES})
target_link_libraries(ichol_HLevel

        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread

        m
        dl
        metis
        )





project(fs_csc_performance)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/fs_csc_performance.cpp)
add_executable(fs_csc_performance ${SOURCE_FILES})
target_link_libraries(fs_csc_performance
        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )





project(ic0_csc_performance)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/ic0_csc_performance.cpp)
add_executable(ic0_csc_performance ${SOURCE_FILES})
target_link_libraries(ic0_csc_performance
        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )


project(serialcholeskyTest01)

set(SOURCE_FILES test/serialCholeskyTest01.cpp)

include_directories(${MKL_INC}  ${METIS_INC}
        "../cholesky/" "../common/")
link_directories(${MKL_LIB} ${MKL_LIB2} ${METIS_LIB})
add_executable(serialcholeskyTest01 ${SOURCE_FILES} test/serialCholeskyTest01.cpp)
target_link_libraries(serialcholeskyTest01
        #gomp
        iomp5
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )


project(simpCholeskyWF)

set(SOURCE_FILES test/simplCholeskyWavefront.cpp)

include_directories(${MKL_INC} ${METIS_INC}
        "../cholesky/" "../common/")
link_directories(${MKL_LIB} ${MKL_LIB2} ${METIS_LIB})
add_executable(simpCholeskyWF ${SOURCE_FILES} test/simplCholeskyWavefront.cpp)
target_link_libraries(simpCholeskyWF
        #gomp
        iomp5
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )


project(simpCholeskyHLevel)

set(SOURCE_FILES test/HLevel_simplicial_cholesky.cpp)

include_directories(${MKL_INC}  ${METIS_INC}
        "../cholesky/" "../common/")
link_directories(${MKL_LIB} ${MKL_LIB2} ${SUITE_LIB} ${METIS_LIB})
add_executable(simpCholeskyHLevel ${SOURCE_FILES} test/HLevel_simplicial_cholesky.cpp)
target_link_libraries(simpCholeskyHLevel
        #gomp
        iomp5
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )


#[[project(fs_csc_performance101)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/fs_csc_performance101.cpp)
add_executable(fs_csc_performance101 ${SOURCE_FILES})
target_link_libraries(fs_csc_performance101
        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )]]



project(fs_csr_performance)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/fs_csr_performance.cpp)
add_executable(fs_csr_performance ${SOURCE_FILES})
target_link_libraries(fs_csr_performance
        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )


project(gs_csr_performance)

include_directories(${MKL_INC} src/)
link_directories(${MKL_LIB} ${MMM} )
set(SOURCE_FILES test/gs_csr_performance.cpp)
add_executable(gs_csr_performance ${SOURCE_FILES})
target_link_libraries(gs_csr_performance
        iomp5
        #gomp
        #mkl_sequential
        mkl_intel_lp64
        mkl_core
        #mkl_gnu_thread
        mkl_intel_thread
        pthread
        m
        dl
        metis
        )



