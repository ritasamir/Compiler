//
// Created by meritv on 3/24/19.
//

#include <regex>
#include "ReToNFA.h"

vector<NFA*> ReToNFA::constructNFA() {
    normalizeRe();
    vector<NFA*> result;
    for (int i = 0; i < keywords.size(); ++i) {
        NFA* nfa;
        nfa = re_to_NFA(keywords.at(i), keywords.at(i));
        result.push_back(nfa);
    }
    for (int j = 0; j < punctuations.size(); ++j) {
        NFA* nfa;
        nfa = re_to_NFA(punctuations.at(j), punctuations.at(j));
        result.push_back(nfa);
    }
    map <string, string>::iterator it;
    for (it = regularExpressions.begin(); it != regularExpressions.end(); ++it) {
        NFA* nfa;
        nfa = re_to_NFA(it->second, it->first);
        result.push_back(nfa);
    }
    return result;
}

void ReToNFA::normalizeRe() {
    /* TODO mention assumption that any group must be between parentheses */
    string re;
    map <string, string>::iterator it;
    vector<char> reservedSymbols = {'L', '(', ')', '+', '*'};
    for (it = regularExpressions.begin(); it != regularExpressions.end(); ++it) {
        re = it->second;
        for (int i = 0; i < re.size(); ++i) {
            if (re.at(i) == '[') {
                int pos = re.find(']');
                if (re.at(pos + 1) == '[') {
                    re.replace(pos, 2, "");
                    i--;
                    continue;
                }
                string toberepalced = re.substr(i, pos - i + 1);
                regex reg1(toberepalced);
                string replacement = "(";
                for (int j = 0; j < 128; ++j) {
                    if(regex_match(string(1, (char)j), reg1)) {
                        replacement += string(1, (char)j) + "|";
                    }
                }
                replacement = replacement.substr(0, replacement.size() - 1) + ")";
                re.replace(i, pos - i + 1, replacement);
                if(i > 0 && re.at(i - 1) == '(' && re.at(i + replacement.size()) == ')') {
                    re.replace(i, 1, "");
                    re.replace(i + replacement.size() - 1, 1, "");
                }
            } else if (re[i] == '\\' && find(reservedSymbols.begin(),
                    reservedSymbols.end(), re[i + 1]) == reservedSymbols.end()) {
                /* preceeded by backslash but not reserved symbol */
                re.replace(i, 1, "");
            }

        }
        regularExpressions[it->first] = re;
    }
}

NFA* ReToNFA::re_to_NFA(string re, string tokenType) {
    stack<char> operators;
    stack<NFA*> operands;
    char op_sym;
    int or_count;
    string cur_sym;
    bool prev_is_symbol = false;
    printf("RE : %s\n", re.c_str());
    for(int i = 0; i < re.size(); ++i) {
        cur_sym = re.at(i);
        if (cur_sym != "(" && cur_sym != ")" && cur_sym != "*"
            && cur_sym != "|" && cur_sym != "+" && cur_sym != "\\") { /* not any operation or epsilon */
            if (prev_is_symbol) {
                NFA *nfa = new NFA();
                nfa->addStates(2);
                nfa->setFinalState(1);
                nfa->addTransition(0, 1, cur_sym);
                NFA *prev = operands.top();
                operands.pop();
                operands.push(concatenate(prev, nfa));
            } else {
                NFA *new_nfa;
                new_nfa = new NFA();
                new_nfa->addStates(2);
                new_nfa->addTransition(0, 1, cur_sym);
                new_nfa->setFinalState(1);
                operands.push(new_nfa);
                prev_is_symbol = true;
            }
        } else {
            prev_is_symbol = false;
            if (cur_sym == "*") {
                NFA *kleene_nfa = operands.top();
                operands.pop();
                operands.push(kleeneClosure(kleene_nfa));
            } else if (cur_sym == "+") {
                NFA *positive_nfa = operands.top();
                operands.pop();
                operands.push(positiveClosure(positive_nfa));
            } else if (cur_sym == "|") {
                operators.push(re.at(i));
            } else if (cur_sym == "(") {
                operators.push(re.at(i));
            } else if (cur_sym == "\\") {
                if (re.at(i + 1) == 'L') {
                    NFA *new_nfa = new NFA();
                    new_nfa->addStates(2);
                    new_nfa->addTransition(0, 1, re.substr(i, 1));
                    new_nfa->setFinalState(1);
                    operands.push(new_nfa);
                    i++;
                    /* TODO mention assumption that A\LB = AB */
                } else if (re.at(i + 1) == '(') {
                    NFA *new_nfa = new NFA();
                    new_nfa->addStates(2);
                    new_nfa->addTransition(0, 1, "(");
                    new_nfa->setFinalState(1);
                    operands.push(new_nfa);
                    i++;
                    prev_is_symbol = true;
                } else if (re.at(i + 1) == ')') {
                    NFA *new_nfa = new NFA();
                    new_nfa->addStates(2);
                    new_nfa->addTransition(0, 1, ")");
                    new_nfa->setFinalState(1);
                    operands.push(new_nfa);
                    i++;
                    prev_is_symbol = true;
                } else if (re.at(i + 1) == '*') {
                    NFA *new_nfa = new NFA();
                    new_nfa->addStates(2);
                    new_nfa->addTransition(0, 1, "*");
                    new_nfa->setFinalState(1);
                    operands.push(new_nfa);
                    i++;
                    prev_is_symbol = true;
                } else if (re.at(i + 1) == '+') {
                    NFA *new_nfa = new NFA();
                    new_nfa->addStates(2);
                    new_nfa->addTransition(0, 1, "+");
                    new_nfa->setFinalState(1);
                    operands.push(new_nfa);
                    i++;
                    prev_is_symbol = true;
                }
            } else {    /* ')' */
                or_count = 0;
                char c;
                op_sym = operators.top();
                if (op_sym == '(') {
                    NFA *op2 = operands.top();
                    operands.pop();
                    NFA *op1 = operands.top();
                    operands.pop();
                    operands.push(concatenate(op1, op2));
                    operators.pop();
                    continue;
                }
                do {
                    operators.pop();
                    or_count++;
                } while (operators.top() != '(');
                operators.pop();
                vector<NFA *> selections;
                selections.assign(or_count + 1, new NFA());
                int tracker = or_count;
                for (int i = 0; i < or_count + 1; i++) {
                    selections.at(tracker) = operands.top();
                    tracker--;
                    operands.pop();
                }
                operands.push(unionNFA(selections, or_count + 1));
            }
        }
    }

    while(!operators.empty()) {
        int or_count = 0;
        char op = operators.top();
        operators.pop();
        if (op == '(') {
            NFA* op2 = operands.top();
            operands.pop();
            NFA* op1 = operands.top();
            operands.pop();
            operands.push(concatenate(op1, op2));
        } else if (op == '|') {
            or_count++;
            while (!operators.empty() && operators.top() == '|') {
                or_count++;
                operators.pop();
            }
            vector<NFA*> temp;
            temp.assign(or_count + 1, new NFA);
            int tracker = or_count;
            for (int i = 0; i < or_count + 1; i++) {
                temp.at(tracker) = operands.top();
                tracker--;
                operands.pop();
            }
            operands.push(unionNFA(temp, or_count + 1));
        }
    }
    if (operands.size() > 1) {
        stack<NFA*> temp;
        while (!operands.empty()) {
            temp.push(operands.top());
            operands.pop();
        }
        while (temp.size() > 1) {
            NFA* op2 = temp.top();
            temp.pop();
            NFA* op1 = temp.top();
            temp.pop();
            temp.push(concatenate(op1, op2));
        }
        operands.push(temp.top());
    }
    operands.top()->setTokenType(tokenType);
    operands.top()->printTransitions();
    printf("number of NFA = %d\n", operands.size());
    return operands.top();
}

