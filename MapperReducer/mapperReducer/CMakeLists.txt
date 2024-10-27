# Add source to this project's executable.
add_executable (MapperReducer  
"main.cpp" 
)

# Specify the relative path for the input files directory
set(INPUT_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/InputFiles)

# Specify the output directory for the build (where executable is generated)
set(OUTPUT_DIR $<TARGET_FILE_DIR:MapperReducer>)

# Add a post-build command to copy the input files to the build output directory
add_custom_command(TARGET MapperReducer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${INPUT_FILES_DIR} ${OUTPUT_DIR}/InputFiles
    COMMENT "Copying InputFiles folder to the output directory after build."
)