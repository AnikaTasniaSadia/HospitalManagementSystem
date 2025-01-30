#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>


using namespace std;

struct patient {
    long long ID;
    string firstname;
    string lastname;
    int age;
    char blood[5];
    char gender;
    patient* next;
};

class linkedqueue {
    patient* head, * last;

public:
    char departmentname[50];

    linkedqueue() {
        head = NULL;
        last = NULL;
    }

    patient input();
    void insertatend();
    void insertatbeg();
    void getpatientout();
    void listofpatients();
    int search(int);
    void saveToFile(patient* p);
    void updateFile();
    void loadFromFile();
};


int linkedqueue::search(int item) {
    patient* p = head;
    while (p != NULL) {
        if (p->ID == item)
            return true;
        p = p->next;
    }
    return false;
}


int readnumber() {
    string input;
    getline(cin, input);
    return atoi(input.c_str());
}


patient linkedqueue::input() {
    patient* p = new patient();
    cout << "\n Enter patient details:\n";
    cout << " First name: ";
    getline(cin, p->firstname);
    cout << " Last name: ";
    getline(cin, p->lastname);

    cout << " Blood Group: ";
    cin >> p->blood;

    cout << " Gender (m/f): ";
    cin >> p->gender;

    cout << " Age: ";
    cin >> p->age;

    cout << " Mobile number: ";
    cin >> p->ID;
    cin.ignore();

    if (search(p->ID)) {
        cout << "\n Data not valid. Operation cancelled.\n";
        p->ID = 0;
    }
    return *p;
}


void output(patient* p) {
    cout << "\n --------------------------";
    cout << "\n Patient Details:";
    cout << "\n First Name: " << p->firstname;
    cout << "\n Last Name: " << p->lastname;
    cout << "\n Gender: " << p->gender;
    cout << "\n Age: " << p->age;
    cout << "\n Blood Group: " << p->blood;
    cout << "\n Mobile Number: " << p->ID;
    cout << "\n --------------------------\n";
}


void linkedqueue::saveToFile(patient* p) {
    ofstream file("patients.txt", ios::app);
    if (file.is_open()) {
        file << p->ID << " " << p->firstname << " " << p->lastname << " "
             << p->age << " " << p->blood << " " << p->gender << "\n";
        file.close();
    } else {
        cout << "Error opening file!\n";
    }
}


void linkedqueue::loadFromFile() {
    ifstream file("patients.txt");
    if (!file.is_open()) return;

    patient* p;
    while (!file.eof()) {
        p = new patient();
        file >> p->ID >> p->firstname >> p->lastname >> p->age >> p->blood >> p->gender;
        if (file.fail()) break;

        if (head == NULL) {
            head = p;
            last = p;
            p->next = NULL;
        } else {
            last->next = p;
            last = p;
            p->next = NULL;
        }
    }
    file.close();
}


void linkedqueue::updateFile() {
    ofstream file("patients.txt", ios::trunc);
    if (!file.is_open()) return;

    patient* p = head;
    while (p != NULL) {
        file << p->ID << " " << p->firstname << " " << p->lastname << " "
             << p->age << " " << p->blood << " " << p->gender << "\n";
        p = p->next;
    }
    file.close();
}


void linkedqueue::insertatbeg() {
    patient* p = new patient();
    *p = input();
    if (p->ID == 0) return;

    p->next = head;
    head = p;
    if (last == NULL) last = p;

    saveToFile(p);
    cout << "\n Patient added:";
    output(p);
}

void linkedqueue::insertatend() {
    patient* p = new patient();
    *p = input();
    if (p->ID == 0) return;

    if (head == NULL) {
        head = p;
        last = p;
        p->next = NULL;
    } else {
        last->next = p;
        last = p;
        p->next = NULL;
    }

    saveToFile(p);
    cout << "\n Patient added:";
    output(p);
}


void linkedqueue::getpatientout() {
    if (head == NULL) {
        cout << "\n No Patient to operate\n";
        return;
    }
    patient* p = head;
    head = head->next;
    cout << "\n Patient to operate:";
    output(p);
    delete p;
    updateFile();
}


void linkedqueue::listofpatients() {
    ifstream file("patients.txt");
    if (!file.is_open()) {
        cout << "\n No patient data available.\n";
        return;
    }

    patient p;
    cout << "\n -------- Patient List --------";
    while (file >> p.ID >> p.firstname >> p.lastname >> p.age >> p.blood >> p.gender) {
        cout << "\n First Name: " << p.firstname;
        cout << "\n Last Name: " << p.lastname;
        cout << "\n Age: " << p.age;
        cout << "\n Blood Group: " << p.blood;
        cout << "\n Mobile Number: " << p.ID;
        cout << "\n ------------------------------";
    }
    file.close();
}


void departmentmenu(linkedqueue* q) {
    int choice = 0;
    while (choice != 5) {
        cout << "\n " << q->departmentname;
        cout << "\n [1] Add normal patient";
        cout << "\n [2] Add critically ill patient";
        cout << "\n [3] Take patient to Doctor";
        cout << "\n [4] Display list";
        cout << "\n [5] Change department or exit";
        cout << "\n Enter your choice: ";
        choice = readnumber();

        switch (choice) {
        case 1: q->insertatend(); break;
        case 2: q->insertatbeg(); break;
        case 3: q->getpatientout(); break;
        case 4: q->listofpatients(); break;
        }
    }
}


int main() {
    int choice = 0;
    linkedqueue departments[4];


    strcpy(departments[0].departmentname, "GENERAL CLINIC");
    strcpy(departments[1].departmentname, "HEART CLINIC");
    strcpy(departments[2].departmentname, "LUNG CLINIC");
    strcpy(departments[3].departmentname, "PLASTIC SURGERY");


    for (int i = 0; i < 4; i++) {
        departments[i].loadFromFile();
    }

    while (choice != 5) {
        cout << "\n Main Menu";
        for (int i = 0; i < 4; i++) {
            cout << "\n " << (i + 1) << ": " << departments[i].departmentname;
        }
        cout << "\n 5: Exit";
        cout << "\n Enter your choice: ";
        choice = readnumber();
        if (choice >= 1 && choice <= 4) {
            departmentmenu(&departments[choice - 1]);
        }
    }
    cout << "\n Thank you!\n";
    return 0;
}
