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

# Deploy Qt using macdeployqt and windeployqt scripts
if(NOT CMAKE_INSTALL_PREFIX STREQUAL "" AND (APPLE OR WIN32))
    install(CODE "
        set(_target_file_dir \"${CMAKE_INSTALL_PREFIX}\")
        set(_target_file_name \"${_target_file_name}\")
        set(_qt_bin_dir \"${_qt_bin_dir}\")
        set(LANGS \"${LANGS}\")
        set(_qt_translations_dir \"${_qt_translations_dir}\")
        configure_file(\"${CMAKE_SOURCE_DIR}/deploy/deployqt.cmake.in\" \"${CMAKE_BINARY_DIR}/deploy/deployqt-install.cmake\" @ONLY)"
    )
    install(SCRIPT "${CMAKE_BINARY_DIR}/deploy/deployqt-install.cmake")
endif()

install(CODE "
    file(DOWNLOAD https://github.com/julianxhokaxhiu/FFNx/releases/download/1.10.1/FFNx-Steam-v1.10.1.0.zip \"${CMAKE_BINARY_DIR}/FFNx-Steam.zip\")
    file(ARCHIVE_EXTRACT INPUT \"${CMAKE_BINARY_DIR}/FFNx-Steam.zip\" DESTINATION \"${CMAKE_BINARY_DIR}/FFNx\")
" COMPONENT FFNxComponent)
