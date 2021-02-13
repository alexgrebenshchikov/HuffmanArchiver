#include <fstream>
#include <iostream>
#include "huffman_test.h"
#include "huffman.h"


using namespace Huffman;

void Huffman_test::run_all_tests() {
    Make_small_file();
    Make_empty_file();
    Make_one_symbol_file();
    Test_build_tree_build_table();
    Test_tree_from_table();
    Test_save_table_load_table();
    Test_make_archive1();
    Test_unpack_archive1();
    Test_unpack_archive2();
}

void Huffman_test::Test_build_tree_build_table() {
    Huffman_archiver arc;
    std::ifstream is;
    is.open("small_file.txt", std::ios::in);
    arc.tree.Build_tree(is, arc.file_size);
    arc.Build_table(arc.tree.get_root(), "");
    DO_CHECK(arc.table['b'].length() < arc.table['o'].length() && arc.table['o'].length() < arc.table['r'].length())
    is.close();
}

void Huffman_test::Test_tree_from_table() {
    Huffman_archiver arc, arc2;
    std::ifstream is;
    is.open("small_file.txt", std::ios::in);
    arc.tree.Build_tree(is, arc.file_size);
    arc.Build_table(arc.tree.get_root(), "");
    arc2.tree.Build_tree_from_table(arc.table);
    arc2.Build_table(arc2.tree.get_root(), "");
    DO_CHECK(arc.table == arc2.table)
}

void Huffman_test::Test_save_table_load_table() {
    Huffman_archiver arc, arc2;
    std::ifstream is;
    std::ofstream os;
    is.open("small_file.txt", std::ios::in);
    arc.tree.Build_tree(is, arc.file_size);
    arc.Build_table(arc.tree.get_root(), "");
    is.close();
    os.open("arc.bin", std::ios::out);
    arc.Save_table(os);
    os.close();
    is.open("arc.bin", std::ios::in);
    arc2.Load_table(is);
    DO_CHECK(arc.table == arc2.table)
}

void Huffman_test::Test_make_archive1() {
    Huffman_archiver arc;
    arc.Make_archive("empty_file.txt", "empty.bin");
    DO_CHECK(arc.get_file_size() == 0 && arc.get_archive_size() == 0)
}



void Huffman_test::Test_unpack_archive1() {
    Huffman_archiver arc, arc2;
    arc.Make_archive("small_file.txt", "arc.bin");
    arc2.Unpack_archive("arc.bin", "unpacked.txt");
    DO_CHECK(Comp_files("small_file.txt", "unpacked.txt") && arc.get_file_size() == arc2.get_file_size() &&
             arc.get_archive_size() == arc2.get_archive_size() &&
             arc.get_used_place_size() == arc2.get_used_place_size())
}



void Huffman_test::Test_unpack_archive2() {
    Huffman_archiver arc, arc2;
    arc.Make_archive("one_symbol.txt", "arc.bin");
    arc2.Unpack_archive("arc.bin", "unpacked.txt");
    DO_CHECK(Comp_files("one_symbol.txt", "unpacked.txt") && arc.get_file_size() == arc2.get_file_size() &&
             arc.get_archive_size() == arc2.get_archive_size() &&
             arc.get_used_place_size() == arc2.get_used_place_size())
}



bool Huffman_test::Comp_files(const std::string &file1, const std::string &file2) {
    std::ifstream is, in;
    uint8_t c;
    std::vector<uint8_t> data1, data2;
    is.open(file1, std::ios::in);
    in.open(file2, std::ios::in);
    while (is.read((char *) &c, sizeof(char))) {
        data1.push_back(c);
    }
    while (in.read((char *) &c, sizeof(char))) {
        data2.push_back(c);
    }
    is.close();
    in.close();
    return data1 == data2;
}

void Huffman_test::Make_small_file() {
    std::ofstream os;
    os.open("small_file.txt", std::ios::out);
    os << "beep boop beer boop biip!!";
    os.close();
}

void Huffman_test::Make_empty_file() {
    std::ofstream os;
    os.open("empty_file.txt", std::ios::out);
    os.close();
}

void Huffman_test::Make_one_symbol_file() {
    std::ofstream os;
    os.open("one_symbol.txt", std::ios::out);
    os << "FFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    os.close();
}





