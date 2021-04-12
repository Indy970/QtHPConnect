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
    source/main.cpp \
    source/abstractdata.cpp \
    source/cntfilesystemmodel.cpp \
    source/datamodel.cpp \
    source/errorhandler.cpp \
    source/eventthread.cpp \
    source/eventtimer.cpp \
    source/getnumber.cpp \
    source/hp_infodialog.cpp \
    source/hp_mdilogwindow.cpp \
    source/hp_mditexteditor.cpp \
    source/hp_mdivariableedit.cpp \
    source/hp_mdiwindow.cpp \
    source/hp_settingsdlg.cpp \
    source/hpdata.cpp \
#    source/hptoolbox.cpp \
    source/hptreeitem.cpp \
    source/hpusb.cpp \
    source/mainwindow.cpp \
    source/matrixdata.cpp \
    source/options.cpp \
    source/texteditor.cpp \
    source/treemodel.cpp \
    source/variableview.cpp \
    source/vartablemodel.cpp \
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
    include/options.h \
    include/texteditor.h \
    include/treemodel.h \
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
    hidapi/hidapi.h

FORMS += \
    source/getnumber.ui \
    source/hp_infodialog.ui \
    source/hp_mdiwindow.ui \
    source/hp_settingsdlg.ui \
    source/mainwindow.ui \
    source/options.ui \
    source/variableview.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lhpcalcs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lhpcalcs
else:unix: LIBS +=

#-L$$PWD/../../../../usr/local/lib/ -lhpcalcs

INCLUDEPATH += inc
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/libhpcalcs/include/
INCLUDEPATH += $$PWD/hidapi/

INCLUDEPATH += /usr/include/libusb-1.0
INCLUDEPATH += /usr/local/include
#INCLUDEPATH += /usr/include/linux

DEPENDPATH += /usr/local/include

RESOURCES += \
    qthpconnect.qrc

DISTFILES += \
    icons/about_16x16.png \
    icons/about_32x32.png \
    icons/add_background_16x16.png \
    icons/add_background_22x22.png \
    icons/add_background_32x32.png \
    icons/add_icon_16x16.png \
    icons/add_icon_22x22.png \
    icons/add_icon_32x32.png \
    icons/add_new_16x16.png \
    icons/add_new_22x22.png \
    icons/add_new_32x32.png \
    icons/apps_16x16.png \
    icons/apps_32x32.png \
    icons/backup_16x16.png \
    icons/backup_32x32.png \
    icons/calc_tree_16x16.png \
    icons/calc_tree_22x22.png \
    icons/calc_tree_32x32.png \
    icons/casFolder_16x16.png \
    icons/casFolder_32x32.png \
    icons/casVars_16x16.png \
    icons/casVars_32x32.png \
    icons/chars_16x16.png \
    icons/chars_32x32.png \
    icons/chat_16x16.png \
    icons/chat_22x22.png \
    icons/chat_32x32.png \
    icons/check_update_16x16.png \
    icons/check_update_32x32.png \
    icons/clear_16x16.png \
    icons/clear_32x32.png \
    icons/clone_from_16x16.png \
    icons/clone_from_32x32.png \
    icons/clone_to_16x16.png \
    icons/clone_to_32x32.png \
    icons/complex_16x16.png \
    icons/complex_32x32.png \
    icons/content_16x16.png \
    icons/content_22x22.png \
    icons/content_32x32.png \
    icons/copy_16x16.png \
    icons/copy_32x32.png \
    icons/cut_16x16.png \
    icons/cut_32x32.png \
    icons/delete_16x16.png \
    icons/delete_32x32.png \
    icons/delete_col_16x16.png \
    icons/delete_col_32x32.png \
    icons/delete_row_16x16.png \
    icons/delete_row_32x32.png \
    icons/exam_mode_16x16.png \
    icons/exam_mode_32x32.png \
    icons/fileFolder_16x16.png \
    icons/fileFolder_32x32.png \
    icons/file_16x16.png \
    icons/file_32x32.png \
    icons/firmware_16x16.png \
    icons/firmware_32x32.png \
    icons/help_16x16.png \
    icons/help_32x32.png \
    icons/insert_col_16x16.png \
    icons/insert_col_32x32.png \
    icons/insert_row_16x16.png \
    icons/insert_row_32x32.png \
    icons/internet_16x16.png \
    icons/internet_32x32.png \
    icons/language_16x16.png \
    icons/language_32x32.png \
    icons/list_16x16.png \
    icons/list_32x32.png \
    icons/monitor_16x16.png \
    icons/monitor_22x22.png \
    icons/monitor_32x32.png \
    icons/new_folder_16x16.png \
    icons/new_folder_22x22.png \
    icons/new_folder_32x32.png \
    icons/note_16x16.png \
    icons/note_32x32.png \
    icons/open_16x16.png \
    icons/open_32x32.png \
    icons/paste_16x16.png \
    icons/paste_32x32.png \
    icons/poll_16x16.png \
    icons/poll_32x32.png \
    icons/preferences_16x16.png \
    icons/preferences_32x32.png \
    icons/proctor_16x16.png \
    icons/proctor_22x22.png \
    icons/proctor_32x32.png \
    icons/program_16x16.png \
    icons/program_32x32.png \
    icons/project_16x16.png \
    icons/project_32x32.png \
    icons/properties_16x16.png \
    icons/properties_32x32.png \
    icons/real_16x16.png \
    icons/real_32x32.png \
    icons/refresh_16x16.png \
    icons/refresh_32x32.png \
    icons/rename_16x16.png \
    icons/rename_32x32.png \
    icons/reset_16x16.png \
    icons/reset_32x32.png \
    icons/restore_16x16.png \
    icons/restore_32x32.png \
    icons/results_16x16.png \
    icons/results_32x32.png \
    icons/save_16x16.png \
    icons/save_22x22.png \
    icons/save_32x32.png \
    icons/save_all_16x16.png \
    icons/save_all_22x22.png \
    icons/save_all_32x32.png \
    icons/save_as_16x16.png \
    icons/save_as_32x32.png \
    icons/screenshot_16x16.png \
    icons/screenshot_32x32.png \
    icons/send_16x16.png \
    icons/send_32x32.png \
    icons/start_16x16.png \
    icons/start_32x32.png \
    icons/stop_16x16.png \
    icons/stop_32x32.png \
    icons/table_16x16.png \
    icons/table_32x32.png \
    icons/varFolder_16x16.png \
    icons/varFolder_32x32.png \
    icons/vars_16x16.png \
    icons/vars_32x32.png \
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
