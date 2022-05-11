QT       += core gui network positioning widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    highlighter.cpp \
    settingswidget.cpp \
    tree.cpp

HEADERS += \
    mainwindow.h \
    highlighter.h \
    settingswidget.h \
    tree.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    stylesheet.qrc

TRANSLATIONS += QtLanguage_by.ts
CODECFORSRC = UTF-8
