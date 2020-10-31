QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql  network quickwidgets
#androidextras

CONFIG += c++11



SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qmllistitem.cpp \
    qmllistobject.cpp

HEADERS += \
    mainwindow.h \
    qmllistitem.h \
    qmllistobject.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/http

ANDROID_PACKAGE_SOURCE_DIR = /home/yusuf/Documents/workspace/mobicine
DISTFILES += \
    QtAndroidToastJava.java \

INSTALLS += target
RESOURCES += \
    images/ \
    images/images.qrc
    /

DISTFILES += \
    AndroidManifest.xml \
    QmlListObjectWidget.qml \
    database \
    images/add_pressed.png \
    images/add_unpressed.png \
    images/delete_pressed.png \
    images/delete_unpressed.png \
    images/down_pressed.png \
    images/down_unpressed.png \
    images/laser_locked.png \
    images/laser_unlocked.png \
    images/left_pressed.png \
    images/left_unpressed.png \
    images/pass_pressed.png \
    images/pass_previous_pressed.png \
    images/pass_previous_unpressed.png \
    images/pass_unpressed.png \
    images/refresh_pressed.png \
    images/refresh_unpressed.png \
    images/right_pressed.png \
    images/right_unpressed.png \
    images/show_pressed.png \
    images/show_unpressed.png \
    images/start_pressed.png \
    images/start_unpressed.png \
    images/stop_pressed.png \
    images/stop_unpressed.png \
    images/template.png \
    images/toggle_button_off.png \
    images/toggle_button_on.png \
    images/up_pressed.png \
    images/up_unpressed.png \
    images/wifi_locked.png \
    images/wifi_unlocked.png \
    res/drawable-hdpi/icon.png \
    res/drawable-hdpi/logo.png \
    res/drawable-hdpi/logo_landscape.png \
    res/drawable-hdpi/logo_portrait.png \
    res/drawable-ldpi/icon.png \
    res/drawable-ldpi/logo.png \
    res/drawable-ldpi/logo_landscape.png \
    res/drawable-ldpi/logo_portrait.png \
    res/drawable-mdpi/icon.png \
    res/drawable-mdpi/logo.png \
    res/drawable-mdpi/logo_landscape.png \
    res/drawable-mdpi/logo_portrait.png \
    rest_api/mobicine.php \
    widget.qml