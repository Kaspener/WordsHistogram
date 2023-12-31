cmake_minimum_required(VERSION 3.16)

project(WordsHistogram VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appWordsHistogram
    main.cpp
)

qt_add_qml_module(appWordsHistogram
    URI WordsHistogram
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES controller.h controller.cpp
    SOURCES histogramModel.h histogramModel.cpp
    SOURCES fileReader.h fileReader.cpp
)

set_target_properties(appWordsHistogram PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appWordsHistogram
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appWordsHistogram
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
