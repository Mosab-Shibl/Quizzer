#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

class Quiz {
private:
    string name;
    int num;

public:
    void set_name(string n) { name = n; }
    void set_num(int n) { num = n; }

    string get_name() { return name; }
    int get_num() { return num; }
};

void create_quiz();
char options_menu();
void show_quizzes();
int quizzes_num();
bool check_file();
void input_questions(string name, int num);
void start_quiz(string name, int q_num);
string find_quiz(int in_num);
int find_questions(int in_num);
bool get_n_check(char r);
void score(int q_num, int correct);
void delete_question (int in_num, int q_num);
void delete_quiz (int in_num, int q_num);
void add_question (string quiz, int q_num);
void edit_quiz (string quiz, int in_num, int q_num);

int main() {
    cout << "\t\t\t== Welcome to Quizzer ==\n";
    char choice = options_menu();

    if (choice == '2') {
        show_quizzes();
    } else {
        create_quiz();
    }

    return 0;
}

void create_quiz() {
    Quiz Q1;
    string name;
    int num;

    cout << "Enter your quiz name: ";
    cin >> name;
    Q1.set_name(name);

    do {
        cout << "Enter the number of questions: ";
        if (cin >> num) {
            Q1.set_num(num);
            break;
        } else {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "You can only add a number.\n";
        }
    } while (true);

    ofstream out("Data/Quizzes.txt", ios::app);
    out << Q1.get_name() << endl;
    out << Q1.get_num() << endl;
    out.close();

    cin.ignore();
    input_questions(Q1.get_name(), Q1.get_num());

    cout << "\t\t\t== " << Q1.get_name() << " is created successfully ==\n";
    cout << "Do you want to show all available quizzes? (y/n) ";

    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        show_quizzes();
    } else {
        exit(0);
    }
}

char options_menu() {
    char choice;
    cout << "1. Create a new quiz." << endl;
    cout << "2. Show existing quizzes." << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice != '1' && choice != '2') { cout << "You have to choose between 1, and 2.\n"; options_menu(); }

    else
        return choice;
}

void show_quizzes() {

    if (check_file()) {
        ifstream in("Data/Quizzes.txt");

        cout << "\t\t\t== Available quizzes ==\n";

        int quizzes_number = quizzes_num();
        string name;
        int* num = new int[quizzes_number];

        int i = 0;
        int j = 1;
        while (getline(in, name)) {
            cout << j++ << ". " << name << '\n';
            in >> num[i];
            in.ignore();
            i++;
        }

        in.close();

        cout << "Choose a quiz: ";
        int in_num;
        cin >> in_num;

        string quiz = find_quiz(in_num);
        int q_num = find_questions(in_num);

        cout << "\t\t\t = " << quiz << " =" << " = "<< q_num  <<"Q =" <<endl;
        cout << "1. Start. \n";
        cout << "2. Edit. \n";
        cout << "3. Delete. \n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        if (choice == '1')
            start_quiz(quiz, q_num);

        else if (choice == '2')
            edit_quiz(quiz, in_num, q_num);

        else if (choice == '3')
            delete_quiz(in_num, q_num);

        else
            cout << "You have to choose between 1, 2, and 3.\n";


        delete[] num;

    } else {
        cout << "\t\t\t== No quizzes are available ==\n";
        cout << "Do you want to create a new quiz? (y/n) ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            create_quiz();
        } else {
            exit(0);
        }
    }
}

int quizzes_num() {
    ifstream in("Data/Quizzes.txt");
    string line;
    int count = 0;
    while (getline(in, line)) {
        count++;
    }
    in.close();
    return count / 2;
}

bool check_file() {
    ifstream in("Data/Quizzes.txt");
    in.seekg(0, ios::end);
    bool isEmpty = (in.tellg() == 0);
    in.close();
    return !isEmpty;
}

void input_questions(string name, int q_num) {

    ofstream out("Data/" + name + ".txt", ios::app);
    string q;
    string a[4];
    char right;
    char ch = 'a';

    for (int i = 0; i < q_num; i++) {
        if (q_num == 1) { cout << "Enter the question: "; }

        else if (q_num <= 0) {

                cout << "You have to add at least one question. \n";
                input_questions(name,  q_num);
        }

        else { cout << "Enter question number " << (i + 1) << " : "; }

        getline(cin, q);
        out << q << endl;

        for (int j = 0; j < 4; j++) {
            cout << "Enter choice (" << (char)(ch + j) << "): ";
            getline(cin, a[j]);
            out << a[j] << endl;
        }

        cout << "Which one is the right choice (a/b/c/d): ";
        cin >> right;
        out << right << endl;
        cin.ignore();
    }

    out.close();
}

void start_quiz(string name, int q_num) {
    int correct = 0;
    ifstream in("Data/" + name + ".txt");

    for (int i = 0; i < q_num; i++) {
        string question;
        getline(in, question);
        cout << (i + 1) << ". " << question << endl;

        for (int j = 0; j < 4; j++) {
            string choices;
            char ch = 'a';
            getline(in, choices);
            cout << (char)(ch + j) << ") " << choices << endl;
        }

        string ans;
        getline(in, ans);
        char answer = ans[0];

        if (get_n_check(answer)) {
            cout << "\t\t\t= Correct =\n";
            correct++;
        } else {
            cout << "\t\t\t= Wrong =\n";
        }
    }

    in.close();
    score(q_num, correct);
}

