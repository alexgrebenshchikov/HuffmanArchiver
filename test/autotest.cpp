//
// Created by Alexander Gr on 18.03.2020.
//
#include <iostream>
#include "autotest.h"

int Test::failed_num = 0;
int Test::total_num = 6;

void Test::check(bool expr, const char *func, const char *filename, size_t line_num) {
    if (!expr) {
        std::cout << "Test failed: " << func << ", in file: " << filename << ", line: " << line_num << ".\n";
        failed_num++;
    }
}

void Test::show_final_result() {
    std::cout << "Tests failed: " << failed_num << ", total number of tests: " << total_num << ".\n";
}