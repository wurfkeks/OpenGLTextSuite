QT += opengl

DEFINES += QT_FORCE_ASSERT

HEADERS += \
    main.h \
    mainwindow.h \
    oglwidget.h \
    fontrenderer.h \
    qtfontrenderer.h \
    fontstashrenderer.h \
    fontstash/fontstash.h \
    fontstash/glfontstash.h \
    fontstash/stb_truetype.h \
    mylfontrenderer.h \
    lfontrenderer/lfontrenderer.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    oglwidget.cpp \
    fontrenderer.cpp \
    qtfontrenderer.cpp \
    fontstashrenderer.cpp \
    mylfontrenderer.cpp \
    lfontrenderer/lfontrenderer.cpp

FORMS += \
    mainwindow.ui
