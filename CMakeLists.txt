cmake_minimum_required(VERSION 3.9)
project(MentalFX VERSION 1.0.0 LANGUAGES C CXX OBJC)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

option(MENTAL_ENABLE_GLFW "Enable GLFW support" ON)
option(MENTAL_ENABLE_VULKAN "Enable Vulkan support" ON)
option(MENTAL_ENABLE_MOLTENVK "Enable MoltenVK on Apple platforms" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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


# Поиск GLFW в системе
if(MENTAL_ENABLE_GLFW)
    find_package(glfw3 3.3 QUIET)

    if(NOT glfw3_FOUND AND NOT TARGET glfw)
        message(STATUS "GLFW не найден в системе. Загружаем и собираем из исходников...")

        include(FetchContent)

        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.3.8  # Можно использовать последний стабильный тег
            GIT_SHALLOW TRUE
        )

        # Опции сборки GLFW
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)  # Не устанавливать в систему

        FetchContent_MakeAvailable(glfw)

        message(STATUS "GLFW успешно загружен и собран из исходников")
    else()
        message(STATUS "GLFW найден в системе")
    endif()
endif()


# Vulkan setup
if(MENTAL_ENABLE_VULKAN)
    # Try to find system Vulkan
    find_package(Vulkan QUIET)

    if(NOT Vulkan_FOUND)
        message(STATUS "Vulkan SDK not found in system. Attempting to locate...")

        # Common Vulkan SDK paths
        if(WIN32)
            set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}")
            if(EXISTS "${VULKAN_SDK_PATH}")
                message(STATUS "Found Vulkan SDK at: ${VULKAN_SDK_PATH}")
                set(Vulkan_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
                set(Vulkan_LIBRARY "${VULKAN_SDK_PATH}/Lib/vulkan-1.lib")
            endif()
        elseif(APPLE)
            # Check for Vulkan SDK or MoltenVK
            set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}")
            if(EXISTS "${VULKAN_SDK_PATH}")
                message(STATUS "Found Vulkan SDK at: ${VULKAN_SDK_PATH}")
                set(Vulkan_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
                set(Vulkan_LIBRARY "${VULKAN_SDK_PATH}/Lib/libvulkan.dylib")
            elseif(MENTAL_ENABLE_MOLTENVK)
                message(STATUS "Vulkan SDK not found. Will use MoltenVK for Vulkan support on macOS/iOS")

                # Check for MoltenVK via Homebrew or local installation
                find_path(MOLTENVK_INCLUDE_DIR
                    NAMES "MoltenVK/mvk_vulkan.h"
                    PATHS
                        "/usr/local/include"
                        "/opt/homebrew/include"
                        "$ENV{HOME}/homebrew/include"
                        "$ENV{VULKAN_SDK}/Include"
                )

                find_library(MOLTENVK_LIBRARY
                    NAMES MoltenVK
                    PATHS
                        "/usr/local/lib"
                        "/opt/homebrew/lib"
                        "$ENV{HOME}/homebrew/lib"
                        "$ENV{VULKAN_SDK}/Lib"
                )

                if(MOLTENVK_INCLUDE_DIR AND MOLTENVK_LIBRARY)
                    message(STATUS "Found MoltenVK: ${MOLTENVK_LIBRARY}")
                    set(Vulkan_INCLUDE_DIR ${MOLTENVK_INCLUDE_DIR})
                    set(Vulkan_LIBRARY ${MOLTENVK_LIBRARY})

                    # Add MoltenVK frameworks on macOS
                    if(APPLE)
                        find_library(COCOA_LIBRARY Cocoa)
                        find_library(QUARTZCORE_LIBRARY QuartzCore)
                        find_library(IOKIT_LIBRARY IOKit)
                        find_library(METAL_LIBRARY Metal)
                        find_library(UIKIT_LIBRARY UIKit)  # For iOS

                        set(Vulkan_EXTRA_LIBRARIES
                            ${COCOA_LIBRARY}
                            ${QUARTZCORE_LIBRARY}
                            ${IOKIT_LIBRARY}
                            ${METAL_LIBRARY}
                        )
                    endif()
                endif()
            endif()
        elseif(UNIX)
            # Linux paths
            find_path(Vulkan_INCLUDE_DIR "vulkan/vulkan.h"
                PATHS /usr/include /usr/local/include)
            find_library(Vulkan_LIBRARY "vulkan"
                PATHS /usr/lib /usr/lib64 /usr/local/lib)
        endif()

        if(Vulkan_INCLUDE_DIR AND Vulkan_LIBRARY)
            set(Vulkan_FOUND TRUE)
            message(STATUS "Vulkan found - Include: ${Vulkan_INCLUDE_DIR}, Library: ${Vulkan_LIBRARY}")
        else()
            message(WARNING "Vulkan not found. Some features may be disabled.")
        endif()
    endif()
endif()


find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX
)