NFA* ReToNFA::concatenate(NFA *nfa1, NFA *nfa2) {
    int oldNFA1stateno = nfa1->getNumberOfStates();
    nfa1->addStates(nfa2->getNumberOfStates() - 1);
    transition transition;
    for (int i = 0; i < nfa2->getTransitions().size(); ++i) {
        transition = nfa2->getTransitions().at(i);
        nfa1->addTransition(transition.fromState + nfa1->getFinalState(),
                transition.toState + nfa1->getFinalState(), transition.trans_symbol);
    }
    nfa1->setFinalState(oldNFA1stateno + nfa2->getNumberOfStates() - 2);
    return nfa1;
}

NFA* ReToNFA::kleeneClosure(NFA* nfa) {
    int oldNFAstateno = nfa->getNumberOfStates();
    NFA *kleene = new NFA();
    kleene->addStates(oldNFAstateno + 2);
    kleene->addTransition(0, 1, "\\L");
    transition transition;
    for (int i = 0; i < nfa->getTransitions().size(); ++i) {
        transition = nfa->getTransitions().at(i);
        kleene->addTransition(transition.fromState + 1, transition.toState + 1, transition.trans_symbol);
    }
    kleene->addTransition(oldNFAstateno, 1, "\\L");
    kleene->addTransition(oldNFAstateno, oldNFAstateno + 1, "\\L");
    kleene->setFinalState(oldNFAstateno + 1);
    kleene->addTransition(0, kleene->getFinalState(), "\\L");
    return kleene;
}

NFA* ReToNFA::positiveClosure(NFA* nfa) {
    int oldNFAstateno = nfa->getNumberOfStates();
    NFA* positive = new NFA();
    positive->addStates(oldNFAstateno + 2);
    positive->addTransition(0, 1, "\\L");
    transition transition;
    for (int i = 0; i < nfa->getTransitions().size(); ++i) {
        transition = nfa->getTransitions().at(i);
        positive->addTransition(transition.fromState + 1, transition.toState + 1, transition.trans_symbol);
    }
    positive->addTransition(oldNFAstateno, 1, "\\L");
    positive->addTransition(oldNFAstateno, oldNFAstateno + 1, "\\L");
    positive->setFinalState(oldNFAstateno + 1);
    return positive;
}

NFA* ReToNFA::unionNFA(vector<NFA*> selections, int noOfSelections) {
    NFA* unioned = new NFA();
    int noOfStates = 2; /* initially */
    NFA *med;
    transition trans;
    for(int i = 0; i < noOfSelections; i++) {
        noOfStates += selections.at(i)->getNumberOfStates();
    }
    unioned->addStates(noOfStates);
    unioned->setFinalState(noOfStates - 1);
    int tracker = 1;
    for(int i = 0; i < noOfSelections; i++) {
        unioned->addTransition(0, tracker, "\\L");
        med = selections.at(i);
        for(int j = 0; j < med->getTransitions().size(); j++) {
            trans = med->getTransitions().at(j);
            unioned->addTransition(trans.fromState + tracker, trans.toState + tracker, trans.trans_symbol);
        }
        tracker += med->getNumberOfStates();
        unioned->addTransition(tracker - 1, unioned->getFinalState(), "\\L");
    }
    return unioned;
}