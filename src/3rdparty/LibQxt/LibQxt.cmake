# Files that make up LibQxt

set(screencloud_SOURCES ${screencloud_SOURCES}
    src/3rdparty/LibQxt/qxtglobalshortcut.cpp
    src/3rdparty/LibQxt/qxtwindowsystem.cpp
)

set(screencloud_HEADERS_MOC ${screencloud_HEADERS_MOC}
	src/3rdparty/LibQxt/qxtglobal.h
    src/3rdparty/LibQxt/qxtglobalshortcut.h
    src/3rdparty/LibQxt/qxtglobalshortcut_p.h
    src/3rdparty/LibQxt/qxtwindowsystem.h
)

if(WIN32)
    set(screencloud_SOURCES ${screencloud_SOURCES}
    src/3rdparty/LibQxt/win/qxtwindowsystem_win.cpp
    src/3rdparty/LibQxt/win/qxtglobalshortcut_win.cpp
    )
elseif(APPLE)
    set(screencloud_SOURCES ${screencloud_SOURCES}
    src/3rdparty/LibQxt/mac/qxtwindowsystem_mac.cpp
    src/3rdparty/LibQxt/mac/qxtglobalshortcut_mac.cpp
    )
    set(screencloud_HEADERS ${screencloud_HEADERS}
        src/3rdparty/LibQxt/mac/qxtwindowsystem_mac.h
    )
else()
    set(screencloud_SOURCES ${screencloud_SOURCES}
    src/3rdparty/LibQxt/linux/qxtwindowsystem_linux.cpp
    src/3rdparty/LibQxt/linux/qxtglobalshortcut_linux.cpp
    )
endif() 

# Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/LibQxt)

#Required libs
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    set(REQUIRED_LIBS ${REQUIRED_LIBS}
      ${X11_LIBRARIES}
    )
    set(KF_MIN_VERSION "5.88.0")
    find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
    find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS GlobalAccel)
    include_directories(KF5::GlobalAccel)
    set(REQUIRED_LIBS ${REQUIRED_LIBS}
            KF5::GlobalAccel
            )
elseif(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    set(REQUIRED_LIBS ${REQUIRED_LIBS}
      ${CARBON_LIBRARY}
    )
endif()

#We need access to the private gui api when using qt5
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

#Fixes some dllimport errors on windows
add_definitions(-DQXT_STATIC=1)
