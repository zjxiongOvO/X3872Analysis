cmake_minimum_required(VERSION 3.9)
project(Pythia8_ROOT_Example)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Find ROOT package and define the necessary variables
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

link_directories(${EVTGEN_ROOT}/Evtgen/lib)
include_directories(${EVTGEN_ROOT}/Evtgen/include)

# Include the necessary directories
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${PYTHIA8_INCLUDE_DIR})

# Define the sources
set(SOURCES1 src/RunX3872JpsiRho.cpp src/TreeMaker.h src/TreeMaker.cpp src/VarManager.h)
set(SOURCES2 src/RunX3872JpsiRho_FromSpec.cpp src/TreeMaker.h src/TreeMaker.cpp src/VarManager.h)
set(SOURCES3 src/RunX3872JpsiPiPi_FromSpec.cpp src/TreeMaker.h src/TreeMaker.cpp src/VarManager.h)
set(SOURCES4 src/RunPsi2SJpsiPiPi_FromSpec.cpp src/TreeMaker.h src/TreeMaker.cpp src/VarManager.h)

# Create the executable
add_executable(runX3872JpsiRho ${SOURCES1})
add_executable(runX3872JpsiRho_FromSpec ${SOURCES2})
add_executable(runX3872JpsiPiPi_FromSpec ${SOURCES3})
add_executable(runPsi2SJpsiPiPi_FromSpec ${SOURCES4})

# Link the ROOT and Pythia8 libraries
target_link_libraries(runX3872JpsiRho ${ROOT_LIBRARIES} libpythia8.so libEvtGen.so)
target_link_libraries(runX3872JpsiRho_FromSpec ${ROOT_LIBRARIES} libpythia8.so libEvtGen.so)
target_link_libraries(runX3872JpsiPiPi_FromSpec ${ROOT_LIBRARIES} libpythia8.so libEvtGen.so)
target_link_libraries(runPsi2SJpsiPiPi_FromSpec ${ROOT_LIBRARIES} libpythia8.so libEvtGen.so)