//
// Created by pavel on 21.11.2025.
//



#include "fs.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <thread>
#include <unordered_set>

#include "../Entity/desc.h"
#include "../Entity/fileDesc.h"
#include "../Table/orderedTable.h"




std::vector<std::string_view> Fs::parse(const std::string_view &path) {


    if (!path.size() || path[0] != '/')
        throw std::invalid_argument("[parse] invalid path");

    std::vector<std::string_view> names;

    size_t start = 1;
    while (start != path.size()) {
        auto pos = path.find('/', start);
        if (pos == std::string::npos) {
            break;
        }
        if (pos == start) {
            throw std::invalid_argument("[parse] invalid path");
        }
        names.push_back(path.substr(start, pos - start));
        start = pos + 1;
    }
    /*if (start == path.size())
        throw std::invalid_argument("invalid path");*/

    names.push_back(path.substr(start, path.size() - start));
    return names;
}

std::bitset<3> Fs::parse_permissions(const std::string_view& permissions) {

    std::bitset<3> ret;

    if (isdigit(permissions[0]) && permissions.size() == 1) {
        int val = stoi(std::string(permissions));
        if (val < 0 || val > 7) {
            throw std::invalid_argument("invalid permissions");
        }
        return std::bitset<3>(val);
    }


    if (permissions.size() != 3) {
        throw std::invalid_argument("invalid permissions");
    }

    if (permissions[0] != 'r' && permissions[0] != '-') {
        throw std::invalid_argument("invalid permissions");
    }
    if (permissions[0] == 'r') {
        ret.set(2);
    }
    else if (permissions[0] == '-') {
        ret.reset(2);
    }

    if (permissions[1] != 'w' && permissions[1] != '-') {
        throw std::invalid_argument("invalid permissions");
    }
    if (permissions[1] == 'w') {
        ret.set(1);
    }
    else if (permissions[1] == '-') {
        ret.reset(1);
    }

    if (permissions[2] != 'x' && permissions[2] != '-') {
        throw std::invalid_argument("invalid permissions");
    }
    if (permissions[2] == 'x') {
        ret.set(0);
    }
    else if (permissions[2] == '-') {
        ret.reset(0);
    }

    return ret;
}


std::time_t Fs::parse_time(const std::string& time) {
    std::tm tm{};

    // Берём текущий год
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    tm.tm_year = now_tm->tm_year; // годы с 1900

    std::istringstream iss(time);
    iss >> std::get_time(&tm, "%b %d %H:%M");

    if (iss.fail()) {
        throw std::invalid_argument("Invalid time format: " + time);
    }

    tm.tm_isdst = -1; // пусть система сама определит DST

    return std::mktime(&tm);
}

Mode Fs::parse_mode(const std::string_view& mode) {
    if (mode == "Character") {
        return Mode::Character;
    }
    if (mode == "Block") {
        return Mode::Block;
    }
    throw std::invalid_argument("[parse mode] invalid mode");
}

std::string Fs::timeToString(std::time_t t) {
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%b %d %H:%M");
    return oss.str();
}



int Fs::get_random_int(int min, int max) {
    std::random_device rd;  // Источник энтропии
    std::mt19937 gen(rd()); // Генератор Mersenne Twister
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}


std::string Fs::get_random_string(size_t length) {
    const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += chars[distribution(generator)];
    }

    return result;
}


//возвращает std::pair 1) директория 2) имя файла (директории)
/*
1)
надо проверить что все директории по пути существуют

2)
надо проверить права на исполнение у всех директорий
если нет x - не можем перейти
если у последней нет w - не можем создать
*/
std::pair<std::string_view, DirDesc*> Fs::search(const std::string_view& path) const {
    std::vector<std::string_view> names = parse(path);
    if (names.empty()) {
        //значит нет даже имени файла, который надо добавить
        throw std::invalid_argument("[search]1 invalid path");
    }

    Desc* cur_dir = root;
    DirDesc* dir = root;

    auto it_vec = names.begin();
    for (; it_vec != names.end() - 1; ++it_vec) {
        if (dir = dynamic_cast<DirDesc*>(cur_dir)) {
            auto it_desc = dir->table->find(std::string(*it_vec));
            //возвращает итератор на данную ячейку таблицы (она содержит name и указатель на Desc)
            //first std::string (name)
            //second std::unique_ptr<Desc> (указатель на Desc)


            //if directory in the path doesn't exist
            if (it_desc == dir->table->end()) {
                throw std::invalid_argument(std::format("[search]2 invalid path {}", *it_vec));
            }

            //проверяем права на x, что можем перейти в директорию
            if (!it_desc->second->getPermissions().test(0))
                throw std::logic_error("[search] Permission denied: no execute permission to pass through directory");

            cur_dir = it_desc->second; //указатель на дескриптор директории
        }
        else {
            //если это имя файла или спец устройства
            throw std::invalid_argument(std::format("[search] Invalid path '{}' because {} is file, not directory", path,
                                                    *it_vec));
        }
    }
    //dynamic_cast<DirDesc*>(cur_dir)

    if (!(dir = dynamic_cast<DirDesc*>(cur_dir))) {
        throw std::invalid_argument("[search]3 invalid path");
    }

    std::pair<std::string_view, DirDesc*> res = std::make_pair(*it_vec, dir);
    return res;
}

