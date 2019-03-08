cmake_minimum_required (VERSION 2.6)
project (BruteForce)

include_directories(${CMAKE_CURRENT_LIST_DIR}/sources)
include_directories(${CMAKE_CURRENT_LIST_DIR}/includes)

add_executable (Bf  ${CMAKE_CURRENT_LIST_DIR}/sources/main.cpp
		    		        ${CMAKE_CURRENT_LIST_DIR}/sources/zipCracker.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/zipCracker.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/sources/zipReader.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/zipReader.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/sources/zipCrypto.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/IBrute.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/ICracker.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/zipCrypto.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/randomBrute.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/sources/randomBrute.cpp
                    )

find_package( OpenSSL REQUIRED )
find_package( OpenCL )
include_directories( ${OPENCL_INCLUDE_DIR} )
set (CMAKE_CXX_FLAGS "-lssl -lcrypto -v -Wall -fsanitize=address  -std=c++11") #-Wshadow
target_link_libraries(Bf OpenSSL::SSL)
target_link_libraries(Bf ${OPENCL_LIBRARIES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("use clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message("use gcc") 
endif()

