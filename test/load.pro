TEMPLATE = app
CONFIG += console c++11
CONFIG += app_bundle
CONFIG += qt

SOURCES += main.cpp \
    load.cpp \
    vector.cpp \
    smart_ptr.cpp \
    operation.cpp

HEADERS += \
    vector.h \
    smart_ptr.h \
    vector.hpp \
    exceptions.hpp \
    utility.hpp \
    map.hpp \
    list.hpp \
    memory.hpp

DISTFILES += \
    operation.out