Fs::~Fs() {
    if (root)
        delete root;
}

std::vector<EntityDto> Fs::find(const std::string& name) const {

    std::vector<EntityDto> result;

    if (!root) {
        return result;
    }

    // стек: текущая директория + путь до неё
    std::stack<std::pair<DirDesc*, std::string>> st;
    st.push({ root, "/" });

    while (!st.empty()) {
        auto [dir, path] = st.top();
        st.pop();

        for (const auto& [entry_name, desc] : *dir->table) {

            std::string full_path =
                (path == "/" ? "/" + entry_name
                             : path + "/" + entry_name);

            if (auto* subdir = dynamic_cast<DirDesc*>(desc)) {
                // DFS: кладём в стек и идём глубже
                st.push({ subdir, full_path });
            }
            else if (entry_name == name) {
                EntityDto dto = desc->info();
                dto.name = entry_name;
                dto.path = full_path;

                result.push_back(dto);
            }


        }
    }

    return result;
}

std::vector<EntityDto> Fs::find_threads(const std::string& name) const {
    std::vector<EntityDto> result;

    if (!root) {
        return result;
    }

    struct WorkItem {
        DirDesc* dir;
        std::string path;
    };

    std::queue<WorkItem> tasks;
    std::mutex tasks_mutex;
    std::mutex result_mutex;


    tasks.push({ root, "/" });

    auto worker = [&]() {
        while (true) {
            WorkItem task;
            {
                std::lock_guard lock(tasks_mutex);
                if (tasks.empty()) {
                    return; // работы больше нет
                }
                task = tasks.front();
                tasks.pop();
            }

            DirDesc* dir = task.dir;
            const std::string& path = task.path;

            for (const auto& [entry_name, desc] : *dir->table) {

                std::string full_path =
                    (path == "/" ? "/" + entry_name
                                 : path + "/" + entry_name);

                if (auto* subdir = dynamic_cast<DirDesc*>(desc)) {
                    std::lock_guard lock(tasks_mutex);
                    tasks.push({ subdir, full_path });
                }
                else if (entry_name == name) {
                    EntityDto dto = desc->info();
                    dto.name = entry_name;
                    dto.path = full_path;

                    std::lock_guard lock(result_mutex);
                    result.push_back(std::move(dto));
                }
            }
        }
    };

    const unsigned threads_count =
        std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::thread> threads;
    threads.reserve(threads_count);

    for (unsigned i = 0; i < threads_count; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}


void Fs::touch(const std::string &path, const std::string &perm, size_t size, const std::string &creation, const std::string &modification) const {

    auto permissions = parse_permissions(perm);
    auto creation_time = parse_time(creation);
    auto modification_time = parse_time(modification);

    //при выходе it_vec указывает на последнее оставшееся имя - это и есть создаваемый файл
    //он создается в папке curr_dir

    auto [name, dir] = search(path);

    if (!dir->getPermissions().test(1))
        throw std::logic_error("Permission denied: no write permission to create file in the directory");


    dir->include_file(name, permissions, size, creation_time, modification_time);
}


void Fs::device(const std::string& path, const std::string& perm, size_t size, const std::string& mode) {
    Mode m = parse_mode(mode);
    auto permissions = parse_permissions(perm);

    auto [name, dir] = search(path);
    if (!dir->getPermissions().test(1))
        throw std::logic_error("Permission denied: no write permission to create device in the directory");
    if (dir != root)
        throw std::logic_error("Permission denied: device can't be created not in the root directory");

    dir->include_device(name, permissions, size, m);
}


void Fs::mkdir(const std::string& path, const std::string &perm, size_t size) {
    auto permissions = parse_permissions(perm);

    auto [name, dir] = search(path);

    if (!dir->getPermissions().test(1))
        throw std::logic_error("Permission denied: no write permission to create directory in the directory");

    dir->include_dir(name, permissions, size);
}

void Fs::chmod(const std::string& path, const std::string &perm) {
    auto permissions = parse_permissions(perm);

    auto [name, dir] = search(path);

    auto it = dir->table->find(std::string(name));
    if (it == dir->table->end()) {
        throw std::invalid_argument("can not find such file or directory");
    }

    it->second->setPermissions(permissions);
}

void Fs::truncate(const std::string& path, size_t size) {
    auto [name, dir] = search(path);

    auto it = dir->table->find(std::string(name));
    if (it == dir->table->end()) {
        throw std::invalid_argument(std::format("truncate: cannot access '{}': No such file or directory", std::string(name)));
    }

    it->second->setSize(size);
}


void Fs::rm_r(const std::string& path) {
    auto [name, dir] = search(path);

    if (name == "") {   //если был запрос rm /
        throw std::invalid_argument("rm: cannot delete the root");
    }

    auto obj = dir->table->find(std::string(name));
    if (obj == dir->table->end()) {
        throw std::invalid_argument(std::format("rm: cannot access '{}': No such file or directory", std::string(name)));
    }

    delete obj->second;         //вызываем деструктор, для директории он рекурсивно удалит все, что в ней
    dir->table->erase(obj);
}

void Fs::rm(const std::string& path) {
    auto [name, dir] = search(path);

    if (name == "") {   //если был запрос rm /
        throw std::invalid_argument("rm: cannot delete the root");
    }

    auto obj = dir->table->find(std::string(name));
    if (obj == dir->table->end()) {
        throw std::invalid_argument(std::format("rm: cannot access '{}': No such file or directory", std::string(name)));
    }

    if (dynamic_cast<DirDesc*>(obj->second)) {
        throw std::invalid_argument(std::format( "rm: cannot remove '{}': Is a directory", std::string(name)));
    }

    delete obj->second;
    dir->table->erase(obj);
}



void Fs::upload(const std::string &filename) const {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("cannot open file");
    }

    // стек: пара (таблица + путь до этой таблицы)
    std::stack<std::pair<OrderedTable<std::string, Desc*>*, std::string>> stk;
    std::unordered_set<OrderedTable<std::string, Desc*>*> visited;

    stk.push({root->table.get(), ""}); // стартуем с корня
    visited.insert(root->table.get());

    while (!stk.empty()) {
        auto [table, current_path] = stk.top();
        stk.pop();

        for (auto it = table->rbegin(); it != table->rend(); ++it) { // rbegin чтобы порядок как рекурсия
            const std::string &name = it->first;
            Desc* entity = it->second;
            std::string full_path = current_path + "/" + name;

            if (auto file = dynamic_cast<FileDesc*>(entity)) {
                // записываем файл с полным путем
                out << full_path << " " << "file" << " ";
                auto info = file->info();
                info.addr = "";
                out << info << "\n";
            }
            if (auto file = dynamic_cast<SpDesc*>(entity)) {
                out << full_path << " " << "dev" << " ";
                auto info = file->info();
                info.addr = "";
                out << info << "\n";
            }
            else if (auto dir = dynamic_cast<DirDesc*>(entity)) {
                if (!visited.contains(dir->table.get())) {
                    // записываем директорию с полным путем
                    out << full_path << " " << "dir" << " ";
                    auto info = dir->info();
                    info.addr = "";
                    out << info << "\n";
                    // добавляем директорию в стек
                    stk.push({dir->table.get(), full_path});
                }
            }
        }
    }
}

