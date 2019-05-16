TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    checknode.cpp \
    variablenode.cpp \
    alist.cc \
    ldpc.cpp

HEADERS += \
    checknode.h \
    variablenode.h \
    alist.h \
    debug.h \
    comm.h \
    ldpc_encoder.h \
    ldpc.h




#unix:!macx: LIBS += -litpp
