# CMake最低版本 
cmake_minimum_required(VERSION 3.20)
# 项目名称
set(PROJECT_NAME "empty")
# 链接脚本路径  位于SDK目录下source\ti\devices\msp\m0p\linker_files\gcc\MSPM0L1306.lds
#   因为vscode默认配置是在build/下，所以这里设置成了../
set (LINKER_SCRIPT            "../MSPM0L1306.lds")
# 构建类型为Debug  
set (CMAKE_BUILD_TYPE         Debug)

# 使用GLOB命令收集项目源文件（注意：GLOB通常不推荐用于生产级项目） 
## 注意：GLOB命令会匹配所有符合条件的文件，因此可能匹配到不需要的文件，需要手动排除
#### Core/src/main.c 主函数入口
#### Core/src/ti_msp_dl_config.c sysconfig生成的配置文件
file (GLOB PROJECT_SOURCES
    Core/*.c
    Driver/ti/driverlib/*.c
    Driver/ti/driverlib/m0p/*.c
    Driver/ti/driverlib/m0p/sysctl/*.c
)
 
# 设置编译器定义 
set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	__MSPM0L1306__
)

# 设置包含目录 
set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
    Core
    Driver/CMSIS/Core/Include
    Driver
)



# 编译工具链设置 
## 使用arm-none-eabi-gcc工具链  
## 从path环境变量中获取工具链路径
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)
set(TOOLCHAIN_PREFIX                arm-none-eabi-)
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER}) 
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)




#项目配置
# 定义项目名称和使用的语言（C和ASM）
#   注意配置工程名要在配置工具链之前，否则会使用默认的cl工具链，导致编译失败。
project(${PROJECT_NAME} C ASM)
# 编译选项设置
## 添加可执行文件并设置源文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
## 添加构建后自定义命令来显示可执行文件大小  
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
## 添加编译器定义
add_compile_definitions (${PROJECT_DEFINES})
## 添加包含目录 
include_directories (${PROJECT_INCLUDES})
## 设置可执行文件和静态库的后缀
set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")

# 编译选项设置
## 根据构建类型设置C编译选项 
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
set (CMAKE_C_FLAGS "-mcpu=cortex-m0plus -std=gnu11 -mfloat-abi=soft --specs=nano.specs -mthumb -Wall -Og -g3 -ggdb -ffunction-sections -fdata-sections -fstack-usage -gstrict-dwarf")
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
set (CMAKE_C_FLAGS "-mcpu=cortex-m0plus -std=gnu11 -mfloat-abi=soft --specs=nano.specs -mthumb -Wall -Ofast -ffunction-sections -fdata-sections -fstack-usage -gstrict-dwarf")
endif()
## 链接选项设置
set (CMAKE_EXE_LINKER_FLAGS "-nostartfiles -T${LINKER_SCRIPT} --specs=nosys.specs -mthumb -static -lgcc -Wl,-Map=${PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -Wl,--print-memory-usage")
## 编译汇编文件
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")



add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}>
          ${PROJECT_NAME}.hex)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}>
          ${PROJECT_NAME}.bin)


##参考文献 
###C:\ti\mspm0_sdk_2_01_00_03\examples\nortos\LP_MSPM0L1306\driverlib\empty\gcc\makefile
###STM32CubeMX生成的CMakeLists.txt