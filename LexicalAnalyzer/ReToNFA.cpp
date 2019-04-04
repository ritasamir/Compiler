//
// Created by meritv on 3/24/19.
//

#include <regex>
#include <iostream>
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
        string punc = punctuations.at(j);
        punc.erase(remove(punc.begin(), punc.end(), '\\'), punc.end());
        nfa = re_to_NFA(punctuations.at(j), punc);
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
    vector<char> reservedSymbols = {'L', '(', ')', '+', '*', '?', '[', ']'};
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
                /* preceded by backslash but not reserved symbol */
                re.replace(i, 1, "");
            }

        }
        regularExpressions[it->first] = re;
    }
}

NFA* ReToNFA::re_to_NFA(string re, string tokenType) {
    stack<char> operandsTracker;
    stack<NFA*> operands;
    char op_sym;
    string cur_sym;
    int level = 0;
  //  printf("RE : %s\n", re.c_str());
    for(int i = 0; i < re.size(); ++i) {
        cur_sym = re.at(i);
        if (cur_sym != "(" && cur_sym != ")" && cur_sym != "*"
            && cur_sym != "|" && cur_sym != "+") { /* symbol */
            bool concatenated = false;
            if (cur_sym == "\\") {
                if (re.at(i + 1) == 'L') {cur_sym = "\\L";}
                else if (re.at(i + 1) == '(') {cur_sym = "(";}
                else if (re.at(i + 1) == ')') {cur_sym = ")";}
                else if (re.at(i + 1) == '*') {cur_sym = "*";}
                else if (re.at(i + 1) == '+') {cur_sym = "+";}
                i++;
            }
            while (!operandsTracker.empty() && operandsTracker.top() == 's') {
                concatenated = true;
                operandsTracker.pop();
                NFA *nfa = new NFA();
                nfa->addStates(2);
                nfa->setFinalState(1);
                nfa->addTransition(0, 1, cur_sym);
                NFA *prev = operands.top();
                operands.pop();
                operands.push(concatenate(prev, nfa));
            }
            if (!concatenated) {
                    NFA *nfa = new NFA();
                    nfa->addStates(2);
                    nfa->setFinalState(1);
                    nfa->addTransition(0, 1, cur_sym);
                    operands.push(nfa);
            }
            while (!operandsTracker.empty() && operandsTracker.top() == '|' && level == 0) {
                operandsTracker.pop();
                operandsTracker.pop();
                NFA *nfa2 = operands.top();
                operands.pop();
                NFA *nfa1 = operands.top();
                operands.pop();
                operands.push(unionNFA({nfa1, nfa2}, 2));
            }
            operandsTracker.push('s');
        } else if (cur_sym == "*") {
            operandsTracker.pop();
            NFA *kleene_nfa = operands.top();
            operands.pop();
            operands.push(kleeneClosure(kleene_nfa));
            while (!operandsTracker.empty() && operandsTracker.top() == 's') {
                operandsTracker.pop();
                NFA *nfa2 = operands.top();
                operands.pop();
                NFA *nfa1 = operands.top();
                operands.pop();
                operands.push(concatenate(nfa1, nfa2));
            }
            while (!operandsTracker.empty() && operandsTracker.top() == '|' && level == 0) {
                operandsTracker.pop();
                operandsTracker.pop();
                NFA *nfa2 = operands.top();
                operands.pop();
                NFA *nfa1 = operands.top();
                operands.pop();
                operands.push(unionNFA({nfa1, nfa2}, 2));
            }
            operandsTracker.push('s');
        } else if (cur_sym == "+") {
            operandsTracker.pop(); /* s */
            NFA *positive_nfa = operands.top();
            operands.pop();
            operands.push(positiveClosure(positive_nfa));
            while (!operandsTracker.empty() && operandsTracker.top() == 's') {
                operandsTracker.pop();
                NFA *nfa2 = operands.top();
                operands.pop();
                NFA *nfa1 = operands.top();
                operands.pop();
                operands.push(concatenate(nfa1, nfa2));
            }
            while (!operandsTracker.empty() && operandsTracker.top() == '|' && level == 0) {
                operandsTracker.pop();
                operandsTracker.pop();
                NFA *nfa2 = operands.top();
                operands.pop();
                NFA *nfa1 = operands.top();
                operands.pop();
                operands.push(unionNFA({nfa1, nfa2}, 2));
            }
            operandsTracker.push('s');
        } else if (cur_sym == "|") {
            operandsTracker.push(re.at(i));
        } else if (cur_sym == "(") {
            level++;
            operandsTracker.push(re.at(i));
        } else if (cur_sym == ")") {
            int or_count = 0;
            char c;
            vector<NFA *> selections;
            op_sym = operandsTracker.top();
            while (op_sym != '(') {
                operandsTracker.pop();
                op_sym = operandsTracker.top();
                if (op_sym == '|') {or_count++;}
            }
            operandsTracker.pop();
            if (or_count > 0) {
                selections.assign(or_count + 1, new NFA());
                int tracker = or_count;
                for (int i = 0; i < or_count + 1; i++) {
                    selections.at(tracker) = operands.top();
                    tracker--;
                    operands.pop();
                }
                operands.push(unionNFA(selections, or_count + 1));
            }
            level--;
            if (i < re.size() - 1 && re.at(i + 1) != '*' && re.at(i + 1) != '+') {
                while (!operandsTracker.empty() && operandsTracker.top() == 's') {
                    operandsTracker.pop();
                    NFA *nfa2 = operands.top();
                    operands.pop();
                    NFA *nfa1 = operands.top();
                    operands.pop();
                    operands.push(concatenate(nfa1, nfa2));
                }
                while (!operandsTracker.empty() && operandsTracker.top() == '|') {
                    operandsTracker.pop();
                    operandsTracker.pop();
                    NFA *nfa2 = operands.top();
                    operands.pop();
                    NFA *nfa1 = operands.top();
                    operands.pop();
                    operands.push(unionNFA({nfa1, nfa2}, 2));
                }
            } else if (i == re.size() - 1){
                while (!operandsTracker.empty() && operandsTracker.top() == 's') {
                    operandsTracker.pop();
                    NFA *nfa2 = operands.top();
                    operands.pop();
                    NFA *nfa1 = operands.top();
                    operands.pop();
                    operands.push(concatenate(nfa1, nfa2));
                }
                while (!operandsTracker.empty() && operandsTracker.top() == '|') {
                    operandsTracker.pop();
                    operandsTracker.pop();
                    NFA *nfa2 = operands.top();
                    operands.pop();
                    NFA *nfa1 = operands.top();
                    operands.pop();
                    operands.push(unionNFA({nfa1, nfa2}, 2));
                }
            }
            operandsTracker.push('s');
        }
    }
    operands.top()->setTokenType(tokenType);
    //operands.top()->printTransitions();
   // printf("\nnumber of NFA = %d\n", operands.size());
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
