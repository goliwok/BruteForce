cmake_minimum_required (VERSION 2.6)
project (BruteForce)

include_directories(${CMAKE_CURRENT_LIST_DIR}/sources)
include_directories(${CMAKE_CURRENT_LIST_DIR}/includes)

add_executable (Bf ${CMAKE_CURRENT_LIST_DIR}/sources/bruteforce.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/bruteForce.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/sources/arguments.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/arguments.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/sources/main.cpp
		    		        ${CMAKE_CURRENT_LIST_DIR}/sources/zipCracker.cpp
 	            	    ${CMAKE_CURRENT_LIST_DIR}/includes/zipCracker.hpp
 	            	    ${CMAKE_CURRENT_LIST_DIR}/sources/zipReader.cpp
 	            	    ${CMAKE_CURRENT_LIST_DIR}/includes/zipReader.hpp
                    ${CMAKE_CURRENT_LIST_DIR}/includes/Lib/argparse.hpp)


find_package(OpenSSL)
set (CMAKE_CXX_FLAGS "-lssl -lcrypto -v -Wall -fsanitize=address") #-Wshadow
target_link_libraries(Bf OpenSSL::SSL)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("use clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message("use gcc") 
endif()