string find_quiz(int in_num) {

    ifstream in("Data/Quizzes.txt");
    string line;
    int quiz = (in_num + (in_num-1));

    for (int i=0; i<quiz; i++) { getline(in , line); }

    in.close();
    return line;

}

int find_questions(int in_num) {
    ifstream in("Data/Quizzes.txt");
    string line;
    int target_line = (in_num * 2) - 1;

    for (int i = 0; i <= target_line; i++) {
        getline(in, line);
    }

    in.close();
    return stoi(line);
}

bool get_n_check(char r) {
    char choice;
    cout << "\nChoose the right answer a, b, c, or d: ";
    cin >> choice;

    return (choice == r);
}

void score(int q_num, int correct) {
    cout << "Do you want to view your score? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "\t== Your score: " << correct << " / " << q_num << " ==\n";
    }

    else {
        cout << "Are you sure? (Your score won't be saved) (y/n): ";
        char ch;
        cin >> ch;

        if (ch == 'y' || ch == 'Y')
            exit(0);
    }
}


void edit_quiz (string quiz, int in_num, int q_num) {

    cout << "What do you want to do with " << quiz << "? \n";
    cout << "1. Add a question.\n";
    cout << "2. delete a question.\n";

    cout << "Enter your choice: ";
    char choice;
    cin >> choice;

    if (choice == '1') {
        add_question (quiz, q_num);
        q_num = find_questions(in_num);
    }

    else if (choice == '2') {
        delete_question (in_num,  q_num);
        q_num--;
    }

    else {
        cout << "You have to choose between 1, and 2.\n";
        edit_quiz (quiz, in_num, q_num);
    }

    char ch;

    do {
        cout << "1. Show Available quizzes.\n";
        cout << "2. Start " << quiz << " after your edit.\n";
        cout << "Enter your choice: ";
        cin >> ch;

        if (ch == '1') { show_quizzes(); }

        else if (ch == '2') { start_quiz(quiz, q_num); }

        else {
            cout << "You have to choose between 1, and 2.\n";
        }

    } while (ch != '1' && ch != '2');
}


void delete_question(int in_num, int q_num) {

    string q = find_quiz(in_num);
    string quiz = "Data/" + q + ".txt";

    cout << "Which question do you want to delete? ";
    int choice;
    cin >> choice;

    if (choice <= 0 || choice > q_num) return;

    ifstream in(quiz);
    ofstream out("Data/temp.txt");

    string line;
    int first_part = (choice - 1) * 6;
    int rest = (q_num * 6) - (first_part + 6);

    while (first_part--) {
        getline(in, line);
        out << line << endl;
    }

    for (int i = 0; i < 6; i++) getline(in, line);

    while (rest--) {
        getline(in, line);
        out << line << endl;
    }

    in.close();
    out.close();

    remove(quiz.c_str());
    rename("Data/temp.txt", quiz.c_str());

    ifstream qin("Data/Quizzes.txt");
    ofstream qout("Data/temp.txt");

    int line_num = 0;
    int target_line = (in_num * 2) - 1;
    while (getline(qin, line)) {
        if (line_num == target_line)
            qout << (q_num - 1) << endl;
        else
            qout << line << endl;
        line_num++;
    }

    qin.close();
    qout.close();

    remove("Data/Quizzes.txt");
    rename("Data/temp.txt", "Data/Quizzes.txt");

    cout << "\t\t\t = Question (" << choice << ") deleted successfully = \n";
}

void add_question(string quiz, int q_num) {
    ofstream out("Data/" + quiz + ".txt", ios::app);
    string q;
    string a[4];
    char right;
    char ch = 'a';
    int numbers;

    cout << "How many questions do you want to add? ";
    cin >> numbers;
    cin.ignore();

    for (int i = 0; i < numbers; i++) {
        if (numbers == 1) { cout << "Enter the question: "; }
        else if (numbers <= 0) {
            cout << "You have to add at least one question. \n";
            add_question(quiz, q_num);
        } else {
            cout << "Enter question number " << (q_num + i + 1) << " : ";
        }

        getline(cin, q);
        out << q << endl;

        for (int j = 0; j < 4; j++) {
            cout << "Enter choice (" << (char)(ch + j) << "): ";
            getline(cin, a[j]);
            out << a[j] << endl;
        }

        cout << "Which one is the right choice (a/b/c/d): ";
        cin >> right;
        out << right << endl;
        cin.ignore();
    }

    out.close();

    ifstream in("Data/Quizzes.txt");
    ofstream outq("Data/temp.txt");

    string line;
    int line_num = 0;
    int target = -1;

    while (getline(in, line)) {
        if (line == quiz) {
            target = line_num + 1;
        }

        if (line_num == target) {
            outq << (q_num + numbers) << endl;
        } else {
            outq << line << endl;
        }

        line_num++;
    }

    in.close();
    outq.close();

    remove("Data/Quizzes.txt");
    rename("Data/temp.txt", "Data/Quizzes.txt");
}



void delete_quiz(int in_num, int q_num) {
    string q = find_quiz(in_num);
    string quiz = "Data/" + q + ".txt";
    remove(quiz.c_str());

    ifstream in("Data/Quizzes.txt");
    ofstream out("Data/temp.txt");

    string line;
    int i = 1;

    while (getline(in, line)) {
        if (i != in_num * 2 - 1 && i != in_num * 2) {
            out << line << endl;
        }
        i++;
    }

    in.close();
    out.close();

    remove("Data/Quizzes.txt");
    rename("Data/temp.txt", "Data/Quizzes.txt");

    cout << "\t\t\t = " << q << " deleted successfully = \n";
}
