# MSPM0L1306-CMAKE-GCC-Template
这是一个MSPM0L1306的gcc cmake工程模板



# 根据本工程创建项目
1. 准备所需的文件
新建一个文件夹，例如：C:\MSPM0_workspace\my_project。

在该目录下创建子文件夹，最终文件夹结构如下所示：
```
my_project
├── Core  // 单片机核心文件
│   ├── main.c // 主函数入口
│   ├── ti_msp_dl_config.c // sysconfig生成的配置文件源文件
│   ├── ti_msp_dl_config.h // sysconfig生成的配置文件头文件
│   └── Project.syscfg // sysconfig生成的配置文件
├── Driver // 单片机驱动文件
│   ├── CMSIS // Cortex-M系列CPU驱动库
│   └── ti // 芯片相关驱动库
├── CMakeLists.txt // CMake配置文件
├── README.md // 说明文档
└── MSPM0L1306.lds // 链接脚本
```
* 1.1 MSPM0硬件驱动库
复制<SDK_PATH>/source/ti/driverlib文件夹到项目文件夹\Driver\ti。删除my_project\Driver\ti\driverlib\lib这个文件夹。

* 1.2 MSPM0设备标识文件
复制<SDK_PATH>/source/ti/devices文件夹到my_project\Driver\ti,  
复制my_project\Driver\ti\devices\msp\m0p\linker_files\gcc\里的MSPM0L1306.lds到my_project下，其他型号同理    

删除以下文件夹:  
my_project\Driver\ti\devices\msp\m0p\linker_files  
my_project\Driver\ti\devices\msp\m0p\startup_system_files  

* 1.3 CMSIS
复制<SDK_PATH>/source/third_party/CMSIS/Core/到my_project\Driver\CMSIS\

* 1.4 主函数入口
复制<SDK_PATH>/examples/nortos/LP_MSPM0L1306/core/src/main.c到my_project\Core\src\main.c

* 1.5 sysconfig生成的配置文件
复制<SDK_PATH>/examples/nortos/LP_MSPM0L1306/core/src/ti_msp_dl_config.c到my_project\Core\src\ti_msp_dl_config.c


## 2. 配置工程
打开CMakeLists.txt，修改如下内容：
```
# 设置项目名称和使用的语言（C和ASM）
project("my_project" C ASM)
# 设置构建类型为Debug
set (CMAKE_BUILD_TYPE         Debug)

# 设置编译器定义
set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	__MSPM0L1306__
)
# 添加所需源文件
file (GLOB PROJECT_SOURCES
    Core/*.c
    Driver/ti/driverlib/*.c
    Driver/ti/driverlib/m0p/*.c
    Driver/ti/driverlib/m0p/sysctl/*.c
)

# 设置包含目录 
set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
    Core
    Driver/CMSIS/Core/Include
    Driver
)


# 设置链接脚本
## 注：这个路径是相对于CMakeLists.txt配置生成的build目录的路径，如果是相对于工程根目录，则为"../MSPM0L1306.lds"
set (LINKER_SCRIPT            "../MSPM0L1306.lds")

```

## 3. 生成工程
在命令行中进入工程目录，执行如下命令：
```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE  -SMSPM0L1306-CMAKE-GCC-Template -BMSPM0L1306-CMAKE-GCC-Template/build -G Ninja
```


## 4. 编译工程
```
cmake --build build --config Debug --target all
```


## 5. 烧写程序
将编译生成的my_project.elf烧写到单片机中运行。
使用pyocd
```
cd build
pyocd erase -t mspm0l1306
pyocd flash -t mspm0l1306 my_project.hex
```