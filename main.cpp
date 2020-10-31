#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("qrc:/images/cover_vertical.png");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();
    QTimer::singleShot(50000, &splash, &QWidget::close); // keep displayed for 5 seconds

    MainWindow w;
    w.show();
    return a.exec();
}
