project( BacteriaColorCounter ) 
cmake_minimum_required( VERSION 2.8.7 )

#Make fast, lean and platform independent binaries..
set(CMAKE_CXX_FLAGS "-s -O3 -fPIC -march=native -mtune=native")
set(CMAKE_C_FLAGS "-s -O3 -fPIC -march=native -mtune=native") 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set(CMAKE_CXX_STANDARD 11)  


#JPEG/PNG libraries..
set(JPG_Libs jpeg )
set(PNG_Libs png ) 

 
#------------------------------------------------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------------------------
 SET(wxWidgets_USE_LIBS)
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
   INCLUDE("${wxWidgets_USE_FILE}") 
   #target_link_libraries(flashyslideshowsgui ${wxWidgets_LIBRARIES} )
   # and for each of your dependant executable/library targets: 
 ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
 ENDIF(wxWidgets_FOUND)


#Autosearch for the opencv maybe installed by the user..!
find_package(OpenCV PATHS dependencies/opencv-3.2.0/build NO_DEFAULT_PATH)
find_package (OpenCV)
if (OpenCV_FOUND)
  add_definitions(-DUSE_OPENCV)
  MESSAGE("OpenCV code found and will be used..")   
  MESSAGE(${OpenCV_DIR})   
ENDIF(OpenCV_FOUND)

   

if (OpenCV_FOUND)
add_subdirectory (src/) 
IF(wxWidgets_FOUND)
add_subdirectory (src/PetriDishCounter) 
ENDIF(wxWidgets_FOUND)
ENDIF(OpenCV_FOUND)
 
