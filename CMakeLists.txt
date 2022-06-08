cmake_minimum_required(VERSION 3.16)

project(ComicsViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Quick Widgets LinguistTools REQUIRED)
find_package(QuaZip-Qt6)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE ComicsViewer.icns)
    set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/ComicsViewer.icns")
    set_source_files_properties(${app_icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
elseif(MINGW)
    set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/ComicsViewer.rc")
endif()

add_executable(ComicsViewer
    main.cpp 
    comicmodel.cpp 
    pageitem.cpp
    qml.qrc
    ${app_icon}
)

qt_add_translations(ComicsViewer
    TS_FILES i18n/ComicsViewer_ja.ts)

target_link_libraries(ComicsViewer PRIVATE Qt6::Widgets Qt6::Quick QuaZip::QuaZip)

set_target_properties(ComicsViewer PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
