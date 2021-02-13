#include <fstream>
#include <iostream>
#include <utility>
#include <climits>
#include "huffman.h"


static const size_t MAX_FILE_SIZE = 5242880;

using namespace Huffman;

Tree_node::Tree_node(uint8_t s, int count) {
    _s = s;
    _count = count;
    _left = nullptr;
    _right = nullptr;
}

Tree_node::Tree_node(Tree_node *left, Tree_node *right) {
    _s = 0;
    _count = left->_count + right->_count;
    _left = left;
    _right = right;
}

bool Tree_node::operator<(const Tree_node &r) const {
    return _count > r._count;
}


Huff_tree::Huff_tree() {
    _root = nullptr;
}

Huff_tree::~Huff_tree() {
    if (_root != nullptr)
        free_node(_root);
}

void Huff_tree::free_node(Tree_node *node) {
    if (node->_left == nullptr || node->_right == nullptr) {
        delete node;
    } else {
        free_node(node->_left);
        free_node(node->_right);
        delete node;
    }
}


void Huff_tree::Build_tree(std::ifstream &is, size_t &file_size) {
    uint8_t c;
    while (is.read((char *) &c, sizeof(char))) {
        mp[c]++;
        file_size++;
        if (file_size > MAX_FILE_SIZE)
            throw Huffman_archiver_exception("Error: file is too large.");
    }

    if (file_size == 0)
        return;

    for (auto i: mp) {
        auto tr = new Tree_node(i.first, i.second);
        pq.push(tr);
    }

    if (mp.size() == 1) {
        auto it = mp.begin();
        auto tr = new Tree_node(it->first, it->second);
        pq.push(tr);
    }


    while (pq.size() > 1) {
        auto lf = pq.top();
        pq.pop();
        auto rt = pq.top();
        pq.pop();

        auto nw = new Tree_node(lf, rt);
        pq.push(nw);
    }

    _root = pq.top();
}

Tree_node *Huff_tree::get_root() const {
    return _root;
}






void Huff_tree::Build_tree_from_table(std::unordered_map<uint8_t, std::string> &codes) {
    auto ptr = new Tree_node();
    Tree_node *cur;
    for (auto &i: codes) {
        cur = ptr;
        for (auto j: i.second) {
            if (j == '0') {
                if (cur->_left == nullptr) {
                    cur->_left = new Tree_node();
                }
                cur = cur->_left;
            }
            if (j == '1') {
                if (cur->_right == nullptr) {
                    cur->_right = new Tree_node();
                }
                cur = cur->_right;
            }
        }
        cur->_s = i.first;
    }

    if (codes.size() == 1)
        ptr->_left = new Tree_node();

    _root = ptr;
}




void Huffman_archiver::Build_table(const Tree_node *ptr, const std::string &code) {
    if (ptr->_left == nullptr || ptr->_right == nullptr) {
        table[ptr->_s] = code;
        return;
    }
    Build_table(ptr->_left, code + "0");
    Build_table(ptr->_right, code + "1");
}

void Huffman_archiver::Make_archive(const std::string &input, const std::string &output) {
    std::ifstream is;
    std::ofstream os;
    is.open(input, std::ios::in);
    os.open(output, std::ios::out);

    if ((is.rdstate() & std::ios::failbit) != 0)
        throw Huffman_archiver_exception("Unable to open file.");

    tree.Build_tree(is, file_size);


    if (file_size == 0)
        return;

    Build_table(tree.get_root(), "");


    Save_table(os);

    is.clear();
    is.seekg(0);
    uint8_t i, pos = 0, byte = 0;

    while (is.read((char *) &i, sizeof(char))) {
        write_bytes(os, table[i], pos, byte);
        archive_size += table[i].size();
    }

    os.write((char *) &byte, sizeof(char));
    num_of_bits += pos;
    os.seekp(0, std::ios::beg);
    os.write((char *) &num_of_bits, sizeof(char));
    archive_size = (archive_size + CHAR_BIT - 1) / CHAR_BIT;

    is.close();
    os.close();
}

