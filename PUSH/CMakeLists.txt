# CMakeList.txt : projet CMake pour PUSH, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (PUSH "PUSH.cpp" "PUSH.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PUSH PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(external/glfw)
add_subdirectory(external/glew)

# Incluez les sous-projets.

target_include_directories(PUSH PUBLIC external/glfw/include)
target_include_directories(PUSH PUBLIC external/glew/include)

target_link_directories(PUSH PRIVATE external/glfw/src)
target_link_directories(PUSH PRIVATE external/glew/src)

target_link_libraries(PUSH glfw)
target_link_libraries(PUSH libglew_static)