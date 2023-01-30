#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/config/user.hpp>
#include "rsaencryptor.h"

class FolderService {
public:
    FolderService();
    void list_files_and_encrypt(RSAEncryptor* encrytpor);
    void list_files_and_decrypt(RSAEncryptor* encrytpor, std::string key);
    const std::vector<std::string> get_data_files() const { return m_data_files; }

private:
    void list_files_in_directory_and_encrypt(boost::filesystem::path directory, RSAEncryptor* encrytpor);
    void list_files_in_directory_and_decrypt(boost::filesystem::path directory, RSAEncryptor* encrytpor, std::string key);
    std::string m_username;
    std::vector<std::string> m_exclude_paths;
    std::vector<std::string> m_data_files;
};


#endif // FOLDERSERVICE_H