void Huffman_archiver::Unpack_archive(const std::string &input, const std::string &output) {
    std::ifstream is;
    std::ofstream os;
    is.open(input, std::ios::in);
    os.open(output, std::ios::out);

    if ((is.rdstate() & std::ios::failbit) != 0)
        throw Huffman_archiver_exception("Unable to open file.");

    Load_table(is);
    if (is.eof())
        return;

    tree.Build_tree_from_table(table);

    auto tmp = tree.get_root();
    uint8_t c1, c2;
    is.read((char *) &c2, sizeof(char));
    while (!is.eof()) {
        is.read((char *) &c1, sizeof(char));
        archive_size++;
        uint8_t loop = is.eof() ? num_of_bits : CHAR_BIT;
        for (uint8_t j = 0; j < loop; j++) {
            bool d = c2 & (1u << j);
            if (tmp->_left == nullptr && tmp->_right == nullptr) {
                os.write((char *) &tmp->_s, sizeof(char));
                file_size++;
                tmp = tree.get_root();
            }
            if (d)
                tmp = tmp->_right;
            else
                tmp = tmp->_left;
        }
        c2 = c1;
    }
    os.write((char *) &tmp->_s, sizeof(char));
    file_size++;

    is.close();
    os.close();
}


void Huffman_archiver::write_bytes(std::ofstream &os, const std::string &code, uint8_t &pos, uint8_t &byte) {
    for (auto j : code) {
        if (pos == CHAR_BIT) {
            os.write((char *) &byte, sizeof(char));
            byte = 0;
            pos = 0;
        }
        if (j == '1')
            byte += 1u << pos;
        pos++;
    }
}

void Huffman_archiver::Save_table(std::ofstream &os) {
    uint8_t c1 = 0, c2 = table.size() - 1, pos = 0, byte = 0;
    int mem = 0;
    os.write((char *) &c1, sizeof(char));
    os.write((char *) &c2, sizeof(char));
    used_place_size += 2;
    for (auto &i: table) {
        c2 = i.second.length();
        os.write((char *) &i.first, sizeof(char));
        os.write((char *) &c2, sizeof(char));
        used_place_size += 2;
    }
    for (auto &i:table) {
        write_bytes(os, i.second, pos, byte);
        mem += i.second.length();
    }
    os.write((char *) &byte, sizeof(char));
    used_place_size += (mem + CHAR_BIT - 1) / CHAR_BIT;
}

void Huffman_archiver::Load_table(std::ifstream &is) {
    uint8_t c, n, len, beg, end, pos = CHAR_BIT, byte = 0;
    int cnt, j = 0;
    std::string code;
    is.read((char *) &num_of_bits, sizeof(char));
    if (is.eof())
        return;
    is.read((char *) &n, sizeof(char));
    std::vector<uint8_t> lens(n + 2), sym(n + 2);
    used_place_size += 2;

    for (int i = 0; i <= n; i++) {
        is.read((char *) &c, sizeof(char));
        is.read((char *) &len, sizeof(char));
        sym[i] = c;
        lens[i] = len;
        used_place_size += 2;
    }

    cnt = lens[0];
    while (j <= n) {
        if (pos == CHAR_BIT) {
            pos = 0;
            is.read((char *) &byte, sizeof(char));
            used_place_size++;
        }
        beg = pos;
        end = pos + cnt >= CHAR_BIT ? CHAR_BIT : pos + cnt;
        for (uint8_t g = beg; g < end; g++) {
            bool d = byte & (1u << g);
            if (d)
                code.push_back('1');
            else
                code.push_back('0');
            pos++;
            cnt--;
        }
        if (cnt == 0) {
            table[sym[j]] = code;
            code.clear();
            j++;
            cnt = lens[j];
        }
    }
}

size_t Huffman_archiver::get_file_size() const {
    return file_size;
}

size_t Huffman_archiver::get_archive_size() const {
    return archive_size;
}

size_t Huffman_archiver::get_used_place_size() const {
    return used_place_size;
}


const char *Huffman_archiver_exception::what() const noexcept {
    return _error.c_str();
}

Huffman_archiver_exception::Huffman_archiver_exception(std::string error) noexcept {
    _error = std::move(error);
}

bool Cmp_tree_ptr::operator()(const Tree_node *lhs, const Tree_node *rhs) const {
    return *lhs < *rhs;
}