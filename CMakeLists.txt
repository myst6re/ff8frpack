###############################################################################
## Copyright (C) 2022 Arzel Jérôme <myst6re@gmail.com>
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

find_package(QT NAMES Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(PROJECT_SOURCES
    src/core/ff8installation.cpp
    src/core/ff8installation.h
    src/core/ff8frpackinstallation.cpp
    src/core/ff8frpackinstallation.h
    src/core/ffnxconfig.cpp
    src/core/ffnxconfig.h
    src/core/ffnxinstallation.cpp
    src/core/ffnxinstallation.h
    src/core/process.cpp
    src/core/process.h
    src/core/provisionner.cpp
    src/core/provisionner.h
    src/main.cpp
    src/pages/configpage.cpp
    src/pages/configpage.h
    src/pages/errorpage.cpp
    src/pages/errorpage.h
    src/pages/welcomepage.cpp
    src/pages/welcomepage.h
    src/widgets/flatcombobox.cpp
    src/widgets/flatcombobox.h
    src/widgets/helpwidget.cpp
    src/widgets/helpwidget.h
    src/widgets/resizablelabel.cpp
    src/widgets/resizablelabel.h
    src/window.cpp
    src/window.h
)

set(PATCHER_SOURCES
    src/core/ff8installation.cpp
    src/core/ff8installation.h
    src/core/ff8frpackinstallation.cpp
    src/core/ff8frpackinstallation.h
    src/core/ffnxinstallation.cpp
    src/core/ffnxinstallation.h
    src/core/provisionner.cpp
    src/core/provisionner.h
    src/patcher/arguments.cpp
    src/patcher/arguments.h
    src/patcher/argumentsconfigure.cpp
    src/patcher/argumentsconfigure.h
    src/patcher/argumentsprovision.cpp
    src/patcher/argumentsprovision.h
    src/patcher/commonarguments.cpp
    src/patcher/commonarguments.h
    src/patcher/helparguments.cpp
    src/patcher/helparguments.h
    src/patcher/main.cpp
)

qt_add_executable(FF8frPack WIN32 MACOSX_BUNDLE
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES} ${QM_FILES}
)
qt_add_executable(FF8frPackPatcher
    MANUAL_FINALIZATION
    ${PATCHER_SOURCES}
)
target_link_libraries(FF8frPack PRIVATE
    Qt::Widgets
    tomlplusplus::tomlplusplus
)

set_target_properties(FF8frPack PROPERTIES
    OUTPUT_NAME "FF8Fr Pack"
    MACOSX_BUNDLE_BUNDLE_NAME "FF8Fr Pack"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_BUNDLE_GUI_IDENTIFIER org.myst6re.ff8frpack
)

target_compile_definitions(
    FF8frPack PRIVATE
    APPLICATION_NAME="${PROJECT_NAME}"
    APPLICATION_DESCRIPTION="${PROJECT_DESCRIPTION}"
    APPLICATION_COMPANY="myst6re"
    APPLICATION_VERSION="${PROJECT_VERSION}"
    APPLICATION_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APPLICATION_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APPLICATION_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    APPLICATION_VERSION_TWEAK=0
)

target_compile_definitions(
    FF8frPackPatcher PRIVATE
    APPLICATION_NAME="FF8frPackPatcher"
    APPLICATION_DESCRIPTION="FF8.fr's Pack Patcher"
    APPLICATION_COMPANY="myst6re"
    APPLICATION_VERSION="${PROJECT_VERSION}"
    APPLICATION_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APPLICATION_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APPLICATION_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    APPLICATION_VERSION_TWEAK=0
)

add_subdirectory(lang)
add_subdirectory(dependencies)

