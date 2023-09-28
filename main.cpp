#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "global.h"

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
        std::cout << line << std::endl;
        std::stringstream ss(line);
        if (line[0] == 'p') {
            ss >> trash >> trash;
            ss >> num_literal >> num_clause;
            read_flag = true;
        }

        if (read_flag) {
            Clause *clause = new Clause();
            while (ss >> literal && literal != 0) {
                clause->literal_list.push_back(literal);
            }
            formula->insert(clause);
        }
    }
    cout << "num literal : " << num_literal << " num clause : " << num_clause << endl;

    sat = DPLL(formula);

    cout << "sat : " << sat << endl;

    delete formula;
    return 0;
}