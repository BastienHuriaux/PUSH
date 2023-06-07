# CMakeList.txt : projet CMake pour PUSH, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (PUSH "PUSH.cpp" "PUSH.hpp" "shader.cpp" "structure.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PUSH PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(external/freetype2)
add_subdirectory(external/glut)
add_subdirectory(external/glui)

# Incluez les sous-projets.

target_include_directories(PUSH PUBLIC external/glfw/include 
										external/glew/include
										external/freetype2/include
										external/glut/include
										external/glui/include)

target_link_directories(PUSH PRIVATE external/glfw/src 
									external/glew/src
									external/freetype2/src
									external/glut/src
									external/glui/src)

target_link_libraries(PUSH glfw 
						libglew_static
						glm
						freetype
						freeglut
						glui)