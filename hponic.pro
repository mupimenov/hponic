#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T07:11:18
#
#-------------------------------------------------

QT       += core gui serialport sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hponic
TEMPLATE = app


SOURCES += main.cpp\
        widget_main.cpp \
    widget_ioslots.cpp \
    ioslot.cpp \
    hponic.cpp \
    ioslots_model.cpp \
    widget_config_slot.cpp \
    program.cpp \
    programs_model.cpp \
    ioslots_xml_composer.cpp \
    widget_programs.cpp \
    widget_config_program.cpp \
    command.cpp \
    interface.cpp \
    transmission.cpp \
    widget_config_transmission.cpp \
    widget_monitoring.cpp \
    ioslot_manager.cpp \
    program_manager.cpp \
    monitoring.cpp \
    monitoring_model.cpp \
    monitoring_filter_proxy.cpp \
    database_service.cpp \
    widget_database.cpp \
    widget_plot.cpp \
    ioslots_bin_composer.cpp \
    programs_bin_composer.cpp \
    programs_xml_composer.cpp

HEADERS  += widget_main.h \
    widget_ioslots.h \
    ioslot.h \
    hponic.h \
    ioslots_model.h \
    widget_config_slot.h \
    program.h \
    programs_model.h \
    ioslots_xml_composer.h \
    widget_programs.h \
    widget_config_program.h \
    command.h \
    interface.h \
    serialport_settings.h \
    transmission.h \
    hponic_commands.h \
    widget_config_transmission.h \
    widget_monitoring.h \
    ioslot_manager.h \
    program_manager.h \
    monitoring.h \
    monitoring_model.h \
    monitoring_filter_proxy.h \
    database_service.h \
    widget_database.h \
    widget_plot.h \
    ioslots_bin_composer.h \
    programs_bin_composer.h \
    programs_xml_composer.h

FORMS    += widget_main.ui \
    widget_ioslots.ui \
    widget_programs.ui \
    widget_config_transmission.ui \
    widget_monitoring.ui \
    widget_database.ui \
    widget_plot.ui

QWT_ROOT = /usr/local/qwt-6.1.2
include( $${QWT_ROOT}/features/qwt.prf )

QWT_OUT_ROOT = $${QWT_ROOT}

INCLUDEPATH += $${QWT_ROOT}/include
DEPENDPATH  += $${QWT_ROOT}/include

QMAKE_RPATHDIR *= $${QWT_OUT_ROOT}/lib
qwtAddLibrary($${QWT_OUT_ROOT}/lib, qwt)

greaterThan(QT_MAJOR_VERSION, 4) {

    QT += printsupport
    QT += concurrent
}

contains(QWT_CONFIG, QwtOpenGL ) {

    QT += opengl
}
else {

    DEFINES += QWT_NO_OPENGL
}

win32 {
    contains(QWT_CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
    }
}
