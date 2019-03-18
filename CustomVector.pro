CONFIG -= qt
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

LIBS += $${PWD}/depends/gtest/lib/googletest.lib
INCLUDEPATH += $${PWD}/depends/gtest/include

HEADERS += CVector.h

SOURCES += main.cpp
