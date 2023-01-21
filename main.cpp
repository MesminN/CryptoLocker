#include <QApplication>
#include <iostream>
#include "view/screenblocker.h"
#include "service/Folderservice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string key = "";

    FolderService folderservice;

    folderservice.list_files();

    ScreenBlocker* screenBlocker = new ScreenBlocker();
    // Set the main window
    screenBlocker->setWindowTitle("My Window");
    screenBlocker->setStyleSheet("background-color: red;");
    // Show the window in fullscreen
    // screenBlocker->setGeometry(QApplication::desktop()->screenGeometry());
    screenBlocker->showFullScreen();
    screenBlocker->show();
    a.exec();

    QString Qkey = screenBlocker->getKey();

    if(Qkey != nullptr) {
        key = Qkey.toStdString();
    }
    std::cout<<key<<std::endl;
    return 0;
}
