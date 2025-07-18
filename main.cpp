#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

class Quiz {
private:
    string name;
    int num;
    int pass_percnetage;

public:
    void set_name(string n) { name = n; }
    void set_num(int n) { num = n; }
    void set_percentage(int n) { pass_percnetage = n; }

    string get_name() { return name; }
    int get_num() { return num; }
    int get_percentage() { return pass_percnetage; }
};

void create_quiz();
char options_menu();
void show_quizzes();
int quizzes_num();
bool check_file();
void input_questions(string name, int num);
void start_quiz(string name, int q_num, int pass_percentage);
string find_quiz(int in_num);
int find_questions(int in_num);
bool get_n_check(char r);
void score(int q_num, int correct, int pass_percentage, string name);
void delete_question (int in_num, int q_num);
void delete_quiz (int in_num, int q_num);
void add_question (string quiz, int q_num);
void edit_quiz (string quiz, int in_num, int q_num);
void change_name (string quiz, int in_num);
void check_pass (float result, int pass_percentage, string name, int q_num);

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
    int pass;

    cout << "Enter your quiz name: ";
    cin >> name;
    Q1.set_name(name);

    do {

    cout << "Enter the required percentage to pass: ";

     if (!(cin >> pass)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "You can only add a number.\n";
    }

    else if (pass > 100 || pass < 0) { cout << "Required percentage to pass has to be between 0 and 100.\n"; }

    else { Q1.set_percentage(pass); break; }

    } while (true);

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
            start_quiz(quiz, q_num, 50);

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

void start_quiz(string name, int q_num, int pass_percentage) {

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
    score(q_num, correct, pass_percentage, name);

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

void score(int q_num, int correct, int pass_percentage, string name) {

    cout << "Do you want to view your score? (y/n): ";
    char choice;
    cin >> choice;

    float percentage = ((float)correct / q_num) * 100;

    if (choice == 'y' || choice == 'Y') {
        cout << "\t\t= Your score: " << correct << " / " << q_num << " = " " = " << percentage << "% =\n";
    }

    else {
        cout << "Are you sure? (Your score won't be saved) (y/n): ";
        char ch;
        cin >> ch;

        if (ch == 'y' || ch == 'Y')
            exit(0);
    }

    check_pass(percentage, pass_percentage, name, q_num);
}


void edit_quiz (string quiz, int in_num, int q_num) {

    cout << "What do you want to do with " << quiz << "? \n";
    cout << "1. Add a question.\n";
    cout << "2. delete a question.\n";
    cout << "3. Rename " << quiz <<".\n";

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

    else if (choice == '3') {
        change_name (quiz, in_num);

    }

    else {
        cout << "You have to choose between 1, 2, and 3.\n";
        edit_quiz (quiz, in_num, q_num);
    }

    char ch;

    do {
        cout << "1. Show Available quizzes.\n";

        if (choice == '3') { cout << "2. Start " << find_quiz(in_num) <<".\n"; }
        else { cout << "2. Start " << quiz << " after your edit.\n"; }
        cout << "Enter your choice: ";
        cin >> ch;

        if (ch == '1') { show_quizzes(); }

        else if (ch == '2') { start_quiz(quiz, q_num, 50); }

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

void change_name (string quiz, int in_num) {

    string name;
    cout << "Enter new name: ";
    cin >> name;

    string q = "Data/" + quiz + ".txt";
    string n = "Data/" + name + ".txt";
    rename(q.c_str(), n.c_str());

    ifstream in("Data/Quizzes.txt");
    ofstream out("Data/temp.txt");

    int target_line = (in_num * 2) -1;
    int first_part = target_line -1;
    int total = quizzes_num()*2;
    int rest = total - (first_part + 2);
    string line;

    while (first_part --) {

    getline(in, line);
    out << line << endl;

    }

    out << name << endl;
    getline(in, line);
    getline(in, line);
    out << line << endl;

    while (rest --) {

    getline(in, line);
    out << line << endl;

    }

    in.close();
    out.close();

    remove("Data/Quizzes.txt");
    rename("Data/temp.txt", "Data/Quizzes.txt");

}

void check_pass (float result, int pass_percentage, string name, int q_num) {

    if (result >= pass_percentage) {

        cout << "\t\t == Congratulation! You have passed ==\n";
        cout << "Do you want to start a new quiz? (y/n)";

        char ch;
        cin >> ch;

        if (ch == 'Y' || ch == 'y')
            show_quizzes();

        else
            exit(0);
    }
    else {

        cout << " \t\t == Unfortunately you have Failed! ==\n";
        cout << "\t\t == the required percentage to pass is: " << pass_percentage << " ==\n";
        cout << "Do you want to try again? (y/n)";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
            start_quiz(name, q_num, pass_percentage);

        else
            options_menu();
    }
}

