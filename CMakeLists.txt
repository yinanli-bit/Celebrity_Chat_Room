cmake_minimum_required(VERSION 3.29)
project(TCproject_0_2)

set(CMAKE_CXX_STANDARD 17)

# 新增：自动复制网页文件到构建目录
file(GLOB_RECURSE WEB_FILES
        "web/Login.html"
        "web/Register.html"
        "web/Chat.html"
        "web/Style.css"
)
file(COPY ${WEB_FILES}
        DESTINATION ${CMAKE_BINARY_DIR}/web_resources
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
if (Python3_EXECUTABLE)
    message(STATUS "Python3 Executable Path: ${Python3_EXECUTABLE}")
endif()


# 设置 OpenSSL 路径
set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/mingw64/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "C:/msys64/mingw64/lib/libssl.dll.a")

find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})

# 添加目标
add_executable(TCproject_0_2
        main.cpp
        src/User.cpp
        src/FileManager.cpp
        src/ChatHandler.cpp
)

# 链接 OpenSSL 和 Windows 系统库
target_include_directories(TCproject_0_2 PRIVATE include)
target_link_libraries(TCproject_0_2 OpenSSL::SSL OpenSSL::Crypto ws2_32 crypt32)
