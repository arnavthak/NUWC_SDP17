# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appCircuitGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appCircuitGUI_autogen.dir\\ParseCache.txt"
  "appCircuitGUI_autogen"
  )
endif()
