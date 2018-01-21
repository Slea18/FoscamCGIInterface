#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.
#############################################################################
# macOS Extra #
###############
SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)

ADD_CUSTOM_TARGET(prepare
    "${CMAKE_SOURCE_DIR}/scripts/macos/vlc_libraries_prepare.sh" "${LIBVLC_BIN_DIR}" "${CMAKE_BINARY_DIR}")

SET(LIBVLC_PROCESSED_DIR "${CMAKE_BINARY_DIR}/libvlc-processed")
SET(LIBVLC_LIBRARY "${LIBVLC_PROCESSED_DIR}/libvlc.dylib")
SET(LIBVLCCORE_LIBRARY "${LIBVLC_PROCESSED_DIR}/libvlccore.dylib")

FILE(GLOB Vlc_Libs ${LIBVLC_PROCESSED_DIR}/*.dylib)
FILE(GLOB Vlc_Plugins ${LIBVLC_PROCESSED_DIR}/vlc/plugins/*.dylib)

IF(Qt5Core_VERSION VERSION_LESS "5.5.0")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_SOURCE_DIR}/scripts/macos/qt_old_postprocess.sh ${VLCQT_CORE_NAME} WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/${VLCQT_CORE_NAME}.framework/Versions/${VLCQT_VERSION}/)")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_SOURCE_DIR}/scripts/macos/qt_old_postprocess.sh ${VLCQT_WIDGETS_NAME} WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/${VLCQT_WIDGETS_NAME}.framework/Versions/${VLCQT_VERSION}/)")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_SOURCE_DIR}/scripts/macos/qt_old_postprocess.sh ${VLCQT_QML_NAME} WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/${VLCQT_QML_NAME}.framework/Versions/${VLCQT_VERSION}/)")
ENDIF()
