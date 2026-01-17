//
// Created by pavel on 10.12.2025.
//



#define private public
#include "../Table/tableIterator.h"
#include "../Table/orderedTable.h"
#undef private

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>


TEST_CASE("Conctructors") {
    SECTION("Default constructor creates empty OrderedTable") {
        OrderedTable<int, int> table;

        REQUIRE(table.size() == 0);
        REQUIRE(table.root == nullptr);
        REQUIRE(table.min == nullptr);
        REQUIRE(table.max == nullptr);
    }

    SECTION("Copy constructor creates deep copy") {
        OrderedTable<int, int> original;
        int keys[20] = {10, 5, 15, 11, 7, 12, 3, 18, 1, 6, 9, 2, 4, 8, 13, 14, 16, 17, 19, 20};
        for (int i = 0; i < 20; ++i) {
            original.insert({keys[i], keys[i]*10});
        }

        OrderedTable<int, int> copy(original);

        // Проверяем размер
        REQUIRE(copy.size() == original.size());

        // Проверяем, что корни разные (глубокая копия)
        REQUIRE(copy.root != original.root);

        // Проверяем, что значения совпадают
        REQUIRE(copy.root->data.first == original.root->data.first);
        REQUIRE(copy.root->data.second == original.root->data.second);

        // Проверяем min/max
        REQUIRE(copy.min->data.first == original.min->data.first);
        REQUIRE(copy.max->data.first == original.max->data.first);

        // Проверяем, что изменение копии не влияет на оригинал
        copy.insert({44, 200});
        REQUIRE(copy.size() == original.size() + 1);
        REQUIRE(original.size() == 20);
    }


    SECTION("Move constructor transfers ownership") {
        OrderedTable<int, int> original;
        int keys[20] = {10, 5, 15, 11, 7, 12, 3, 18, 1, 6, 9, 2, 4, 8, 13, 14, 16, 17, 19, 20};
        for (int i = 0; i < 20; ++i) {
            original.insert({keys[i], keys[i]*10});
        }

        OrderedTable<int, int> moved(std::move(original));

        // Проверяем, что moved содержит данные
        REQUIRE(moved.size() == 20);
        REQUIRE(moved.root != nullptr);
        REQUIRE(moved.min != nullptr);
        REQUIRE(moved.max != nullptr);

        // Проверяем, что оригинал теперь пуст
        REQUIRE(original.size() == 0);
        REQUIRE(original.root == nullptr);
        REQUIRE(original.min == nullptr);
        REQUIRE(original.max == nullptr);

        REQUIRE(moved.root->data.first == 10);
    }
}


TEST_CASE("Assigments") {
    SECTION("Copy assignment") {
        OrderedTable<int,int> table;
        table.insert({10,100});
        table.insert({5,50});
        table.insert({15,150});

        OrderedTable<int,int> other;
        other = table;

        REQUIRE(other.size_ == 3);
        REQUIRE(other.root != nullptr);
        REQUIRE(other.root != table.root);        // должен быть глубокий копи
        REQUIRE(other.find(10)->second == 100);
        REQUIRE(other.find(5)->second == 50);
        REQUIRE(other.find(15)->second == 150);
    }


    SECTION("Copy self-assignment") {
        OrderedTable<int,int> t;
        t.insert({3,30});
        t.insert({1,10});

        REQUIRE_NOTHROW(t = t);
        REQUIRE(t.size_ == 2);
    }
    SECTION("Move assignment") {
        OrderedTable<int,int> src;
        src.insert({8,800});
        src.insert({4,400});

        OrderedTable<int,int> dst;
        dst = std::move(src);

        REQUIRE(dst.size_ == 2);
        REQUIRE(dst.find(8)->second == 800);
        REQUIRE(dst.find(4)->second == 400);

        REQUIRE(src.root == nullptr);
        REQUIRE(src.size_ == 0);
    }
    SECTION("Move self-assignment") {
        OrderedTable<int,int> t;
        t.insert({7,70});
        REQUIRE_NOTHROW(t = std::move(t));
        REQUIRE(t.size_ == 1);
    }
}


