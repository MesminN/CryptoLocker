#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/config/user.hpp>

class FolderService {
public:
    FolderService();
    std::vector<std::string> list_files();

private:
    const std::vector<std::string> get_data_files() const { return m_data_files; }
    void list_files_in_directory(boost::filesystem::path directory);
    std::string m_username;
    std::vector<std::string> m_exclude_paths;
    std::vector<std::string> m_data_files;
};


#endif // FOLDERSERVICE_H
