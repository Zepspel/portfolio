//
// Created by pavel on 18.12.2025.
//


/*
#define private public
#include "../tableIterator.h"
#include "../orderedTable.h"
#undef private
*/

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../Entity/dirDesc.h"

//#include "../desc.h"
//#include "../fs.h"



TEST_CASE("Desc") {
    SECTION("Default constructor") {
        DirDesc dir;

        REQUIRE(dir.getSize() == SIZE_4K);
        REQUIRE(dir.getAddr() != nullptr);
        REQUIRE(dir.getPermissions() == std::bitset<3>(0b111));
    }

    SECTION("DirDesc: constructor with permissions", "[DirDesc]") {
        std::bitset<3> perms{0b111};
        DirDesc dir(perms);

        REQUIRE(dir.getPermissions() == perms);
        //REQUIRE(dir.getType() == Type::Directory);
    }

    SECTION("DirDesc: constructor with permissions and size", "[DirDesc]") {
        std::bitset<3> perms{0b111};
        DirDesc dir(perms, 8096);

        REQUIRE(dir.getPermissions() == perms);
        REQUIRE(dir.getSize() == 8096);
    }

    /*
    SECTION("DirDesc: include_file()", "[DirDesc]") {
        DirDesc dir;

        dir.include_file("file.txt", std::bitset<3>(0b110)); //кажется нормально не вставляется
        dir.include_file("file2.txt", std::bitset<3>(0b110)); //кажется нормально не вставляется

        auto info = dir.info();

        SECTION("info() contains file name") {
            //auto it = info.find("Table");
            auto it = std::find(info.begin(), info.end(), "file.txt");
            REQUIRE(it != info.end());
            REQUIRE(*it != "file.txt\nfile2.txt\n");
        }
    }*/

    SECTION("DirDesc: include_dir()", "[DirDesc]") {
        DirDesc dir;

        REQUIRE_NOTHROW(
            dir.include_dir("subdir", std::bitset<3>(0b111))
        );

        /*
        auto info = dir.info();

        SECTION("info() contains directory name") {
            //auto it = info.find("Table");
            auto it = std::find(info.begin(), info.end(), "Table");
            REQUIRE(it  != info.end());
            REQUIRE(*it == "subdir\n");
        }*/
    }
    SECTION("DirDesc: polymorphism via Desc*", "[DirDesc][polymorphism]") {
        Desc* d = new DirDesc();

        //REQUIRE(d->getType() == Type::Directory);

        delete d; // проверка виртуального деструктора
    }
}


