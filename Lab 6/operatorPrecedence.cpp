#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

map<string, set<string>> firstOP;
map<string, set<string>> lastOP;
vector<string> productions;
map<string, vector<string>> production_dictionary;
vector<vector<string>> table_list;
vector<string> terminals;
vector<vector<string>> terminal_matrix;

void add_to_firstOP(string nterm, string symbol) {
    if (firstOP.find(nterm) == firstOP.end()) {
        firstOP[nterm] = set<string>();
    }
    firstOP[nterm].insert(symbol);
}

void add_to_lastOP(string nterm, string symbol) {
    if (lastOP.find(nterm) == lastOP.end()) {
        lastOP[nterm] = set<string>();
    }
    lastOP[nterm].insert(symbol);
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

#include <iterator>

vector<string> splitWhitespace(const string& s) {
    istringstream iss(s);
    vector<string> tokens;
    copy(std::istream_iterator<string>(iss), std::istream_iterator<string>(), back_inserter(tokens));
    return tokens;
}

vector<string> parse_expression(string str);

vector<string> stack;
vector<string> input_buffer;
int index;

vector<string> parse_expression(string str) {
    stack.push_back("$");
    vector<string> input_buffer = splitWhitespace(str);
    index = 0;

    while (!stack.empty()) {
        string top_stack = stack.back();
        string current_input = input_buffer[index];
        int top_stack_index = find(terminals.begin(), terminals.end(), top_stack) - terminals.begin();
        int current_input_index = find(terminals.begin(), terminals.end(), current_input) - terminals.begin();

        string relation = terminal_matrix[top_stack_index][current_input_index];

        if (relation == "<" || relation == "=") {
            stack.push_back(current_input);
            index++;
        } else if (relation == ">") {
            string popped = "";
            while (relation != "<") {
                popped = stack.back();
                stack.pop_back();
                top_stack = (stack.empty() ? "" : stack.back());
                top_stack_index = find(terminals.begin(), terminals.end(), top_stack) - terminals.begin();
                relation = terminal_matrix[top_stack_index][find(terminals.begin(), terminals.end(), popped) - terminals.begin()];
            }
        } else if (relation == "acc") {
            cout << "Input string is accepted." << endl;
            return stack;
        } else {
            cout << "Input string is not accepted." << endl;
            return stack;
        }
    }

    return stack;
}

int main() {
    int num_terminals;
    cout << "Enter the number of terminals: ";
    cin >> num_terminals;
    cout << "Enter the terminals:" << endl;
    for (int i = 0; i < num_terminals; i++) {
        string terminal;
        cin >> terminal;
        terminals.push_back(terminal);
    }

    int num_non_terminals;
    cout << "Enter the number of non-terminals: ";
    cin >> num_non_terminals;
    cout << "Enter the non-terminals:" << endl;
    vector<string> non_terminals(num_non_terminals);
    for (int i = 0; i < num_non_terminals; i++) {
        string non_terminal;
        cin >> non_terminal;
        non_terminals[i] = non_terminal;
    }

    string starting_symbol;
    cout << "Enter the starting symbol: ";
    cin >> starting_symbol;

    int num_productions;
    cout << "Enter the number of productions: ";
    cin >> num_productions;
    cout << "Enter the productions:" << endl;
    cin.ignore();  // Clear newline character from previous input
    for (int i = 0; i < num_productions; i++) {
        string production;
        getline(cin, production);
        productions.push_back(production);
    }

    for (const string& nT : non_terminals) {
        production_dictionary[nT] = vector<string>();
    }

    for (const string& production : productions) {
        vector<string> nonterm_to_prod = split(production, '-');
        vector<string> alternatives = split(nonterm_to_prod[1], '|');
        for (const string& alternative : alternatives) {
            production_dictionary[nonterm_to_prod[0]].push_back(alternative);
        }
    }

    cout << "Populated production dictionary:" << endl;
    for (const auto& pair : production_dictionary) {
        cout << pair.first << " -> ";
        for (const string& prod : pair.second) {
            cout << prod << " | ";
        }
        cout << endl;
    }

    string parsing_string;
    cout << "Enter an expression to parse: ";
    getline(cin, parsing_string);

    // Compute firstOP for each non-terminal
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (const string& symbol : symbols) {
                if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                    add_to_firstOP(non_terminal, symbol);
                } else if (find(terminals.begin(), terminals.end(), symbol) != terminals.end()) {
                    add_to_firstOP(non_terminal, symbol);
                    break;
                }
            }
        }
    }

    // Compute lastOP for each non-terminal
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
                const string& symbol = *it;
                if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                    add_to_lastOP(non_terminal, symbol);
                } else if (find(terminals.begin(), terminals.end(), symbol) != terminals.end()) {
                    add_to_lastOP(non_terminal, symbol);
                    break;
                }
            }
        }
    }

    // Print the firstOP and lastOP sets
    cout << "firstOP:" << endl;
    for (const auto& pair : firstOP) {
        cout << "firstOP(" << pair.first << ") = {";
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    cout << "lastOP:" << endl;
    for (const auto& pair : lastOP) {
        cout << "lastOP(" << pair.first << ") = {";
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    // Compute the terminal matrix
    for (const string& terminal : terminals) {
        vector<string> row;
        for (const string& terminal2 : terminals) {
            row.push_back("");
        }
        terminal_matrix.push_back(row);
    }

    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (int i = 0; i < symbols.size() - 1; i++) {
                const string& symbol1 = symbols[i];
                const string& symbol2 = symbols[i + 1];
                if (find(terminals.begin(), terminals.end(), symbol1) != terminals.end() &&
                    find(terminals.begin(), terminals.end(), symbol2) != terminals.end()) {
                    int symbol1_index = find(terminals.begin(), terminals.end(), symbol1) - terminals.begin();
                    int symbol2_index = find(terminals.begin(), terminals.end(), symbol2) - terminals.begin();
                    if (terminal_matrix[symbol1_index][symbol2_index] == "") {
                        terminal_matrix[symbol1_index][symbol2_index] = "=";
                    } else {
                        cout << "Error: Conflicting relations between " << symbol1 << " and " << symbol2 << endl;
                        return 1;
                    }
                }
            }
        }
    }

    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (int i = 0; i < symbols.size() - 1; i++) {
                const string& symbol1 = symbols[i];
                const string& symbol2 = symbols[i + 1];
                if (find(terminals.begin(), terminals.end(), symbol1) != terminals.end() &&
                    find(non_terminals.begin(), non_terminals.end(), symbol2) != non_terminals.end()) {
                    int symbol1_index = find(terminals.begin(), terminals.end(), symbol1) - terminals.begin();
                    int symbol2_index = find(non_terminals.begin(), non_terminals.end(), symbol2) - non_terminals.begin();
                    if (terminal_matrix[symbol1_index][symbol2_index] == "") {
                        terminal_matrix[symbol1_index][symbol2_index] = "<";
                    } else {
                        cout << "Error: Conflicting relations between " << symbol1 << " and " << symbol2 << endl;
                        return 1;
                    }
                }
            }
        }
    }

    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (int i = 0; i < symbols.size() - 1; i++) {
                const string& symbol1 = symbols[i];
                const string& symbol2 = symbols[i + 1];
                if (find(non_terminals.begin(), non_terminals.end(), symbol1) != non_terminals.end() &&
                    find(terminals.begin(), terminals.end(), symbol2) != terminals.end()) {
                    int symbol1_index = find(non_terminals.begin(), non_terminals.end(), symbol1) - non_terminals.begin();
                    int symbol2_index = find(terminals.begin(), terminals.end(), symbol2) - terminals.begin();
                    if (terminal_matrix[symbol1_index][symbol2_index] == "") {
                        terminal_matrix[symbol1_index][symbol2_index] = ">";
                    } else {
                        cout << "Error: Conflicting relations between " << symbol1 << " and " << symbol2 << endl;
                        return 1;
                    }
                }
            }
        }
    }

    // Print the terminal matrix
    cout << "Terminal matrix:" << endl;
    cout << "    ";
    for (const string& terminal : terminals) {
        cout << terminal << " ";
    }
    cout << endl;
    for (int i = 0; i < terminal_matrix.size(); i++) {
        cout << terminals[i] << " ";
        for (int j = 0; j < terminal_matrix[i].size(); j++) {
            cout << terminal_matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Parse the input string
    parse_expression(parsing_string);

    return 0;
}
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    cout << "lastOP:" << endl;
    for (const auto& pair : lastOP) {
        cout << "lastOP(" << pair.first << ") = {";
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    int counter = 0;
    while (counter < num_productions) {
        for (const auto& pair : firstOP) {
            const string& non_terminal = pair.first;
            set<string> first_set = pair.second;
            set<string> first_set_copy = first_set;
            for (const string& symbol : first_set_copy) {
                if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                    firstOP[non_terminal].insert(firstOP[symbol].begin(), firstOP[symbol].end());
                }
            }
        }
        counter++;
    }
    
    // Remove non-terminals from lastOP sets
    counter = 0;
    while (counter < num_productions) {
        for (const auto& pair : lastOP) {
            const string& non_terminal = pair.first;
            set<string> last_set = pair.second;
            set<string> last_set_copy = last_set;
            for (const string& symbol : last_set_copy) {
                if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                    lastOP[non_terminal].insert(lastOP[symbol].begin(), lastOP[symbol].end());
                }
            }
        }
        counter++;
    }

    // Remove non-terminals from firstOP sets
    for (const auto& pair : firstOP) {
        const string& non_terminal = pair.first;
        set<string> first_set = pair.second;
        set<string> first_set_copy = first_set;
        for (const string& symbol : first_set_copy) {
            if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                firstOP[non_terminal].erase(symbol);
            }
        }
    }

    // Remove non-terminals from lastOP sets
    for (const auto& pair : lastOP) {
        const string& non_terminal = pair.first;
        set<string> last_set = pair.second;
        set<string> last_set_copy = last_set;
        for (const string& symbol : last_set_copy) {
            if (find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                lastOP[non_terminal].erase(symbol);
            }
        }
    }

    cout << "FirstOP:" << endl;
    for (const auto& pair : firstOP) {
        cout << "FirstOP(" << pair.first << ") = {";
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    cout << "LastOP:" << endl;
    for (const auto& pair : lastOP) {
        cout << "LastOP(" << pair.first << ") = {";
        for (const string& symbol : pair.second) {
            cout << symbol << ", ";
        }
        cout << "}" << endl;
    }

    terminals.push_back("$");
    terminal_matrix = vector<vector<string>>(terminals.size(), vector<string>(terminals.size(), " "));

    // Rule 1: Whenever terminal 'a' immediately precedes non-terminal B in any production, put a <·α where α is any terminal in the firstOP+ list of B
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (size_t i = 0; i < symbols.size() - 1; i++) {
                if (find(terminals.begin(), terminals.end(), symbols[i]) != terminals.end() &&
                    find(non_terminals.begin(), non_terminals.end(), symbols[i + 1]) != non_terminals.end()) {
                    for (const string& alpha : firstOP[symbols[i + 1]]) {
                        int row_index = find(terminals.begin(), terminals.end(), symbols[i]) - terminals.begin();
                        int col_index = find(terminals.begin(), terminals.end(), alpha) - terminals.begin();
                        terminal_matrix[row_index][col_index] = "<";
                    }
                }
            }
        }
    }

    // Rule 2: Whenever terminal 'b' immediately follows non-terminal C in any production, put β ·>b where β is any terminal in the lastOP+ list of C
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (size_t i = 1; i < symbols.size(); i++) {
                if (find(non_terminals.begin(), non_terminals.end(), symbols[i - 1]) != non_terminals.end() &&
                    find(terminals.begin(), terminals.end(), symbols[i]) != terminals.end()) {
                    for (const string& beta : lastOP[symbols[i - 1]]) {
                        int row_index = find(terminals.begin(), terminals.end(), beta) - terminals.begin();
                        int col_index = find(terminals.begin(), terminals.end(), symbols[i]) - terminals.begin();
                        terminal_matrix[row_index][col_index] = ">";
                    }
                }
            }
        }
    }

    // Rule 3: Whenever a sequence aBc or ac occurs in any production, put a ≐ c
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            vector<string> symbols = splitWhitespace(production);
            for (size_t i = 1; i < symbols.size() - 1; i++) {
                if (find(terminals.begin(), terminals.end(), symbols[i - 1]) != terminals.end() &&
                    find(terminals.begin(), terminals.end(), symbols[i + 1]) != terminals.end()) {
                    int row_index = find(terminals.begin(), terminals.end(), symbols[i - 1]) - terminals.begin();
                    int col_index = find(terminals.begin(), terminals.end(), symbols[i + 1]) - terminals.begin();
                    terminal_matrix[row_index][col_index] = "=";
                }
            }
        }
    }

    // Rule 4: Whenever a production A → ε occurs, put $ ·> $
    for (const string& non_terminal : non_terminals) {
        for (const string& production : production_dictionary[non_terminal]) {
            if (production == "ε") {
                int row_index = find(terminals.begin(), terminals.end(), "$") - terminals.begin();
                int col_index = find(terminals.begin(), terminals.end(), "$") - terminals.begin();
                terminal_matrix[row_index][col_index] = ">";
            }
        }
    }

    // Rule 5: Whenever a production S → α occurs, put $ <· α
    for (const string& production : production_dictionary[starting_symbol]) {
        int row_index = find(terminals.begin(), terminals.end(), "$") - terminals.begin();
        int col_index = find(terminals.begin(), terminals.end(), production) - terminals.begin();
        terminal_matrix[row_index][col_index] = "<";
    }

    // Print the terminal matrix
    cout << "Terminal matrix:" << endl;
    cout << "  ";
    for (const string& terminal : terminals) {
        cout << terminal << " ";
    }
    cout << endl;
    for (size_t i = 0; i < terminal_matrix.size(); i++) {
        cout << terminals[i] << " ";
        for (size_t j = 0; j < terminal_matrix[i].size(); j++) {
            cout << terminal_matrix[i][j] << " ";
        }
        cout << endl;
    }

    parse_expression(parsing_string);

    return 0;
}

