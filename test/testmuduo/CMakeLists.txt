cmake_minimum_required(VERSION 3.0)
project(main)

# 一个项目文件的配置
# bin：可执行文件
# lib：库目录
# include：头文件
# src：源文件
# build：构建
# example：例子
# thridparty：三方库
# CMakeLists.txt
# autobuild.sh  编译脚本

# 一条编译命令 g++ 可执行文件名 参数-g、-o...  源文件  -I头文件 -L库文件 -l动态链接库

# 指定使用c++11的标准
set(CMAKE_CXX_STANDARD 11)

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 配置头文件的搜索路径
# include_directories()

# 配置库文件搜索路径
# link_directories()

# 设置需要编译的源文件列表
set(SRC_LIST ./muduoserver.cpp)

# 设置可执行文件最终存储的路径  PROJECT_SOURCE_DIR表示工程的根目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
# aux_source_directory(. SRC_LIST)

# 表示生成可执行文件server 由SRC_LIST变量所定义的源文件编译而来
add_executable(server ${SRC_LIST})

# 表示server这个目标程序，需要连接muduo_net muduo_base pthread这三个库文件
target_link_libraries(server muduo_net muduo_base pthread)
