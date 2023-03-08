cmake_minimum_required(VERSION 3.5)

# ##############################################################################
# CMake Project ##
# ##############################################################################

# include useful macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/util.cmake)

# read metadata from package.xml and setup project
zetton_cc_metadata()

# set project name and version
project(
  ${extracted_name}
  VERSION ${extracted_version}
  LANGUAGES C CXX)

# =========#
# Settings #
# =========#

# introduce default building settings
zetton_cc_settings()

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

zetton_cc_apps()
zetton_cc_examples()

# -------------#
# Test targets #
# -------------#

zetton_cc_tests("inference")

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
