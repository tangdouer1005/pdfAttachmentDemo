cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(your_project_name)

# 添加PDFHummus的头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Zlib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/PDFWriter)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/LibTiff)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/LibPng)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/LibJpeg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/LibAesgm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include/freetype)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include/dlg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include/freetype/config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include/freetype/internal)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FreeType/include/freetype/internal/services)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加PDFHummus的库文件路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 创建可执行文件
file(GLOB SRC_FILES "src/*.cpp")

add_executable(creatattachment ${SRC_FILES})

# 链接所需要的库文件

target_link_libraries(creatattachment PRIVATE libPDFWriter.a libFreeType.a libLibJpeg.a libLibAesgm.a libLibTiff.a libLibPng.a libZlib.a)

