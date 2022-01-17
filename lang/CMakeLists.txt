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

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

find_package(QT NAMES Qt6 COMPONENTS LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools)

set(LANGS
    en
    fr
CACHE INTERNAL "")
list(TRANSFORM LANGS REPLACE ".+" "FF8frPack_\\0.ts" OUTPUT_VARIABLE TS_FILES)

# Clear QM_FILES list before using it
set(QM_FILES "")
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/src ${TS_FILES} OPTIONS -locations none)

add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Make it available for other CMakelists files
if(APPLE)
    set(TRANSLATIONS_PATH "FF8Fr Pack.app/Contents/Resources" CACHE INTERNAL "")
else()
    set(TRANSLATIONS_PATH translations CACHE INTERNAL "")
endif()
set(QM_FILES ${QM_FILES} CACHE INTERNAL "")

install(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_PATH}")
