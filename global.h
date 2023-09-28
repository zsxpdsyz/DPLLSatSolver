#include <iostream>
#include <vector>
#include <unordered_map>

#define CNF_IN_FILE "./src/Standard/functional_test/sat-20.cnf"

using namespace std;

class Clause {
public:
    vector<int> literal_list;
    Clause *next_clause;

    Clause() : next_clause(nullptr){}
};

class Formula {
public:
    Clause *head_clause;

    unordered_map<int, int> literal_freq;

    Formula() : head_clause(nullptr){}

    void insert(Clause *newClause) {
        Clause *temp = head_clause;
        head_clause = newClause;
        head_clause->next_clause = temp;

        // if (!head_clause) {
        //     head_clause = newClause;
        // } else {
        //     Clause *curr_clause = head_clause;
        //     while (curr_clause->next_clause) {
        //         curr_clause = curr_clause -> next_clause;
        //     } 
        //     curr_clause->next_clause = newClause;
        // }
    }

    void Get_Literal_Frequency();
    void Remove_Clause_of_Literal(int literal);
    bool Empty();
    bool Exist_Empty_Clause();
    void Delete_Assign_Var(int literal, bool value);
};

bool DPLL(Formula *formula);
int Unit_Propagation(Formula *formula);
int Pure_Literal_Elimination(Formula *formula);
int Find_Single_Clause(Formula *formula);