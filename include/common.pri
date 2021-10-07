CONFIG += accessible
DEFINES += DUI_ACCESSIBLE

include(buildoption.pri)

FRAMEWORK_PATH = ../../../sdk
 
INCLUDEPATH += $${FRAMEWORK_PATH}/include
INCLUDEPATH += $${FRAMEWORK_PATH}/include/yyvideosdk
INCLUDEPATH += ../../include

CONFIG(debug, debug|release) {
    QMAKE_LIBDIR += $${FRAMEWORK_PATH}/lib/debug
}
else {
    QMAKE_LIBDIR += $${FRAMEWORK_PATH}/lib/release
}

QMAKE_LIBDIR += "$(TargetDir)"

CONFIG(debug, debug|release) {
    DESTDIR = ../../bin/debug
}
else {
    DESTDIR = ../../bin/release
}

CharacterSet = 1
