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
    //Minisat::SimpSolver S1,S2;
    //    for (int i = 0; i < data.size(); i++) {
    //        data_type *item = data[i];
    //        for (int j = 0; j < item->size(); j++) {
    //            Minisat::Lit lit = (*item)[j];
    //            printf("...%d.%d_", Minisat::sign(lit), lit.x);
    //            while (lit.x >= S1.nVars()) S1.newVar();
    //        }
    //        printf("\n");
    //        data_type copy;
    //        item->copyTo(copy);
    //        S1.addClause_(copy);
    //    }
    //  
    bool result = minisolve(S);
    if (result == true) {
        printf("sat\n");
    } else {
        L_count = data.size();
        M_count = 0;
        printf("%d\n", L_count);
        item_type item;
        item = std::make_pair(0, L_count - 1); //The initial set of clauses C.
        L.push_back(item); //Push back C to L;

        //printf("%d,%d",L[0].first, L[0].second);
        item_type P, P1, P2, test;
        int P1_count, P2_count, CL;
        bool res1, res2;
        while (L.size()) {
            printf("L.size:%d\n", L.size());
            printf("M.size:%d\n", M.size());
            P = L.back();
            L.pop_back(); //P is the top element of L, and pop P from L;
            if (P.first != P.second) {
                float b = P.first;
                float a = (P.second - b) / 2;
                printf("a:%f\n", a);
                P1 = std::make_pair(P.first, P.first + ceil(a) - 1);
                P2 = std::make_pair(P1.second + 1, P.second);
            } else {
                P2 = P;
                P1 = std::make_pair(-1, -1);
            }
            printf("P:%d,%d\n", P.first, P.second);
            printf("P1:%d,%d\n", P1.first, P1.second);
            printf("P2:%d,%d\n", P2.first, P2.second);

            if (P1.first != -1) {
                Minisat::SimpSolver S1;
                P1_count = P1.second - P1.first + 1;
                for (int i = P1.first; i <= P1.second; i++) {
                    //printf("print---");
                    data_type *item_data = data[i];
                    //printf("item.size:%d\n", item_data->size());
                    for (int j = 0; j < item_data->size(); j++) {
                        Minisat::Lit lit = (*item_data)[j];
                        // printf("x.%d_\n", lit.x);
                        while (lit.x >= S1.nVars()) S1.newVar();
                    }
                    // printf("\n");
                    data_type copy;
                    item_data->copyTo(copy);
                    S1.addClause_(copy);
                }
                for (int m = 0; m < M.size(); m++) {
                    for (int i = M[m].first; i <= M[m].second; i++) {
                        // printf("print---");
                        data_type *item_data = data[i];
                        // printf("item.size:%d\n", item_data->size());
                        for (int j = 0; j < item_data->size(); j++) {
                            Minisat::Lit lit = (*item_data)[j];
                            //printf("x.%d_\n", lit.x);
                            while (lit.x >= S1.nVars()) S1.newVar();
                        }
                        //printf("\n");
                        data_type copy;
                        item_data->copyTo(copy);
                        S1.addClause_(copy);
                    }
                }
                for (int n = 0; n < L.size(); n++) {
                    for (int i = L[n].first; i <= L[n].second; i++) {
                        // printf("print---");
                        data_type *item_data = data[i];
                        //printf("item.size:%d\n", item_data->size());
                        for (int j = 0; j < item_data->size(); j++) {
                            Minisat::Lit lit = (*item_data)[j];
                            // printf("x.%d_\n", lit.x);
                            while (lit.x >= S1.nVars()) S1.newVar();
                        }
                        //printf("\n");
                        data_type copy;
                        item_data->copyTo(copy);
                        S1.addClause_(copy);
                    }
                }
                res1 = minisolve(S1);
                printf("res1:%d\n", res1);
                if (!res1) {
                    if (P1.first == P1.second) {
                        M.push_back(P1);
                        test = M.back();
                        printf("test1:%d,%d\n", test.first, test.second);
                        continue;
                    } else {
                        L.push_back(P1);
                        continue;
                    }
                }
            }
            printf("-----------------\n");

            if (P2.first != -1) {
                Minisat::SimpSolver S2;
                P2_count = P2.second - P2.first + 1;
                for (int i = P2.first; i <= P2.second; i++) {
                    data_type *item_data = data[i];
                    for (int j = 0; j < item_data->size(); j++) {
                        Minisat::Lit lit = (*item_data)[j];
                        //printf("...%d.%d_", Minisat::sign(lit), lit.x);
                        while (lit.x >= S2.nVars()) S2.newVar();
                    }
                    //printf("\n");
                    data_type copy;
                    item_data->copyTo(copy);
                    S2.addClause_(copy);
                }
                for (int m = 0; m < M.size(); m++) {
                    for (int i = M[m].first; i <= M[m].second; i++) {
                        //printf("print---");
                        data_type *item_data = data[i];
                        //printf("item.size:%d\n", item_data->size());
                        for (int j = 0; j < item_data->size(); j++) {
                            Minisat::Lit lit = (*item_data)[j];
                            //printf("x.%d_\n", lit.x);
                            while (lit.x >= S2.nVars()) S2.newVar();
                        }
                        //printf("\n");
                        data_type copy;
                        item_data->copyTo(copy);
                        S2.addClause_(copy);
                    }
                }
                for (int n = 0; n < L.size(); n++) {
                    for (int i = L[n].first; i <= L[n].second; i++) {
                        // printf("print---");
                        data_type *item_data = data[i];
                        // printf("item.size:%d\n", item_data->size());
                        for (int j = 0; j < item_data->size(); j++) {
                            Minisat::Lit lit = (*item_data)[j];
                            //printf("x.%d_\n", lit.x);
                            while (lit.x >= S2.nVars()) S2.newVar();
                        }
                        // printf("\n");
                        data_type copy;
                        item_data->copyTo(copy);
                        S2.addClause_(copy);
                    }
                }
                res2 = minisolve(S2);
                printf("res2:%d\n", res2);
                if (!res2) {
                    if (P2.first == P2.second) {
                        M.push_back(P2);
                        test = M.back();
                        
                        printf("test2:%d,%d\n", test.first, test.second);
                        continue;
                    } else {
                        L.push_back(P2);
                        continue;
                    }
                }
            }

            if (P1.first != -1) {
                L.push_back(P1);
            }
            if (P2.first != -1) {
                L.push_back(P2);
            }
            printf("========================\n");
        }


        printf("-------------------M:----------------------------\n");
        int num = M.size();
        printf("M.size():%d\n",M.size());
        for (int c = 0; c < num; c++) {
            test = M.back();
            printf("test:%d,%d\n", test.first, test.second);
            M.pop_back();
        }





    }

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

