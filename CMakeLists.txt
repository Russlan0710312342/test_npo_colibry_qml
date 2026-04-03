cmake_minimum_required(VERSION 3.16)

project(test_npo_colibry_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(apptest_npo_colibry_qml
    main.cpp
)

qt_add_qml_module(apptest_npo_colibry_qml
    URI test_npo_colibry_qml
    QML_FILES
        Main.qml
    RESOURCES
        resources.qrc
        QML_FILES MyCalculator.qml
        QML_FILES MyCalculateButton.qml
        SOURCES   controller.h enums.h mainwindow.cpp  pow.h
RESOURCES mainwindow.ui
SOURCES calculator.h
)

qt_add_resources(RESOURCE_SOURCES resources.qrc)
target_sources(apptest_npo_colibry_qml PRIVATE ${RESOURCE_SOURCES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptest_npo_colibry_qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_npo_colibry_qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptest_npo_colibry_qml
    PRIVATE Qt6::Quick Qt6::Core
)

install(TARGETS apptest_npo_colibry_qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
