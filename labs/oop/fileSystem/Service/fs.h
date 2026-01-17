//
// Created by pavel on 21.11.2025.
//

#ifndef INC_3_FS_H
#define INC_3_FS_H

#include <vector>
#include <bitset>



#include "../Entity/desc.h"
#include "../Entity/dirDesc.h"
#include "../Entity/spDesc.h"
#include "../Table/orderedTable.h"
#include "../Controller/entityDto.h"



class FsTest;

class Fs {
    DirDesc* root;

    /**
     * @searching for any public methods
     * @param path full path to the file or directory
     * @return last name from the path and pointer to directory which will contain it
     */
    std::pair<std::string_view, DirDesc*> search(const std::string_view& path) const; //абсолютное (маршрутное) имя, используется public функциями для поиска
    static std::vector<std::string_view> parse(const std::string_view &path); //static не работает с членами класса и не требует создания объекта класса
    static std::bitset<3> parse_permissions(const std::string_view &permissions);
    static std::time_t parse_time(const std::string &time);
    static Mode parse_mode(const std::string_view &mode);
    static std::string timeToString(std::time_t t);
    static int get_random_int(int min, int max);
    static std::string get_random_string(size_t length);

    /**
     * upload content of one table to the file
     * @param table
     * @param file
     */
    //static void uploadTable(OrderedTable<std::string, Desc*> *table, std::ofstream& file);

    friend FsTest;
public:
    Fs() : root(new DirDesc()) {}

    ~Fs();

    /**
     * @brief search file with such name in all directories in single mode
     * @param name relative file name
     */
    std::vector<EntityDto> find(const std::string &name) const;

    /**
     * @brief search file with such name in all directories in multithreaded mode
     * @param name relative file name
     * @return
     */
    std::vector<EntityDto> find_threads(const std::string &name) const;


    /**
     * @brief create a new file
     * @details if file already exist do nothing
     * you must:
     * have appropriate permissions
     * assign correct path
     * @param path complete path from the root to the file
     * @param perm for a new file
     * @param size of file in bytes
     * @param creation_time
     * @param modification_time
     */
    void touch(const std::string &path, const std::string &perm, size_t size, const std::string &creation_time, const std::string &modification_time) const;
    void touch(const std::string &path, const std::string &perm) const {touch(path, perm, 0, timeToString(std::time(nullptr)), timeToString(std::time(nullptr)));}
    void touch(const std::string &path) const {touch(path, "7");}



    void device(const std::string &path, const std::string &perm, size_t size, const std::string &mode);
    void device(const std::string &path, const std::string &perm) {device(path, perm, 0, "Character");}
    void device(const std::string &path) {device(path, "7", 0, "Character");}

    /**
     * @brief create a new empty directory
     * @details if directory already exist do nothing
     * you must:
     * have appropriate permissions
     * assign correct path
     * @param path complete path from the root to the directory
     * @param perm for a new directory
     */
    void mkdir(const std::string &path, const std::string &perm, size_t size);
	void mkdir(const std::string &path, const std::string &perm) {mkdir(path, perm, 4096); }
    void mkdir(const std::string &path) {mkdir(path, "7"); }

    /**
     * @brief change file or directory permissions
     * @details you can do it without having permissions initially
     * @param path to the entity
     * @param perm new permissions
     */
    void chmod(const std::string &path, const std::string &perm);

    /**
     * @brief change size of file or directory
     * @param path to the entity
     * @param size
     */
    void truncate(const std::string &path, size_t size);

    /**
     * remove entity recursively (even directory)
     * @param path
     */
    void rm_r(const std::string &path);

    /**
     * remove file or device
     * @param path
     */
    void rm(const std::string &path);


    /**
     * @brief upload condition of file system to the file
     * @details using DFS
     * format:
     * PATH TYPE PERMISSIONS SIZE CREATION_TIME(only for file) MODIFICATION_TIME(only for file)
     */
    void upload(const std::string &file) const;

    /**
     * download condition of file system from the file
     * format:
     * PATH TYPE PERMISSIONS SIZE CREATION_TIME(only for file) MODIFICATION_TIME(only for file)
     */
    void download(const std::string &file);

    void generate(int entity_count);


    /**
     * @brief get info about files within directory or certain file
     * @param path
     * @return
     * for each file:
     * permissions
     * size
     * creation time
     * modification time
     * name
     *
     * for each directory:
     * permissions
     * size
     * number of descs inside
     * name
     *
     * for each device:
     * permissions
     * size
     * mode
     * name
     */
    std::vector<EntityDto> info(const std::string &path);

    /**
    * @brief get names within directory or relative file name
    * @param path complete path from the root to the directory or file
    * @return for directory: list of files and directories within directory
    * for file: relative file name
    */
    std::vector<EntityDto> getNames(const std::string &path);


    //void ln(std::string &path, std::string &another_path);
};




#endif //INC_3_FS_H
