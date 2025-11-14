
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

struct Question { 
    string type; // "MCQ" or "TF"
    string text;
    vector<string> options;
    int correctAnswer;
};

vector<Question> loadQuestions(const string& filename) {
    vector<Question> questions;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return questions;
    }

    while (!file.eof()) {
        Question q;
        getline(file, q.type);
        if (q.type.empty()) break; // Avoid reading empty lines at end
        getline(file, q.text);

        if (q.type == "MCQ") {
            q.options.resize(4);
            for (int i = 0; i < 4; i++) {
                getline(file, q.options[i]);
            }
        } else if (q.type == "TF") {
            q.options = {"True", "False"};
        }

        file >> q.correctAnswer;
        file.ignore(); // Ignore newline after number
        questions.push_back(q);
    }
    return questions;
}

int main() {
    vector<Question> questions = loadQuestions("questions.txt");
    if (questions.empty()) {
        cout << "No questions loaded. Check your file format." << endl;
        return 1;
    }

    srand(static_cast<unsigned>(time(0)));
    random_shuffle(questions.begin(), questions.end());

    int score = 0;

    for (size_t i = 0; i < questions.size(); i++) {
        cout << "\nQ" << i + 1 << ": " << questions[i].text << endl;
        for (int j = 0; j < questions[i].options.size(); j++) {
            cout << j + 1 << ". " << questions[i].options[j] << endl;
        }

        int answer;
        while (true) {
            cout << "Your answer: ";
            cin >> answer;
            if (cin.fail() || answer < 1 || answer > (int)questions[i].options.size()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number between 1 and "
                     << questions[i].options.size() << ".\n";
            } else break;
        }

        if (answer == questions[i].correctAnswer) {
            cout << "✅ Correct!\n";
            score++;
        } else {
            cout << "❌ Wrong! Correct answer: "
                 << questions[i].options[questions[i].correctAnswer - 1] << "\n";
        }
    }

    cout << "\nYour final score: " << score << "/" << questions.size() << endl;
    double percentage = (double)score / questions.size() * 100;
    cout << "Percentage: " << percentage << "%\n";

    return 0;
}
