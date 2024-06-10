include(Ctest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_definitions(-DRELATIVE_EPSILON=1e-6)

add_executable(test_neural test_neural.cpp)
target_link_libraries(test_neural Catch2::Catch2WithMain)
target_include_directories(test_neural PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME test_neural COMMAND test_neural)

