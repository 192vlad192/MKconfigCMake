cmake_minimum_required(VERSION 3.15.3)

project(testCMake)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


#файлы программы--------------------------------------------
set(coreFiles 
    core/inc/main.h
    core/src/main.c)

set(driverFiles
    driver/CMSIS/inc/stm32f2xx.h
    driver/CMSIS/inc/stm32f207xx.h
    driver/CMSIS/inc/system_stm32f2xx.h
    driver/CMSIS/src/system_stm32f2xx.c
    driver/CMSIS/src/startup_stm32f207xx.s
    #HAL
    )
#-----------------------------------------------------------

#===========================================================
set(mcpu cortex-m3) #-mcpu параметр компиляции arm-none-eabi-gcc
set(targetDevice STM32F207xx)#устройство например - STM32F207xx
#===========================================================

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} 
    ${coreFiles}
    ${driverFiles})

target_compile_definitions(${EXECUTABLE} PRIVATE
    # -DUSE_HAL_DRIVER
    -D${targetDevice})

target_include_directories(${EXECUTABLE} PRIVATE
    core/inc
    core/src
    driver/CMSIS/core
    driver/CMSIS/inc
    driver/CMSIS/src)

target_compile_options(${EXECUTABLE} PRIVATE
    -mcpu=${mcpu}
    -mthumb
    # -mfpu=fpv4-sp-d16
    # -mfloat-abi=hard
    -fdata-sections
    -ffunction-sections
    -Wall
    $<$<CONFIG:Debug>:-Og>)

target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F207VGTx_FLASH.ld
    -mcpu=${mcpu}
    -mthumb
    # -mfpu=fpv4-sp-d16
    # -mfloat-abi=hard
    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections)

#создание hex и bin файлов 
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)

#вывод размерности elf и hex файлов
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf
    COMMAND arm-none-eabi-size ${PROJECT_NAME}.hex)