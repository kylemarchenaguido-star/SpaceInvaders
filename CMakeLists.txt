cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("SpaceInvaders")

# vcpkg toolchain - must be set before find_package calls
set(CMAKE_TOOLCHAIN_FILE "C:/Users/kyled/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find packages first
find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Define your executable BEFORE linking
add_executable(main
    "Space Invaders/Space Invaders.cpp"
    "Space Invaders/Space Invaders.h"
)

# Set C++ standard
set_property(TARGET main PROPERTY CXX_STANDARD 17)

# Include GLAD headers
target_include_directories(main PRIVATE "Space Invaders/include")

# Now link libraries AFTER the target exists
target_link_libraries(main PRIVATE glfw glm::glm opengl32)
target_link_libraries(main PRIVATE GLEW::GLEW)

