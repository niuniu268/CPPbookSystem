#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// ContactInformation struct represents the contact information
struct ContactInformation {
    std::string Username;
    std::string Password;
};

// Users class represents the users with contact information
class Users {
public:
    std::vector<ContactInformation> Data;

    // Member function to display the menu for the users
    void Questions();

    // Member function to add a new contact information to the user's data
    void Add();

    // Member function to prompt the user to choose a search method
    void Seek();

    // Member functions for searching by Username, Password, and free searching
    void UsernameSeek();
    void PasswordSeek();
    void FreeSeek();

    // Member function to display the search results
    static void ShowSearchedItem(const std::vector<ContactInformation>& results);

    // Member function to write data to a file
    void OutputTextFile() const;
};

// GuestUsers class represents guest users
class GuestUsers : public Users {
};

// Administrator class represents an administrator
class Administrator : public Users {
};

// Function to read data from a file and return a vector of ContactInformation
std::vector<ContactInformation> InputText() {
    std::ifstream file("/Users/Mac/CLionProjects/SearchNCheck/database.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<ContactInformation> data;
    std::string line;

    while (std::getline(file, line)) {
        size_t commaPos = line.find(',');
        ContactInformation contactInfo = {
                line.substr(0, commaPos),
                line.substr(commaPos + 1)
        };
        data.push_back(contactInfo);
    }

    file.close();
    return data;
}

// Main function
int main() {
    std::vector<ContactInformation> data = InputText();

    while (true) {
        std::cout << "Which account do you want to use?" << std::endl;
        std::cout << "1. Users" << std::endl;
        std::cout << "2. Quit" << std::endl;

        int option;
        std::cout << "Choose an option: ";
        std::cin >> option;

        Users users;
        users.Data = data;

        switch (option) {
            case 1:
                users.Questions();
                break;
            case 2:
                // Save data to a file before exiting
                users.OutputTextFile();
                return 0;
            default:
                std::cout << "Invalid option" << std::endl;
        }
    }

    return 0;
}

// Member function definitions for Users class
void Users::Questions() {
    std::cout << "1. Add" << std::endl;
    std::cout << "2. Search" << std::endl;

    int option;
    std::cout << "Choose an option: ";
    std::cin >> option;

    switch (option) {
        case 1:
            Add();
            break;
        case 2:
            Seek();
            break;
        default:
            std::cout << "Invalid option" << std::endl;
    }
}

void Users::Add() {
    ContactInformation contactInfo;
    std::cout << "Username: ";
    std::cin >> contactInfo.Username;
    std::cout << "Password: ";
    std::cin >> contactInfo.Password;

    Data.push_back(contactInfo);

    std::cout << "New contact information added:" << std::endl;
    std::cout << contactInfo.Username << "," << contactInfo.Password << std::endl;

    OutputTextFile();  // Save changes to the file
}

void Users::Seek() {
    std::cout << "1. Search by Username" << std::endl;
    std::cout << "2. Search by Password" << std::endl;
    std::cout << "3. Free searching" << std::endl;

    int option;
    std::cout << "Choose a search method: ";
    std::cin >> option;

    switch (option) {
        case 1:
            UsernameSeek();
            break;
        case 2:
            PasswordSeek();
            break;
        case 3:
            FreeSeek();
            break;
        default:
            std::cout << "Invalid option" << std::endl;
    }
}

void Users::UsernameSeek() {
    std::string Username;
    std::cout << "Enter Username: ";
    std::cin >> Username;

    std::vector<ContactInformation> results;
    std::copy_if(Data.begin(), Data.end(), std::back_inserter(results),
                 [Username](const ContactInformation& ci) {
                     return ci.Username == Username;
                 });

    ShowSearchedItem(results);
}

void Users::PasswordSeek() {
    std::string Password;
    std::cout << "Enter Password: ";
    std::cin >> Password;

    std::vector<ContactInformation> results;
    std::copy_if(Data.begin(), Data.end(), std::back_inserter(results),
                 [Password](const ContactInformation& ci) {
                     return ci.Password == Password;
                 });

    ShowSearchedItem(results);
}

void Users::FreeSeek() {
    std::string search;
    std::cout << "Enter search query: ";
    std::cin >> search;

    std::vector<ContactInformation> results;
    std::copy_if(Data.begin(), Data.end(), std::back_inserter(results),
                 [search](const ContactInformation& ci) {
                     return ci.Username.find(search) != std::string::npos ||
                            ci.Password.find(search) != std::string::npos;
                 });

    ShowSearchedItem(results);
}

void Users::ShowSearchedItem(const std::vector<ContactInformation>& results) {
    if (results.empty()) {
        std::cout << "No results found." << std::endl;
    } else {
        std::cout << "Search results:" << std::endl;
        for (const auto& ci : results) {
            std::cout << ci.Username << "," << ci.Password << std::endl;
        }
    }
}

void Users::OutputTextFile() const {
    std::ofstream file("/Users/Mac/CLionProjects/SearchNCheck/database.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& ci : Data) {
        file << ci.Username << "," << ci.Password << "\n";
    }

    file.close();
}
