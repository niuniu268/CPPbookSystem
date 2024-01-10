#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// ContactInformation struct represents the contact information
struct ContactInformation {
    std::string Firstname;
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

    // Member functions for searching by firstname, Password, and free searching
    void FirstnameSeek();
    void PasswordSeek();
    void FreeSeek();

    // Member function to display the search results
    void ShowSearchedItem(const std::vector<ContactInformation>& results);
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

// Function to write data to a file
void OutputTextFile(const std::vector<ContactInformation>& data) {
    std::ofstream file("/Users/Mac/CLionProjects/SearchNCheck/database.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file 1." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& ci : data) {
        file << ci.Firstname << "," << ci.Password << "\n";
    }

    file.close();
}

// Main function
int main() {
    std::vector<ContactInformation> data = InputText();

    while (true) {
        std::cout << "Which account do you want to use?" << std::endl;
        std::cout << "1. Guest users" << std::endl;
        std::cout << "2. Administrator" << std::endl;
        std::cout << "3. Quit" << std::endl;

        int option;
        std::cout << "Choose an option: ";
        std::cin >> option;

        switch (option) {
            case 1: {
                GuestUsers guestUsers;
                guestUsers.Data = data;
                guestUsers.Questions();
                break;
            }
            case 2: {
                Administrator administrator;
                administrator.Data = data;
                administrator.Questions();
                break;
            }
            case 3:
                // Save data to a file before exiting
                OutputTextFile(data);
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
    std::cout << "Firstname: ";
    std::cin >> contactInfo.Firstname;
    std::cout << "Password: ";
    std::cin >> contactInfo.Password;

    Data.push_back(contactInfo);

    std::cout << "New contact information added:" << std::endl;
    std::cout << contactInfo.Firstname << "," << contactInfo.Password << std::endl;
}

void Users::Seek() {
    std::cout << "1. Search by firstname" << std::endl;
    std::cout << "2. Search by Password" << std::endl;
    std::cout << "3. Free searching" << std::endl;

    int option;
    std::cout << "Choose a search method: ";
    std::cin >> option;

    switch (option) {
        case 1:
            FirstnameSeek();
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

void Users::FirstnameSeek() {
    std::string firstname;
    std::cout << "Enter firstname: ";
    std::cin >> firstname;

    std::vector<ContactInformation> results;
    std::copy_if(Data.begin(), Data.end(), std::back_inserter(results),
                 [firstname](const ContactInformation& ci) {
                     return ci.Firstname == firstname;
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
                     return ci.Firstname.find(search) != std::string::npos ||
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
            std::cout << ci.Firstname << "," << ci.Password << std::endl;
        }
    }
}
