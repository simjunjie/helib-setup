include(ExternalProject)
include(ProcessorCount)
ProcessorCount(CORES_COUNT)

# Download and Install Google Test
## Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

## Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
				${CMAKE_CURRENT_BINARY_DIR}/googletest-build
				EXCLUDE_FROM_ALL)

## The gtest/gtest_main targets carry header search path dependencies automatically when using CMake 2.8.11 or later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Download and Install GMP 6.2.1
ExternalProject_Add(
	build-gmp
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	URL https://ftp.gnu.org/gnu/gmp/gmp-6.2.1.tar.lz
	CONFIGURE_COMMAND sh -c "./configure --prefix=<INSTALL_DIR>"
	BUILD_IN_SOURCE 1
)

ExternalProject_Get_Property(build-gmp INSTALL_DIR)
set(GMP_ROOT_DIR "${INSTALL_DIR}" CACHE PATH "GMP Root Directory")
set(GMP_INCLUDE_DIRS "${INSTALL_DIR}/include" CACHE PATH "GMP Include Directory")
set(GMP_LIBRARIES "${INSTALL_DIR}/lib/libgmp.so" CACHE PATH "GMP Library Path")

# Download and Install NTL 11.4.3
ExternalProject_Add(
	build-ntl
	DEPENDS build-gmp
	URL https://libntl.org/ntl-11.4.3.tar.gz
	CONFIGURE_COMMAND sh -c "cd src && ./configure SHARED=on NTL_GMP_LIP=on GMP_PREFIX=${INSTALL_DIR} NTL_THREADS=on NTL_THREAD_BOOST=on NTL_ENABLE_AVX_FFT=on PREFIX=<INSTALL_DIR>"
	BUILD_COMMAND cd src && make -j${CORES_COUNT}
	INSTALL_COMMAND cd src && make install
	BUILD_IN_SOURCE 1
)

ExternalProject_Get_Property(build-ntl INSTALL_DIR)
set(NTL_ROOT_DIR "${INSTALL_DIR}" CACHE PATH "NTL Root Directory")
set(NTL_INCLUDE_DIRS "${INSTALL_DIR}/include" CACHE PATH "NTL Include Directory")
set(NTL_LIBRARIES "${INSTALL_DIR}/lib/libntl.so" CACHE PATH "NTL Library Path")

# Download and Install HElib 2.0
ExternalProject_Add(
	build-HElib
	DEPENDS build-ntl
	GIT_REPOSITORY https://github.com/simjunjie/HElib.git
	CMAKE_ARGS -DNTL_DIR=${NTL_ROOT_DIR} -DGMP_DIR=${GMP_ROOT_DIR} -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED=ON -DENABLE_THREADS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DPEDANTIC_BUILD=OFF
)

# # Build HElib from local repo
# ExternalProject_Add(
#   build-HElib
#   DEPENDS build-ntl
#   URL ${CMAKE_CURRENT_SOURCE_DIR}/HElib_mod.tar
#   CMAKE_ARGS -DNTL_DIR=${NTL_ROOT_DIR} -DGMP_DIR=${GMP_ROOT_DIR} -DCMAKE_BUILD_jTYPE=RelWithDebInfo -DBUILD_SHARED=ON -DENABLE_THREADS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DPEDANTIC_BUILD=OFF
# )

ExternalProject_Get_Property(build-HElib INSTALL_DIR)
set(HELIB_ROOT_DIR "${INSTALL_DIR}" CACHE PATH "HElib Root Directory")
set(HELIB_INCLUDE_DIRS "${INSTALL_DIR}/include" CACHE PATH "HElib Include Directory")
set(HELIB_LIBRARIES "${INSTALL_DIR}/lib/libhelib.so" CACHE PATH "HElib Library Path")