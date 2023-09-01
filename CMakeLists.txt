# 指定CMake最低版本
cmake_minimum_required(VERSION 3.0)

# 设置变量，给文件名/路径名或其他字符串起别名，用${变量}获取变量内容
# set(变量 文件名/路径/...)
# set(Source_one src/main.cpp)
# ${PROJECT_NAME}：本CMakeLists.txt的project名称
set(PROJECT_NAME OpenGl_DEMO)

# project命令用于指定cmake工程的名称，它还可以指定cmake工程的版本号（VERSION关键字）、简短的描述（DESCRIPTION关键字）、主页URL（HOMEPAGE_URL关键字）和编译工程使用的语言（LANGUAGES关键字）
# project(<PROJECT-NAME> [<language-name>...])
# project(<PROJECT-NAME> [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]] [DESCRIPTION <project-description-string>][HOMEPAGE_URL <url-string>] [LANGUAGES <language-name>...])
# project(mytest VERSION 1.2.3.4)
# project(mytest HOMEPAGE_URL “https://www.XXX(示例).com”)
project(${PROJECT_NAME})

# 给编译器添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/3rdParty/include
                    ${PROJECT_SOURCE_DIR}/include)

# ${PROJECT_SOURCE_DIR} 、 CMAKE_CURRENT_SOURCE_DIR 和 <PROJECT-NAME>_SOURCE_DIR：本CMakeLists.txt所在的文件夹路径
# GLOB SOURCES 获取目录下的所有cpp文件（不包括子目录），并赋值给SOURCES
# file(
#     GLOB SOURCES
#     ${PROJECT_SOURCE_DIR}/src/*.cpp
# )

file(
    GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/3rdParty/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 将着色器等资源，拷贝到cmake文件下
file(
    INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}
)

# 获取路径下所有的.cpp/.c/.cc文件（不包括子目录），并赋值给变量
# aux_source_directory(路径 变量)
aux_source_directory(src DIR_SRCS)

# 给编译器添加库目录
link_directories(3rdParty/lib)

# 添加编译选项FOO BAR
# add_definitions定义宏，但是这种定义方式无法给宏具体值 等价C语言中的#define  MG_ENABLE_OPENSSL
# add_definitions(-DFOO -DBAR ...)
# add_compile_definitions定义宏，这种方式可以给宏具体值，但是这个指令只要高版本的cmake支持 等价C语言中 #define  MG_ENABLE_OPENSSL   1
# add_compile_definitions(MG_ENABLE_OPENSSL=1)

# 打印消息
message("CMake Test Succeed!")

# 编译子文件夹的CMakeLists.txt，父目录必须
# add_subdirectory(子文件夹名称)

# 将.cpp/.c/.cc文件生成.a静态库，通常在子目录
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
# add_library(库文件名称 STATIC 文件)

# 将.cpp/.c/.cc文件生成可执行文件，通常在父目录
# add_executable(可执行文件名称 文件)
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置编译选项及默认值
# option(TEST_DEBUG "option for debug" OFF)

# 对add_library或add_executable生成的文件进行链接操作
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
# target_link_libraries(库文件名称/可执行文件名称 链接的库文件名称)
target_link_libraries(${PROJECT_NAME} opengl32.a glfw3.a)