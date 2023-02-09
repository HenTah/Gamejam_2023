cmake_minimum_required(VERSION 3.10)

# Set the project name and the language
project(Root_Killer CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/GameJam_SFML_022023)	
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(CMAKE_SUPPRESS_REGENERATION true)
# Set the SFML framework paths

if(WIN32)
	## If you want to link SFML statically
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
	set(SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/include")
	set(SFML_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib")
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/cmake/SFML" ${CMAKE_MODULE_PATH})
	find_package(SFML COMPONENTS graphics window system audio REQUIRED)
	#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/bin/ DESTINATION ${CMAKE_BINARY_DIR}/GameJam_SFML_022023/)

elseif(APPLE)
	set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1")
	set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
	set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
	# Find the SFML libraries
	find_library(SFML_GRAPHICS_LIBRARY sfml-graphics HINTS "${SFML_LIBRARY_DIR}")
	find_library(SFML_WINDOW_LIBRARY sfml-window HINTS "${SFML_LIBRARY_DIR}")
	find_library(SFML_SYSTEM_LIBRARY sfml-system HINTS "${SFML_LIBRARY_DIR}")
	find_library(SFML_AUDIO_LIBRARY sfml-audio HINTS "${SFML_LIBRARY_DIR}")
endif()

# Add the source files
set(SOURCES
  GameJam_SFML_022023/main.cpp
  GameJam_SFML_022023/menu.cpp
  GameJam_SFML_022023/game.cpp
  GameJam_SFML_022023/plant.cpp
  GameJam_SFML_022023/player.cpp
  GameJam_SFML_022023/Explosion.cpp
  GameJam_SFML_022023/Enemy.cpp
  GameJam_SFML_022023/audio.cpp
)

# Add the header files
set(HEADERS
  GameJam_SFML_022023/main.h
  GameJam_SFML_022023/menu.h
  GameJam_SFML_022023/game.h
  GameJam_SFML_022023/plant.h
  GameJam_SFML_022023/player.h
  GameJam_SFML_022023/Explosion.h
  GameJam_SFML_022023/Enemy.h
  GameJam_SFML_022023/audio.h
)

# Add the include directories
include_directories("${SFML_INCLUDE_DIR}")

# Create the executable
add_executable(Root_Killer ${SOURCES} ${HEADERS})

# Link the SFML libraries to the executable
target_link_libraries(${PROJECT_NAME}
  sfml-graphics
  sfml-window
  sfml-system
  sfml-audio
)
set_target_properties(Root_Killer PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
# Using INSTALL_RPATH
set_target_properties(Root_Killer PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/assets")
# Using INSTALL_RPATH_USE_LINK_PATH

install(TARGETS Root_Killer DESTINATION GameJam_SFML_022023)
