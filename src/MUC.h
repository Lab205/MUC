/* 
 * File:   MUC.h
 * Author: saffie
 *
 * Created on May 11, 2015, 4:33 PM
 */

#ifndef MUC_H
#define	MUC_H

#include <stdio.h>
#include <utility>
#include <vector>

#include "MUCCommon.h"

class MUC {
    
private:
    std::vector<data_type *> data;
    std::vector<item_type> L; //L is a list in which each element is a set of clauses.
    std::vector<item_type> M;//M is a minimal unsat core.
    
public:
    MUC();
    MUC(const MUC& orig);
    MUC(const char* path);
    virtual ~MUC();
    
    void read_from_file(const char* path);
    void solve();

    // for test
    void print_data();
    
private:
    void clear();
    bool sat_check(item_type P);
    void add_clause(item_type clause, Minisat::SimpSolver& S);
    bool sat(Minisat::SimpSolver& S);
    void split(item_type P, item_type& P1, item_type& P2);
    bool minisolve(item_type& P);
   
    inline bool is_atom(item_type P) { return P.first == P.second;}
    inline bool is_empty(item_type P) { return P.first == -1; }

};

#endif	/* MUC_H */

