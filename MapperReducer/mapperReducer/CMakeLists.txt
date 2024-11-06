find_package(nlohmann_json CONFIG REQUIRED)
find_package(log4cxx REQUIRED)

# Add source to this project's executable.
add_executable (MapperReducer  
"main.cpp"
)

target_link_libraries(MapperReducer PUBLIC log4cxx)
target_link_libraries(MapperReducer PRIVATE nlohmann_json::nlohmann_json)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

include(GNUInstallDirs)

# Specify the relative path for the input files directory
set(INPUT_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Assets)

# Specify the output directory for the build (where executable is generated)
set(OUTPUT_DIR $<TARGET_FILE_DIR:MapperReducer>)

# Add a post-build command to copy the input files to the build output directory
add_custom_command(TARGET MapperReducer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${INPUT_FILES_DIR} ${OUTPUT_DIR}/Assets
    COMMENT "Copying InputFiles folder to the output directory after build."
)