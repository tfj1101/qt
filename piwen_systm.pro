QT       += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# include(D:QtXlsx/QtXlsxWriter-master/src/qtxlsx.pri)

# 添加Windows SDK库路径
# win32 {
#     INCLUDEPATH += "D:\\Windows Kits\\10\\Include\\10.0.22621.0\\winrt"
#     LIBS += -lwindowsapp
# }

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ApprovalDialog.cpp \
    ApprovalWorkflow.cpp \
    DocumentApplyDialog.cpp \
    DocumentDetailsDialog.cpp \
    DocumentManager.cpp \
    DocumentWidget.cpp \
    StatusColorDelegate.cpp \
    WorkflowConfigDialog.cpp \
    dbmanager.cpp \
    login_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    rolemanagementdialog.cpp \
    user.cpp \
    userwidget.cpp

HEADERS += \
    ApprovalDialog.h \
    ApprovalWorkflow.h \
    DocumentApplyDialog.h \
    DocumentDetailsDialog.h \
    DocumentManager.h \
    DocumentWidget.h \
    StatusColorDelegate.h \
    WorkflowConfigDialog.h \
    dbmanager.h \
    login_dialog.h \
    mainwindow.h \
    rolemanagementdialog.h \
    user.h \
    userwidget.h

FORMS += \
    login_dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    approvals.sql \
    document_status.sql \
    documents.sql \
    permissions.json

RESOURCES += \
    res.qrc
