<<<<<<< HEAD
# Install script for directory: /home/marta/robocomp/components/Robotica-2020-Marene/chocachoca
=======
# Install script for directory: /home/salabeta/robocomp/components/Robotica-2020-Marene/chocachoca
>>>>>>> f5136dc37e23032504b5654fdce499a68b9d60ff

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/robocomp/etc-default/MyFirstComp.conf")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
<<<<<<< HEAD
file(INSTALL DESTINATION "/opt/robocomp/etc-default" TYPE FILE RENAME "MyFirstComp.conf" FILES "/home/marta/robocomp/components/Robotica-2020-Marene/chocachoca/etc/config")
=======
file(INSTALL DESTINATION "/opt/robocomp/etc-default" TYPE FILE RENAME "MyFirstComp.conf" FILES "/home/salabeta/robocomp/components/Robotica-2020-Marene/chocachoca/etc/config")
>>>>>>> f5136dc37e23032504b5654fdce499a68b9d60ff
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
<<<<<<< HEAD
  include("/home/marta/robocomp/components/Robotica-2020-Marene/chocachoca/src/cmake_install.cmake")
=======
  include("/home/salabeta/robocomp/components/Robotica-2020-Marene/chocachoca/src/cmake_install.cmake")
>>>>>>> f5136dc37e23032504b5654fdce499a68b9d60ff

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< HEAD
file(WRITE "/home/marta/robocomp/components/Robotica-2020-Marene/chocachoca/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "/home/salabeta/robocomp/components/Robotica-2020-Marene/chocachoca/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> f5136dc37e23032504b5654fdce499a68b9d60ff
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
