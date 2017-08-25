#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2014 Tadej Novak <tadej@tano.si>
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
# Windows Extra #
#################
# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ".")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF()

SET(WINDOWS_RUN_PATH "${LIBVLC_BIN_DIR}\;${QT_BIN_DIR}\;${CMAKE_BINARY_DIR}/src/core\;${CMAKE_BINARY_DIR}/src/qml\;${CMAKE_BINARY_DIR}/src/widgets\;%PATH%;")
STRING(REGEX REPLACE "/" "\\\\" WINDOWS_RUN_PATH "${WINDOWS_RUN_PATH}")

SET(Vlc_Plugins_Type
    3dnow
    access
    access_output
    audio_filter
    audio_mixer
    audio_output
    codec
    demux
    meta_engine
    misc
    mmx
    mmxext
    mux
    packetizer
    services_discovery
    sse2
    stream_filter
    stream_out
    text_renderer
    video_chroma
    video_filter
    video_output
)

MACRO(VLC_INSTALL_PLUGINS)
    FOREACH(type ${ARGN})
        FILE(GLOB Vlc_Plugins ${LIBVLC_BIN_DIR}/plugins/${type}/*.dll)
        INSTALL(FILES ${Vlc_Plugins} DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/plugins/${type}")
    ENDFOREACH(type)
ENDMACRO(VLC_INSTALL_PLUGINS)

SET(Vlc_Libs
    ${LIBVLC_BIN_DIR}/libvlc.dll
    ${LIBVLC_BIN_DIR}/libvlccore.dll
)

FILE(GLOB Vlc_Gcc "${LIBVLC_BIN_DIR}/libgcc_s_*-1.dll")
IF(Vlc_Gcc)
    SET(Extra_Libs
        ${Vlc_Gcc}
    )
ENDIF()

FILE(GLOB Vlc_Std "${LIBVLC_BIN_DIR}/libstdc++-6.dll")
IF(Vlc_Std)
    SET(Extra_Libs
        ${Extra_Libs}
        ${LIBVLC_BIN_DIR}/libstdc++-6.dll
        ${LIBVLC_BIN_DIR}/libwinpthread-1.dll
    )
ENDIF()


################
# Installation #
################
INSTALL(FILES ${Vlc_Libs} DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")
IF(Vlc_Gcc OR Vlc_Std)
    INSTALL(FILES ${Extra_Libs} DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")
ENDIF()

VLC_INSTALL_PLUGINS(${Vlc_Plugins_Type})
