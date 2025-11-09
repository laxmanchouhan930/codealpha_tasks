#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>   // for SHA256 hashing (OpenSSL library)
using namespace std;

// Function to hash passwords using SHA-256
string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)str.c_str(), str.size(), hash);

    stringstream ss;
    for (unsigned char c : hash)
        ss << hex << setw(2) << setfill('0') << (int)c;
    return ss.str();
}

// Function to load existing users from file into a map
unordered_map<string, string> loadUsers(const string &filename) {
    unordered_map<string, string> users;
    ifstream file(filename);
    if (!file.is_open()) return users;

    string username, passwordHash;
    while (file >> username >> passwordHash) {
        users[username] = passwordHash;
    }
    file.close();
    return users;
}

// Function to save a new user to the file
void saveUser(const string &filename, const string &username, const string &passwordHash) {
    ofstream file(filename, ios::app); // append mode
    if (file.is_open()) {
        file << username << " " << passwordHash << "\n";
        file.close();
    }
}

// Function to validate a username and password
bool isValidInput(const string &username, const string &password) {
    if (username.size() < 3 || password.size() < 5) {
        cout << "❌ Username must be at least 3 characters and password at least 5 characters long.\n";
        return false;
    }
    for (char c : username) {
        if (!isalnum(c)) {
            cout << "❌ Username must be alphanumeric only.\n";
            return false;
        }
    }
    return true;
}

// Registration function
void registerUser(const string &filename) {
    string username, password;
    cout << "\n--- User Registration ---\n";
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    if (!isValidInput(username, password))
        return;

    auto users = loadUsers(filename);
    if (users.find(username) != users.end()) {
        cout << "⚠️ Username already exists. Please choose another.\n";
        return;
    }

    string passwordHash = sha256(password);
    saveUser(filename, username, passwordHash);
    cout << "✅ Registration successful for user: " << username << "\n";
}

// Login function
void loginUser(const string &filename) {
    string username, password;
    cout << "\n--- User Login ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    auto users = loadUsers(filename);
    if (users.find(username) == users.end()) {
        cout << "❌ Username not found. Please register first.\n";
        return;
    }

    string inputHash = sha256(password);
    if (users[username] == inputHash) {
        cout << "✅ Login successful. Welcome back, " << username << "!\n";
    } else {
        cout << "❌ Incorrect password.\n";
    }
}

// Main program menu
int main() {
    const string filename = "users.txt";
    int choice;

    cout << "====== Secure Registration & Login System ======\n";

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nChoose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser(filename);
                break;
            case 2:
                loginUser(filename);
                break;
            case 3:
                cout << "👋 Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }
    }
}
