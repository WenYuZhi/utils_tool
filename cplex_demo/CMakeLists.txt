cmake_minimum_required(VERSION 3.21)

project(cplex_api)

if (NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Release)
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

set(CPLEX_ROOT_DIR "/home/wangyuan@sribd.cn/cplex_studio2210/cplex")
include_directories(${CPLEX_ROOT_DIR}/include)

# Library and link directories
link_directories(${CPLEX_ROOT_DIR}/lib)

# Test source for lll_reduction
set(sources main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${sources})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CPLEX_ROOT_DIR}/lib/x86-64_linux/static_pic/libcplex.a)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CPLEX_ROOT_DIR}/lib/x86-64_linux/static_pic/libilocplex.a)