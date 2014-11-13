cmake_minimum_required(VERSION 2.6)
######################################################################################
### 0) Include the install configuration file where are defined the main variables
######################################################################################

INCLUDE(${CMAKE_SOURCE_DIR}/install.cmake)

######################################################################################


######################################################################################
### 1) Set your application properties
######################################################################################

# Define your project name
PROJECT(MetaH)

######################################################################################


######################################################################################
### 2) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(${PARADISEO_EO_SRC_DIR}/src
                    ${PARADISEO_MO_SRC_DIR}/src
                    ${PARADISEO_DIR}/problems
                    lib
                    )

#INCLUDE_DIRECTORIES(${PARADISEO_EO_SRC_DIR}/src
#                    ${PARADISEO_MO_SRC_DIR}/src
#                    ${PARADISEO_MOEO_SRC_DIR}/src
#                    ${PARADISEO_PEO_SRC_DIR}/src)

######################################################################################


######################################################################################
### 3) Specify where CMake can find the libraries
######################################################################################

# Link with the EO library directory, only if you need it
LINK_DIRECTORIES(${PARADISEO_EO_BIN_DIR}/lib)

# Link with the MOEO library directory, only if you need it
# LINK_DIRECTORIES(${PARADISEO_MOEO_BIN_DIR}/lib)

# Link with the PEO library directory, only if you need it
# LINK_DIRECTORIES(${PARADISEO_PEO_BIN_DIR}/lib)

######################################################################################



######################################################################################
### 4) Define your target: just an executable here
######################################################################################

ADD_EXECUTABLE(lesson1_simpleHC lesson1_simpleHC.cpp)
ADD_EXECUTABLE(localizacionHC localizacionHC.cpp)
ADD_EXECUTABLE(localizacion localizacion.cpp)



######################################################################################


######################################################################################
### 5) Link the libraries for your executable
######################################################################################

# Only if you need to link libraries
TARGET_LINK_LIBRARIES(localizacionHC ga eo eoutils)
TARGET_LINK_LIBRARIES(localizacion ga eo eoutils)
TARGET_LINK_LIBRARIES(lesson1_simpleHC ga eo eoutils)

######################################################################################






