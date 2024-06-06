<<<<<<< Updated upstream
<<<<<<< Updated upstream
# Install script for directory: C:/Users/Pacheco/Documents/Visual Studio Code/ProyectoFinal_GCA
=======
# Install script for directory: D:/GitRepositories/ProyectoFinal_GCA
>>>>>>> Stashed changes
=======
# Install script for directory: D:/GitRepositories/ProyectoFinal_GCA
>>>>>>> Stashed changes

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/CGA")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/ucrt64/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
<<<<<<< Updated upstream
<<<<<<< Updated upstream
  include("C:/Users/Pacheco/Documents/Visual Studio Code/ProyectoFinal_GCA/build/CGALib/cmake_install.cmake")
  include("C:/Users/Pacheco/Documents/Visual Studio Code/ProyectoFinal_GCA/build/ProyectoFinal-StarRace/cmake_install.cmake")
=======
  include("D:/GitRepositories/ProyectoFinal_GCA/build/CGALib/cmake_install.cmake")
  include("D:/GitRepositories/ProyectoFinal_GCA/build/ProyectoFinal-StarRace/cmake_install.cmake")
>>>>>>> Stashed changes
=======
  include("D:/GitRepositories/ProyectoFinal_GCA/build/CGALib/cmake_install.cmake")
  include("D:/GitRepositories/ProyectoFinal_GCA/build/ProyectoFinal-StarRace/cmake_install.cmake")
>>>>>>> Stashed changes

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< Updated upstream
<<<<<<< Updated upstream
file(WRITE "C:/Users/Pacheco/Documents/Visual Studio Code/ProyectoFinal_GCA/build/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "D:/GitRepositories/ProyectoFinal_GCA/build/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> Stashed changes
=======
file(WRITE "D:/GitRepositories/ProyectoFinal_GCA/build/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> Stashed changes
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
