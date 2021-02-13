#include <iostream>
#include <huffman.h>

using std::cout;
using std::string;
using namespace Huffman;


int main(int argc, char *argv[]) {
    if (argc == 6) {
        Huffman_archiver arc;
        bool f = false;
        if (string(argv[1]) == "-c") {
            f = true;
            try {
                arc.Make_archive(string(argv[3]), string(argv[5]));
                cout << arc.get_file_size() << '\n' << arc.get_archive_size() << '\n' << arc.get_used_place_size()
                     << '\n';
            } catch (Huffman_archiver_exception &e) {
                cout << e.what() << '\n';
            }
        }

        if (string(argv[1]) == "-u") {
            f = true;
            try {
                arc.Unpack_archive(string(argv[3]), string(argv[5]));
                cout << arc.get_archive_size() << '\n' << arc.get_file_size() << '\n' << arc.get_used_place_size()
                     << '\n';
            } catch (Huffman_archiver_exception &e) {
                cout << e.what() << '\n';
            }
        }

        if (!f)
            cout << "Unknown command." << '\n';

    } else {
        cout << "Wrong number of argument" << '\n';
    }
    return 0;
}
