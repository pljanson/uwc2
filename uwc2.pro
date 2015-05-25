#-------------------------------------------------
#
# Project created by QtCreator 2015-01-16T19:37:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uwc2
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    MainWindow.cpp \
    cWebshots/Collection.cpp \
    cWebshots/Datum.cpp \
    cWebshots/MetaData.cpp \
    cWebshots/Picture.cpp \
    cWebshots/TXT.cpp \
    cWebshots/WsMisc.cpp \
    CollectionModel.cpp \
    CollectionWindow.cpp \
    LoadingDialog.cpp \
    MetaDataDialog.cpp \
    MetaDataWidget.cpp \
    Misc.cpp \
    PreviewWidget.cpp \
    PropertiesWidget.cpp \
    QLinkedAction.cpp \
    Exportwindow.cpp \
    ExportArchive.cpp \
    ClickableLabel.cpp

HEADERS  += \
    MainWindow.h \
    cWebshots/Collection.h \
    cWebshots/Datum.h \
    cWebshots/Exceptions.h \
    cWebshots/MetaData.h \
    cWebshots/Picture.h \
    cWebshots/TXT.h \
    cWebshots/WsMisc.h \
    CollectionModel.h \
    CollectionWindow.h \
    LoadingDialog.h \
    MetaDataDialog.h \
    MetaDataWidget.h \
    Misc.h \
    PreviewWidget.h \
    PropertiesWidget.h \
    QLinkedAction.h \
    Exportwindow.h \
    ExportArchive.h \
    ClickableLabel.h

DISTFILES += \
    plj-roadmap.txt \
    cWebshots/ChangeLog.txt \
    cWebshots/GNU General Public License.txt \
    cWebshots/README.md

FORMS += \
    LoadingDialog.ui

RESOURCES = ressources.qrc
