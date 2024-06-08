<<<<<<< HEAD
<<<<<<< HEAD
# Install script for directory: C:/Users/Pacheco/Documents/Visual Studio Code/ProyectoFinal_GCA/ProyectoFinal-StarRace
=======
# Install script for directory: D:/GitRepositories/ProyectoFinal_GCA/ProyectoFinal-StarRace
>>>>>>> bd7dcf4c0a375759dd74e542b487c1ebb7c4d31b
=======
# Install script for directory: D:/Respaldo PC/Many/Tareas/Decimo Semestre Pt.3/Computacion Grafica Avanzada/Proyecto/ProyectoFinal_GCA/ProyectoFinal-StarRace
>>>>>>> 0842faebebb1a70b944f25a6b28bda5e72ef37a3

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

