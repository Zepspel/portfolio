//
// Created by pavel on 18.12.2025.
//

/*
#define private public
#include "../fs.h"
#undef private*/


#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../Service/fs.h"


class FsTest {
public:
    //static DirDesc* getRoot(Fs& fs) {return fs.root;}
    /*static Desc* getDesc(Fs& fs, const std::string &path) {
        auto it = fs.root->table->find(path);
        return it->second;
    }*/
    static auto parse(Fs& fs, const std::string_view &path) {
        return fs.parse(path);
    }
    static auto search(Fs& fs, const std::string_view &path) {
        return fs.search(path);
    }

};


TEST_CASE("Private methods", "[Fs]") {
    SECTION("parse") {
        SECTION("simple path") {
            Fs fs;
            std::string path("/some/path/to/the/File/tRy.../..");
            auto words = FsTest::parse(fs, path);

            REQUIRE(words[0] == "some");
            REQUIRE(words[1] == "path");
            REQUIRE(words[2] == "to");
            REQUIRE(words[3] == "the");
            REQUIRE(words[4] == "File");
            REQUIRE(words[5] == "tRy...");
            REQUIRE(words[6] == "..");
        }
        SECTION("wrong path") {
            Fs fs;
            std::string path("///..");
            REQUIRE_THROWS_AS(FsTest::parse(fs, path), std::invalid_argument);
            std::string str2("/file/");
            REQUIRE_THROWS_AS(FsTest::parse(fs, path), std::invalid_argument);
        }
    }
    SECTION("search") {
        Fs fs;

        std::string path("/dir1");
        fs.mkdir(path);

        path += "/dir2";
        fs.mkdir(path);

        auto saved_path = path;

        path += "/dir3";
        fs.mkdir(path);
        path += "/file";
        fs.touch(path);

        auto ret = FsTest::search(fs, path);
        REQUIRE(ret.first == "file");
        REQUIRE(ret.second != nullptr);


        ret = FsTest::search(fs, saved_path);
        REQUIRE(ret.first == "dir2");
        REQUIRE(ret.second != nullptr);
    }
}

TEST_CASE("Create new") {
    SECTION("touch") {
        SECTION("Can't create file in file") {
            Fs fs;
            std::string path("/file.txt");
            fs.touch(path);
            path += "/file2";
            REQUIRE_THROWS_AS(fs.touch(path), std::invalid_argument);
        }
        SECTION("Creating existing file") {
            Fs fs;
            std::string path("/file");
            fs.touch(path);
            auto [s1, desc1] = FsTest::search(fs, path);

            fs.touch(path);
            auto [s2, desc2] = FsTest::search(fs, path);

            REQUIRE(desc1 == desc2);
        }
    }

    SECTION("mkdir") {
        Fs fs;
        fs.mkdir("/dir1");
        fs.mkdir("/dir2");
        fs.mkdir("/dir1/subdir");
        fs.mkdir("/dir2/subdir");
    }
}

/*
TEST_CASE("ls") {
    /*должно
    1) для директории - выводить имена всех файлов в ней
    2) для файла - выводить его имя#1#
    SECTION("ls with directory") {
        Fs fs;
        fs.mkdir("/dir1");

        fs.mkdir("/dir1/subdir1");
        fs.mkdir("/dir1/subdir2");
        fs.mkdir("/dir1/subdir2/directory");
        fs.touch("/dir1/file1");
        fs.touch("/dir1/file2");
        std::forward_list<std::string> check_list{"file1", "file2", "subdir1", "subdir2"};

        auto list = fs.ls("/dir1");
        REQUIRE(check_list == list);
    }
    SECTION("ls with file") {
        Fs fs;
        fs.mkdir("/dir1");
        fs.touch("/dir1/file1");

        std::forward_list<std::string> check_list{"file1"};

        auto list = fs.ls("/dir1/file1");
        REQUIRE(check_list == list);
    }
    SECTION("ls with non existing file") {
        Fs fs;
        fs.mkdir("/dir1");
        fs.touch("/dir1/file1");

        REQUIRE_THROWS_AS(fs.ls("/dir1/file100"), std::invalid_argument);
    }
}
*/



TEST_CASE("getNames") {
    Fs fs;

    // Настройка минимальной тестовой файловой системы
    fs.mkdir("/test");
    fs.touch("/test/file.txt");
    fs.mkdir("/test/subdir");

    SECTION("ls директории - возвращает список содержимого") {
        auto result = fs.getNames("/test");

        REQUIRE(result.size() == 2);  // file.txt и subdir

        bool has_file = false, has_dir = false;
        for (const auto& dto : result) {
            if (dto.name == "file.txt") has_file = true;
            if (dto.name == "subdir") has_dir = true;
        }

        REQUIRE(has_file);
        REQUIRE(has_dir);
    }

    SECTION("ls файла - возвращает сам файл") {
        auto result = fs.getNames("/test/file.txt");

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].name == "file.txt");
    }

    SECTION("ls несуществующего пути - кидает исключение") {
        REQUIRE_THROWS_AS(fs.getNames("/nonexistent"), std::invalid_argument);
    }
}

TEST_CASE("rm -r") {
    SECTION("rm directory recursively") {
        Fs fs;
        fs.mkdir("/dir1");
        fs.touch("/dir1/file1");

        REQUIRE_NOTHROW(fs.getNames("/dir1/file1"));

        fs.rm_r("/dir1/file1");

        REQUIRE_THROWS_AS(fs.getNames("/dir1/file1"), std::invalid_argument);

    }
}


TEST_CASE("chmod") {
    Fs fs;
    fs.mkdir("/dir1");
    fs.touch("/dir1/file1");

    fs.chmod("/dir1/file1", "7");

    auto dtos = fs.info("/dir1/file1");
    REQUIRE(dtos.size() == 1);
    REQUIRE(dtos[0].permissions == "rwx");

}



TEST_CASE("truncate") {
    Fs fs;
    fs.mkdir("/dir1");
    fs.touch("/dir1/file1");

    fs.truncate("/dir1/file1", 500);

    auto dtos = fs.info("/dir1/file1");
    REQUIRE(dtos.size() == 1);
    REQUIRE(dtos[0].size == "500");
}

TEST_CASE("find") {
    Fs fs;
    fs.download("file2");

    SECTION("single thread") {
        auto res = fs.find("MainActivity.java");
        REQUIRE(!res.empty());
        res = fs.find("M");
        REQUIRE(res.empty());

    }
    SECTION("multithread") {
        auto res = fs.find_threads("MainActivity.java");
        REQUIRE(!res.empty());
        res = fs.find("Ma");
        REQUIRE(res.empty());
    }
}
