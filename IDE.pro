QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clickablelabel.cpp \
    codeeditor.cpp \
    dockersettings.cpp \
    languagedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    newclass.cpp \
    newfile.cpp \
    screensaver.cpp \
    settingslanguage.cpp \
    settingsproject.cpp \
    settingswindow.cpp \
    syntaxhighlighter.cpp

HEADERS += \
    applicationstyle.h \
    clickablelabel.h \
    codeeditor.h \
    dockersettings.h \
    hub.h \
    languagedialog.h \
    mainwindow.h \
    newclass.h \
    newfile.h \
    screensaver.h \
    settingslanguage.h \
    settingsproject.h \
    settingswindow.h \
    syntaxhighlighter.h

FORMS += \
    dockersettings.ui \
    languagedialog.ui \
    mainwindow.ui \
    newclass.ui \
    newfile.ui \
    screensaver.ui \
    settingslanguage.ui \
    settingsproject.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES +=
