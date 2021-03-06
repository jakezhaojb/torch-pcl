CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(torch-pcl)

FIND_PACKAGE(PCL 1.8 REQUIRED)
FIND_PACKAGE(Torch REQUIRED)
FIND_PACKAGE(VTK REQUIRED)

INCLUDE(${VTK_USE_FILE})

INCLUDE_DIRECTORIES(${PCL_INCLUDE_DIRS} ${Torch_INSTALL_INCLUDE})
LINK_DIRECTORIES(${PCL_LIBRARY_DIRS} "${Torch_INSTALL_LIB}")
ADD_DEFINITIONS(${PCL_DEFINITIONS})

SET(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Library path
SET(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

SET(src
  "${SOURCE_DIR}/indices.cpp"
  "${SOURCE_DIR}/indices_vector.cpp"
  "${SOURCE_DIR}/correspondences.cpp"
  "${SOURCE_DIR}/cloudviewer.cpp"
  "${SOURCE_DIR}/xyz.cpp"
  "${SOURCE_DIR}/xyzi.cpp"
  "${SOURCE_DIR}/xyzrgba.cpp"
  "${SOURCE_DIR}/xyznormal.cpp"
  "${SOURCE_DIR}/xyzinormal.cpp"
  "${SOURCE_DIR}/xyzrgbnormal.cpp"
  "${SOURCE_DIR}/normal.cpp"
  "${SOURCE_DIR}/fpfhsignature33.cpp"
  "${SOURCE_DIR}/vfhsignature308.cpp"
  "${SOURCE_DIR}/boundary.cpp"
  "${SOURCE_DIR}/visualizer.cpp"
  "${SOURCE_DIR}/label.cpp"
)

FILE(GLOB luasrc *.lua)

ADD_LIBRARY(pcl MODULE ${src})

### Torch packages supposes libraries prefix is "lib"
SET_TARGET_PROPERTIES(pcl PROPERTIES PREFIX "lib" IMPORT_PREFIX "lib" INSTALL_NAME_DIR "@executable_path/${Torch_INSTALL_BIN2CPATH}")
    
IF(APPLE)
  SET_TARGET_PROPERTIES(pcl PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ENDIF()

TARGET_LINK_LIBRARIES(pcl TH ${PCL_LIBRARIES} ${VTK_LIBRARIES})

INSTALL(TARGETS pcl LIBRARY DESTINATION ${Torch_INSTALL_LUA_CPATH_SUBDIR})
INSTALL(DIRECTORY "doc" DESTINATION "${Torch_INSTALL_LUA_PATH_SUBDIR}/pcl")
INSTALL(FILES "README.md" DESTINATION "${Torch_INSTALL_LUA_PATH_SUBDIR}/pcl")
INSTALL(FILES ${luasrc} DESTINATION "${Torch_INSTALL_LUA_PATH_SUBDIR}/pcl")