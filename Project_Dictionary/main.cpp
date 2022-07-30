#include "GUI/mainwindow.h"

#include <QApplication>

QString splashCut(QString t){
    QString tmp = "";
    QList a = t.split('/');
    for (int i = 1; i < a.size()-3; i++) {
        tmp += "/" + a[i];
    }
    return tmp;
}

bool copyRes(QString path1, QString path) {
    std::filesystem::copy((path1 + "/resources").toStdString(), (path + "/resources").toStdString());

    return true;
}

bool OSXHandle(QString path1, QString path) {
    if (!std::filesystem::exists((path + "/resources").toStdString())) {
         while (copyRes(path1, path) == false) {}
    }
    if (!std::filesystem::exists((path + "/dataset").toStdString())) {
         std::filesystem::copy((path1 + "/dataset").toStdString(), (path + "/dataset").toStdString());
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if __APPLE__
    QString path = a.applicationDirPath();
    OSXHandle(splashCut(path), path);
#endif

    mainpage w;

#if __APPLE__
    w.setUnifiedTitleAndToolBarOnMac(true); // activate Mac-style toolbar
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.setWindowOpacity(0.985);
    //w.setAttribute(Qt::WA_MacShowFocusRect, 1);
#endif
    w.setWindowTitle("");

    w.show();
    return a.exec();
}
