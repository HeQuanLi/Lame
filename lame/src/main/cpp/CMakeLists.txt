cmake_minimum_required(VERSION 3.4.1)

# 引入你的 LAME 库
add_library(mp3lame SHARED IMPORTED)
set_target_properties(mp3lame PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmp3lame.so)

# 编译你的 LameWrapper.cpp 文件
add_library(Lames SHARED Lames.cpp)

# 链接 LAME 库
target_link_libraries(Lames mp3lame log)