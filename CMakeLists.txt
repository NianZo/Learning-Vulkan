cmake_minimum_required(VERSION 3.15)

set (Recipe_Name "3_0_DeviceHandshake")

# AUTO_LOCATE_VULKAN - accepted value ON or OFF
# ON - Use CMake to auto locate the Vulkan SDK.
# OFF - Vulkan SDK path can be specified manually
# This is helpful to test the build on various Vulkan versions.
option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN:=" ON)

if (AUTO_LOCATE_VULKAN)

    message(STATUS "Attempting to auto locate Vulkan using CMake...")

    # Find Vulkan Path using CMake's Vulkan Module
    # This will return boolean 'Vulkan_FOUND' indicating
    # the status of finding as success (TRUE) or fail (FALSE).
    # Include directory path - 'Vulkan_INCLUDE_DIRS'
    # and 'Vulkan_LIBRARY' with required libraries.
    find_package(Vulkan)

    # Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
    if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
        set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
        STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
    endif()

    if (NOT Vulkan_FOUND)
        # CMake may fail to locate the libraries but could be able to
        # provide some path in VulkanSDK include directory variable
        # 'Vulkan_INCLUDE_DIRS', try to extract path from this
        message(STATUS "Failed to locate VulkanSDK, retrying from 'Vulkan_INCLUDE_DIRS' variable)

        # Check if Vulkan path is valid, if not switch to manual mode
        if (EXISTS "${VULKAN_PATH})
            message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH})
        else()
            message(ERROR "Unable to locate VulkanSDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
            message(ERROR "and specify manual path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in CMakeLists.txt")
            return()
        endif()
    endif()
else()
    message(STATUS "Attempting to locate VulkanSDK using manual path...")
    set(VULKAN_SDK "C:/VulkanSDK")
    set(VULKAN_VERSION "1.0.33.0")
    set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION})
    message(STATUS "Using manual specified path: ${VULKAN_PATH}")

    # Check if manual set path exists
    if (NOT EXISTS "${VULKAN_PATH})
        message(ERROR "Unable to locate this VulkanSDK path: ${VULKAN_PATH}")
        return()
    endif()
endif()

# Specify project name
project(${Recipe_Name})

# Add preprocessor definitions here
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

# Add 'vulkan-1' library for built Vulkan application
set(VULKAN_LINK_LIST "vulkan-1")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Include Vulkan header files from VulkanSDK
    include_directories(AFTER ${VULKAN_PATH}/Include)

    # Link directory for vulkan-1
    link_directories(${VULKAN_PATH}/Bin)
endif()

# Bunch the header and source files together
if (WIN32)
    source_group("include" REGULAR_EXPRESSION "include/*")
    source_group("source" REGULAR_EXPRESSION "source/*")
endif()

# Define include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)

# Build project, provide name and include files to be compiled
add_executable(${Recipe_Name} ${CPP_FILES} ${HPP_FILES})

# Link the debug and release libraries to the project
target_link_libraries(${Recipe_Name} ${VULKAN_LIB_LIST})

# Define project properties
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

# Define C++ version to be used for building the project
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD_REQUIRED ON)

# Define C version to be used for building the project
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD 99)
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD_REQUIRED ON)

