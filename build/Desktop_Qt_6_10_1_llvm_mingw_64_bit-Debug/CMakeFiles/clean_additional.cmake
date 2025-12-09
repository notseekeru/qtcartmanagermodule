# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appcartmanager_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appcartmanager_autogen.dir\\ParseCache.txt"
  "appcartmanager_autogen"
  )
endif()
