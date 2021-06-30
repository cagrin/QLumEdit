TARGET = QLumEdit
TEMPLATE = app
QT += core gui
qtHaveModule(widgets): QT += widgets
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
RESOURCES +=   application.qrc
TRANSLATIONS +=   translations/qlumedit_pl.ts \
  translations/qlumedit_de.ts
macos: ICON = images/socket.icns
win32: RC_FILE = application.rc