find_package(GTest REQUIRED)

add_executable(is_alphanumeric_test "is_alphanumeric.test.cpp")

target_link_libraries(is_alphanumeric_test PRIVATE 
	mapper_reducer 
	GTest::gmock_main
	GTest::gtest_main
)

# set(INPUT_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../InputFiles)
# set(OUTPUT_DIR $<TARGET_FILE_DIR:is_alphanumeric_test>)
# add_custom_command(TARGET is_alphanumeric_test POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${INPUT_FILES_DIR} ${OUTPUT_DIR}/InputFiles
#    COMMENT "Copying InputFiles folder to the output directory after build."
#)

add_test(NAME IsAlphanumericTest COMMAND is_alphanumeric_test)
gtest_discover_tests(is_alphanumeric_test)