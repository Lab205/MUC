/* 
 * File:   main.cpp
 * Author: user
 *
 * Created o 2015年4月30日, 下午4:47
 */

#include <cstdlib>
#include <stdio.h>
#include <errno.h>
#include <zlib.h>
#include <vector>
#include "lib/minisat/utils/System.h"
#include "lib/minisat/utils/ParseUtils.h"
#include "lib/minisat/utils/Options.h"
#include "lib/minisat/core/Dimacs.h"
#include "lib/minisat/simp/SimpSolver.h"
#include "src/MUC.h"

using namespace std;

/*
 * 
 */

//static Minisat::Solver* solver;
int main(int argc, char** argv) {
    
    MUC m("test.cnf");
    m.solve();
    m.print_data();
    return 0;
}

