cmake_minimum_required(VERSION 3.16)

project(NUWC_SDP17 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt setup ---
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
qt_standard_project_setup(REQUIRES 6.8)

# --- Fetch yaml-cpp from GitHub ---
include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master  # you can replace 'master' with a specific tag like yaml-cpp-0.8.0
)

FetchContent_MakeAvailable(yaml-cpp)

# --- Define your app executable ---
qt_add_executable(appNUWC_SDP17
    main.cpp
)

qt_add_qml_module(appNUWC_SDP17
    URI NUWC_SDP17
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        yamlprocessor.h
        yamlprocessor.cpp
        SOURCES serialcomms.h serialcomms.cpp
)

# --- macOS/iOS bundle setup (optional) ---
set_target_properties(appNUWC_SDP17 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --- Link libraries ---
target_link_libraries(appNUWC_SDP17
    PRIVATE
        Qt6::Quick
        yaml-cpp::yaml-cpp   # ← Added this line to link yaml-cpp
)
target_link_libraries(appNUWC_SDP17 PRIVATE Qt6::Core)
target_link_libraries(appNUWC_SDP17 PRIVATE Qt6::SerialPort)

# --- Install rules ---
include(GNUInstallDirs)
install(TARGETS appNUWC_SDP17
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