void Fs::generate(int entity_count) {
    std::stack<std::string> stk;
    stk.push(""); // стартуем с корня

    while (!stk.empty()) {
        std::string current_path = stk.top();
        stk.pop();

        int count = get_random_int(500, 1000);

        for (int i = 0; i < count; ++i) {
            --entity_count;
            if (entity_count < 0) {
                break;
            }

            std::string name = get_random_string(10);
            std::string full_path = current_path + "/" + name;

            int is_file = get_random_int(0, 5);
            if (!is_file) {
                mkdir(full_path);
                stk.push(full_path);
            }
            else {
                touch(full_path);
            }




        }
    }
}


void Fs::download(const std::string& file) {
    //root = std::make_unique<DirDesc>(/* permissions */);
    delete root;
    root = new DirDesc();

    std::ifstream in(file);
    if (!in) {
        throw std::runtime_error("cannot open file for reading");
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string path, typeStr, perms;
        size_t size = 0;

        // читаем базовые поля
        iss >> path >> typeStr >> perms >> size;

        if (typeStr == "dir") {
            // вставляем через сервис
            mkdir(path, perms, size);
        }
        else if (typeStr == "file") {
            std::string m, d, t;

            //iss >> creation_time >> modification_time; // или более сложная дата

            if (!(iss >> m >> d >> t)) {
                throw std::runtime_error("invalid time format");
            }
            std::string creation_time = m + " " + d + " " + t;

            if (!(iss >> m >> d >> t)) {
                throw std::runtime_error("invalid time format");
            }
            std::string modification_time = m + " " + d + " " + t;

            touch(path, perms, size, creation_time, modification_time);
        }
        else if (typeStr == "dev") {
            std::string mode;
            if (!(iss >> mode)) {
                throw std::runtime_error("invalid time format");
            }
            device(path, perms, size, mode);
        }
        else {
            throw std::runtime_error("[download] invalid entity format");
        }
    }
}