set(MFW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFramework/mental_glfw_window.cc
)

set(MFW_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFramework/mental_framework.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFramework/mental_glfw_window.h


)

add_library(MentalFramework STATIC ${MFW_SOURCES} ${MFW_HEADERS})
target_include_directories(MentalFramework
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/MentalFramework
)
target_compile_features(MentalFramework PUBLIC cxx_std_23)
set_target_properties(MentalFramework PROPERTIES
    OUTPUT_NAME MentalFramework
    DEBUG_POSTFIX "_d"
)

# Подключаем GLFW к MentalFramework
if(MENTAL_ENABLE_GLFW AND TARGET glfw)
    target_link_libraries(MentalFramework PUBLIC glfw)
    target_include_directories(MentalFramework PRIVATE
        $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
    )
elseif(MENTAL_ENABLE_GLFW AND TARGET GLFW::GLFW)
    target_link_libraries(MentalFramework PUBLIC GLFW::GLFW)
endif()

# Подключаем Vulkan к MentalFramework
if(MENTAL_ENABLE_VULKAN AND Vulkan_FOUND)
    target_compile_definitions(MentalFramework PUBLIC MENTAL_ENABLE_VULKAN)

    if(APPLE AND MOLTENVK_LIBRARY)
        target_compile_definitions(MentalFramework PUBLIC MENTAL_ENABLE_MOLTENVK)
    endif()

    target_include_directories(MentalFramework PUBLIC ${Vulkan_INCLUDE_DIR})
    target_link_libraries(MentalFramework PUBLIC ${Vulkan_LIBRARY})

    if(Vulkan_EXTRA_LIBRARIES)
        target_link_libraries(MentalFramework PUBLIC ${Vulkan_EXTRA_LIBRARIES})
    endif()

    message(STATUS "Vulkan enabled for MentalFramework")
endif()


set(FX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX/mental_fx.cc
)

set(FX_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX/mental_fx.h
)

add_executable(${PROJECT_NAME} ${FX_SOURCES} ${FX_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MentalFramework
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MentalFX
        ${CMAKE_CURRENT_SOURCE_DIR}/MentalFramework  # Для заголовков
)

if(MENTAL_ENABLE_VULKAN AND Vulkan_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MENTAL_ENABLE_VULKAN)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME ${PROJECT_NAME}
    DEBUG_POSTFIX "_d"
    CXX_STANDARD 23
    C_STANDARD 11
)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND NOT WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Creating symlink to compile_commands.json in project root"
    )
endif()


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${MFW_HEADERS} DESTINATION include/MentalFramework)
install(FILES ${FX_HEADERS} DESTINATION include/MentalFX)


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
        # For macOS, include Vulkan and MoltenVK dependencies in package
        set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
        set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
        set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.icns")
    elseif(UNIX)
        list(APPEND CPACK_GENERATOR "DEB;RPM")
        if(Vulkan_FOUND)
            set(CPACK_DEBIAN_PACKAGE_DEPENDS "libvulkan1")
            set(CPACK_RPM_PACKAGE_REQUIRES "vulkan-loader")
        endif()
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
message(STATUS "")
message(STATUS "  GLFW: ${MENTAL_ENABLE_GLFW}")
if(MENTAL_ENABLE_GLFW AND TARGET glfw)
    get_target_property(GLFW_SOURCE_DIR glfw SOURCE_DIR)
    message(STATUS "    GLFW source: ${GLFW_SOURCE_DIR}")
endif()
message(STATUS "")
message(STATUS "  Vulkan: ${MENTAL_ENABLE_VULKAN}")
if(MENTAL_ENABLE_VULKAN AND Vulkan_FOUND)
    message(STATUS "    Vulkan include: ${Vulkan_INCLUDE_DIR}")
    message(STATUS "    Vulkan library: ${Vulkan_LIBRARY}")
    if(APPLE)
        if(MOLTENVK_LIBRARY)
            message(STATUS "    MoltenVK: Enabled (${MOLTENVK_LIBRARY})")
        else()
            message(STATUS "    MoltenVK: Not found - using system Vulkan loader")
        endif()
    endif()
elseif(MENTAL_ENABLE_VULKAN AND NOT Vulkan_FOUND)
    message(STATUS "    Vulkan: NOT FOUND (some features disabled)")
endif()
message(STATUS "========================================")
