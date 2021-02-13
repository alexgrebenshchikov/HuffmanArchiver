#ifndef HW_3_HUFFMAN_H
#define HW_3_HUFFMAN_H

#include <memory>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <queue>

namespace Huffman {

    class Tree_node {
    public:
        explicit Tree_node(uint8_t s = 0, int count = 0);

        Tree_node(Tree_node *left, Tree_node *right);

        bool operator<(const Tree_node &r) const;

    private:
        uint8_t _s;
        int _count;
        Tree_node *_left;
        Tree_node *_right;

        friend class Huff_tree;

        friend class Huffman_archiver;
    };


    struct Cmp_tree_ptr {
        bool operator()(const Tree_node *lhs, const Tree_node *rhs) const;
    };


    class Huff_tree {
    public:
        Huff_tree();

        ~Huff_tree();

        static void free_node(Tree_node *node);

        void Build_tree(std::ifstream &is, size_t &file_size);

        void Build_tree_from_table(std::unordered_map<uint8_t, std::string> &codes);

        [[nodiscard]] Tree_node *get_root() const;

    private:
        Tree_node *_root;
        std::priority_queue<Tree_node *, std::vector<Tree_node *>, Cmp_tree_ptr> pq;
        std::unordered_map<uint8_t, int> mp;
    };

    class Huffman_archiver {
    public:
        Huffman_archiver() : file_size(0), archive_size(0), used_place_size(0), num_of_bits(0) {}

        ~Huffman_archiver() = default;

        void Make_archive(const std::string &input, const std::string &output);

        void Unpack_archive(const std::string &input, const std::string &output);

        size_t get_file_size() const;

        size_t get_archive_size() const;

        size_t get_used_place_size() const;


    private:
        void Build_table(const Tree_node *ptr, const std::string &code);

        void Save_table(std::ofstream &os);

        void Load_table(std::ifstream &is);

        static void write_bytes(std::ofstream &os, const std::string &code, uint8_t &pos, uint8_t &byte);

        size_t file_size;
        size_t archive_size;
        size_t used_place_size ;
        uint8_t num_of_bits;

        std::unordered_map<uint8_t, std::string> table;
        Huff_tree tree;

        friend class Huffman_test;
    };


    class Huffman_archiver_exception : public std::exception {
    public:
        explicit Huffman_archiver_exception(std::string error) noexcept;

        [[nodiscard]] const char *what() const noexcept final;

    private:
        std::string _error;
    };

}
#endif //HW_3_HUFFMAN_H