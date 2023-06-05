cmake_minimum_required(VERSION 3.16)
project(helib-setup)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ffast-math")
set(CMAKE_BUILD_TYPE Release)

### OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

### Boost (for polynomial class and constant PI)
find_package(Boost)

### Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOXYFILE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs" CACHE PATH "Doxygen output directory")

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

    add_custom_command(TARGET doc_doxygen
        POST_BUILD
        COMMAND "${CMAKE_MAKE_PROGRAM}"
        COMMENT	"Running LaTeX for Doxygen documentation in ${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}..."
        WORKING_DIRECTORY "${DOXYFILE_OUTPUT_DIR}/latex")

else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
option(BUILD_DOC "Build documentation" ON)

add_subdirectory(3rdparty)

### GMP
include_directories(${GMP_INCLUDE_DIRS})
add_library(GMP SHARED IMPORTED GLOBAL)
set_target_properties(GMP PROPERTIES IMPORTED_LOCATION ${GMP_LIBRARIES})

### NTL
include_directories(${NTL_INCLUDE_DIRS})
add_library(NTL SHARED IMPORTED GLOBAL)
set_target_properties(NTL PROPERTIES IMPORTED_LOCATION ${NTL_LIBRARIES})

### HElib
include_directories(${HELIB_INCLUDE_DIRS})
add_library(HElib SHARED IMPORTED GLOBAL)
set_target_properties(HElib PROPERTIES IMPORTED_LOCATION ${HELIB_LIBRARIES})

# must be ordered as such
include_directories(include)
add_subdirectory (source)
add_subdirectory (tests)