project( petriDishCounterCV ) 
#cmake_minimum_required( VERSION 2.8.7 )   
cmake_minimum_required(VERSION 3.5)
find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

 

add_executable(petriDishCounterCV webcam.cpp)   
target_link_libraries(petriDishCounterCV rt dl m ${OpenCV_LIBRARIES}    )
set_target_properties(petriDishCounterCV PROPERTIES DEBUG_POSTFIX "D") 
       

set_target_properties(petriDishCounterCV PROPERTIES 
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                      )



   
set(AmmClient_INGREDIENTS 
                AmmClient/tools.c
                AmmClient/network.c
                AmmClient/protocol.c
                AmmClient/AmmClient.c
              )


add_executable(petriDishCounterStream stream.cpp ${AmmClient_INGREDIENTS})   
target_link_libraries(petriDishCounterStream rt dl m ${OpenCV_LIBRARIES}    )
set_target_properties(petriDishCounterStream PROPERTIES DEBUG_POSTFIX "D") 
       

set_target_properties(petriDishCounterStream PROPERTIES 
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
                      )

