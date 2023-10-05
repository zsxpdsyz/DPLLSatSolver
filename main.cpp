#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "global.h"

// #define CNF_IN_FILE "./src/Sat/S/problem2-50.cnf"
#define CNF_IN_FILE "./src/Standard/functional_test/sat-20.cnf"

using namespace std;

int main() {
    // 读取目标文件
    std::ifstream inFile(CNF_IN_FILE);
    if (! inFile.is_open()) {
        perror("CNF file open fail.");
    }

    bool sat;
    bool read_flag = false;
    std::string line;
    std::string trash;
    int num_literal, num_clause;
    int literal;
    Formula *formula = new Formula;
    
    while (std::getline(inFile, line)) {
        // std::cout << line << std::endl;
        std::stringstream ss(line);
        if (read_flag && !line.empty()) {
            Clause *clause = new Clause();
            // cout << "read line is empty : " << line << endl;
            while (ss >> literal && literal != 0) {
                clause->literal_list.push_back(literal);
            }
            formula->insert(clause);
        }

        if (line[0] == 'p') {
            ss >> trash >> trash;
            ss >> num_literal >> num_clause;
            read_flag = true;
        }   
    }
    cout << "num literal : " << num_literal << " num clause : " << num_clause << endl;
    // formula->print();

    sat = DPLL(formula);

    cout << "sat : " << sat << endl;

    delete formula;
    return 0;
}