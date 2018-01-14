#-------------------------------------------------
#
# Project created by QtCreator 2017-01-01T02:22:36
#
#-------------------------------------------------

QT       += core gui sql gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zclip
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(external/qxtglobalshortcut5/qxt.pri)

SOURCES += main.cpp\
    CClipboardManagerTray.cpp \
    CHistoryDialog.cpp

HEADERS  += \
    CClipboardManagerTray.hpp \
    Constants.hpp \
    CHistoryDialog.hpp

RESOURCES += \
    resources.qrc

FORMS += \
    CHistoryDialog.ui
