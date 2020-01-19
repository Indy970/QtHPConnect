#-------------------------------------------------
#
# Project created by QtCreator 2019-01-21T20:42:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtHPConnect
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

VERSION_MAJOR = HP_MAJOR_VERSION
VERSION_MINOR = HP_MINOR_VERSION
VERSION_PATCH = HP_MINOR_PATCH
VERSION_BUILD = HP_MINOR_BUILD

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}$${VERSION_BUILD}

#QMAKE_CFLAGS += -Wno-unused-parameter -Werror=shadow -Werror=write-strings -Werror=redundant-decls -Werror=format -Werror=format-nonliteral -Werror=date-time -Werror=missing-prototypes -Werror=pointer-arith -Wunreachable-code -Werror=format-security -Werror=declaration-after-statement -Werror=implicit-function-declaration -Werror=return-type -D_FORTIFY_SOURCE=2 -D__LINUX__ -fvisibility=hidden
#QMAKE_CXXFLAGS += -include cctype -include cstdlib
#QMAKE_CXXFLAGS += -Wno-unused-parameter -Werror=shadow -Werror=write-strings -Werror=redundant-decls -Werror=format -Werror=format-nonliteral -Werror=date-time -Werror=missing-prototypes -Werror=pointer-arith -Wunreachable-code -Werror=format-security -Werror=declaration-after-statement -Werror=implicit-function-declaration -Werror=return-type -D_FORTIFY_SOURCE=2 -D__LINUX__ -fvisibility=hidden

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    mainwindow.cpp \
    main.cpp \
    errorhandler.cpp \
    libhpcalcs/src/calc_none.c \
    libhpcalcs/src/calc_prime.c \
    libhpcalcs/src/error.c \
    libhpcalcs/src/filetypes.c \
    libhpcalcs/src/hpcables.c \
    libhpcalcs/src/hpcalcs.c \
    libhpcalcs/src/hpfiles.c \
    libhpcalcs/src/hpopers.c \
    libhpcalcs/src/link_nul.c \
    libhpcalcs/src/link_prime_hid.c \
    libhpcalcs/src/logging.c \
    libhpcalcs/src/prime_cmd.c \
    libhpcalcs/src/prime_rpkt.c \
    libhpcalcs/src/prime_vpkt.c \
    libhpcalcs/src/type2str.c \
    libhpcalcs/src/typesprime.c \
    libhpcalcs/src/utils.c \
    datamodel.cpp \
    treemodel.cpp \
    hpdata.cpp \
    texteditor.cpp \
    variableview.cpp \
    hptreeitem.cpp \
    hp_mdiwindow.cpp \
    hp_mditexteditor.cpp \
    hp_mdivariableedit.cpp \
    vartablemodel.cpp \
    hp_infodialog.cpp \
    getnumber.cpp \
    hpusb.cpp \
    hp_settingsdlg.cpp \
    cntfilesystemmodel.cpp \
    abstractdata.cpp \
    matrixdata.cpp \
    options.cpp \
    eventtimer.cpp \
    eventthread.cpp

HEADERS += \
        mainwindow.h \
        main.h \
    global.h \
    hidapi/hidapi.h \
    errorhandler.h \
    libhpcalcs/include/error.h \
    libhpcalcs/include/export.h \
    libhpcalcs/include/filetypes.h \
    libhpcalcs/include/gettext.h \
    libhpcalcs/include/hpcables.h \
    libhpcalcs/include/hpcalcs.h \
    libhpcalcs/include/hpfiles.h \
    libhpcalcs/include/hplibs.h \
    libhpcalcs/include/hpopers.h \
    libhpcalcs/include/internal.h \
    libhpcalcs/include/logging.h \
    libhpcalcs/include/prime_cmd.h \
    libhpcalcs/include/typesprime.h \
    libhpcalcs/include/utils.h \
    datamodel.h \
    treemodel.h \
    hpdata.h \
    texteditor.h \
    variableview.h \
    hptreeitem.h \
    hp_mdiwindow.h \
    hp_mditexteditor.h \
    hp_mdivariableedit.h \
    vartablemodel.h \
    hp_infodialog.h \
    getnumber.h \
    hpusb.h \
    hp_settingsdlg.h \
    version.h \
    cntfilesystemmodel.h \
    abstractdata.h \
    matrixdata.h \
    options.h \
    hp_typedef.h \
    eventtimer.h \
    eventthread.h \
    eventthread.h

FORMS += \
    mainwindow.ui \
    variableview.ui \
    hp_mdiwindow.ui \
    getnumber.ui \
    hp_infodialog.ui \
    hp_settingsdlg.ui \
    options.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lhpcalcs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lhpcalcs
else:unix: LIBS +=

#-L$$PWD/../../../../usr/local/lib/ -lhpcalcs

INCLUDEPATH += $$PWD/../../../../usr/local/include
INCLUDEPATH += $$PWD/libhpcalcs/include/
INCLUDEPATH += $$PWD/hidapi/

INCLUDEPATH += /usr/include/libusb-1.0


DEPENDPATH += $$PWD/../../../../usr/local/include

RESOURCES += \
    qthpconnect.qrc

DISTFILES += \
    model.qmodel \
    README.md \
    LICENSE \
    CHANGELOG.md \
    NEWS \
    AUTHORS \
    Notes \
    TODO.md
   ../hplp-master/libhpcalcs/tests/test_hpcalcs.c

unix|win32: LIBS += -lhidapi-libusb

unix|win32: LIBS += -lusb-1.0