std::vector<EntityDto> Fs::info(const std::string& path) {
    std::vector<EntityDto> ret;
    EntityDto dto;
    //std::list<std::string> horizontally;

    auto [name, dir] = search(path);

    if (name == "") {   //если был запрос ls -l /
        for (auto it = dir->table->rbegin(); it != dir->table->rend(); ++it) {
            dto = it->second->info();
            dto.name = it->first;
            dto.path = path + dto.name;

            ret.push_back(dto);
        }
        return ret;
    }

    auto obj = dir->table->find(std::string(name));
    if (obj == dir->table->end()) {
        throw std::invalid_argument(std::format("ls: cannot access '{}': No such file or directory", std::string(name)));
    }

    //auto target = dynamic_cast<DirDesc*>(obj->second);


    if (auto file = dynamic_cast<FileDesc*>(obj->second))  { //если в ls был запрошен файл
        dto = file->info();
        dto.name = std::string(name);
        dto.path = path + "/" + dto.name;

        ret.push_back(dto);
    }
    else if (auto device = dynamic_cast<SpDesc*>(obj->second)) {
        dto = device->info();
        dto.name = std::string(name);
        dto.path = path + "/" + dto.name;

        ret.push_back(dto);
    }
    else if (auto target = dynamic_cast<DirDesc*>(obj->second)) {
        for (auto it = target->table->rbegin(); it != target->table->rend(); ++it) {
            dto = it->second->info();
            dto.name = it->first;
            dto.path = path + "/" + dto.name;

            ret.push_back(dto);
        }
    }
    else {
        throw std::logic_error("Unknown file type");
    }

    return ret;
}

std::vector<EntityDto> Fs::getNames(const std::string& path) {
    std::vector<EntityDto> ret;
    EntityDto dto;
    //std::list<std::string> horizontally;

    auto [name, dir] = search(path);

    if (name == "") {   //если был запрос ls /
        for (auto it = dir->table->rbegin(); it != dir->table->rend(); ++it) {
            dto.name = it->first;
            ret.push_back(dto);
        }
        return ret;
    }

    auto obj = dir->table->find(std::string(name));
    if (obj == dir->table->end()) {
        throw std::invalid_argument(std::format("ls: cannot access '{}': No such file or directory", std::string(name)));
    }

    //auto target = dynamic_cast<DirDesc*>(obj->second);


    if (auto file = dynamic_cast<FileDesc*>(obj->second))  { //если в ls был запрошен файл
        dto.name = std::string(name);
        ret.push_back(dto);
    }
    else if (auto device = dynamic_cast<SpDesc*>(obj->second)) {
        dto.name = std::string(name);
        ret.push_back(dto);
    }
    else if (auto target = dynamic_cast<DirDesc*>(obj->second)) {
        for (auto it = target->table->rbegin(); it != target->table->rend(); ++it) {
            dto.name = it->first;
            ret.push_back(dto);
        }
    }
    else {
        throw std::logic_error("Unknown file type");
    }

    return ret;
}



