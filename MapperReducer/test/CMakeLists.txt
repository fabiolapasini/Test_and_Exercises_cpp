find_package(GTest REQUIRED)

add_library(mapper_reducer 
"../mapperReducer/include/utils.h" 
"../mapperReducer/src/utils.cpp"
)

target_include_directories(mapper_reducer PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../mapperReducer/include> 
    $<INSTALL_INTERFACE:include>                         
)

add_subdirectory(unit_test)