TEST_CASE("Modificators") {
    SECTION("Clear table with multiple elements") {
        OrderedTable<int, int> table;
        int keys[20] = {10, 5, 15, 11, 7, 12, 3, 18, 1, 6, 9, 2, 4, 8, 13, 14, 16, 17, 19, 20};
        for (int i = 0; i < 20; ++i) {
            table.insert({keys[i], keys[i]*10});
        }

        REQUIRE(table.size() == 20);
        REQUIRE(table.root != nullptr);
        REQUIRE(table.min != nullptr);
        REQUIRE(table.max != nullptr);

        table.clear();

        REQUIRE(table.size() == 0);
        REQUIRE(table.root == nullptr);
        REQUIRE(table.min == nullptr);
        REQUIRE(table.max == nullptr);
    }
    SECTION("Insert") {
        OrderedTable<int, int> table;

        SECTION("Insert new element") {
            auto [it, inserted] = table.insert({5, 50});
            REQUIRE(inserted == true);
            REQUIRE(it->first == 5);
            REQUIRE(it->second == 50);
        }

        SECTION("Insert duplicate key") {
            table.insert({5, 50});
            auto [it, inserted] = table.insert({5, 100});
            REQUIRE(inserted == false); // не вставляем дубликат
            REQUIRE(it->first == 5);
            REQUIRE(it->second == 50); // старое значение осталось
        }
    }

    SECTION("Emplace") {
        OrderedTable<int, int> table;

        SECTION("Emplace new element") {
            auto [it, inserted] = table.emplace(5, 50);
            REQUIRE(inserted == true);
            REQUIRE(it->first == 5);
            REQUIRE(it->second == 50);
        }

        SECTION("Emplace duplicate key") {
            table.emplace(5, 50);
            auto [it, inserted] = table.emplace(5, 100);
            REQUIRE(inserted == false); // не вставляем дубликат
            REQUIRE(it->first == 5);
            REQUIRE(it->second == 50); // старое значение осталось
        }
    }

    SECTION("OrderedTable erase by iterator") {
        OrderedTable<int, int> table;
        table.insert({1, 10});
        table.insert({2, 20});
        table.insert({3, 30});

        SECTION("Erase existing element") {
            auto it = table.begin();
            ++it; // элемент с ключом 2
            auto next_it = table.erase(it);
            REQUIRE(next_it->first == 3);
            REQUIRE(table.size() == 2);
        }


        SECTION("Erase begin element") {
            auto it = table.begin(); // ключ 1
            table.erase(it);
            REQUIRE(table.size() == 2);
            REQUIRE(table.begin()->first == 2);
        }
    }


    SECTION("OrderedTable erase by key") {
        OrderedTable<int, int> table;
        table.insert({1, 10});
        table.insert({2, 20});
        table.insert({3, 30});

        SECTION("Erase existing key") {
            size_t count = table.erase(2);
            REQUIRE(count == 1);
            REQUIRE(table.size() == 2);
            REQUIRE(table.begin()->first == 1);
            auto it = table.begin();
            ++it;
            REQUIRE(it->first == 3);
        }

        SECTION("Erase non-existing key") {
            size_t count = table.erase(100);
            REQUIRE(count == 0);
            REQUIRE(table.size() == 3);
        }

    }


}



TEST_CASE("Iterators") {
    SECTION("Begin/end on empty table") {
        OrderedTable<int,int> table;

        auto it = table.begin();
        auto cit = table.cbegin();

        // begin() and cbegin() should equal end() and cend() for empty table
        REQUIRE(it == table.end());
        REQUIRE(cit == table.cend());
    }

    SECTION("Begin/end with elements") {
        OrderedTable<int,int> table;
        OrderedTable<int,int> other;
        table.insert({10,100});
        table.insert({5,50});
        table.insert({15,150});

        // begin() should point to the minimum key
        auto it = table.begin();
        REQUIRE(it->first == 5);
        REQUIRE(it->second == 50);

        // const iterator also points to minimum
        const OrderedTable<int,int>& const_table = table;
        auto cit = const_table.begin();
        REQUIRE(cit->first == 5);
        REQUIRE(cit->second == 50);


        const OrderedTable<int,int>& const_other = other;
        // end() should be nullptr
        auto it_end = table.end();
        auto it2_end = other.end();
        REQUIRE(it_end == it2_end);

    }

    SECTION("Iterate through table in-order") {
        OrderedTable<int,int> table;
        table.insert({10,100});
        table.insert({5,50});
        table.insert({15,150});
        table.insert({7,70});
        table.insert({12,120});

        std::vector<int> keys;
        // iterate with non-const iterator
        for (auto it = table.begin(); it != table.end(); ++it) {
            keys.push_back(it->first);
        }

        // keys should be in ascending order (in-order traversal)
        REQUIRE(keys == std::vector<int>({5,7,10,12,15}));

        // iterate with const iterator
        const OrderedTable<int,int>& ctable = table;
        std::vector<int> const_keys;
        for (auto it = ctable.cbegin(); it != ctable.cend(); ++it) {
            const_keys.push_back(it->first);
        }
        REQUIRE(const_keys == keys);
    }

    SECTION("Iterate through table in reverse order") {
        OrderedTable<int,int> table;
        table.insert({10,100});
        table.insert({5,50});
        table.insert({15,150});
        table.insert({7,70});
        table.insert({12,120});

        std::vector<int> keys;
        // iterate with non-const iterator
        for (auto it = table.rbegin(); it != table.rend(); ++it) {
            keys.push_back(it->first);
        }

        // keys should be in ascending order (in-order traversal)
        REQUIRE(keys == std::vector<int>({15,12,10,7,5}));


        // iterate with const iterator
        const OrderedTable<int,int>& ctable = table;
        std::vector<int> const_keys;
        for (auto it = ctable.rbegin(); it != ctable.rend(); ++it) {
            const_keys.push_back(it->first);
        }
        REQUIRE(const_keys == keys);
    }
}

