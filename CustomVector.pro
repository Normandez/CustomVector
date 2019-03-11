TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += $${PWD}/depends/gtest/lib/googletest.lib
INCLUDEPATH += $${PWD}/depends/gtest/include

HEADERS += CVector.h

SOURCES += main.cpp
