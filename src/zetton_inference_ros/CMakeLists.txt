cmake_minimum_required(VERSION 3.5)

# ##############################################################################
# Metadata #
# ##############################################################################

# Read the package manifest.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/package.xml" package_xml_str)

# Extract project name.
if(NOT package_xml_str MATCHES "<name>([A-Za-z0-9_]+)</name>")
  message(
    FATAL_ERROR "Could not parse project name from package manifest (aborting)")
else()
  set(extracted_name ${CMAKE_MATCH_1})
endif()

# Extract project version.
if(NOT package_xml_str MATCHES "<version>([0-9]+.[0-9]+.[0-9]+)</version>")
  message(
    FATAL_ERROR
      "Could not parse project version from package manifest (aborting)")
else()
  # At this point we either have a proper version string, or we've errored out
  # with a FATAL_ERROR above. So assume CMAKE_MATCH_1 contains our package's
  # version.
  set(extracted_version ${CMAKE_MATCH_1})
endif()

# ##############################################################################
# CMake Project ##
# ##############################################################################

project(
  ${extracted_name}
  VERSION ${extracted_version}
  LANGUAGES CXX)

# useful macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/util.cmake)

# =============#
# Dependencies #
# =============#

# ----------------------#
# Third-party libraries #
# ----------------------#

fetch_googletest()
find_package(
  catkin REQUIRED
  COMPONENTS roscpp
             roslib
             rospy
             image_transport
             cv_bridge
             sensor_msgs
             message_generation
             zetton_common_ros)

# -----------------#
# Zetton libraries #
# -----------------#

find_package(zetton_common REQUIRED)
find_package(zetton_stream REQUIRED)
find_package(zetton_inference REQUIRED)

# =============#
# ROS Settings #
# =============#

# catkin_python_setup()
catkin_package(
  INCLUDE_DIRS
  include
  LIBRARIES
  ${PROJECT_NAME}
  CATKIN_DEPENDS
  roscpp
  roslib
  rospy
  DEPENDS
  zetton_common_ros)

# =========#
# Settings #
# =========#

# shared libraries
if(NOT DEFINED BUILD_SHARED_LIBS)
  message(STATUS "${PROJECT_NAME}: Building dynamically-linked binaries")
  option(BUILD_SHARED_LIBS "Build dynamically-linked binaries" ON)
  set(BUILD_SHARED_LIBS ON)
endif()

# build type
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  message(STATUS "${PROJECT_NAME}: Defaulting build type to RelWithDebInfo")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# win32
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# global compilations
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-O2)

# ##############################################################################
# Build #
# ##############################################################################

# ==============#
# Build targets #
# ==============#

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ---------------#
# Library target #
# ---------------#

# find all source files
file(GLOB_RECURSE src_files ${PROJECT_SOURCE_DIR}/src/*.cc
     ${PROJECT_SOURCE_DIR}/src/*/*.cc)

# common library
zetton_cc_library(
  NAME
  ${PROJECT_NAME}
  SRCS
  ${src_files}
  INCLUDES
  ${catkin_INCLUDE_DIRS}
  DEPS
  ${catkin_LIBRARIES}
  zetton_common::zetton_common
  zetton_stream::zetton_stream
  zetton_inference::zetton_inference)

# -------------------#
# Executable targets #
# -------------------#
add_simple_apps()
add_simple_examples()

# -------------#
# Test targets #
# -------------#
add_tests_in_dir("inference")

# ##############################################################################
# Install #
# ##############################################################################

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  PATTERN ".svn" EXCLUDE)

# install( DIRECTORY launch/ DESTINATION
# ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch PATTERN ".svn" EXCLUDE)

# install( DIRECTORY config/ DESTINATION
# ${CATKIN_PACKAGE_SHARE_DESTINATION}/config PATTERN ".svn" EXCLUDE)

# install( DIRECTORY asset/ DESTINATION
# ${CATKIN_PACKAGE_SHARE_DESTINATION}/asset PATTERN ".svn" EXCLUDE)

# file(GLOB_RECURSE python_excutables ${PROJECT_SOURCE_DIR}/example/*.py)
# catkin_install_python(PROGRAMS ${python_excutables} DESTINATION
# ${CATKIN_PACKAGE_BIN_DESTINATION})
