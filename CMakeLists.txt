###############################################################################
## Copyright (C) 2021 Arzel Jérôme <myst6re@gmail.com>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

cmake_minimum_required(VERSION 3.15)

project(FF8frPack VERSION 0.1 LANGUAGES CXX
    DESCRIPTION "FF8.fr's Pack"
    HOMEPAGE_URL "https://github.com/myst6re/ff8frpackconfig"
)

# Try to autodetect Qt path if not specified
include(qt.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(QT NAMES Qt6 COMPONENTS Network Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network Widgets LinguistTools REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/ffnxconfig.cpp
    src/ffnxconfig.h
    src/ffnxinstallation.cpp
    src/ffnxinstallation.h
    src/helpwidget.cpp
    src/helpwidget.h
    src/installprogression.cpp
    src/installprogression.h
    src/window.cpp
    src/window.h
    src/wizard.cpp
    src/wizard.h
    src/wizardpages/wizardpageinstall.cpp
    src/wizardpages/wizardpageinstall.h
    src/wizardpages/wizardpageintro.cpp
    src/wizardpages/wizardpageintro.h
)

qt_add_executable(FF8frPack
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES} ${QM_FILES}
)

target_link_libraries(FF8frPack PRIVATE
    Qt::Network
    Qt::Widgets
    tomlplusplus::tomlplusplus
)

set_target_properties(FF8frPack PROPERTIES OUTPUT_NAME "FF8Fr Pack")

add_subdirectory(lang)
add_subdirectory(dependencies)

# Build qrc file automatically
qt_add_resources(FF8frPack ff8frpacklang PREFIX /i18n/ BASE "${CMAKE_CURRENT_BINARY_DIR}/lang" FILES ${QM_FILES})

if(WIN32)
    if(NOT QT_DEPLOY_TMP_DIR)
        set(QT_DEPLOY_TMP_DIR "${CMAKE_BINARY_DIR}/win32-deploy" CACHE PATH "Directory to run deployqt scripts")
    endif()
    install(TARGETS FF8frPack RUNTIME DESTINATION "${QT_DEPLOY_TMP_DIR}")
endif()

add_subdirectory(deploy)

if(WIN32)
    install(DIRECTORY "${QT_DEPLOY_TMP_DIR}/" DESTINATION ".")
    install(CODE "file(REMOVE_RECURSE \"${QT_DEPLOY_TMP_DIR}\")")
endif()

qt_finalize_executable(FF8frPack)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TXZ")
endif()

set(CPACK_PACKAGE_NAME FF8frPack)
set(CPACK_SOURCE_IGNORE_FILES ".git/*;*.user;*.yml;*.github/*")
set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")

include(CPack)
