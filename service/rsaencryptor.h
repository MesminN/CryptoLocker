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
    // Constants
    static const std::string SECRET_FILE;
    static const std::string IV_FILE;

    // Constructors
    RSAEncryptor(const char *secretFile, const char* ivFile, unsigned int keyLength = 2048);

    //Methods
    void encryptFile(path filePath);
    void decryptFile(path filePath);
    void loadPrivateKeyAndRetrieveSecret(std::string key);

private:
    // methods
    void writePublicKeyToFile(const std::string& filepath, const CryptoPP::RSA::PublicKey& key);
    void writePrivateKeyToFile(const std::string& filepath, const CryptoPP::RSA::PrivateKey& key);
    void readPublicKeyFromString(const std::string& key_string, CryptoPP::RSA::PublicKey& key);
    void readPrivateKeyFromString(const std::string& key_string, CryptoPP::RSA::PrivateKey& key);
    void writeBytesToFile(const std::string& filepath, const byte* bytes, const size_t size);
    void readBytesFromFile(const std::string& filepath, CryptoPP::byte* bytes, const size_t size);
    CryptoPP::byte* encryptBytes(const CryptoPP::byte* data, const size_t size, size_t& cipherSize);
    CryptoPP::byte* decryptBytes(const CryptoPP::byte* ciphertext, const size_t cipherSize, size_t& plainSize);
    std::string readBytesAsStringFromFile(const std::string& filePath, std::string s);
    void writeBytes(const std::string& filePath, CryptoPP::byte* bytes, const size_t size);

    // Constants
    static const std::string PUBLIC_KEY;

    //Attributes
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    RSA::PrivateKey privateKey;
    RSA::PublicKey publicKey;
    CryptoPP::byte keyAES[AES::DEFAULT_KEYLENGTH] = {0};
    CryptoPP::byte ivAES[AES::BLOCKSIZE] = {0};
};

#endif // RSAENCRYPTOR_H
