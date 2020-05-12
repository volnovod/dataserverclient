QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    TCPLink.h \
    datawriter.h

SOURCES += \
        TCPLink.cpp \
        datawriter.cpp \
        main.cpp
# Setup our build directories
CONFIG(debug, debug|release) {
#    message(Debug flavor)
    CONFIG += DebugBuild
} else:CONFIG(release, debug|release) {
#    message(Release flavor)
    CONFIG += ReleaseBuild
} else {
    error(Unsupported build flavor)
}

# Setup our build directories
BASEDIR = $${IN_PWD}
DebugBuild {
    DESTDIR = $${OUT_PWD}/debug
    BUILDDIR = $${OUT_PWD}/build-debug
}
ReleaseBuild {
    DESTDIR = $${OUT_PWD}/release
    BUILDDIR = $${OUT_PWD}/build-release
}
OBJECTS_DIR = $${BUILDDIR}/obj
MOC_DIR = $${BUILDDIR}/moc
UI_DIR = $${BUILDDIR}/ui
RCC_DIR = $${BUILDDIR}/rcc

# Specify multi-process compilation within Visual Studio.
QMAKE_CXXFLAGS_DEBUG += -MP
QMAKE_CXXFLAGS_RELEASE += -MP
# Use link time code generation for beteer optimization (I believe this is supported in msvc express, but not 100% sure)
QMAKE_LFLAGS += /LTCG
QMAKE_CFLAGS += -GL

# Copy runtime files
QMAKE_POST_LINK += "$(QTDIR)\\BIN\\windeployqt" $$DESTDIR

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

