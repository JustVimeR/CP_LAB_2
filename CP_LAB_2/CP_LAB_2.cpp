#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

// Структура для представлення скінченого автомата
struct FiniteAutomaton {
    set<int> states; // Множина станів
    int initial_state; // Початковий стан
    set<int> final_states; // Множина фінальних станів
    map<pair<int, char>, int> transitions; // Функція переходів (стан, символ) -> новий стан
};

// Функція для завантаження скінченого автомата з файлу
FiniteAutomaton loadAutomatonFromFile(const string& filename) {
    ifstream file(filename);
    FiniteAutomaton automaton;

    int num_states, num_final_states;
    file >> num_states;
    file >> num_final_states;

    for (int i = 0; i < num_states; i++) {
        int state;
        file >> state;
        automaton.states.insert(state);
    }

    file >> automaton.initial_state;

    for (int i = 0; i < num_final_states; i++) {
        int final_state;
        file >> final_state;
        automaton.final_states.insert(final_state);
    }

    int state1, state2;
    char symbol;
    while (file >> state1 >> symbol >> state2) {
        automaton.transitions[{state1, symbol}] = state2;
    }

    return automaton;
}

// Функція для перевірки, чи допускає автомат слово w = w1w0
bool acceptsWord(FiniteAutomaton& automaton, const string& w0, const string& w1) {
    int current_state = automaton.initial_state;

    for (char symbol : w0) {
        // Переход до нового стану згідно з функцією переходів
        if (automaton.transitions.find({ current_state, symbol }) != automaton.transitions.end()) {
            current_state = automaton.transitions[{current_state, symbol}];
        }
        else {
            // Немає переходу для символу, слово не допускається
            return false;
        }
    }

    // Перевірка, чи допускається слово w1 з поточного стану
    for (char symbol : w1) {
        if (automaton.transitions.find({ current_state, symbol }) != automaton.transitions.end()) {
            current_state = automaton.transitions[{current_state, symbol}];
        }
        else {
            return false;
        }
    }

    // Перевірка, чи поточний стан є фінальним
    return automaton.final_states.find(current_state) != automaton.final_states.end();
}

int main() {
    // Завантаження автомата з файлу
    FiniteAutomaton automaton = loadAutomatonFromFile("automaton.txt");

    // Введення слова w0 та w1
    string w0, w1;
    cout << "Enter w0: ";
    cin >> w0;
    cout << "Enter w1: ";
    cin >> w1;

    // Перевірка, чи допускається слово w = w1w0
    if (acceptsWord(automaton, w0, w1)) {
        cout << "The automaton accepts the word w = " << w1 << w0 << endl;
    }
    else {
        cout << "The automaton does not accept the word w = " << w1 << w0 << endl;
    }

    return 0;
}
