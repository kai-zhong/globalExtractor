# add graph lib source code
file(GLOB UTIL_FILE ${PROJECT_SOURCE_DIR}/libsrc/util/*.cpp)
message(STATUS "UTIL_SOURCES: " ${UTIL_FILE})
add_library(util STATIC  ${UTIL_FILE})

file(GLOB GRAPH_FILE ${PROJECT_SOURCE_DIR}/libsrc/graph/*.cpp)
message(STATUS "GRAPH_SOURCES: " ${GRAPH_FILE})
add_library(graph STATIC ${GRAPH_FILE})

file(GLOB GLOBALEXTRACTOR_FILE ${PROJECT_SOURCE_DIR}/libsrc/globalExtractor/*.cpp)
message(STATUS "GLOBALEXTRACTOR_SOURCES: " ${GLOBALEXTRACTOR_FILE})
add_library(globalextractor STATIC  ${GLOBALEXTRACTOR_FILE})

file(GLOB MBPTREE_FILE ${PROJECT_SOURCE_DIR}/libsrc/mbptree/*.cpp)
message(STATUS "MBPTREE_SOURCES: " ${MBPTREE_FILE})
add_library(mbptree STATIC  ${MBPTREE_FILE})