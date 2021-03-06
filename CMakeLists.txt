cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -pg -lpthread -std=gnu++0x -Wall")
    set(CMAKE_SHARED_LINKER_FLAGS "-pg")
endif()


#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(${EIGEN_INCLUDE_DIRS})
find_package(Eigen REQUIRED)
find_package(OpenCV REQUIRED)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

rosbuild_add_library(linearoptionlib
  src/DynaLOEMAgent.cc
  src/LinearQ0Learner.cc
  src/ContinuousRooms.cc
)

rosbuild_add_executable(run_experiment
  src/ContinuousRoomsExperiment.cc
)
target_link_libraries(run_experiment linearoptionlib ${OpenCV_LIBS})
rosbuild_link_boost(run_experiment serialization)

rosbuild_add_executable(learn_options
  src/LearnContinuousRoomsOptions.cc
)
target_link_libraries(learn_options linearoptionlib ${OpenCV_LIBS})
rosbuild_link_boost(learn_options serialization)


rosbuild_add_executable(test_policy_serialization
  src/TestSaveLoadPolicy.cc
)
target_link_libraries(test_policy_serialization linearoptionlib ${OpenCV_LIBS})
rosbuild_link_boost(test_policy_serialization serialization)
