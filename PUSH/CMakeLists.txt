# CMakeList.txt : projet CMake pour PUSH, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (PUSH "PUSH.cpp" "PUSH.h" "shader.cpp" "piece.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PUSH PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(external/glad)
add_subdirectory(external/freetype2)
add_subdirectory(external/glut)

# Incluez les sous-projets.

target_include_directories(PUSH PUBLIC external/glfw/include 
										external/glew/include
										external/freetype2/include
										external/glad/include
										external/glut/include)

target_link_directories(PUSH PRIVATE external/glfw/src 
									external/glew/src
									external/freetype2/src
									external/glad/src
									external/glut/src)

target_link_libraries(PUSH glfw 
						libglew_static
						glm
						freetype
						glad
						freeglut)