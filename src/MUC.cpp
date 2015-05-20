/* 
 * File:   MUC.cpp
 * Author: saffie
 * 
 * Created on May 11, 2015, 4:33 PM
 */

#include "MUC.h"
#include <vector>
#include <minisat/core/SolverTypes.h>
typedef std::pair<int, int> item_type;
typedef Minisat::vec<Minisat::Lit> data_type;

MUC::MUC() {
}

MUC::MUC(const MUC& orig) {
}

bool MUC::solve(const char* path) {


    Minisat::SimpSolver S;
    gzFile in = gzopen(path, "rb");
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    parse(in, S, true); //--parse the input file..saffie
    Minisat::SimpSolver S1;
    for (int i = 0; i < data.size(); i++) {
        data_type *item = data[i];
        for (int j = 0; j < item->size(); j++) {
            Minisat::Lit lit = (*item)[j];
            printf("...%d.%d_", Minisat::sign(lit), lit.x);
            while (lit.x >= S1.nVars()) S1.newVar();
        }
        printf("\n");
        data_type copy;
        item->copyTo(copy);
        S1.addClause_(copy);
    }
//    for (int i = 0; i < data.size(); i++) {
//        printf("%d: ", i);
//        data_type *lits = data[i];
//        
//    }
    bool result = minisolve(S1);
    //bool result = false;
    gzclose(in);
    return result;

}

bool MUC::minisolve(Minisat::SimpSolver& S) {

    Minisat::BoolOption solve("MAIN", "solve", "Completely turn on/off solving after preprocessing.", true);
    //Minisat::StringOption dimacs ("MAIN", "dimacs", "If given, stop after preprocessing and write the result to this file.");
    S.eliminate(true); //------execute the main algorithm..saffie
    if (!S.okay()) {
        //printf("unsat okay\n");
        return false;
        exit(20);
    }
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = S.solveLimited(dummy);
    }
    //if (dimacs && ret == Minisat::l_Undef)
    //S.toDimacs((const char*)dimacs);
    //printf(ret == Minisat::l_True ? "SATISFIABLE\n" : ret == Minisat::l_False ? "UNSATISFIABLE\n" : "INDETERMINATE\n");
    if (ret == Minisat::l_True) {
        return true;
    } else if (ret == Minisat::l_False) {
        return false;
    } else {
        return false;
    }
}

MUC::~MUC() {
}

