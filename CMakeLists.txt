# CMakeList.txt : CMake project for raysanEncoding, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Dependencies
set(RAYLIB_VERSION 5.0)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib )


project ("raysanEncoding")

# Add source to this project's executable.
add_executable (raysanEncoding "raysanEncoding.cpp" "raysanEncoding.h")

target_link_libraries(raysanEncoding raylib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET raysanEncoding PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
