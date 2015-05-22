/* 
 * File:   MUC.cpp
 * Author: saffie
 * 
 * Created on May 11, 2015, 4:33 PM
 */

#include "MUC.h"
#include <vector>
#include <stdio.h>
#include "MUCParser.h"

MUC::MUC() {
}

MUC::MUC(const MUC& orig) {
}

MUC::MUC(const char* path) {
    // init

    this->read_from_file(path);
}

MUC::~MUC() {
    this->clear();
}

/**
 * read from file
 * @param path
 */
void MUC::read_from_file(const char* path) {
    // TODO: clear
    this->clear();

    gzFile in = gzopen(path, "rb");
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    Minisat::StreamBuffer buffer(in);
    int vars = 0;
    int clauses = 0;
    int cnt = 0;
    int i = 1;
    for (;;) {
        Minisat::skipWhitespace(buffer);
        if (*buffer == EOF) break;
        else if (*buffer == 'p') {
            if (Minisat::eagerMatch(buffer, "p cnf")) {
                vars = Minisat::parseInt(buffer);
                clauses = Minisat::parseInt(buffer);
            } else {
                printf("PARSE ERROR! Unexpected char: %c\n", *buffer), exit(3);
            }
        } else if (*buffer == 'c' || *buffer == 'p')
            Minisat::skipLine(buffer);
        else {
            cnt++;
            data_type *lits = new data_type();
            read_clause(buffer, lits);
            data.push_back(lits);
        }
    }

    if (cnt != clauses)
        printf("PARSE ERROR! DIMACS header mismatch: wrong number of clauses\n");
}

void MUC::solve() {
    // algo...
    item_type C = std::make_pair(0, data.size() - 1);
    if (this->sat_check(C)) {
        printf("sat\n");
    } else {
        L.push_back(C);
        item_type P, P1, P2;
        while (!L.empty()) {
            P = L.back();
            L.pop_back();
            this->split(P, P1, P2);
            if (minisolve(P1)) {
                continue;
            }
            if (minisolve(P2)) {
                continue;
            }
            if (!is_empty(P1)) {
                L.push_back(P1);
            }
            if (!is_empty(P2)) {
                L.push_back(P2);
            }
        }
    }

}

void MUC::print_data() {
    //    for (int i = 0; i < data.size(); ++i) {
    //        data_type* lits = data[i];
    //        for (int j = 0; j < lits->size(); ++j) {
    //            char sign = Minisat::sign((*lits)[j]) ? '-' : '+';
    //            printf("%c%d ", sign, Minisat::var((*lits)[j]));
    //        }
    //        printf("\n");
    //    }

    printf("-------------M--------------\n");
    int num = M.size();
    printf("M_size:%d\n", num);
//    for (int a = 0; a < num; a++) {
//        // printf("(%d,%d)\n",M[a].first, M[a].second);
//        data_type *item = data[M[a].first];
//        for (int i = 0; i < item->size(); i++) {
//            char sign = Minisat::sign((*item)[i]) ? '-' : ' ';
//            printf("%c%d ", sign, Minisat::var((*item)[i]) + 1);
//        }
//        printf("0\n");
//    }
}

/**
 * clear data
 */
void MUC::clear() {
    for (int i = data.size() - 1; i >= 0; --i) {
        delete data[i];
    }

    data.clear();
    L.clear();
    M.clear();
}

/**
 * fun
 * @param P
 * @return 
 */
bool MUC::sat_check(item_type P) {
    Minisat::SimpSolver S;
    add_clause(P, S);
    for (int m = 0; m < M.size(); m++) {
        add_clause(M[m], S);
    }
    for (int n = 0; n < L.size(); n++) {
        add_clause(L[n], S);
    }
    bool res = sat(S);
    return res;
}

/**
 * 
 * @param clause
 * @param S
 */
void MUC::add_clause(item_type clause, Minisat::SimpSolver& S) {
    for (int i = clause.first; i <= clause.second; i++) {
        // printf("print---");
        data_type *item_data = data[i];
        for (int j = 0; j < item_data->size(); j++) {
            Minisat::Lit lit = (*item_data)[j];
            while (lit.x >= S.nVars()) S.newVar();
        }
        data_type copy;
        item_data->copyTo(copy);
        S.addClause_(copy);
    }
}

bool MUC::sat(Minisat::SimpSolver& S) {

    Minisat::BoolOption solve("MAIN", "solve", "Completely turn on/off solving after preprocessing.", true);
    S.eliminate(true); //------execute the main algorithm..saffie
    if (!S.okay()) {
        return false;
        exit(20);
    }
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = S.solveLimited(dummy);
    }
    if (ret == Minisat::l_True) {
        return true;
    } else if (ret == Minisat::l_False) {
        return false;
    } else {
        return false;
    }
}

void MUC::split(item_type P, item_type& P1, item_type& P2) {
    if (P.first != P.second) {
        float b = P.first;
        float a = (P.second - b) / 2;
        P1 = std::make_pair(P.first, P.first + ceil(a) - 1);
        P2 = std::make_pair(P1.second + 1, P.second);
    } else {
        P2 = P;
        P1 = std::make_pair(-1, -1);
    }
}

bool MUC::minisolve(item_type& P) {
    if (!is_empty(P) && !sat_check(P)) {
        if (is_atom(P)) {
            M.push_back(P);
            data_type *item = data[P.first];
            printf("%d: ", P.first+2);
            for (int i = 0; i < item->size(); i++) {
                char sign = Minisat::sign((*item)[i]) ? '-' : ' ';
                printf("%c%d ", sign, Minisat::var((*item)[i]) + 1);
            }
            printf("0\n");
        } else {
            L.push_back(P);
        }
        return true;
        //continue;
    }
    return false;
}

