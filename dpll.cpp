#include "global.h"

bool DPLL(Formula *formula) {
    formula->Get_Literal_Frequency();

    while (Unit_Propagation(formula)); 
    // formula->print();
    cout << "complete unit propagation." << endl;

    while (Pure_Literal_Elimination(formula));

    if (formula->Empty())
        return true;
    if (formula->Exist_Empty_Clause()) 
        return false;

    // 选择一个变量对其赋值
    int x;
    for (auto pair : formula->literal_freq) {
        if (pair.second > 0) {
            x = pair.first;
            break;
        }
    }
    cout << "delete var x : " << x << endl;
    // 分别对该变量赋正值和负值
    Formula *formula1 = formula, *formula2 = formula;
    // 因为这两个指针都指向了一个共享的内存地址，因此对于formula1中值的修改也会传递到formula和formula2
    // TODO：需要修改
    formula1->Delete_Assign_Var(x, true);
    formula2->Delete_Assign_Var(x, false);
    // formula1->print();
    // cout << "print formula1:" << endl;
    // formula2->print();
    // cout << "print formula2:" << endl;
    // cout << "delete complete" << endl;

    return (DPLL(formula1) | DPLL(formula2));
}

int Unit_Propagation(Formula *formula) {
    // formula->print();
    int unit_literal;
    unit_literal = Find_Single_Clause(formula);
    // 如果为0，说明当前不存在单子句，可以跳过单子句传播的过程
    if (!unit_literal)
        return 0;
    // 因为单子句被删除了，所以他的数量统计减1
    formula->literal_freq[abs(unit_literal)] -= 1;

    Clause *temp = formula->head_clause;
    Clause *pre_clause = new Clause, *dummy_clause = new Clause;
    pre_clause->next_clause = formula->head_clause;
    dummy_clause = pre_clause;

    while (temp) {
        auto it = temp->literal_list.begin();
        for (it; it != temp->literal_list.end(); ++it) {
            // 如果匹配到正好相反的文字，那么就把该文字删除掉
            if (*it == -unit_literal) {
                formula->literal_freq[abs(unit_literal)] -= 1;
                it = temp->literal_list.erase(it);
                break;
            }
            // 匹配到相同的文字，则把整个语句删除掉
            else if (*it == unit_literal) {
                for (auto elem : temp->literal_list) {
                    formula->literal_freq[abs(elem)] -= 1;
                }
                pre_clause->next_clause = temp->next_clause;
                goto HasDelete;
            }
        }
        // 如果没有删除掉当前的clause，那么需要修改上一个clause的指针
        pre_clause = temp;

        HasDelete:
        temp = temp->next_clause;
    }
    formula->head_clause = dummy_clause->next_clause;
    return 1;
}

// 找到单子句返回单子句的值，并直接将单子句删除
int Find_Single_Clause(Formula *formula) {
    Clause *temp = formula->head_clause;
    Clause *pre_clause = new Clause;
    Clause *dummy_clause = new Clause;
    dummy_clause->next_clause = formula->head_clause;
    pre_clause = dummy_clause;
    size_t length;
    // 遍历链表找到长度为1的clause
    while (temp) {
        length = temp->literal_list.size();
        if (length == 1) {
            // 直接删除当前clause
            pre_clause->next_clause = temp->next_clause;
            // 恢复链表头节点
            formula->head_clause = dummy_clause->next_clause;
            return temp->literal_list[0];
        }
        pre_clause = temp;
        temp = temp->next_clause;
    }
    return 0;
}

// 孤立文字消去的过程。如果找到了孤立文字返回1，没找到返回0
int Pure_Literal_Elimination(Formula *formula) {
    for (auto &pair : formula->literal_freq) {
        // 存在孤立文字
        // cout << "PLE hash map pair : " << pair.first << " " << pair.second << endl;
        if (pair.second == 1) {
            pair.second = 0;
            // 查找该literal所在clause的位置，并将其删掉
            formula->Remove_Clause_of_Literal(pair.first);
            return 1;
        }
    }
    // cout << "PLE tranverse end." << endl;
    // 如果没有找到孤立文字，那么返回0即可
    return 0;
}

// hash map的键都是正值，使用abs函数
void Formula::Get_Literal_Frequency() {
    // 每次需要对hash map清空
    literal_freq.clear();
    Clause *temp_clause = head_clause;
    while (temp_clause) {
        for (auto it : temp_clause->literal_list) {
            // 如果hash map中没有这个元素，那么需要把他加进去
            if (literal_freq.find(abs(it)) == literal_freq.end()) {
                literal_freq.insert(make_pair(abs(it), 1));
            } else {
                literal_freq[abs(it)]++;
            }
        }
        temp_clause = temp_clause->next_clause;
    }
}

// 删除孤立文字所在的子句
void Formula::Remove_Clause_of_Literal(int target_literal) {
    Clause *curr_clause = head_clause;
    Clause *pre_clause = new Clause, *dummy_clause = new Clause;
    dummy_clause->next_clause = head_clause;
    pre_clause = dummy_clause;

    while (curr_clause) {
        auto it = curr_clause->literal_list.begin();
        for (it; it != curr_clause->literal_list.end(); ++it) {
            // 删除包含该literal的clause
            cout << "Remove clause loop it = " << *it;
            if (abs(*it) == target_literal) {
                for (auto elem : curr_clause->literal_list) {
                    literal_freq[abs(elem)] -= 1;
                }
                pre_clause->next_clause = curr_clause->next_clause;
                head_clause = dummy_clause->next_clause;
                return;
            }
        }
        cout << endl;
        // 如果没有找到，就不用进行删除
        pre_clause = curr_clause;
        curr_clause = curr_clause->next_clause;
    }
    head_clause = dummy_clause->next_clause;
}

bool Formula::Empty() {
    if (head_clause == nullptr)
        return true;
    else
        return false;
}

bool Formula::Exist_Empty_Clause() {
    Clause *curr_clause = head_clause;
    while (curr_clause) {
        // cout << "current clause size = " << curr_clause->literal_list.size() << endl;
        if (curr_clause->literal_list.size() == 0)
            return true;
        curr_clause = curr_clause->next_clause;
    }
    return false;
}

void Formula::Delete_Assign_Var(int target_literal, bool mode) {
    Clause *curr_clause = head_clause;
    Clause *dummy_clause = new Clause, *pre_clause = new Clause;
    dummy_clause->next_clause = head_clause;
    pre_clause = dummy_clause;

    while (curr_clause) {
        auto it = curr_clause->literal_list.begin();
        // cout << "it = ";
        for (it; it != curr_clause->literal_list.end(); ) {
            // 如果给变量赋的是正值
            // cout << *it << "  ";
            if (mode) {
                if (*it == target_literal) {
                    // 删除整个子句
                    pre_clause->next_clause = curr_clause->next_clause;
                    goto EndLoop;
                } else if (*it == -target_literal) {
                    // 删除一个文字
                    it = curr_clause->literal_list.erase(it);
                } else 
                    it++;
            } else {
                if (*it == -target_literal) {
                    // 删除整个子句
                    pre_clause->next_clause = curr_clause->next_clause;
                    goto EndLoop;
                } else if (*it == target_literal) {
                    // 删除一个文字
                    it = curr_clause->literal_list.erase(it);
                } else 
                    it++;
            }
        }
        pre_clause = curr_clause;

        EndLoop:
        // cout << endl;
        curr_clause = curr_clause->next_clause;
    }
    head_clause = dummy_clause->next_clause;
}