TEST_CASE("OrderedTable capacity methods") {
    OrderedTable<int, int> t;

    SECTION("empty() works on a new table") {
        REQUIRE(t.empty());
        REQUIRE(t.size() == 0);
    }

    SECTION("size() increases after insert") {
        t.insert({1, 10});
        REQUIRE_FALSE(t.empty());
        REQUIRE(t.size() == 1);

        t.insert({2, 20});
        REQUIRE(t.size() == 2);
    }

    SECTION("size() does not increase on duplicate insert") {
        t.insert({1, 10});
        t.insert({1, 999});   // Вставка с существующим ключом не увеличивает size
        REQUIRE(t.size() == 1);
    }

    SECTION("size() decreases after erase(iterator)") {
        t.insert({1, 10});
        t.insert({2, 20});
        auto it = t.find(1);

        t.erase(it);
        REQUIRE(t.size() == 1);
    }

    SECTION("size() decreases after erase(key)") {
        t.insert({1, 10});
        t.insert({2, 20});

        REQUIRE(t.erase(1) == 1);
        REQUIRE(t.size() == 1);
        REQUIRE_FALSE(t.empty());
    }

    SECTION("erase(key) does not change size for missing key") {
        t.insert({1, 10});
        REQUIRE(t.erase(777) == 0);
        REQUIRE(t.size() == 1);
    }

    SECTION("empty() becomes true again after removing all elements") {
        t.insert({1, 10});
        t.insert({2, 20});

        t.erase(1);
        t.erase(2);

        REQUIRE(t.empty());
        REQUIRE(t.size() == 0);
    }

    SECTION("max_size() returns a very large value") {
        // max_size не обязан иметь какое-то определённое значение,
        // но должен быть >= текущему size и > 0
        REQUIRE(t.max_size() > 0);
        REQUIRE(t.max_size() >= t.size());
    }
}

TEST_CASE("OrderedTable searching methods") {
    OrderedTable<int, int> t;

    SECTION("find() on empty table returns end()") {
        REQUIRE(t.find(10) == t.end());
        REQUIRE(t.count(10) == 0);
    }

    SECTION("find() finds inserted keys") {
        t.insert({1, 100});
        t.insert({2, 200});
        t.insert({5, 500});

        auto it = t.find(2);
        REQUIRE(it != t.end());
        REQUIRE(it->first == 2);
        REQUIRE(it->second == 200);

        REQUIRE(t.count(2) == 1);
    }

    SECTION("find() returns end() for missing key") {
        t.insert({1, 100});
        t.insert({3, 300});

        REQUIRE(t.find(2) == t.end());
        REQUIRE(t.count(2) == 0);
    }

    SECTION("const find() works correctly") {
        t.insert({1, 10});
        t.insert({2, 20});
        t.insert({3, 30});

        const OrderedTable<int, int>& ct = t;

        auto it = ct.find(3);
        REQUIRE(it != ct.end());
        REQUIRE(it->first == 3);
        REQUIRE(it->second == 30);

        REQUIRE(ct.count(3) == 1);
    }

    SECTION("count() returns 0 or 1 like std::map") {
        t.insert({1, 10});
        t.insert({2, 20});

        REQUIRE(t.count(1) == 1);
        REQUIRE(t.count(2) == 1);
        REQUIRE(t.count(999) == 0);
    }

    SECTION("find after erase does not find removed key") {
        t.insert({1, 10});
        t.insert({2, 20});
        t.insert({3, 30});

        t.erase(2);

        REQUIRE(t.find(2) == t.end());
        REQUIRE(t.count(2) == 0);
    }

    SECTION("find returns iterator to correct node after multiple operations") {
        t.insert({10, 100});
        t.insert({5, 50});
        t.insert({20, 200});
        t.insert({7, 70});

        t.erase(5);     // Удалили узел с двумя детьми?

        auto it = t.find(7);
        REQUIRE(it != t.end());
        REQUIRE(it->second == 70);
    }
}

