find_package(GTest REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(mapper_reducer 
"../mapperReducer/include/utils.h" 
"../mapperReducer/src/utils.cpp"
)

target_include_directories(mapper_reducer PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../mapperReducer/include> 
    $<INSTALL_INTERFACE:include>                         
)

target_link_libraries(mapper_reducer PUBLIC nlohmann_json::nlohmann_json)


add_subdirectory(unit_test)