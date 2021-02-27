project( petriDishCounter ) 
cmake_minimum_required( VERSION 2.8.7 ) 
# Extend cmake's module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules ${CMAKE_MODULE_PATH})
    
add_executable(petriDishCounter 
               PetriDishCounterMain.cpp
               PetriDishCounterApp.cpp
               cv.cpp
              ) 
target_link_libraries(petriDishCounter ${wxWidgets_LIBRARIES}  ${OpenCV_LIBRARIES} AmmClient)

set_target_properties(petriDishCounter PROPERTIES 
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                      )

 
