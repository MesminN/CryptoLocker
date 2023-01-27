#include "rsaencryptor.h"


using namespace std;

const string RSAEncryptor::PUBLIC_KEY =
        "308201080282010100AF02F3C61A7A89337243334EC9DCFDA7"
        "4E958BA09377D178DC2016DDCCFD380BF68111648AB2467141"
        "5169048D768A7BDDC26D007DD447A72A2F55E19E2D9C791128"
        "29D8674EA18B5D67D1FEFD0915271813DC05FB650E9E07F6AF"
        "2873E0E0E5EC51C9353F0E02E81445041C7411497ECD33816D"
        "4A963943BE303DC5584E880D564C3CB41AA0AC9C4598E3AED2"
        "3A1F244DF655EA3A82A6FB95E754382784DC8EAED27C76DDC3"
        "B9015ED1E66BAAC71274803090EC5BAC708B8D52D925BA2706"
        "7A468B3492430E16BEE954D12390019902B51259EB6816744C"
        "55328FC0334484AD6D7655570949D35A44273DD69D4F768A07"
        "97215A7DC8732DD37EC51008BB3085020111";

RSAEncryptor::RSAEncryptor() {
    RSAEncryptor("private.key", "public.key");
}

RSAEncryptor::RSAEncryptor(const char *privFilename, const char *pubFilename, unsigned int keyLength) {
    params.GenerateRandomWithKeySize(rng, keyLength);

    // generate RSA Keys
    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    encodePrivateKey(privFilename, privateKey);
    encodePublicKey(pubFilename, publicKey);

    // generate RSA Keys
    OS_GenerateRandomBlock(true, keyAES, sizeof(keyAES));
    OS_GenerateRandomBlock(true, ivAES, sizeof(ivAES));
}

void RSAEncryptor::encodePrivateKey(const string& filename, const RSA::PrivateKey& key)
{
    ByteQueue queue;
    key.DEREncodePrivateKey(queue);

    encode(filename, queue);
}

void RSAEncryptor::encodePublicKey(const string& filename, const RSA::PublicKey& key)
{
    ByteQueue queue;

    key.DEREncodePublicKey(queue);

    encode(filename, queue);
}

void RSAEncryptor::encode(const string& filename, const BufferedTransformation& bt)
{
    HexEncoder keySink(new FileSink(filename.c_str()));

    bt.CopyTo(keySink);
    keySink.MessageEnd();
}

void RSAEncryptor::encryptFile(path filePath) {
    string newFilePath = filePath.string() + ".enc";

    // Open input and output files
    ifstream inFile(filePath.string().c_str(), ios::binary);
    ofstream outFile(newFilePath.c_str(), ios::binary);

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(keyAES, sizeof(keyAES), ivAES);

    try {
        FileSource fs( filePath.filename().c_str(), true,
            new StreamTransformationFilter( encryptor,
                new FileSink( newFilePath.c_str() )
            )
        );
    } catch(Exception ex) {
        cout<<ex.what()<<endl;
    }
}

void RSAEncryptor::decryptFile(path filePath) {
    string newFilePath = "x_" + filePath.string().substr(0, filePath.size() - 4);

    // Open input and output files
    ifstream inFile(filePath.string().c_str(), ios::binary);
    ofstream outFile(newFilePath.c_str(), ios::binary);

    CBC_Mode<AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(keyAES, sizeof(keyAES), ivAES);

    try {
        FileSource fs( filePath.filename().c_str(), true,
            new StreamTransformationFilter( decryptor,
                new FileSink( newFilePath.c_str() )
            )
        );
    } catch(Exception ex) {
        cout<<ex.what()<<endl;
    }
}
