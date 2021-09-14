QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./qtpropertybrowser/qtpropertybrowser.pri)

SOURCES += \
    gui/mainwindow.cpp \
    gui/open_file_window.cpp \
    gui/property_browser_widget.cpp \
    gui/view_project_window.cpp \
    main.cpp \
    plotter/plotter.cpp \
    plotter/qcustomplot/qcustomplot.cpp \
    project/project_manager.cpp \
    project/project_struct.cpp \
    project/trace/Trace.cpp \
    project/trace/header_parser.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/open_file_window.h \
    gui/property_browser_widget.h \
    gui/view_project_window.h \
    plotter/plotter.h \
    plotter/qcustomplot/qcustomplot.h \
    project/project_manager.h \
    project/project_struct.h \
    project/trace/Trace.h \
    project/trace/header_parser.h

FORMS += \
    gui/mainwindow.ui \
    gui/open_file_window.ui \
    gui/view_project_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