TEST_CASE("Access operators") {
    OrderedTable<int, std::string> t;

    SECTION("operator[] inserts default value if key does not exist") {
        auto& val = t[5];  // ключа нет, создаётся значение по умолчанию
        REQUIRE(val == ""); // std::string{} → пустая строка
        REQUIRE(t.size() == 1);

        val = "hello";      // можно присвоить
        REQUIRE(t[5] == "hello"); // теперь вернёт обновлённое значение
    }

    SECTION("operator[] returns existing value if key exists") {
        t[10] = "world";
        auto& val = t[10]; // ключ уже есть
        REQUIRE(val == "world");
        REQUIRE(t.size() == 1); // размер не увеличивается
    }


    SECTION("at() returns reference to existing element") {
        t[1] = "first";
        t[2] = "second";

        auto& val = t.at(1);
        REQUIRE(val == "first");
        val = "updated";
        REQUIRE(t.at(1) == "updated");
    }

    SECTION("at() throws std::out_of_range if key does not exist") {
        t[1] = "a";
        t[2] = "b";

        REQUIRE_THROWS_AS(t.at(999), std::out_of_range);
        //REQUIRE_THROWS_AS(static_cast<const OrderedTable<int,std::string>&>(t).at(999), std::out_of_range);
    }

    SECTION("const at() returns const reference") {
        t[3] = "three";
        const auto& ct = t;
        const auto& val = ct.at(3);
        REQUIRE(val == "three");
    }

    SECTION("operator[] and at() work together") {
        t[7]; // создаёт пустое значение
        REQUIRE(t.at(7) == ""); // at видит созданный элемент
        t[7] = "seven";
        REQUIRE(t.at(7) == "seven");
    }

}

TEST_CASE("OrderedTable lower_bound() and upper_bound() methods") {
    OrderedTable<int, std::string> t;

    SECTION("lower_bound and upper_bound on empty table") {
        REQUIRE(t.lower_bound(10) == t.end());
        REQUIRE(t.upper_bound(10) == t.end());
    }

    SECTION("lower_bound finds first >= key") {
        t.insert({1, "a"});
        t.insert({3, "b"});
        t.insert({5, "c"});
        t.insert({7, "d"});

        REQUIRE(t.lower_bound(0)->first == 1);
        REQUIRE(t.lower_bound(1)->first == 1);
        REQUIRE(t.lower_bound(2)->first == 3);
        REQUIRE(t.lower_bound(5)->first == 5);
        REQUIRE(t.lower_bound(6)->first == 7);
        REQUIRE(t.lower_bound(8) == t.end());
    }

    SECTION("upper_bound finds first > key") {
        t.insert({1, "a"});
        t.insert({3, "b"});
        t.insert({5, "c"});
        t.insert({7, "d"});

        REQUIRE(t.upper_bound(0)->first == 1);
        REQUIRE(t.upper_bound(1)->first == 3);
        REQUIRE(t.upper_bound(2)->first == 3);
        REQUIRE(t.upper_bound(5)->first == 7);
        REQUIRE(t.upper_bound(6)->first == 7);
        REQUIRE(t.upper_bound(7) == t.end());
        REQUIRE(t.upper_bound(8) == t.end());
    }

    SECTION("lower_bound and upper_bound after deletion") {
        t.insert({1, "a"});
        t.insert({3, "b"});
        t.insert({5, "c"});
        t.insert({7, "d"});

        t.erase(5);

        REQUIRE(t.lower_bound(5)->first == 7);
        REQUIRE(t.upper_bound(5)->first == 7);
        REQUIRE(t.lower_bound(6)->first == 7);
        REQUIRE(t.upper_bound(6)->first == 7);
    }
}



