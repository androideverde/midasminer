cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

# set the project name
set (PROJECT_NAME midasminer)
project(${PROJECT_NAME})

# add the executable
add_executable(${PROJECT_NAME})

# add SDL library dependency
include_directories(/Library/Frameworks/SDL2.framework/Headers)
target_link_libraries(${PROJECT_NAME} /Library/Frameworks/SDL2.framework/SDL2)

# add SDL_image library dependency
include_directories(/Library/Frameworks/SDL2_image.framework/Headers)
target_link_libraries(${PROJECT_NAME} /Library/Frameworks/SDL2_image.framework/SDL2_image)

# add SDL_ttf library dependency
include_directories(/Library/Frameworks/SDL2_ttf.framework/Headers)
target_link_libraries(${PROJECT_NAME} /Library/Frameworks/SDL2_ttf.framework/SDL2_ttf)

# add the sources
file(GLOB project_files "src/*.cpp" "include/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${project_files})

# add the includes
include_directories(include)

# ensure ./assets is copied to the build directory
set (source "${CMAKE_SOURCE_DIR}/assets")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
 DEPENDS ${destination}
 COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

# add executable for tests
add_executable(${PROJECT_NAME}_tests)
add_subdirectory(dependencies/googletest-1.10.x)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC gtest)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC /Library/Frameworks/SDL2.framework/SDL2)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC /Library/Frameworks/SDL2_ttf.framework/SDL2_ttf)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC /Library/Frameworks/SDL2_image.framework/SDL2_image)
# remove src/main.cpp from list of sources to compile in the tests target
list(REMOVE_ITEM project_files /Users/Jordi.Bernabeu/personal/c++/midasminer/game/src/main.cpp)
# add sources including code and tests
target_sources(${PROJECT_NAME}_tests PRIVATE ${project_files} tests/main.cpp)
