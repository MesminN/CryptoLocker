#ifndef RSAENCRYPTOR_H
#define RSAENCRYPTOR_H

#include <iostream>
#include <string>
#include <cryptopp/rsa.h>
#include <cryptopp/modes.h>
#include <cryptopp/rdrand.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <boost/filesystem.hpp>

using namespace CryptoPP;
using namespace boost::filesystem;

class RSAEncryptor {
public:
    RSAEncryptor();
    RSAEncryptor(const char *privFilename, const char *pubFilename, unsigned int keyLength = 2048);
    void encryptFile(path filePath);
    void decryptFile(path filePath);

private:
    // methods
    void encodePrivateKey(const std::string& filename, const RSA::PrivateKey& key);
    void encodePublicKey(const std::string& filename, const RSA::PublicKey& key);
    void encode(const std::string& filename, const BufferedTransformation& bt);

    // Attributes
    static const std::string PUBLIC_KEY;
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    byte keyAES[AES::DEFAULT_KEYLENGTH];
    byte ivAES[AES::DEFAULT_KEYLENGTH];
};

#endif // RSAENCRYPTOR_H