# Build qrc file automatically
qt_add_resources(FF8frPack ff8frpacklang PREFIX /i18n/ BASE "${CMAKE_CURRENT_BINARY_DIR}/lang" FILES ${QM_FILES})
file(GLOB FF8FRPACK_IMAGE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/images/*.*")
qt_add_resources(FF8frPack ff8frpackimages PREFIX /images/ BASE "${CMAKE_CURRENT_SOURCE_DIR}/images" FILES ${FF8FRPACK_IMAGE_PATHS})

qt_finalize_executable(FF8frPack)
qt_finalize_executable(FF8frPackPatcher)

if(APPLE)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/Applications" CACHE PATH "..." FORCE)
    endif()
    install(TARGETS FF8frPack BUNDLE DESTINATION "." COMPONENT FF8frPackComponent)
    set(_target_file_name "FF8Fr Pack.app")
elseif(WIN32)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND DEFINED ENV{ProgramFiles})
        # Force Program Files for x64 programs (CMake uses x86 by default)
        set(CMAKE_INSTALL_PREFIX "$ENV{ProgramFiles}/${PROJECT_NAME}" CACHE PATH "..." FORCE)
    endif()
    if(NOT QT_DEPLOY_TMP_DIR)
        set(QT_DEPLOY_TMP_DIR "${CMAKE_BINARY_DIR}/win32-deploy" CACHE PATH "Directory to run deployqt scripts")
    endif()
    install(TARGETS FF8frPack RUNTIME DESTINATION "." COMPONENT FF8frPackComponent)
    set(_target_file_name "FF8Fr Pack.exe")
endif()

install(TARGETS FF8frPackPatcher RUNTIME DESTINATION "." COMPONENT FF8frPackComponent)

# Install FFNx
add_subdirectory(deploy)
install(DIRECTORY "${CMAKE_BINARY_DIR}/FFNx" DESTINATION "." COMPONENT FFNxComponent)

# Try to detect IFW path, since the auto detection cannot detect every versions
file(GLOB QT_INSTALLER_FRAMEWORK_BIN_PATHS "${_qt_tools}/QtInstallerFramework/*/bin")
list(SORT QT_INSTALLER_FRAMEWORK_BIN_PATHS)
list(REVERSE QT_INSTALLER_FRAMEWORK_BIN_PATHS)
if(DEFINED QT_INSTALLER_FRAMEWORK_BIN_PATHS)
    # Get last version
    list(GET QT_INSTALLER_FRAMEWORK_BIN_PATHS 0 QTIFWDIR)
    # According the the manual, the path should not contains the bin directory
    get_filename_component(CPACK_IFW_ROOT "${QTIFWDIR}" DIRECTORY)
endif()

set(CPACK_GENERATOR "IFW")
set(CPACK_PACKAGE_NAME FF8frPack)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
configure_file("deploy/deployqt.cmake.in" "deploy/deployqt.cmake" @ONLY)
set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_BINARY_DIR}/deploy/deployqt.cmake")
set(CPACK_Unspecified_COMPONENT_INSTALL OFF)
set(CPACK_tomlplusplus_Development_COMPONENT_INSTALL OFF)
set(CPACK_COMPONENTS_ALL "FF8frPackComponent;FFNxComponent")

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_SYSTEM_NAME "macos")
endif()

set(CPACK_IFW_VERBOSE ON)
set(CPACK_IFW_PRODUCT_URL https://www.ff8.fr)
set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "FF8Fr Pack Installer")
set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_INI_FILE "ff8frpackinstaller.ini")
set(CPACK_IFW_PACKAGE_WIZARD_SHOW_PAGE_LIST OFF)
set(CPACK_IFW_TARGET_DIRECTORY "@ApplicationsDirX64@/${CPACK_PACKAGE_INSTALL_DIRECTORY}")

set(CPACK_SOURCE_IGNORE_FILES ".git/*;*.user;*.yml;*.github/*")
set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")

include(CPack)
include(CPackIFW)

cpack_add_component(FF8frPackComponent REQUIRED)
cpack_add_component(FFNxComponent REQUIRED)

set(I18N_FF8FRPACK_DESC
    "Main component"
    fr "Composant principal")
set(I18N_FFNX_DESC
    "FF8 Driver and modding platform"
    fr "Driver FF8 et plate-forme de modding")
set(I18N_MYSTREPO_DESC
    "myst6re's repository"
    fr "Dépôt de myst6re")
set(I18N_FFNXFF8MUSIC_DESC
    "PS1-like musics"
    fr "Musiques à la PS1")

cpack_ifw_configure_component(FF8frPackComponent
    FORCED_INSTALLATION
    DISPLAY_NAME "FF8.fr's Pack"
    DESCRIPTION ${I18N_FF8FRPACK_DESC}
)

cpack_ifw_configure_component(FFNxComponent
    FORCED_INSTALLATION
    DISPLAY_NAME "FFNx"
    DESCRIPTION ${I18N_FFNX_DESC}
)

cpack_ifw_add_repository(myst6re
    URL "https://www.github.com/myst6re/ff8frpack/releases/download/latest/repo"
    DISPLAY_NAME ${I18N_MYSTREPO_DESC}
)

cpack_add_component(FF8MusicComponent
    DISPLAY_NAME "FFNx's FF8 Music"
    DESCRIPTION ${I18N_FFNXFF8MUSIC_DESC}
    DOWNLOADED
)
