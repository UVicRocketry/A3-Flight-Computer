cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME A3-Flight-Computer-Software)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_compile_options(-u _printf_float)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    stm32-mw-fatfs/source
    stm32-mw-fatfs/source/drivers/sd
    stm32-mw-fatfs/source/drivers/template
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    stm32-mw-fatfs/source/drivers/sd/sd_diskio_dma_rtos.c
    stm32-mw-fatfs/source/diskio.c
    stm32-mw-fatfs/source/ff.c
    stm32-mw-fatfs/source/ff_gen_drv.c
    stm32-mw-fatfs/source/ffsystem_cmsis_os.c
    Core/Src/file_system_handler.c
    Core/Src/sensors.c
    Core/Src/ftoa.c
    Core/Src/camera_driver.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths

    stm32-mw-fatfs/source
    stm32-mw-fatfs/source/drivers/sd
    stm32-mw-fatfs/source/drivers/template
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
