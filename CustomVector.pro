CONFIG -= qt
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

LIBS += $${PWD}/depends/gtest/lib/Qt5.9.2/googletest.lib
INCLUDEPATH += $${PWD}/depends/gtest/include

HEADERS += CVector.h

HEADERS += UnitTest.h

SOURCES += main.cpp
