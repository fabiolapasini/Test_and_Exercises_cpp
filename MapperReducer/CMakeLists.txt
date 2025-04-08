cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


option(BUILD_TESTS "Build tests" ON)
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
if(BUILD_TESTS)
    enable_testing()
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()


project ("MapperReducer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_subdirectory(MapperReducer)
add_subdirectory(tests)

include(GNUInstallDirs)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
  # set_property(TARGET MapperReducer PROPERTY CXX_STANDARD 20)
# endif()

