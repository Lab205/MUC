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

static Minisat::Solver* solver;
int main(int argc, char** argv) {
    
//    path;
//    MUC muc(path);
//    muc.solve();
    
    MUC m;
    if (argc == 1)
        printf("Reading from standard input... Use '--help' for help.\n");
    
    //FILE* res = (argc >= 3) ? fopen(argv[2], "wb") : NULL;//--print the result into file..saffie
    bool result;
//    result = m.solve(argv[1]);
    result = m.solve("test.cnf");
    if(result == true){
        printf("sat\n");
    }
    else
    {
        printf("unsat\n");
    }
    return 0;
}

