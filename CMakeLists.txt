//written by Ben

cmake_minimum_required(VERSION 3.16)

project(CircuitGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appCircuitGUI
    main.cpp
)

qt_add_qml_module(appCircuitGUI
    URI CircuitGUI
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES
        main.qml
        panels/ConfigurationPanel.qml
        panels/TestControlPanel.qml
        panels/ResultsPanel.qml
        panels/HelpPanel.qml
)

target_link_libraries(appCircuitGUI PRIVATE Qt6::Quick)
