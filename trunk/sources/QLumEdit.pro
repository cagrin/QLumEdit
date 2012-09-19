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
INSTALLS +=   sources \
  target
RC_FILE +=   application.rc
RESOURCES +=   application.qrc
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
TEMPLATE =   app
sources.files +=   $$FORMS \
  $$HEADERS \
  $$RESOURCES \
  $$SOURCES \
  application.pro \
  images
sources.path +=   $$[QT_INSTALL_EXAMPLES]/mainwindows/application
target.path +=   $$[QT_INSTALL_EXAMPLES]/mainwindows/application
CONFIG +=   release \
  warn_on \
  debug_i_release
TRANSLATIONS +=   translations/qlumedit_pl.ts \
  translations/qlumedit_de.ts
