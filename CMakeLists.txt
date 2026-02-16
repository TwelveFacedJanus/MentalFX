cmake_minimum_required(VERSION 3.9)
project(MentalFX VERSION 1.0.0 LANGUAGES C CXX OBJC)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/WX)
    else()
        add_compile_options(-Werror)
    endif()
endif()


find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX
)


set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX/mental_fx.cc
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX/mental_fx.h
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME ${PROJECT_NAME}
    DEBUG_POSTFIX "_d"
    CXX_STANDARD 23
    C_STANDARD 11
)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)


if (CPACK_ENABLE)

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_VENDOR "Your Company")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MentalFX Application")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    # Package generators
    set(CPACK_GENERATOR "TGZ;ZIP")
    if(WIN32)
        list(APPEND CPACK_GENERATOR "NSIS")
    elseif(APPLE)
        list(APPEND CPACK_GENERATOR "Bundle")
    elseif(UNIX)
        list(APPEND CPACK_GENERATOR "DEB;RPM")
    endif()

    include(CPack)

    # Create version header file
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mentalfx/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mentalfx/version.h
        @ONLY
    )
endif()


message(STATUS "========================================")
message(STATUS "MentalFX Build Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")
