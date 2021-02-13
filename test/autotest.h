//
// Created by Alexander Gr on 18.03.2020.
//

#ifndef HW_3_AUTOTEST_H
#define HW_3_AUTOTEST_H

#include <cstddef>


class Test {
protected:
    static int failed_num;
    static int total_num;

    static void check(bool expr, const char *func, const char *filename, size_t line_num);

public:
    static void show_final_result();

    virtual void run_all_tests() = 0;
};


#endif //HW_3_AUTOTEST_H