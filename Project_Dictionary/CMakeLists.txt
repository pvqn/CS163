cmake_minimum_required(VERSION 3.5)


file(GLOB SOURCES_OPERATION "operation/*.h" "operation/*.hpp" "operation/*.cpp")
file(GLOB SOURCES_GUI "GUI/*.cpp" "GUI/*.h" "GUI/*.ui")
file(GLOB SOURCES_DATA "dataset")
file(GLOB SOURCES_ICON "resources")

file(COPY ${SOURCES_DATA} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SOURCES_ICON} DESTINATION ${CMAKE_BINARY_DIR})

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/win_icon.rc")

set(MACOSX_BUNDLE_ICON_FILE icon.icns)

set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns")

project(Project_CS163 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

set(PROJECT_SOURCES
        ${SOURCES_GUI}
        ${SOURCES_OPERATION}
	${SOURCES_ICON}
        ${app_icon_resource_windows}
        ${app_icon_macos}
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(untitled SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Project_CS163
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

set_target_properties(untitled PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled)
endif()
