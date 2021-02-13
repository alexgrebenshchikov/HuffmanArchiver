//
// Created by Alexander Gr on 18.03.2020.
//

#ifndef HW_3_HUFFMAN_TEST_H
#define HW_3_HUFFMAN_TEST_H

#include "autotest.h"
#include "huffman.h"
#include <iostream>

#define DO_CHECK(EXPR) check(EXPR, __FUNCTION__, __FILE__, __LINE__);

namespace Huffman {

    class Huffman_test : public Test {
    private:
        static void Test_build_tree_build_table();

        static void Test_tree_from_table();

        static void Test_save_table_load_table();

        static void Test_make_archive1();

        static void Test_unpack_archive1();

        static void Test_unpack_archive2();

        static bool Comp_files(const std::string &file1, const std::string &file2);

        static void Make_small_file();

        static void Make_empty_file();

        static void Make_one_symbol_file();
    public:
        void run_all_tests() final;
    };
}


#endif //HW_3_HUFFMAN_TEST_H