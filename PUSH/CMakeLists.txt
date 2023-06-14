# CMakeList.txt : projet CMake pour PUSH, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (PUSH "PUSH.cpp" "PUSH.hpp" "src/structure.cpp" "src/glut_functions.cpp" "src/draw.cpp" "src/draw.h" "src/puzzle.cpp"  "src/puzzle.h" "src/command.cpp" "src/command.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PUSH PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(external/glew)
add_subdirectory(external/glut)
add_subdirectory(external/glui)

# Incluez les sous-projets.

target_include_directories(PUSH PUBLIC external/glew/include
										external/glut/include
										external/glui/include)

target_link_directories(PUSH PRIVATE external/glew/src
									  external/glut/src
									  external/glui/src)

target_link_libraries(PUSH libglew_static
						   freeglut
						   glui)