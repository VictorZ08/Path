include (C:/Users/Victor/Documents/Path/libs/QtXlsx/QtXlsxWriter-master/QtXlsxWriter-master/src/xlsx/qtxlsx.pri)

QT       += core gui
QT       += sql

#QMAKE_CXXFLAGS+=-Zi
#QMAKE_LFLAGS+=/DEBUG

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    generator/generatoraepwidget.cpp \
    generator/generatorpemiwidget.cpp \
    generator/report/reportaepwidget.cpp \
    generator/table/tabledata.cpp \
    generator/table/tablesetswidget.cpp \
    main.cpp \
    modes/modeswidget.cpp \
    modes/passwidget.cpp \
    modes/systemtray.cpp \
    random/random.cpp \
    search/database.cpp \
    search/find.cpp \
    search/searchfileswidget.cpp \
    set.cpp \
    sortfiles.cpp \
    style.cpp \
    timer/timer.cpp \
    timer/timeraepwidget.cpp \
    timer/timerinterface.cpp \
    timer/timerpemiwidget.cpp \
    treeitem.cpp \
    verification/dataverification.cpp \
    verification/logger.cpp


HEADERS += \
    chronometr.h \
    generator/generatoraepwidget.h \
    generator/generatorpemiwidget.h \
    generator/report/reportaepwidget.h \
    generator/table/tabledata.h \
    generator/table/tablesetswidget.h \
    modes/modeswidget.h \
    modes/passwidget.h \
    modes/systemtray.h \
    random/random.h \
    random/randomvalues.h \
    search/database.h \
    search/find.h \
    search/searchfileswidget.h \
    set.h \
    sortfiles.h \
    style.h \
    timer/timer.h \
    timer/timeraepwidget.h \
    timer/timerinterface.h \
    timer/timerpemiwidget.h \
    treeitem.h \
    verification/dataverification.h \
    verification/logger.h


FORMS += \
    generator/generatoraepwidget.ui \
    generator/generatorpemiwidget.ui \
    generator/report/reportaepwidget.ui \
    generator/table/tablesetswidget.ui \
    modes/modeswidget.ui \
    modes/passwidget.ui \
    search/searchfileswidget.ui \
    timer/timeraepwidget.ui \
    timer/timerpemiwidget.ui \
    verification/logger.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    path.qrc \

DISTFILES +=
