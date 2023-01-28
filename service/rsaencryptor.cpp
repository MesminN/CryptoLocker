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

const string RSAEncryptor::SECRET_FILE = "secretkey.txt";
const string RSAEncryptor::IV_FILE = "iv.txt";

RSAEncryptor::RSAEncryptor(const char *secretFile, const char* ivFile, unsigned int keyLength) {
    // generate RSA Keys
    readPublicKeyFromString(PUBLIC_KEY, publicKey);

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

    writeBytes(secretFile, encryptedSecret, cipherSize);
    writeBytes(ivFile, ivAES, sizeof(ivAES));

    delete[] encryptedSecret;
    encryptedSecret = NULL;
}

RSAEncryptor::RSAEncryptor(const char *privFilename, const char *pubFilename,
                           const char *secretFile, const char* ivFile,
                           unsigned int keyLength) {
    params.GenerateRandomWithKeySize(rng, keyLength);

    // generate RSA Keys
    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    writePrivateKeyToFile(privFilename, privateKey);
    writePublicKeyToFile(pubFilename, publicKey);

    // generate AES Keys
    OS_GenerateRandomBlock(true, keyAES, sizeof(keyAES));
    OS_GenerateRandomBlock(true, ivAES, sizeof(ivAES));
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

std::string RSAEncryptor::readBytesAsStringFromFile(const std::string& filePath) {
    std::string result;
    CryptoPP::FileSource fs(filePath.c_str(), true, new HexDecoder(new CryptoPP::StringSink(result)));

    return result;
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

CryptoPP::byte* RSAEncryptor::encryptBytes(const CryptoPP::byte* data, const size_t size, size_t& cipherSize)
{
    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

    cipherSize = encryptor.CiphertextLength(size);
    CryptoPP::byte* ciphertext = new CryptoPP::byte[cipherSize];

    //encryptor.Encrypt(rng, data, size, ciphertext);

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

void RSAEncryptor::loadPrivateKeyAndRetrieveSecret(std::string key) {
    readPrivateKeyFromString(key, privateKey);

    std::string encSecret = readBytesAsStringFromFile(SECRET_FILE);
    std::string iv = readBytesAsStringFromFile(IV_FILE);

    unsigned long long plaintextLength = sizeof(keyAES);
    CryptoPP::byte* secret =
            decryptBytes(reinterpret_cast<const unsigned char *> (encSecret.c_str()), encSecret.size(), plaintextLength);

}

void RSAEncryptor::encryptFile(path filePath) {
    string newFilePath = filePath.string() + ".enc";

    // Open input and output files
    ifstream inFile(filePath.string().c_str(), ios::binary);
    ofstream outFile(newFilePath.c_str(), ios::binary);

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(keyAES, sizeof(keyAES), ivAES);

    try {
        FileSource fs(filePath.filename().c_str(), true,
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
