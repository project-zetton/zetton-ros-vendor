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
find_package(catkin REQUIRED COMPONENTS roscpp)

# -----------------#
# Zetton libraries #
# -----------------#

find_package(zetton_common REQUIRED)

# =============#
# ROS Settings #
# =============#

catkin_package(
  INCLUDE_DIRS
  include
  LIBRARIES
  ${PROJECT_NAME}
  CATKIN_DEPENDS
  roscpp
  DEPENDS
  zetton_common)

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
  ${OpenCV_INCLUDE_DIRS}
  DEPS
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  Threads::Threads
  fmt::fmt
  absl::time
  absl::flags
  absl::flags_parse
  zetton_common::zetton_common)

# -------------------#
# Executable targets #
# -------------------#

zetton_cc_apps()
zetton_cc_examples()

# -------------#
# Test targets #
# -------------#

zetton_cc_tests("common")

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
# install( DIRECTORY config/ DESTINATION
# ${CATKIN_PACKAGE_SHARE_DESTINATION}/config PATTERN ".svn" EXCLUDE)
