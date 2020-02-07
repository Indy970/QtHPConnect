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
    abstractdata.cpp \
    cntfilesystemmodel.cpp \
    datamodel.cpp \
    errorhandler.cpp \
    eventthread.cpp \
    eventtimer.cpp \
    getnumber.cpp \
    hp_infodialog.cpp \
    hp_mdilogwindow.cpp \
    hp_mditexteditor.cpp \
    hp_mdivariableedit.cpp \
    hp_mdiwindow.cpp \
    hp_settingsdlg.cpp \
    hpdata.cpp \
#    hptoolbox.cpp \
    hptreeitem.cpp \
    hpusb.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixdata.cpp \
    options.cpp \
    texteditor.cpp \
    treemodel.cpp \
    variableview.cpp \
    vartablemodel.cpp \
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
    libhpcalcs/src/utils.c

HEADERS += \
    hidapi/hidapi.h \
    include/abstractdata.h \
    include/cntfilesystemmodel.h \
    include/datamodel.h \
    include/errorhandler.h \
    include/eventthread.h \
    include/eventtimer.h \
    include/getnumber.h \
    include/global.h \
    include/hp_infodialog.h \
    include/hp_mdilogwindow.h \
    include/hp_mditexteditor.h \
    include/hp_mdivariableedit.h \
    include/hp_mdiwindow.h \
    include/hp_settingsdlg.h \
    include/hp_typedef.h \
    include/hpdata.h \
    include/hpinterface.h \
    include/hptreeitem.h \
    include/hpusb.h \
    include/main.h \
    include/mainwindow.h \
    include/matrixdata.h \
    include/moc_predefs.h \
    include/options.h \
    include/texteditor.h \
    include/treemodel.h \
    include/ui_getnumber.h \
    include/ui_hp_infodialog.h \
    include/ui_hp_mdiwindow.h \
    include/ui_hp_settingsdlg.h \
    include/ui_mainwindow.h \
    include/ui_options.h \
    include/ui_variableview.h \
    include/variableview.h \
    include/vartablemodel.h \
    include/version.h \
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
    libhpcalcs/include/utils.h

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
INCLUDEPATH += $$PWD/include
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
