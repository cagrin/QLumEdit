QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QLumEdit
TEMPLATE = app
SOURCES +=   eulumdat.cpp \
  eulumdatwidget.cpp \
  eulumdatwidget2.cpp \
  main.cpp \
  maintabwidget.cpp \
  mainwindow.cpp \
  eulumdatwidget3.cpp \
  eulumdatwidget4.cpp \
  eulumdattablewidget.cpp \
  renderwidget.cpp \
  configtabdialog.cpp \
  vars.cpp \
  validator.cpp \
  eulumdatcalculate.cpp \
  newdialog.cpp \
  exportto.cpp
HEADERS +=   eulumdat.h \
  eulumdatwidget.h \
  eulumdatwidget2.h \
  maintabwidget.h \
  mainwindow.h \
  eulumdatwidget3.h \
  eulumdatwidget4.h \
  eulumdattablewidget.h \
  renderwidget.h \
  configtabdialog.h \
  vars.h \
  validator.h \
  newdialog.h
CONFIG += mobility
MOBILITY = 
RC_FILE +=   application.rc
RESOURCES +=   application.qrc
TRANSLATIONS +=   translations/qlumedit_pl.ts \
  translations/qlumedit_de.ts
