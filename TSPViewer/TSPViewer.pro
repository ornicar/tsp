#-------------------------------------------------
#
# Project created by QtCreator 2009-11-09T20:49:07
#
#-------------------------------------------------

TARGET = TSPViewer
TEMPLATE = app
CONFIG += debug_and_release

# sources
SOURCES += *.cpp

INCLUDEPATH += ../TSPLib
HEADERS  += *.h

# gui
RESOURCES = tspviewer.qrc
FORMS    += *.ui

# depencies
LIBS += -L../TSPLib -lTSPLib
