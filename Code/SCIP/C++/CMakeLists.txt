cmake_minimum_required(VERSION 3.3)

project(stackoverflow)

if(TARGET SCIP::SCIP)
  # find package by SCIP PATH
  find_package(SCIP CONFIG PATHS ${SCIP_BINARY_DIR} REQUIRED)
else()
  find_package(SCIP REQUIRED)
endif()

#find_package(GCG REQUIRED)
include_directories(${SCIP_INCLUDE_DIRS})
#include_directories("/home/ytsao/gcg+scip/scipoptsuite-8.0.4/Ipopt/src/Common/")
#include_directories(${GCG_INCLUDE_DIRS})

add_executable(maxflow MaxFlow.cpp)
target_link_libraries(maxflow ${SCIP_LIBRARIES})
#target_link_libraries(nonlinear ${IPOPT_LIBRARIES})
#target_link_libraries(firstscip ${GCG_LIBRARIES})
