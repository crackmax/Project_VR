project("LAB01")

#Put the sources into a variable
set(SOURCES_EX_01 "main.cpp" "shader.h")

set(SOURCES_EX_ADD "main_ex19.cpp" "shader.h")


add_compile_definitions(PATH_TO_LAB1_SHADERS="${CMAKE_CURRENT_SOURCE_DIR}")

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources
add_executable(${PROJECT_NAME}_ex01 ${SOURCES_EX_01})
#Specify which libraries you want to use with your executable
target_link_libraries(${PROJECT_NAME}_ex01 PUBLIC OpenGL::GL glfw glad)

add_executable(${PROJECT_NAME}_ex19 ${SOURCES_EX_ADD})
target_link_libraries(${PROJECT_NAME}_ex19 PUBLIC OpenGL::GL glfw glad)