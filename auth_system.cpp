#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

// Very simple hash (for demonstration only!)
string hashPassword(const string &password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

bool loadUsers(unordered_map<string, string> &users) {
    ifstream file("users.db");
    if (!file) return false;
    string line, user, pass;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, user, ':');
        getline(ss, pass);
        users[user] = pass;
    }
    return true;
}

void saveUsers(const unordered_map<string, string> &users) {
    ofstream file("users.db");
    for (auto &p : users)
        file << p.first << ":" << p.second << "\n";
}

void registerUser() {
    unordered_map<string, string> users;
    loadUsers(users);

    string username, password;
    cout << "Enter username: ";
    cin >> username;

    if (users.find(username) != users.end()) {
        cout << "Error: Username already exists.\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    if (password.length() < 4) {
        cout << "Error: Password too short.\n";
        return;
    }

    users[username] = hashPassword(password);
    saveUsers(users);
    cout << "Registration successful!\n";
}

void loginUser() {
    unordered_map<string, string> users;
    if (!loadUsers(users)) {
        cout << "No registered users found.\n";
        return;
    }

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) == users.end()) {
        cout << "Error: Username not found.\n";
        return;
    }

    if (users[username] == hashPassword(password))
        cout << "Login successful! Welcome, " << username << ".\n";
    else
        cout << "Error: Incorrect password.\n";
}

int main() {
    int choice;
    cout << "1. Register\n2. Login\nEnter choice: ";
    cin >> choice;

    if (choice == 1)
        registerUser();
    else if (choice == 2)
        loginUser();
    else
        cout << "Invalid choice.\n";
    return 0;
}
