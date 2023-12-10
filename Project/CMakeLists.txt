project("Project")

#Put the sources into a variable
set(SOURCES_PROJECT "project.cpp" "camera.h" "shader.h" "object.h" )

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources
add_compile_definitions(PATH_TO_OBJECTS="${CMAKE_CURRENT_SOURCE_DIR}/objects")
add_compile_definitions(PATH_TO_IMAGES="${CMAKE_CURRENT_SOURCE_DIR}/images")
add_compile_definitions(PATH_TO_SHADERS="${CMAKE_CURRENT_SOURCE_DIR}/shaders")




add_executable(${PROJECT_NAME} ${SOURCES_PROJECT}  "particle_generator.h")
#Specify which libraries you want to use with your executable
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL glfw glad)