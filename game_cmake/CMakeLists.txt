cmake_minimum_required(VERSION 3.10)
project(Game CXX C)


set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

   add_executable(Game
       main.cpp
       item.cpp
       object.cpp
       player.cpp
       monster.cpp
       functions.cpp
   )

   target_link_libraries(Game sfml-graphics sfml-window sfml-system)

add_custom_target(copy_resourses ALL)
add_custom_command(TARGET copy_resourses
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:Game>/textures)
add_custom_command(TARGET copy_resourses
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/1 $<TARGET_FILE_DIR:Game>/1)
add_custom_command(TARGET copy_resourses
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/2 $<TARGET_FILE_DIR:Game>/2)
add_custom_command(TARGET copy_resourses
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/save.txt $<TARGET_FILE_DIR:Game>/save.txt)