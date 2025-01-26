#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <list>
#include <functional>
#include <thread>
#include <chrono>
#include <limits>
#include <conio.h>
#include <windows.h>
#include <regex>

using namespace std;

struct Booking {
    int courtID;
    int bookingID; // Unique identifier for each booking
    string courtNumber;
    string courtType;
    double pricePerHour;
    string bookingStatus;
    string availableSlots;
    string courtLocation;
    string name;
    string contact;
    int day;
    int month;
    int year;
    string timeSlot;
    int hours; // Number of hours booked
    string userName; //added to identify user
    string specialRequest;
};

// Forward Declarations
class Court;
bool compareCourtsByID(const Court&, const Court&);
bool compareCourtsByType(const Court&, const Court&);
bool compareCourtsByPrice(const Court&, const Court&);
void displayCourt(const Court&);
void addCourt(vector<Court>&, const string&);
void addPickleballCourt(vector<Court>& courts, const string& filename);
void editCourt(vector<Court>&, const string&);
void deleteCourtFriend(vector<Court>&, int, const string&);
void searchByType(vector<Court>&, const string&);
void searchByPrice(vector<Court>&, double);

// Sorting Function Declarations
void sortCourtsByIDQuickSort(vector<Court>& courts);
int partition(vector<Court>& courts, int low, int high);
void quickSortCourtsByID(vector<Court>& courts, int low, int high);

// Function Declarations
void MainPage();
void UserMenu();
void AdminMenu();
void UserHomepage(const string& username, const string& name, const string& phoneNumber);
void AdminHomepage(const string& adminUsername); // Add the parameter
void bookingMenu(const string& username, const string& name, const string& phoneNumber);
void viewBookingHistory(const string& username, bool isAdmin);
void viewCourtAvailability();
void sortingMenu();
void adminRegister();
void userRegister();
void loading();
bool fileExists(const string& filename);
string getPassword(); // Function for password masking
bool readInteger(int& value); // Function to read integer with validation
bool readDouble(double& value); // Function to read double with validation
bool isBackCommand(const string& input); // Function to check if input is '0'
void editBooking(const string& username, const string& name, const string& phoneNumber);
void changeUserPassword(const string& username);
void changeAdminPassword(const string& username);
void generateAdminReport(const vector<Court>& courts, const vector<Booking>& bookings);
void deleteBookingRecord(const string& username);



// Loading Animation
void loading()
{
    cout << "Loading";
    for (int i = 0; i < 3; i++)
    {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(250)); // Sleep for 250 milliseconds
    }
    cout << endl;
}

// User Structure
struct User
{
    string username;
    string password; // Consider storing hashed passwords
    string name;
    string phoneNumber;
};

// Admin Structure
struct Admin
{
    string username;
    string password; // Consider storing hashed passwords
};

// Comparator functions for Bookings
bool compareBookingsByPrice(const Booking& a, const Booking& b) {
    return (a.pricePerHour * a.hours) < (b.pricePerHour * b.hours);
}

bool compareBookingsByStatus(const Booking& a, const Booking& b) {
    return a.bookingStatus < b.bookingStatus;
}

bool compareBookingsByType(const Booking& a, const Booking& b) {
    return a.courtType < b.courtType;
}

// Function to Sort Bookings Using a Custom Comparator (Added for sorting booking)
void sortBookings(std::vector<Booking>& bookings, bool (*compare)(const Booking&, const Booking&)) {
    std::sort(bookings.begin(), bookings.end(), compare);
}

// LockedUser Structure
struct LockedUser {
    string username;
    time_t unlockTime;
};

// Base Class: Person
class Person {
protected:
    string username;
    string password;

public:
    Person() : username(""), password("") {}
    Person(string uname, string pwd) : username(uname), password(pwd) {}

    virtual void displayInfo() const {
        cout << "Username: " << username << endl;
    }
    
      virtual void displayRole() const {
        cout << "Generic Person Role" << endl;
    }

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Person() {}
};

// Derived Class: UserClass
class UserClass : public Person {
public:
    string name;
    string phoneNumber;

	 void displayRole() const override {
        cout << "User Role" << endl;
    }
	
    UserClass() : Person() {}
    UserClass(string uname, string pwd, string nm, string phone) : Person(uname, pwd), name(nm), phoneNumber(phone) {}

    // Function to validate password strength
    bool isValidPassword(const string& password) {
            if (password.length() < 8)
                return false;

            bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
            for (char ch : password) {
                if (isupper(ch)) hasUpper = true;
                else if (islower(ch)) hasLower = true;
                else if (isdigit(ch)) hasDigit = true;
                else if (ispunct(ch)) hasSymbol = true;

                if(hasUpper && hasLower && hasDigit && hasSymbol){
                    break;
                }
            }
            return hasUpper && hasLower && hasDigit && hasSymbol;
        }

    // Function to check if user is locked
    bool isUserLocked(const string& username) {
        ifstream infile("locked_users.txt");
        if (!infile.is_open()) {
            // If the file doesn't exist, no users are locked
            return false;
        }
        string line;
        time_t currentTime = time(0);
        bool locked = false;
        vector<string> lines;
        while (getline(infile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string lockedUsername;
            time_t unlockTime;
            ss >> lockedUsername >> unlockTime;
            if (lockedUsername == username) {
                if (currentTime < unlockTime) {
                    locked = true;
                }
                else {
                    // Unlock the user by not adding back to lines
                    continue;
                }
            }
            lines.push_back(line);
        }
        infile.close();

        if (locked) {
            return true;
        }
        else {
            // Rewrite the file without the unlocked user
            ofstream outfile("locked_users.txt", ios::trunc);
            for (const auto& l : lines) {
                outfile << l << endl;
            }
            outfile.close();
            return false;
        }
    }



    // Function to lock a user
    void lockUser(const string& username) {
        time_t currentTime = time(0);
        time_t unlockTime = currentTime + 5 * 60; // Lock for 5 minutes
        ofstream outfile("locked_users.txt", ios::app);
        if (outfile.is_open()) {
            outfile << username << " " << unlockTime << endl;
            outfile.close();
        }
        else {
            cout << "Failed to lock the user." << endl;
        }
    }

    void registerUser(vector<User>& users, const string& filenameUsers) {
        User newUser;
        cout << "\n****************************************\n";
        cout << "\tUser Register\n";
        cout << "****************************************\n";
        while (true) {
            cout << "\nEnter username (or '0' to cancel): ";
            cin >> newUser.username;
            if (isBackCommand(newUser.username)) {
                cout << "Registration cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }

            // Check if username already exists
            bool exists = false;
            for (const auto& user : users) {
                if (user.username == newUser.username) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                cout << "Username already exists. Please choose a different username.\n";
            }
            else {
                break;
            }
        }

        while (true) {
            cout << "\nEnter password (or '0' to cancel): ";
            newUser.password = getPassword();
            if (isBackCommand(newUser.password)) {
                cout << "Registration cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }

            if (!isValidPassword(newUser.password)) {
                cout << "Password does not meet the requirements:\n";
                cout << "- At least 8 characters\n";
                cout << "- At least one uppercase letter\n";
                cout << "- At least one lowercase letter\n";
                cout << "- At least one number\n";
                cout << "- At least one symbol\n";
            }
            else {
                break;
            }
        }

        cout << "\nEnter your name (or '0' to cancel): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        getline(cin, newUser.name);
        if (isBackCommand(newUser.name)) {
            cout << "Registration cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        while (true) {
            cout << "\nEnter your phone number (digits only) (or '0' to cancel): ";
            cin >> newUser.phoneNumber;
            if (isBackCommand(newUser.phoneNumber)) {
                cout << "Registration cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }

            // Validate phone number contains only digits
            bool valid = true;
            for (char c : newUser.phoneNumber) {
                if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
            if (!valid) {
                cout << "Invalid phone number. Please enter digits only.\n";
            }
            else {
                break;
            }
        }

        // Save to file and vector
        users.push_back(newUser);
        ofstream file(filenameUsers.c_str(), ios::app);  // Use c_str() to convert string to const char*
        if (file.is_open()) {
            file << newUser.username << " " << newUser.password << " " << newUser.name << " " << newUser.phoneNumber << endl;
            file.close();
        }
        else {
            cout << "Failed to open the users file for writing." << endl;
            return;
        }

        cout << "\nRegistration successful! Returning to menu.\n";
        this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning to menu
        system("cls");
        loading();
        system("cls");
    }

    bool loginUser(const vector<User>& usersList, string& loggedInName, string& loggedInPhone, string& loggedInUsername) {
        if (usersList.empty()) {
            cout << "\nNo users registered. Please register first." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return false;
        }

        string usernameInput;
        cout << "\n****************************************\n";
        cout << "\tUser Login\n";
        cout << "****************************************\n";
        cout << "\nEnter username (or '0' to cancel): ";
        cin >> usernameInput;
        if (isBackCommand(usernameInput)) {
            cout << "Login cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return false;
        }

        int attempts = 0;
        while (attempts < 3) {
            string passwordInput;
            cout << "\nEnter password (or '0' to cancel): ";
            passwordInput = getPassword();
            if (isBackCommand(passwordInput)) {
                cout << "Login cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return false;
            }

            bool found = false;
            for (const auto& user : usersList) {
                if (user.username == usernameInput && user.password == passwordInput) {
                    cout << "\nLogin successful! Returning to homepage.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning
                    system("cls");
                    loading();
                    system("cls");
                    loggedInName = user.name;
                    loggedInPhone = user.phoneNumber;
                    loggedInUsername = user.username;
                    return true;
                }
            }

            cout << "\nInvalid username or password. Please try again.\n";
            attempts++;
            if (attempts == 3) {
                cout << "\nToo many failed attempts. This account is now locked for 5 minutes.\n";
                // lockUser(usernameInput);  // Assuming you have this function
                cout << "Press '0' to return to the menu: ";
                string backInput;
                cin >> backInput;
                if (isBackCommand(backInput)) {
                    cout << "Returning to menu.\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    loading();
                    system("cls");
                }
                return false; // Account is locked
            }
        }

        return false;
    }
};

// Derived Class: AdminClass
class AdminClass : public Person {
public:
    AdminClass() : Person() {}
    AdminClass(string uname, string pwd) : Person(uname, pwd) {}

    bool isValidPassword(const string& password) {
            if (password.length() < 8)
                return false;

            bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
            for (char ch : password) {
                if (isupper(ch)) hasUpper = true;
                else if (islower(ch)) hasLower = true;
                else if (isdigit(ch)) hasDigit = true;
                else if (ispunct(ch)) hasSymbol = true;

                if(hasUpper && hasLower && hasDigit && hasSymbol){
                    break;
                }
            }
            return hasUpper && hasLower && hasDigit && hasSymbol;
        }


    // Function to check if admin is locked
    bool isAdminLocked(const string& username) {
        ifstream infile("locked_admins.txt");
        if (!infile.is_open()) {
            // If the file doesn't exist, no admins are locked
            return false;
        }
        string line;
        time_t currentTime = time(0);
        bool locked = false;
        vector<string> lines;
        while (getline(infile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string lockedUsername;
            time_t unlockTime;
            ss >> lockedUsername >> unlockTime;
            if (lockedUsername == username) {
                if (currentTime < unlockTime) {
                    locked = true;
                }
                else {
                    // Unlock the admin by not adding back to lines
                    continue;
                }
            }
            lines.push_back(line);
        }
        infile.close();

        if (locked) {
            return true;
        }
        else {
            // Rewrite the file without the unlocked admin
            ofstream outfile("locked_admins.txt", ios::trunc);
            for (const auto& l : lines) {
                outfile << l << endl;
            }
            outfile.close();
            return false;
        }
    }

    // Function to lock an admin
    void lockAdmin(const string& username) {
        time_t currentTime = time(0);
        time_t unlockTime = currentTime + 5 * 60; // Lock for 5 minutes
        ofstream outfile("locked_admins.txt", ios::app);
        if (outfile.is_open()) {
            outfile << username << " " << unlockTime << endl;
            outfile.close();
        }
        else {
            cout << "Failed to lock the admin";
        }
    }

    bool loginAdmin(const vector<Admin>& adminsList, string& loggedInAdminUsername) {
        if (adminsList.empty())
        {
            cout << "\nNo admins registered." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return false;
        }

        string usernameInput;
        cout << "\n****************************************\n";
        cout << "\tAdmin Login\n";
        cout << "****************************************\n";
        cout << "\nEnter admin username (or '0' to cancel): ";
        cin >> usernameInput;
        if (isBackCommand(usernameInput)) {
            cout << "Login cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return false;
        }

        // Check if admin is locked
        if (isAdminLocked(usernameInput)) {
            cout << "\nThis admin account is locked due to multiple failed login attempts. Please try again later.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                cout << "Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
            }
            return false;
        }

        int attempts = 0;
        while (attempts < 3) {
            string passwordInput;
            cout << "\nEnter admin password (or '0' to cancel): ";
            passwordInput = getPassword();
            if (isBackCommand(passwordInput)) {
                cout << "Login cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return false;
            }

            bool found = false;
            for (const auto& admin : adminsList)
            {
                if (admin.username == usernameInput && admin.password == passwordInput)
    {
        cout << "\nAdmin login successful! Returning to admin panel.\n";
        this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning
        system("cls");
        loading();
        system("cls");
        loggedInAdminUsername = admin.username; // Store the username
        return true;
    }
            }

            cout << "\nInvalid admin username or password. Please try again.\n";
            attempts++;
            if (attempts == 3) {
                cout << "\nToo many failed attempts. This admin account is now locked for 5 minutes.\n";
                lockAdmin(usernameInput);
                cout << "Press '0' to return to the menu: ";
                string backInput;
                cin >> backInput;
                if (isBackCommand(backInput)) {
                    cout << "Returning to menu.\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    loading();
                    system("cls");
                }
                return false; // Admin account is locked
            }
        }

        return false;
    }

    void registerAdmin(vector<Admin>& adminsList, const string& filenameAdmins) {
        Admin newAdmin;
        cout << "\n****************************************\n";
        cout << "\tAdmin Register\n";
        cout << "****************************************\n";
        while (true) {
            cout << "\nEnter new admin username (or '0' to cancel): ";
            cin >> newAdmin.username;
            if (isBackCommand(newAdmin.username)) {
                cout << "Registration cancelled. Returning to admin panel.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            // Check if username already exists
            bool exists = false;
            for (const auto& admin : adminsList) {
                if (admin.username == newAdmin.username) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                cout << "Username already exists. Please choose a different username.\n";
            }
            else {
                break;
            }
        }

        while (true) {
            cout << "\nEnter new admin password (or '0' to cancel): ";
            newAdmin.password = getPassword();
            if (isBackCommand(newAdmin.password)) {
                cout << "Registration cancelled. Returning to admin panel.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            if (!isValidPassword(newAdmin.password)) {
                cout << "Password does not meet the requirements:\n";
                cout << "- At least 8 characters\n";
                cout << "- At least one uppercase letter\n";
                cout << "- At least one lowercase letter\n";
                cout << "- At least one number\n";
                cout << "- At least one symbol\n";
            }
            else {
                break;
            }
        }

        // Save to file and vector
        adminsList.push_back(newAdmin);
        ofstream file(filenameAdmins.c_str(), ios::app);
        if (file.is_open())
        {
            file << newAdmin.username << " " << newAdmin.password << endl;
            file.close();
        }
        else
        {
            cout << "Failed to open the admins file for writing." << endl;
            return;
        }

        cout << "\nAdmin registration successful! Returning to admin panel.\n";
        this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning
        system("cls");
        loading();
        system("cls");
    }
};

// Derived Class: ManagerClass (Not used in current implementation)
class ManagerClass : public AdminClass {
public:
    ManagerClass() : AdminClass() {}
    ManagerClass(string uname, string pwd) : AdminClass(uname, pwd) {}

    void displayInfo() const override { // Corrected to override base class function
        cout << "Manager Username: " << username << endl;
    }
};

// Court Class with Friend Functions
class Court
{
public:
    int courtID;
    string courtNumber;
    double pricePerHour;
    string bookingStatus;
    string courtType;
    string availableSlots;
    string courtLocation;

    Court() : courtID(0), /*type(""),*/ pricePerHour(0.0), courtNumber(""), bookingStatus("Available"), courtType(""), availableSlots(""), courtLocation("") {}

    Court(int id, /*string tp,*/ double price, const string& cNumber, const string& status, const string& type, const string& slots, const string& location)
        : courtID(id), pricePerHour(price), courtNumber(cNumber), bookingStatus(status), courtType(type), availableSlots(slots), courtLocation(location) {}

    // Accessor methods
    int getCourtID() const { return courtID; }
    //string getType() const { return type; }
    double getPricePerHour() const { return pricePerHour; }
    string getCourtNumber() const { return courtNumber; }
    string getBookingStatus() const { return bookingStatus; }
    string getCourtType() const { return courtType; }
    string getAvailableSlots() const { return availableSlots; }
    string getCourtLocation() const { return courtLocation; }

    // Friend functions
    friend void displayCourt(const Court& court);
    friend void addCourt(vector<Court>& courts, const string& filename);
    friend void addPickleballCourt(vector<Court>& courts, const string& filename);
    friend void editCourt(vector<Court>& courts, const string& filename);
    friend void deleteCourtFriend(vector<Court>& courts, int courtID, const string& filename);
};

// Comparator Functions
bool compareCourtsByID(const Court& a, const Court& b)
{
    return a.courtID < b.courtID;
}

bool compareCourtsByType(const Court& a, const Court& b)
{
    return a.courtType < b.courtType;
}

bool compareCourtsByPrice(const Court& a, const Court& b)
{
    return a.pricePerHour < b.pricePerHour;
}

// Friend Function to Display a Court's Details
void displayCourt(const Court& court) 
{
    cout << left << "| " << setw(10) << court.courtID
         << "| " << setw(10) << court.courtNumber 
         << "| " << setw(15) << court.courtType 
         << "| " << setw(15) << court.pricePerHour 
         << "| " << setw(15) << court.bookingStatus
         << "| " << setw(20) << court.availableSlots 
         << "| " << setw(15) << court.courtLocation 
         << " |" << endl;
}

// Friend Function to Add a Court
void addCourt(vector<Court>& courts, const string& filename, const string& courtType)
{
    int id;
    double price;
    string cNumber;
    string status;
    string type = courtType;
    string slots;
    string location;

    cout << "Enter details for the new " << courtType << " court:\n";

    while (true) {
        cout << "Court Name (or '0' to cancel): ";
        cin >> cNumber;

        if (isBackCommand(cNumber)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        bool unique = true;
        for (const auto& court : courts) {
            if (court.courtNumber == cNumber) {
                unique = false;
                break;
            }
        }

        if (!unique) {
            cout << "Court Name already exists. Please enter a different name.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Price per Hour (or '0' to cancel): RM ";
        string priceInput;
        cin >> priceInput;

        if (isBackCommand(priceInput)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        try {
            price = stod(priceInput);
            if (price < 0) {
                throw invalid_argument("Negative price");
            }
            break;
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid positive price.\n";
        }
    }

    while (true) {
        cout << "Status (Available/Booked/Maintenance) (or '0' to cancel): ";
        cin >> status;

        if (isBackCommand(status)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (status != "Available" && status != "Booked" && status != "Maintenance") {
            cout << "Invalid status. Please enter 'Available', 'Booked', or 'Maintenance'.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Available Slots (e.g., 08:00-22:00) (or '0' to cancel): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        getline(cin, slots);

        if (isBackCommand(slots)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (slots.empty()) {
            cout << "Invalid input. Please enter time range.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Court Location (Indoor/Outdoor) (or '0' to cancel): ";
        
        getline(cin, location);

        if (isBackCommand(location)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (location != "Indoor" && location != "Outdoor") {
            cout << "Invalid location. Please enter either 'Indoor' or 'Outdoor'.\n";
        } else {
            break;
        }
    }

    id = courts.size() + 1;
    Court newCourt(id, price, cNumber, status, type, slots, location);
    courts.push_back(newCourt);

    ofstream outfile(filename.c_str(), ios::app);
    if (outfile.is_open()) {
        outfile << newCourt.courtID << " " << newCourt.courtNumber << " " << fixed << setprecision(2) << newCourt.pricePerHour << " " << newCourt.bookingStatus << " " << newCourt.courtType << " " << newCourt.availableSlots << " " << newCourt.courtLocation << endl;
        outfile.close();
        cout << courtType << " Court added successfully. Returning to menu.\n";
    } else {
        cout << "Failed to open the courts file for writing." << endl;
        return;
    }

    this_thread::sleep_for(chrono::milliseconds(1000));
    system("cls");
    loading();
    system("cls");
}

void addPickleballCourt(vector<Court>& courts, const string& filename) {
    int id;
    double price;
    string cNumber;
    string status;
    string type;
    string slots;
    string location;

    cout << "Enter details for the new pickleball court:\n";

    while (true) {
        cout << "Court Number (or '0' to cancel): ";
        cin >> cNumber;

        if (isBackCommand(cNumber)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        bool unique = true;
        for (const auto& court : courts) {
            if (court.courtNumber == cNumber) {
                unique = false;
                break;
            }
        }

        if (!unique) {
            cout << "Court Number already exists. Please enter a different number.\n";
        } else {
            break;
        }
    }
    while(true){
        cout << "Court Type (Pickleball) (or '0' to cancel): ";
        cin >> type;

        if (isBackCommand(type)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }
        if (type != "Pickleball") {
            cout << "Invalid type. Please enter 'Pickleball'.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Price per Hour (or '0' to cancel): RM ";
        string priceInput;
        cin >> priceInput;

        if (isBackCommand(priceInput)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        try {
            price = stod(priceInput);
            if (price < 0) {
                throw invalid_argument("Negative price");
            }
            break;
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid positive price.\n";
        }
    }

    while (true) {
        cout << "Status (Available/Booked/Maintenance) (or '0' to cancel): ";
        cin >> status;

        if (isBackCommand(status)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (status != "Available" && status != "Booked" && status != "Maintenance") {
            cout << "Invalid status. Please enter 'Available', 'Booked', or 'Maintenance'.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Available Slots (e.g., 07:00-21:00) (or '0' to cancel): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, slots);

        if (isBackCommand(slots)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (slots.empty()) {
            cout << "Invalid input. Please enter time range.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Court Location (Indoor/Outdoor) (or '0' to cancel): ";
        getline(cin, location);

        if (isBackCommand(location)) {
            cout << "Add court cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            loading();
            system("cls");
            return;
        }

        if (location != "Indoor" && location != "Outdoor") {
            cout << "Invalid location. Please enter either 'Indoor' or 'Outdoor'.\n";
        } else {
            break;
        }
    }

    id = courts.size() + 1;
    Court newCourt(id, price, cNumber, status, type, slots, location);
    courts.push_back(newCourt);

    ofstream outfile(filename.c_str(), ios::app);
    if (outfile.is_open()) {
        outfile << newCourt.courtID << " " << newCourt.courtNumber << " " << fixed << setprecision(2) << newCourt.pricePerHour << " " << newCourt.bookingStatus << " " << newCourt.courtType << " " << newCourt.availableSlots << " " << newCourt.courtLocation << endl;
        outfile.close();
        cout << "Pickleball Court added successfully. Returning to menu.\n";
    } else {
        cout << "Failed to open the courts file for writing." << endl;
        return;
    }

    this_thread::sleep_for(chrono::milliseconds(1000));
    system("cls");
    loading();
    system("cls");
}

// Friend Function to Edit a Court
void editCourt(vector<Court>& courts, const string& filename)
{
    string courtNumber;
    cout << "Enter Court Name to edit (or '0' to cancel): ";
    
    cin >> courtNumber;
    if (isBackCommand(courtNumber)) {
        cout << "Edit court cancelled. Returning to menu.\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }
    

    auto it = find_if(courts.begin(), courts.end(), [&](const Court& c) {
        return c.courtNumber == courtNumber;
        });

    if (it != courts.end())
    {
        cout << "Editing Court Number: " << it->courtNumber << endl;
        cout << "Current Type: " << it->courtType << endl;
        cout << "Current Price per Hour: RM " << fixed << setprecision(2) << it->pricePerHour << endl;
        cout << "Current Status: " << it->bookingStatus << endl;
        cout << "Current Available Slots: " << it->availableSlots << endl;
        cout << "Current Location: " << it->courtLocation << endl;

        
        double newPrice;

        
        cout << "Enter new Type (Badminton/Pickleball) or press Enter to keep current: ";
        cin.ignore(); 
        string newType;
        getline(cin, newType);
        if (!newType.empty())
        {
            while (newType != "Badminton" && newType != "Pickleball")
            {
                cout << "Invalid type. Please enter either 'Badminton' or 'Pickleball': ";
                getline(cin, newType);
                if (isBackCommand(newType)) {
                    cout << "Edit court cancelled. Returning to menu.\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    loading();
                    system("cls");
                    return;
                }
            }
            it->courtType = newType;
        }

        cout << "Enter new Price per Hour or press Enter to keep current: RM ";
        
        string priceInput;
        getline(cin, priceInput);
        if (!priceInput.empty())
        {
            try {
                newPrice = stod(priceInput);
                if (newPrice < 0) {
                    throw invalid_argument("Negative price");
                }
                it->pricePerHour = newPrice;
            }
            catch (invalid_argument&) {
                cout << "Invalid price input. Keeping previous price." << endl;
            }
        }

        cout << "Enter new Status (Available/Booked/Maintenance) or press Enter to keep current: ";
        string newStatus;
        getline(cin, newStatus);
        if (!newStatus.empty()) {
            while (newStatus != "Available" && newStatus != "Booked" && newStatus != "Maintenance") {
                cout << "Invalid status. Please enter 'Available', 'Booked', or 'Maintenance': ";
                getline(cin, newStatus);
                if (isBackCommand(newStatus)) {
                    cout << "Edit court cancelled. Returning to menu.\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    loading();
                    system("cls");
                    return;
                }
            }
            it->bookingStatus = newStatus;
        }

        cout << "Enter new Available Slots (e.g., 8 AM - 10 PM) or press Enter to keep current: ";
        string newSlots;
        getline(cin, newSlots);
        if (!newSlots.empty()) {
            it->availableSlots = newSlots;
        }

        cout << "Enter new Location (Indoor/Outdoor) or press Enter to keep current: ";
        string newLocation;
        getline(cin, newLocation);
        if (!newLocation.empty()) {
            while (newLocation != "Indoor" && newLocation != "Outdoor") {
                cout << "Invalid location. Please enter either 'Indoor' or 'Outdoor': ";
                getline(cin, newLocation);
                if (isBackCommand(newLocation)) {
                    cout << "Edit court cancelled. Returning to menu.\n";
                    this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    loading();
                    system("cls");
                    return;
                }
            }
            it->courtLocation = newLocation;
        }

        // Update the file
        ofstream outfile(filename, ios::trunc);
        if (outfile.is_open())
        {
            for (const auto& court : courts)
            {
                outfile << court.courtID << " " << court.courtNumber << " " << court.pricePerHour << " " << court.bookingStatus << " " << court.courtType << " " << court.availableSlots << " " << court.courtLocation << endl;
            }
            outfile.close();
            cout << "Court updated successfully. Returning to menu.\n";
        }
        else
        {
            cout << "Failed to open the courts file for writing." << endl;
        }
    }
    else
    {
        cout << "Court Number not found. Returning to menu.\n";
    }

    this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning to menu
    system("cls");
    loading();
    system("cls");
}

// Friend Function to Delete a Court
void deleteCourtFriend(vector<Court>& courts, int courtNumber, const string& filename)
{
    auto it = find_if(courts.begin(), courts.end(), [&](const Court& c) {
        return c.courtID == courtNumber;
        });

    if (it != courts.end())
    {
        cout << "Are you sure you want to delete Court ID " << it->courtID << " (Court Number: " << it->courtNumber << ")? [Y/N]: ";
        char confirm;
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            // Remove the court from the vector
            courts.erase(it);

            // Update the courts.txt file
            ofstream outfile(filename, ios::trunc);
            if (outfile.is_open())
            {
                for (size_t i = 0; i < courts.size(); ++i)
                {
                    courts[i].courtID = i + 1; // Reassign court IDs
                    outfile << courts[i].courtID << " " << courts[i].courtNumber << " " << courts[i].pricePerHour << " " << courts[i].bookingStatus << " " << courts[i].courtType << " " << courts[i].availableSlots << " " << courts[i].courtLocation << endl;
                }
                outfile.close();
                cout << "Court deleted successfully. IDs have been reassigned. Returning to menu.\n";
            }
            else
            {
                cout << "Failed to open the courts file for writing." << endl;
            }
        }
        else {
            cout << "Deletion cancelled. Returning to menu.\n";
        }
    }
    else
    {
        cout << "Court ID not found. Returning to menu.\n";
    }

    this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning to menu
    system("cls");
    loading();
    system("cls");
}
// Function to Sort Courts Using a Custom Comparator
void sortCourts(std::vector<Court>& courts, bool (*compare)(const Court&, const Court&))
{
    std::sort(courts.begin(), courts.end(), compare);
}

// Receipt Class
class Receipt
{
public:
    // Function to generate receipts from the last booking entry
    // Modify the generateReceiptFromLastBooking function
    void generateReceiptFromLastBookingMultiple(const vector<Booking>& bookings) {
        // Retrieve court details for each booking
        ifstream courtsFile("courts.txt");
        if (!courtsFile.is_open()) {
            cout << "Failed to open the courts file." << endl;
            return;
        }

        vector<Court> allCourts;
        string courtLine;
        while (getline(courtsFile, courtLine)) {
            stringstream ss(courtLine);
            Court court;
            ss >> court.courtID >> court.courtNumber >> court.pricePerHour >> court.bookingStatus >> court.courtType >> court.availableSlots >> court.courtLocation;
            allCourts.push_back(court);
        }
        courtsFile.close();

        // Generate Grand Receipt
        ofstream grandReceiptFile("grand_receipt.txt", ios::app);
        if (grandReceiptFile.is_open()) {
            system("cls");
            loading();
            system("cls");
            grandReceiptFile << "\n>>>> GRAND RECEIPT <<<<" << endl;
            grandReceiptFile << "User Name: " << bookings[0].userName << endl; // Assuming all bookings have the same user
            grandReceiptFile << "User Contact: " << bookings[0].contact << endl; // Assuming all bookings have the same contact
            grandReceiptFile << "==============================================================================================================================================" << endl;
            grandReceiptFile << "\t\t\tCourt Details" << endl;
            grandReceiptFile << "==============================================================================================================================================" << endl;
            grandReceiptFile << left << setw(15) << "Court Number" << setw(15) << "Type" << setw(15) << "Price/Hour" << setw(15) << "Date" << setw(15) << "Time Slot" << setw(15) << "Hours Booked" << endl;
            grandReceiptFile << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

            double totalPrice = 0;
            for (const auto& booking : bookings) {
                auto courtIt = find_if(allCourts.begin(), allCourts.end(), [&](const Court& c) {
                    return c.courtID == booking.courtID;
                    });

                if (courtIt != allCourts.end()) {
                    grandReceiptFile << left << setw(15) << courtIt->courtNumber << setw(15) << courtIt->courtType
                        << setw(15) << fixed << setprecision(2) << courtIt->pricePerHour
                        << setw(15) << to_string(booking.day) + "/" + to_string(booking.month) + "/" + to_string(booking.year)
                        << setw(15) << booking.timeSlot << setw(15) << booking.hours << endl;

                    totalPrice += courtIt->pricePerHour * booking.hours;
                }
            }

            grandReceiptFile << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
            grandReceiptFile << "Total Price: RM " << fixed << setprecision(2) << totalPrice << endl;
            grandReceiptFile << endl;
            grandReceiptFile.close();
        }
        else {
            cout << "Failed to open the grand_receipt.txt file for saving." << endl;
        }

        // Generate Regular Receipt
        ofstream receiptFile("receipt.txt");
        if (receiptFile.is_open()) {
            receiptFile << "\n>>>> RECEIPT <<<<" << endl;
            receiptFile << "Customer Name: " << bookings[0].name << endl; // Assuming all bookings have the same name
            receiptFile << "Customer Contact: " << bookings[0].contact << endl; // Assuming all bookings have the same contact
            receiptFile << "===========================================================================" << endl;
            receiptFile << "\t\t\tCourt Details" << endl;
            receiptFile << "===========================================================================" << endl;
            receiptFile << left << setw(15) << "Court Number" << setw(15) << "Type" << setw(15) << "Price/Hour" << setw(15) << "Date" << setw(15) << "Time Slot" << setw(15) << "Hours Booked" << endl;
            receiptFile << "---------------------------------------------------------------------------" << endl;

            double totalPrice = 0;
            for (const auto& booking : bookings) {
                auto courtIt = find_if(allCourts.begin(), allCourts.end(), [&](const Court& c) {
                    return c.courtID == booking.courtID;
                    });

                if (courtIt != allCourts.end()) {
                    receiptFile << left << setw(15) << courtIt->courtNumber << setw(15) << courtIt->courtType
                        << setw(15) << fixed << setprecision(2) << courtIt->pricePerHour
                        << setw(15) << to_string(booking.day) + "/" + to_string(booking.month) + "/" + to_string(booking.year)
                        << setw(15) << booking.timeSlot << setw(15) << booking.hours << endl;

                    totalPrice += courtIt->pricePerHour * booking.hours;
                }
            }

            receiptFile << "---------------------------------------------------------------------------" << endl;
            receiptFile << "Total Price: RM " << fixed << setprecision(2) << totalPrice << endl;
            receiptFile << endl;
            receiptFile.close();
        }
        else {
            cout << "Failed to open the receipt.txt file for saving." << endl;
        }
    }

    void displayReceipt()
    {
        ifstream receipt("receipt.txt");
        if (!receipt)
        {
            cout << "Failed to print receipt." << endl;
            return;
        }

        string line;
        while (getline(receipt, line))
        {
            cout << line << endl;
        }

        receipt.close();
    }
};

// Function to Read Courts from a Text File
vector<Court> readCourtsFromFile(const string& filename)
{
    vector<Court> courts;
    ifstream infile(filename.c_str());
    if (!infile.is_open())
    {
        cout << "Error: Unable to open file " << filename << endl;
        return courts; // Return empty vector
    }
    string line;

    while (getline(infile, line))
    {
        stringstream ss(line);
        int id;
        //string type;
        double price;
        string cNumber;
        string status;
        string type;
        string slots;
        string location;

        if (ss >> id >> cNumber >> price >> status >> type >> slots >> location)
        {
            courts.emplace_back(id, price, cNumber, status, type, slots, location);
        }
        else
        {
            cout << "Error reading line: " << line << endl;
        }
    }

    if (infile.eof())
    {
        // Reached end of file
    }
    else if (infile.fail())
    {
        cout << "File reading error!" << endl;
    }

    infile.close();
    return courts;
}

// Function to Display the List of Courts
void displayCourts(const vector<Court>& courts)
{
    cout << "\n****************************************\n";
    cout << "\tCourt Lists\n";
cout << "****************************************\n";
    cout << "Available Courts:\n";

   // Display table header
cout << "------------------------------------------------------------------------------------------------------------------" << endl;
cout << left << "| " << setw(10) << "Court ID"
     << "| " << setw(10) << "Court No." 
     << "| " << setw(15) << "Type"
     << "| " << setw(15) << "Price/Hour" 
     << "| " << setw(15) << "Status"
     << "| " << setw(20) << "Available Slots"
     << "| " << setw(15) << "Location"
     << " |" << endl;
cout << "------------------------------------------------------------------------------------------------------------------" << endl;

    // Display each court's details in a row
    for (const auto& court : courts)
    {
        displayCourt(court);
    }
}

// Function to Validate Court ID Uniqueness
bool validateCourtID(const std::vector<Court>& courts, int id)
{
    for (const auto& court : courts)
    {
        if (court.courtID == id)
        {
            return false;
        }
    }
    return true;
}

// Function to Initialize Files if Not Exist
bool fileExists(const string& filename)
{
    ifstream infile(filename.c_str());
    return infile.good();
}

void initializeFiles()
{
    // Initialize admins.txt
    if (!fileExists("admins.txt"))
    {
        ofstream adminFile("admins.txt");
        // Add a default admin with strong password
        adminFile << "admin Admin@123" << endl; // Note: Passwords should be hashed
        adminFile.close();
    }

    // Initialize courts.txt
    if (!fileExists("courts.txt"))
    {
        ofstream courtsFile("courts.txt");
        // Populate courts.txt with 50 courts (25 Singles and 25 Doubles)
        for (int i = 1; i <= 25; ++i)
        {
            courtsFile << i << " " << "B" << i << " " << 50.00 << " Available Badminton 08:00-22:00 Indoor" << endl;
        }
        for (int i = 26; i <= 50; ++i)
        {
            courtsFile << i << " " << "P" << (i-25) << " " << 70.00 << " Available Pickleball 07:00-21:00 Outdoor" << endl;
        }
        courtsFile.close();
    }

    // Initialize users.txt
    if (!fileExists("users.txt"))
    {
        ofstream usersFile("users.txt");
        usersFile.close();
    }

    // Initialize booked_courts.txt
    if (!fileExists("booked_courts.txt"))
    {
        ofstream bookedFile("booked_courts.txt");
        bookedFile.close();
    }

    // Initialize grand_receipt.txt
    if (!fileExists("grand_receipt.txt"))
    {
        ofstream grandReceiptFile("grand_receipt.txt");
        grandReceiptFile.close();
    }

    // Initialize receipt.txt
    if (!fileExists("receipt.txt"))
    {
        ofstream receiptFile("receipt.txt");
        receiptFile.close();
    }

    // Initialize locked_users.txt
    if (!fileExists("locked_users.txt"))
    {
        ofstream lockedUsersFile("locked_users.txt");
        lockedUsersFile.close();
    }

    // Initialize locked_admins.txt
    if (!fileExists("locked_admins.txt"))
    {
        ofstream lockedAdminsFile("locked_admins.txt");
        lockedAdminsFile.close();
    }
}

// Function to Load Users from File
vector<User> LoadUsers(const string& filenameUsers)
{
    vector<User> usersList;
    ifstream file(filenameUsers.c_str());
    if (!file.is_open())
    {
        cout << "Error: Unable to open file " << filenameUsers << endl;
        return usersList;
    }
    User user;
    while (file >> user.username >> user.password)
    {
        file >> ws; // Consume any leading whitespace
        getline(file, user.name, ' ');
        getline(file, user.phoneNumber);
        usersList.push_back(user);
    }
    file.close();
    return usersList;
}


// Function to trim leading and trailing whitespaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to load the admins from file
vector<Admin> LoadAdmins(const string& filename) {
    vector<Admin> admins;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Failed to open admin file.\n";
        return admins;
    }

    string username, password;
    while (infile >> username >> password) {
        Admin admin;
        admin.username = trim(username);  // Trim whitespace
        admin.password = password;
        admins.push_back(admin);
    }

    infile.close();
    return admins;
}

// Function to Validate Booking Date
bool isDateValid(int day, int month, int year)
{
    // Get the current date
    time_t currentTime = time(0);
    struct tm* localTime = localtime(&currentTime);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    // Check if the entered date is in the future
    if (year < currentYear || (year == currentYear && month < currentMonth) ||
        (year == currentYear && month == currentMonth && day <= currentDay))
    {
        return false;
    }

    // Check if the entered date is valid
    bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int daysInMonth[] = { 31, 28 + leapYear, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month < 1 || month > 12 || day < 1 || day > daysInMonth[month - 1])
    {
        return false;
    }

    return true;
}

bool isCourtAvailable(int courtID, int day, int month, int year, string timeSlot) {
    ifstream infile("booked_courts.txt");
    if (!infile.is_open()) {
        cerr << "Error: Unable to open booked_courts.txt." << endl;
        return false; // Assume not available if file can't be opened
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        int bookingID, bookedCourtID, bookedDay, bookedMonth, bookedYear, hours;
        string bookedCourtNumber, bookedTimeSlot, name, contact, userName, courtType;
        ss >> bookingID >> bookedCourtID >> bookedCourtNumber >> bookedDay >> bookedMonth >> bookedYear >> bookedTimeSlot >> hours >> name >> contact >> userName >> courtType;

        if (bookedCourtID == courtID &&
            bookedDay == day &&
            bookedMonth == month &&
            bookedYear == year &&
            bookedTimeSlot == timeSlot) {
            infile.close();
            return false; // Court is booked at this time
        }
    }

    infile.close();

    // Check for "Maintenance" status in courts.txt
    ifstream courtsFile("courts.txt");
    if (!courtsFile.is_open()) {
        cerr << "Error: Unable to open courts.txt." << endl;
        return false;
    }

    while (getline(courtsFile, line)) {
        stringstream ss(line);
        int id;
        double price;
        string cNumber, status, type, slots, location;
        ss >> id >> cNumber >> price >> status >> type >> slots >> location;
        if (id == courtID && status == "Maintenance") {
            courtsFile.close();
            return false; // Court is under maintenance
        }
    }

    courtsFile.close();
    return true; // Court is available
}

bool isTimeSlotWithinRange(const string& availableRange, const string& requestedTimeSlot) {
    if (availableRange.empty() || requestedTimeSlot.empty()) {
        return false;
    }

    // Extract start and end times from available range
    int availableStartHour = stoi(availableRange.substr(0, 2));
    int availableEndHour = stoi(availableRange.substr(6, 2));

    // Extract start and end times from requested time slot
    int requestedStartHour = stoi(requestedTimeSlot.substr(0, 2));
    int requestedEndHour = stoi(requestedTimeSlot.substr(6, 2));

    // Check if requested time slot is within the available range
    return (requestedStartHour >= availableStartHour && requestedEndHour <= availableEndHour);
}

void customerInput(vector<int> courtIDs, const string& name, const string& contact, const string& username)
{
    int day, month, year, hours;
    string timeSlot;

    // Create a vector to store booking details for each court
    vector<Booking> bookings;

    // å»ºç«‹ Receipt ç‰©ä»¶
    Receipt rp; // Receipt class instance

    system("cls");
    loading();
    system("cls");

    cout << "==================================" << endl;
    cout << "User Details and Booking" << endl;
    cout << "==================================" << endl;
    // å‚³é€²ä¾†çš„ name èˆ‡ contact å·²ç¶“äº‹å…ˆè¼¸å…¥ï¼Œç„¡éœ€å†æ¬¡è¼¸å…¥

    // Loop through each selected court to get booking details
    for (int courtID : courtIDs) {
        bool validDate = false;
        cout << "\nBooking details for Court ID " << courtID << endl;
        cout << "==================================" << endl;

        // è¼¸å…¥æ—¥æœŸï¼ˆå¤©/æœˆ/å¹´ï¼‰
        do
        {
            cout << "Enter the day (or '0' to cancel): ";
            string dayInput;
            cin >> dayInput;
            if (isBackCommand(dayInput)) {
                cout << "Booking cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            try {
                day = stoi(dayInput);
            }
            catch (invalid_argument&) {
                cout << "Invalid input. Please enter a valid integer for day.\n";
                continue;
            }

            cout << "Enter the month (or '0' to cancel): ";
            string monthInput;
            cin >> monthInput;
            if (isBackCommand(monthInput)) {
                cout << "Booking cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            try {
                month = stoi(monthInput);
            }
            catch (invalid_argument&) {
                cout << "Invalid input. Please enter a valid integer for month.\n";
                continue;
            }

            cout << "Enter the year (or '0' to cancel): ";
            string yearInput;
            cin >> yearInput;
            if (isBackCommand(yearInput)) {
                cout << "Booking cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            try {
                year = stoi(yearInput);
            }
            catch (invalid_argument&) {
                cout << "Invalid input. Please enter a valid integer for year.\n";
                continue;
            }

            // é©—è­‰æ˜¯å¦ç‚ºæœªä¾†çš„æœ‰æ•ˆæ—¥æœŸ
            if (!isDateValid(day, month, year)) {
                cout << "Invalid date! Please enter a future date.\n";
            }
            else {
                validDate = true;
            }
        } while (!validDate);

        // Get the court details from courts.txt
        ifstream courtsFile("courts.txt");
        string courtLine;
        string availableSlots;
        while (getline(courtsFile, courtLine)) {
            stringstream ss(courtLine);
            Court court;
            ss >> court.courtID >> court.courtNumber >> court.pricePerHour >> court.bookingStatus >> court.courtType >> court.availableSlots >> court.courtLocation;
            if (court.courtID == courtID) {
                availableSlots = court.availableSlots;
                break;
            }
        }
        courtsFile.close();

        // è¼¸å…¥é è¨‚çš„æ™‚æ®µ
        while (true) {
            cout << "Enter Time Slot to book (e.g., 10:00-11:00) (or '0' to cancel): ";
            cin >> timeSlot;
            if (isBackCommand(timeSlot)) {
                cout << "Booking cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }

            if (isTimeSlotWithinRange(availableSlots, timeSlot)) {
                break;
            } else {
                cout << "Time slot is not within the available range. Please try again.\n";
            }
        }


        // è¼¸å…¥é è¨‚å°æ™‚
        while (true) {
            cout << "Enter number of hours to book (or '0' to cancel): ";
            string hoursInput;
            cin >> hoursInput;
            if (isBackCommand(hoursInput)) {
                cout << "Booking cancelled. Returning to menu.\n";
                this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                loading();
                system("cls");
                return;
            }
            try {
                hours = stoi(hoursInput);
                if (hours <= 0) {
                    throw invalid_argument("Non-positive hours");
                }
                // Calculate start and end times from the time slot
                int startTime = stoi(timeSlot.substr(0, 2));
                int endTime = stoi(timeSlot.substr(6, 2));

                if (hours > (endTime - startTime))
                 {
                    cout << "The hours entered do not match the time slot duration. Please try again.\n";
                 } else {
                    break; 
                 }
                
            }
            catch (invalid_argument&) {
                cout << "Invalid input. Please enter a positive integer for hours.\n";
            }
        }

        // æª¢æŸ¥æ‰€é¸çš„çƒå ´åœ¨è¼¸å…¥çš„æ—¥æœŸèˆ‡æ™‚æ®µæ˜¯å¦å¯ç”¨
        if (!isCourtAvailable(courtID, day, month, year, timeSlot)) {
            cout << "\nSorry, Court ID " << courtID << " is not available for this section.\n";
            cout << "Booking cancelled. Returning to menu.\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
            system("cls");
            loading();
            system("cls");
            return;
        }

        //Prompt for special request
        cout << "Enter any special requests (or press Enter to skip): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string specialRequest;
        getline(cin, specialRequest);

        // å‚³é€²ä¾†çš„ name èˆ‡ contact å·²ç¶“äº‹å…ˆè¼¸å…¥ï¼Œç„¡éœ€å†æ¬¡è¼¸å…¥

        // Store the booking details
        Booking booking;
        booking.courtID = courtID;
        booking.day = day;
        booking.month = month;
        booking.year = year;
        booking.timeSlot = timeSlot;
        booking.hours = hours;
        booking.name = name;
        booking.contact = contact;
        booking.userName = username;
        booking.specialRequest = specialRequest;
        bookings.push_back(booking);

        // é¡¯ç¤ºè¼¸å…¥çš„è¨‚ä½è³‡è¨Š
        cout << "Date entered: " << day << "/" << month << "/" << year << endl;
        cout << "Time Slot: " << timeSlot << endl;
        cout << "Hours Booked: " << hours << endl;
        cout << "Special Request: " << (specialRequest.empty() ? "None" : specialRequest) << endl;

    }

    // è®€å–æ‰€æœ‰çƒå ´è³‡æ–™
    vector<Court> allCourts = readCourtsFromFile("courts.txt");

    // å–å¾—ç›®å‰å·²å­˜åœ¨çš„æœ€å¤§ booking ID
    int bookingId = 1;
    ifstream bookingFile("booked_courts.txt");
    string line;
    while (getline(bookingFile, line)) {
        if (!line.empty()) {
            stringstream ss(line);
            int currentBookingId;
            ss >> currentBookingId;
            if (currentBookingId >= bookingId) {
                bookingId = currentBookingId + 1;
            }
        }
    }
    bookingFile.close();

    // å°‡è¨‚ä½è³‡è¨Šå¯«å…¥ "booked_courts.txt"
    ofstream bookedFileOut("booked_courts.txt", ios::app);
    if (bookedFileOut.is_open()) {
        for (Booking& booking : bookings) {
            // æ‰¾åˆ°å°æ‡‰çš„çƒå ´è³‡è¨Š
            auto it = find_if(allCourts.begin(), allCourts.end(), [&](const Court& c) {
                return c.courtID == booking.courtID;
                });
            if (it != allCourts.end()) {
                bookedFileOut << bookingId << " " << booking.courtID << " " << it->courtNumber << " " << booking.day << " " << booking.month << " " << booking.year << " "
                    << booking.timeSlot << " " << booking.hours << " " << booking.name << " " << booking.contact << " " << booking.userName << " " << it->courtType << " " << booking.specialRequest << endl;
                booking.bookingID = bookingId; // Update the booking ID in the vector
                booking.courtNumber = it->courtNumber;
                booking.courtType = it->courtType;
                booking.pricePerHour = it->pricePerHour;

                bookingId++;
            }
        }
        bookedFileOut.close();
    }
    else {
        cout << "Failed to open the \"booked_courts.txt\" file." << endl;
        system("cls");
        loading();
        system("cls");
        return;
    }

    rp.generateReceiptFromLastBookingMultiple(bookings); // Use the updated function here
    rp.displayReceipt();

    cout << "Receipt generated successfully. Returning to menu.\n";
    cout << "Press '0' to return to the menu: ";
    string backInput;
    cin >> backInput;
    if (isBackCommand(backInput)) {
        system("cls");
        loading();
        system("cls");
    }
}

void viewBookingHistory(const string& username, bool isAdmin)
{
    ifstream bookedFile("booked_courts.txt");
    if (!bookedFile.is_open())
    {
        cout << "Failed to open the \"booked_courts.txt\" file." << endl;
        cout << "Press '0' to return to the menu: ";
        string backInput;
        cin >> backInput;
        if (isBackCommand(backInput)) {
            system("cls");
            loading();
            system("cls");
        }
        return;
    }

    cout << "\n==================================\n";
    cout << "\tBooking History\n";
    cout << "==================================\n";

    // Define column widths (adjust these based on your data)
    const int COL_BOOKING_ID = 10;
    const int COL_COURT_ID = 10;
    const int COL_COURT_NUM = 13;
    const int COL_DAY = 6;
    const int COL_MONTH = 7;
    const int COL_YEAR = 6;
    const int COL_TIME_SLOT = 15;
    const int COL_HOURS = 6;
    const int COL_NAME = 10;
    const int COL_CONTACT = 15;
    const int COL_USER = 15;
    const int COL_COURT_TYPE = 15; // Separate column for court type
    const int COL_SPECIAL_REQUEST = 20;

    // Display table header
    cout << left << setw(COL_BOOKING_ID) << "Booking ID"
         << setw(COL_COURT_ID) << "Court No."
         << setw(COL_COURT_NUM) << "Court Name"
         << setw(COL_DAY) << "Day"
         << setw(COL_MONTH) << "Month"
         << setw(COL_YEAR) << "Year"
         << setw(COL_TIME_SLOT) << "Time Slot"
         << setw(COL_HOURS) << "Hours"
         << setw(COL_NAME) << "Name"
         << setw(COL_CONTACT) << "Contact"
         << setw(COL_USER) << "Booked by"
         << setw(COL_COURT_TYPE) << "Court Type" // Display court type separately
         << setw(COL_SPECIAL_REQUEST) << "Special Request" << endl;

    // Calculate the total width of the table for the separator line
    int totalWidth = COL_BOOKING_ID + COL_COURT_ID + COL_COURT_NUM + COL_DAY + COL_MONTH + COL_YEAR + COL_TIME_SLOT + COL_HOURS + COL_NAME + COL_CONTACT + COL_USER + COL_COURT_TYPE + COL_SPECIAL_REQUEST;
    cout << setfill('-') << setw(totalWidth) << "" << setfill(' ') << endl;

    // Read and display each booking
    string line;
    while (getline(bookedFile, line)) {
        istringstream iss(line);
        int bookingId;
        string courtID, courtNumber, courtType, day, month, year, timeSlot, hours, name, contact, user, specialRequest;

        if (!(iss >> bookingId >> courtID >> courtNumber >> day >> month >> year >> timeSlot >> hours >> name >> contact >> user >> courtType)) {
            cerr << "Error: Invalid line format: " << line << endl;
            continue;
        }

        // Extract special request (rest of the line after court type)
        getline(iss >> ws, specialRequest);

        if (!isAdmin && user != username)
            continue;

        // Display data with consistent column widths
        cout << left << setw(COL_BOOKING_ID) << bookingId
             << setw(COL_COURT_ID) << courtID
             << setw(COL_COURT_NUM) << courtNumber
             << setw(COL_DAY) << day
             << setw(COL_MONTH) << month
             << setw(COL_YEAR) << year
             << setw(COL_TIME_SLOT) << timeSlot
             << setw(COL_HOURS) << hours
             << setw(COL_NAME) << name
             << setw(COL_CONTACT) << contact
             << setw(COL_USER) << user
             << setw(COL_COURT_TYPE) << courtType // Directly display court type from file
             << setw(COL_SPECIAL_REQUEST) << specialRequest << endl;
    }

    bookedFile.close();
    cout << "\nPress '0' to return to the menu: ";
    string backInput;
    cin >> backInput;
    if (isBackCommand(backInput)) {
        system("cls");
        loading();
        system("cls");
    }
}


// Node structure for the linked queue
struct BookingNode {
    Booking booking;
    BookingNode* next;

    BookingNode(const Booking& bk) : booking(bk), next(nullptr) {}
};

// Linked Queue to store bookings
class BookingQueue {
public:
    BookingNode* front;
    BookingNode* rear;

    BookingQueue() : front(nullptr), rear(nullptr) {}

    // Enqueue a booking
    void enqueue(const Booking& booking) {
        BookingNode* newNode = new BookingNode(booking);
        if (!rear) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Dequeue a booking
    bool dequeue() {
        if (!front) return false;
        BookingNode* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return true;
    }

    // Clear the queue
    void clear() {
        while (dequeue());
    }

    // Destructor to clean up memory
    ~BookingQueue() {
        clear();
    }
};

// Edit booking function
void editBooking(const string& username, const string& name, const string& phoneNumber) {
    ifstream infile("booked_courts.txt");
    if (!infile.is_open()) {
        cout << "Error: Unable to open booked_courts.txt." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    BookingQueue queue;
    string line;

    // Load bookings into the queue
    while (getline(infile, line)) {
        Booking booking;
        stringstream ss(line);
        ss >> booking.bookingID >> booking.courtID >> booking.courtNumber >> booking.day >> booking.month >> booking.year >> booking.timeSlot >> booking.hours;
        ss >> ws;
        getline(ss, booking.name, ' ');
        getline(ss, booking.contact, ' ');
        getline(ss, booking.userName, ' ');
        getline(ss, booking.courtType, ' ');

        queue.enqueue(booking);
    }
    infile.close();

    // Display user's bookings
    BookingNode* current = queue.front;
    vector<BookingNode*> userBookings;
    int index = 1;

    cout << "\nYour Bookings:\n";
    while (current) {
        if (current->booking.userName == username) {
            userBookings.push_back(current);
            cout << index++ << ". Booking ID: " << current->booking.bookingID
                << ", Court Number: " << current->booking.courtNumber
                << ", Date: " << current->booking.day << "/" << current->booking.month << "/" << current->booking.year
                << ", Time Slot: " << current->booking.timeSlot
                << ", Hours: " << current->booking.hours << endl;
        }
        current = current->next;
    }

    if (userBookings.empty()) {
        cout << "No bookings found for the user." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    // Select booking to edit
    cout << "Enter the booking number to edit (or 0 to cancel): ";
    int choice;
    if (!readInteger(choice) || choice < 0 || choice > userBookings.size()) {
        cout << "Invalid input. Returning to menu." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    // Add cin.ignore() here to clear the newline character
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) {
        cout << "Edit cancelled. Returning to menu." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    BookingNode* selectedNode = userBookings[choice - 1];
    Booking& selectedBooking = selectedNode->booking;

    // Check if the booking is in the future
    time_t now = time(0);
    tm* localTime = localtime(&now);
    if (selectedBooking.year < (localTime->tm_year + 1900) ||
        (selectedBooking.year == (localTime->tm_year + 1900) && selectedBooking.month < (localTime->tm_mon + 1)) ||
        (selectedBooking.year == (localTime->tm_year + 1900) && selectedBooking.month == (localTime->tm_mon + 1) &&
            selectedBooking.day <= localTime->tm_mday)) {
        cout << "You cannot edit past or current bookings." << endl;
        return;
    }

    // Backup the original booking details
    int originalCourtID = selectedBooking.courtID;
    string originalCourtNumber = selectedBooking.courtNumber;
    string originalTimeSlot = selectedBooking.timeSlot;
    int originalDay = selectedBooking.day;
    int originalMonth = selectedBooking.month;
    int originalYear = selectedBooking.year;
    int originalHours = selectedBooking.hours;

    // Modify booking details
    cout << "Editing booking for Booking ID: " << selectedBooking.bookingID << endl;

    // Edit date
    cout << "Current Date: " << selectedBooking.day << "/" << selectedBooking.month << "/" << selectedBooking.year << endl;
    cout << "Enter new day (or press Enter to keep): ";
    string newDay;
    getline(cin, newDay);
    if (!newDay.empty()) {
        try {
            selectedBooking.day = stoi(newDay);
        }
        catch (invalid_argument&) {
            cout << "Invalid day input. Keeping the original value." << endl;
        }
    }

    cout << "Enter new month (or press Enter to keep): ";
    string newMonth;
    getline(cin, newMonth);
    if (!newMonth.empty()) {
        try {
            selectedBooking.month = stoi(newMonth);
        }
        catch (invalid_argument&) {
            cout << "Invalid month input. Keeping the original value." << endl;
        }
    }

    cout << "Enter new year (or press Enter to keep): ";
    string newYear;
    getline(cin, newYear);
    if (!newYear.empty()) {
        try {
            selectedBooking.year = stoi(newYear);
        }
        catch (invalid_argument&) {
            cout << "Invalid year input. Keeping the original value." << endl;
        }
    }

    // Validate the new date
    if (!isDateValid(selectedBooking.day, selectedBooking.month, selectedBooking.year)) {
        cout << "Invalid date entered. Keeping the original date." << endl;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
    }

    // Get the available slots for the selected court
    string availableSlots;
    double pricePerHour = 0.0;
    ifstream courtsFile("courts.txt");
    string courtLine;
    while (getline(courtsFile, courtLine)) {
        stringstream ss(courtLine);
        Court court;
        ss >> court.courtID >> court.courtNumber >> court.pricePerHour >> court.bookingStatus >> court.courtType >> availableSlots >> court.courtLocation;
        if (court.courtID == selectedBooking.courtID) {
            pricePerHour = court.pricePerHour;
            break;
        }
    }
    courtsFile.close();

    cout << "Current Time Slot: " << selectedBooking.timeSlot << ". Enter new Time Slot (or press Enter to keep): ";
    string newTimeSlot;
    getline(cin, newTimeSlot);
    if (!newTimeSlot.empty()) {
        selectedBooking.timeSlot = newTimeSlot;
    }

    // Edit court number (and ID)
    cout << "Current Court Number: " << selectedBooking.courtNumber << ". Enter new Court Number (or press Enter to keep): ";
    string newCourtNumber;
    getline(cin, newCourtNumber);
    if (!newCourtNumber.empty()) {
        // Find the court with the new court number
        vector<Court> allCourts = readCourtsFromFile("courts.txt");
        auto courtIt = find_if(allCourts.begin(), allCourts.end(), [&](const Court& c) {
            return c.courtNumber == newCourtNumber;
        });

        if (courtIt != allCourts.end()) {
            selectedBooking.courtNumber = newCourtNumber;
            selectedBooking.courtID = courtIt->courtID;
        }
        else {
            cout << "Court number not found. Keeping the original value." << endl;
        }
    }

    cout << "Current Hours: " << selectedBooking.hours << ". Enter new Hours (or press Enter to keep): ";
    string newHours;
    getline(cin, newHours);
    if (!newHours.empty()) {
        try {
            selectedBooking.hours = stoi(newHours);
        }
        catch (invalid_argument&) {
            cout << "Invalid hours input. Keeping the original value." << endl;
        }
    }

    // Validate hours based on time slot
    if (!newTimeSlot.empty() || !newHours.empty()) {
        int startTime = stoi(selectedBooking.timeSlot.substr(0, 2));
        int endTime = stoi(selectedBooking.timeSlot.substr(6, 2));
        if (selectedBooking.hours > (endTime - startTime))
        {
            cout << "The hours entered do not match the time slot duration. Please try again.\n" << endl;
            selectedBooking.hours = originalHours;
            this_thread::sleep_for(chrono::milliseconds(1000));
            system("cls");
            loading();
            system("cls");
            return;
        }
    }

    // Check if the new time slot is within the available range
    if (!isTimeSlotWithinRange(availableSlots, selectedBooking.timeSlot)) {
        cout << "The new time slot is not within the available range. Reverting to original booking details." << endl;
        selectedBooking.courtID = originalCourtID;
        selectedBooking.courtNumber = originalCourtNumber;
        selectedBooking.timeSlot = originalTimeSlot;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
        selectedBooking.hours = originalHours;

        return; // Exit the function
    }

    // Check if the new time slot is available
    if (!isCourtAvailable(selectedBooking.courtID, selectedBooking.day, selectedBooking.month, selectedBooking.year, selectedBooking.timeSlot)) {
        cout << "The new time slot or court is not available. Reverting to original booking details." << endl;
        selectedBooking.courtID = originalCourtID;
        selectedBooking.courtNumber = originalCourtNumber;
        selectedBooking.timeSlot = originalTimeSlot;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
        selectedBooking.hours = originalHours;
    }
    else {
        // Calculate additional charges if hours are increased
        double additionalCharges = 0.0;
        if (selectedBooking.hours > originalHours) {
            additionalCharges = (selectedBooking.hours - originalHours) * pricePerHour;
            cout << "Additional charges: RM " << fixed << setprecision(2) << additionalCharges << endl;
            cout << "Please pay the additional charges at the counter." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        // Update court availability status to "Available" for the original booking details
        vector<Court> allCourts = readCourtsFromFile("courts.txt");
        for (Court& court : allCourts) {
            if (court.courtID == originalCourtID) {
                // Remove the old booking from booked_courts.txt
                ifstream bookedInfile("booked_courts.txt");
                ofstream tempOutfile("temp_booked_courts.txt");

                if (!bookedInfile.is_open() || !tempOutfile.is_open()) {
                    cout << "Error: Unable to open booked_courts.txt or temp_booked_courts.txt." << endl;
                    return;
                }

                string line;
                while (getline(bookedInfile, line)) {
                    stringstream ss(line);
                    int bookingID, bookedCourtID, bookedDay, bookedMonth, bookedYear, hours;
                    string bookedCourtNumber, bookedTimeSlot, name, contact, userName, courtType;
                    ss >> bookingID >> bookedCourtID >> bookedCourtNumber >> bookedDay >> bookedMonth >> bookedYear >> bookedTimeSlot >> hours;
                    ss >> ws;
                    getline(ss, name, ' ');
                    getline(ss, contact, ' ');
                    getline(ss, userName, ' ');
                    getline(ss, courtType);

                    if (bookingID != selectedBooking.bookingID) {
                        tempOutfile << line << endl;
                    }
                }

                bookedInfile.close();
                tempOutfile.close();

                remove("booked_courts.txt");
                rename("temp_booked_courts.txt", "booked_courts.txt");

                // Update the booking in booked_courts.txt
                ofstream bookedOutfile("booked_courts.txt", ios::app);
                if (!bookedOutfile.is_open()) {
                    cout << "Error: Unable to open booked_courts.txt for writing." << endl;
                    return;
                }

                bookedOutfile << selectedBooking.bookingID << " " << selectedBooking.courtID << " " << selectedBooking.courtNumber << " "
                    << selectedBooking.day << " " << selectedBooking.month << " " << selectedBooking.year << " "
                    << selectedBooking.timeSlot << " " << selectedBooking.hours << " " << selectedBooking.name << " "
                    << selectedBooking.contact << " " << selectedBooking.userName << " " << selectedBooking.courtType << endl;

                bookedOutfile.close();
                break;
            }
        }
    }

    cout << "Booking updated successfully." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    system("cls");
    loading();
    system("cls");
}

// Function to delete a booking record
void deleteBookingRecord(const string& username) {
      ifstream infile("booked_courts.txt");
    if (!infile.is_open()) {
        cout << "Error: Unable to open booked_courts.txt." << endl;
        return;
    }

    BookingQueue queue;
    string line;

    // Load bookings into the queue
    while (getline(infile, line)) {
        Booking booking;
        stringstream ss(line);
        ss >> booking.bookingID >> booking.courtID >> booking.courtNumber >> booking.day >> booking.month >> booking.year >> booking.timeSlot >> booking.hours;
        ss >> ws; // Consume whitespace
        getline(ss, booking.name, ' ');
        getline(ss, booking.contact, ' ');
        getline(ss, booking.userName, ' ');
        getline(ss, booking.courtType); // Read courtType
        queue.enqueue(booking);
    }
    infile.close();

    // Display user's bookings
    BookingNode* current = queue.front;
    vector<BookingNode*> userBookings;
    int index = 1;

    cout << "\nYour Bookings:\n";
    while (current) {
        if (current->booking.userName == username) {
            userBookings.push_back(current);
            cout << index++ << ". Booking ID: " << current->booking.bookingID
                << ", Court Type: " << current->booking.courtType
                << ", Court Number: " << current->booking.courtNumber
                << ", Date: " << current->booking.day << "/" << current->booking.month << "/" << current->booking.year
                << ", Time Slot: " << current->booking.timeSlot
                << ", Hours: " << current->booking.hours << endl;
        }
        current = current->next;
    }

    if (userBookings.empty()) {
        cout << "No bookings found for the user." << endl;
        return;
    }

    // Select booking to delete
    cout << "Enter the booking ID to delete (or 0 to cancel): ";
    int choice;
    
    if (!(cin >> choice) || choice < 0) {
        cout << "Invalid input. Returning to menu." << endl;
        return;
    }
    cin.ignore(); // Consume the newline character

    if (choice == 0) {
        cout << "Deletion cancelled. Returning to menu." << endl;
        return;
    }

    // Find the selected booking by ID
    BookingNode* selectedNode = nullptr;
    for (BookingNode* node : userBookings) {
        if (node->booking.bookingID == choice) {
            selectedNode = node;
            break;
        }
    }

    if (!selectedNode) {
        cout << "Booking ID not found. Returning to menu." << endl;
        return;
    }

    // Get the selected booking
    Booking& selectedBooking = selectedNode->booking;

    // Update court availability status to "Available"
    vector<Court> allCourts = readCourtsFromFile("courts.txt");
    for (Court& court : allCourts) {
        if (court.courtID == selectedBooking.courtID) {
            court.bookingStatus = "Available";
            break;
        }
    }

    // Rewrite the courts.txt file with updated status
    ofstream courtOutfile("courts.txt", ios::trunc);
    if (!courtOutfile.is_open()) {
        cout << "Error: Unable to open courts.txt for writing." << endl;
        return;
    }
    for (const Court& court : allCourts) {
        courtOutfile << court.courtID << " " << court.courtNumber << " " << court.pricePerHour << " " << court.bookingStatus << " " << court.courtType << " " << court.availableSlots << " " << court.courtLocation << endl;
    }
    courtOutfile.close();

    // Delete the selected booking
    ofstream outfile("booked_courts.txt", ios::trunc);
    if (!outfile.is_open()) {
        cout << "Error: Unable to open booked_courts.txt for writing." << endl;
        return;
    }

    current = queue.front;
    while (current) {
        if (current->booking.bookingID != choice) {
            const Booking& booking = current->booking;
            outfile << booking.bookingID << " " << booking.courtID << " " << booking.courtNumber << " " << booking.day << " " << booking.month << " " << booking.year << " "
                << booking.timeSlot << " " << booking.hours << " " << booking.name << " " << booking.contact << " " << booking.userName << " "
                << booking.courtType << endl;
        }
        current = current->next;
    }

    outfile.close();
    cout << "Booking deleted successfully." << endl;
}

// Function to View Court Availability
// Function to View Court Availability
void viewCourtAvailability()
{
    int day, month, year;
    string timeSlot;

    cout << "\n==================================\n";
    cout << "\tView Court Availability\n";
    cout << "==================================\n";

    // Prompt for date
    cout << "Enter the booking date to check availability:" << endl;
    while (true)
    {
        cout << "Enter the day (or '0' to cancel): ";
        string dayInput;
        cin >> dayInput;
        if (isBackCommand(dayInput)) {
            cout << "Operation cancelled. Returning to menu.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            return;
        }
        try {
            day = stoi(dayInput);
        }
        catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid integer for day.\n";
            continue;
        }

        cout << "Enter the month (or '0' to cancel): ";
        string monthInput;
        cin >> monthInput;
        if (isBackCommand(monthInput)) {
            cout << "Operation cancelled. Returning to menu.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            return;
        }
        try {
            month = stoi(monthInput);
        }
        catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid integer for month.\n";
            continue;
        }

        cout << "Enter the year (or '0' to cancel): ";
        string yearInput;
        cin >> yearInput;
        if (isBackCommand(yearInput)) {
            cout << "Operation cancelled. Returning to menu.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            return;
        }
        try {
            year = stoi(yearInput);
        }
        catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid integer for year.\n";
            continue;
        }

        if (!isDateValid(day, month, year))
        {
            cout << "Invalid date! Please enter a future date." << endl;
        }
        else
        {
            break;
        }
    }

    // Prompt for time slot
    cout << "Enter the Time Slot to check (e.g., 10:00-11:00) (or '0' to cancel): ";
    cin >> timeSlot;
    if (isBackCommand(timeSlot)) {
        cout << "Operation cancelled. Returning to menu.\n";
        cout << "Press '0' to return to the menu: ";
        string backInput;
        cin >> backInput;
        if (isBackCommand(backInput)) {
            system("cls");
            loading();
            system("cls");
        }
        return;
    }

    // Fetch all courts
    vector<Court> allCourts = readCourtsFromFile("courts.txt");

    // Find available courts
    vector<Court> availableCourts;
    for (const auto& court : allCourts)
    {
        if (isCourtAvailable(court.courtID, day, month, year, timeSlot))
        {
            availableCourts.push_back(court);
        }
    }

    // Display available courts
    if (availableCourts.empty())
    {
        cout << "\nNo courts are available for the selected date and time slot.\n";
    }
    else
    {
        cout << "\nAvailable Courts on " << day << "/" << month << "/" << year << " during " << timeSlot << ":\n";
        // Display table header
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << left << "| " << setw(10) << "Court ID"
             << "| " << setw(10) << "Court No."
             << "| " << setw(15) << "Type"
             << "| " << setw(15) << "Price/Hour"
             << "| " << setw(15) << "Status"
             << "| " << setw(20) << "Available Slots"
             << "| " << setw(15) << "Location"
             << " |" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& court : availableCourts)
        {
            displayCourt(court); // displayCourt() function will print the details including Court Number
        }
    }

    cout << "\nPress '0' to return to the menu: ";
    string backInput;
    cin >> backInput;
    if (isBackCommand(backInput)) {
        system("cls");
        loading();
        system("cls");
    }
}

// Function to Search Courts by Type
void searchByType(vector<Court>& courts, const string& type)
{
    vector<Court> filteredCourts;
    for (const auto& court : courts)
    {
        if (court.courtType == type)
        {
            filteredCourts.push_back(court);
        }
    }

    if (filteredCourts.empty())
    {
        cout << "\nNo courts with type " << type << " found.\n";
    }
    else
    {
        cout << "\nCourts found with type " << type << ":\n";
        for (const auto& court : filteredCourts)
        {
            displayCourt(court);
        }
    }
}

// Function to Search Courts by Price
void searchByPrice(vector<Court>& courts, double price)
{
    vector<Court> filteredCourts;
    for (const auto& court : courts)
    {
        if (abs(court.pricePerHour - price) < 1e-2) // Comparing doubles with a tolerance
        {
            filteredCourts.push_back(court);
        }
    }

    if (filteredCourts.empty())
    {
cout << "\nNo courts with price per hour RM " << fixed << setprecision(2) << price << " found.\n";
    }
    else
    {
        cout << "\nCourts found with price per hour RM " << fixed << setprecision(2) << price << ":\n";
        for (const auto& court : filteredCourts)
        {
            displayCourt(court);
        }
    }
}

// Function to Sort Courts by ID
void sortCourtsByID(vector<Court>& courts)
{
    sortCourts(courts, compareCourtsByID);
}

// Function to Sort Courts by Type
void sortCourtsByType(vector<Court>& courts)
{
    sortCourts(courts, compareCourtsByType);
}

// Function to Sort Courts by Price
void sortCourtsByPrice(vector<Court>& courts)
{
    sortCourts(courts, compareCourtsByPrice);
}

// Heapsort Implementation for Sorting Courts by Price
void heapifyByPrice(vector<Court>& courts, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && courts[left].pricePerHour > courts[largest].pricePerHour)
        largest = left;

    if (right < n && courts[right].pricePerHour > courts[largest].pricePerHour)
        largest = right;

    if (largest != i) {
        swap(courts[i], courts[largest]);
        heapifyByPrice(courts, n, largest);
    }
}

void heapSortByPrice(vector<Court>& courts) {
    int n = courts.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyByPrice(courts, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(courts[0], courts[i]);
        heapifyByPrice(courts, i, 0);
    }

}

// Insertion Sort Implementation for Sorting Courts by Type
void insertionSortByType(vector<Court>& courts) {
    int n = courts.size();
    for (int i = 1; i < n; ++i) {
        Court key = courts[i];
        int j = i - 1;

        while (j >= 0 && courts[j].courtType > key.courtType) {
            courts[j + 1] = courts[j];
            j = j - 1;
        }
        courts[j + 1] = key;
    }

}

// Additional Function: Implementing a Hash Table for Quick Search (Using unordered_map)
// Function to create a hash map for courts by type
unordered_map<string, vector<Court>> createCourtTypeHashMap(const vector<Court>& courts) {
    unordered_map<string, vector<Court>> courtTypeMap;
    for (const auto& court : courts) {
        courtTypeMap[court.courtType].push_back(court);
    }
    return courtTypeMap;
}

// Sorting Function Definitions

// Function to Sort Courts Using QuickSort by ID
void sortCourtsByIDQuickSort(vector<Court>& courts)
{
    if (courts.empty())
    {
        cout << "No courts to sort." << endl;
        return;
    }

    // QuickSort Implementation
    // Partition function
    auto partitionFunc = [&](int low, int high) -> int {
        int pivot = courts[high].courtID; // Choosing the last element as pivot
        int i = low - 1; // Index of smaller element

        for (int j = low; j < high; j++)
        {
            // If current courtID is smaller than or equal to pivot
            if (courts[j].courtID <= pivot)
            {
                i++; // increment index of smaller element
                swap(courts[i], courts[j]);
            }
        }
        swap(courts[i + 1], courts[high]);
        return (i + 1);
    };

    // Recursive QuickSort function using lambda
    function<void(int, int)> quickSort = [&](int low, int high) {
        if (low < high)
        {
            // pi is partitioning index, courts[pi] is now at right place
            int pi = partitionFunc(low, high);

            // Separately sort elements before and after partition
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
    };

    quickSort(0, courts.size() - 1);
}

void adminEditBooking() {
    ifstream infile("booked_courts.txt");
    if (!infile.is_open()) {
        cout << "Error: Unable to open booked_courts.txt." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    BookingQueue queue;
    string line;

    // Load all bookings into the queue
    while (getline(infile, line)) {
        Booking booking;
        stringstream ss(line);
        ss >> booking.bookingID >> booking.courtID >> booking.courtNumber >> booking.day >> booking.month >> booking.year >> booking.timeSlot >> booking.hours;
        ss >> ws;
        getline(ss, booking.name, ' ');
        getline(ss, booking.contact, ' ');
        getline(ss, booking.userName, ' ');
        getline(ss, booking.courtType, ' ');

        queue.enqueue(booking);
    }
    infile.close();

    // Display all bookings
    BookingNode* current = queue.front;
    vector<BookingNode*> allBookings;
    int index = 1;

    cout << "\nAll Bookings:\n";
    while (current) {
        allBookings.push_back(current);
        cout << index++ << ". Booking ID: " << current->booking.bookingID
             << ", User: " << current->booking.userName
             << ", Court Number: " << current->booking.courtNumber
             << ", Date: " << current->booking.day << "/" << current->booking.month << "/" << current->booking.year
             << ", Time Slot: " << current->booking.timeSlot
             << ", Hours: " << current->booking.hours << endl;
        current = current->next;
    }

    if (allBookings.empty()) {
        cout << "No bookings found." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    // Select booking to edit
    cout << "Enter the booking number to edit (or 0 to cancel): ";
    int choice;
    if (!readInteger(choice) || choice < 0 || choice > allBookings.size()) {
        cout << "Invalid input. Returning to menu." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) {
        cout << "Edit cancelled. Returning to menu." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }

    BookingNode* selectedNode = allBookings[choice - 1];
    Booking& selectedBooking = selectedNode->booking;

    // Check if the booking is in the future (This check might be less relevant for admins, but keeping it for consistency)
    time_t now = time(0);
    tm* localTime = localtime(&now);
    if (selectedBooking.year < (localTime->tm_year + 1900) ||
        (selectedBooking.year == (localTime->tm_year + 1900) && selectedBooking.month < (localTime->tm_mon + 1)) ||
        (selectedBooking.year == (localTime->tm_year + 1900) && selectedBooking.month == (localTime->tm_mon + 1) &&
         selectedBooking.day <= localTime->tm_mday)) {
        cout << "You cannot edit past or current bookings." << endl;
        return;
    }

    // Backup the original booking details
    int originalCourtID = selectedBooking.courtID;
    string originalCourtNumber = selectedBooking.courtNumber;
    string originalTimeSlot = selectedBooking.timeSlot;
    int originalDay = selectedBooking.day;
    int originalMonth = selectedBooking.month;
    int originalYear = selectedBooking.year;
    int originalHours = selectedBooking.hours;

    // Modify booking details (Same logic as the user editBooking function)
    cout << "Editing booking for Booking ID: " << selectedBooking.bookingID << endl;

    // Edit date
    cout << "Current Date: " << selectedBooking.day << "/" << selectedBooking.month << "/" << selectedBooking.year << endl;
    cout << "Enter new day (or press Enter to keep): ";
    string newDay;
    getline(cin, newDay);
    if (!newDay.empty()) {
        try {
            selectedBooking.day = stoi(newDay);
        } catch (invalid_argument&) {
            cout << "Invalid day input. Keeping the original value." << endl;
        }
    }

    cout << "Enter new month (or press Enter to keep): ";
    string newMonth;
    getline(cin, newMonth);
    if (!newMonth.empty()) {
        try {
            selectedBooking.month = stoi(newMonth);
        } catch (invalid_argument&) {
            cout << "Invalid month input. Keeping the original value." << endl;
        }
    }

    cout << "Enter new year (or press Enter to keep): ";
    string newYear;
    getline(cin, newYear);
    if (!newYear.empty()) {
        try {
            selectedBooking.year = stoi(newYear);
        } catch (invalid_argument&) {
            cout << "Invalid year input. Keeping the original value." << endl;
        }
    }

    // Validate the new date
    if (!isDateValid(selectedBooking.day, selectedBooking.month, selectedBooking.year)) {
        cout << "Invalid date entered. Keeping the original date." << endl;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
    }

    // Get the available slots and price for the selected court
    string availableSlots;
    double pricePerHour = 0.0;
    ifstream courtsFile("courts.txt");
    string courtLine;
    while (getline(courtsFile, courtLine)) {
        stringstream ss(courtLine);
        Court court;
        ss >> court.courtID >> court.courtNumber >> court.pricePerHour >> court.bookingStatus >> court.courtType >> availableSlots >> court.courtLocation;
        if (court.courtID == selectedBooking.courtID) {
            pricePerHour = court.pricePerHour;
            break;
        }
    }
    courtsFile.close();

    cout << "Current Time Slot: " << selectedBooking.timeSlot << ". Enter new Time Slot (or press Enter to keep): ";
    string newTimeSlot;
    getline(cin, newTimeSlot);
    if (!newTimeSlot.empty()) {
        selectedBooking.timeSlot = newTimeSlot;
    }

    // Edit court number (and ID)
    cout << "Current Court Number: " << selectedBooking.courtNumber << ". Enter new Court Number (or press Enter to keep): ";
    string newCourtNumber;
    getline(cin, newCourtNumber);
    if (!newCourtNumber.empty()) {
        vector<Court> allCourts = readCourtsFromFile("courts.txt");
        auto courtIt = find_if(allCourts.begin(), allCourts.end(), [&](const Court& c) {
            return c.courtNumber == newCourtNumber;
        });

        if (courtIt != allCourts.end()) {
            selectedBooking.courtNumber = newCourtNumber;
            selectedBooking.courtID = courtIt->courtID;
        } else {
            cout << "Court number not found. Keeping the original value." << endl;
        }
    }

    cout << "Current Hours: " << selectedBooking.hours << ". Enter new Hours (or press Enter to keep): ";
    string newHours;
    getline(cin, newHours);
    if (!newHours.empty()) {
        try {
            selectedBooking.hours = stoi(newHours);
        } catch (invalid_argument&) {
            cout << "Invalid hours input. Keeping the original value." << endl;
        }
    }

    // Validate hours based on time slot
    if (!newTimeSlot.empty() || !newHours.empty()) {
        int startTime = stoi(selectedBooking.timeSlot.substr(0, 2));
        int endTime = stoi(selectedBooking.timeSlot.substr(6, 2));
        if (selectedBooking.hours > (endTime - startTime)) {
            cout << "The hours entered do not match the time slot duration. Please try again.\n" << endl;
            selectedBooking.hours = originalHours;
            this_thread::sleep_for(chrono::milliseconds(1000));
            system("cls");
            loading();
            system("cls");
            return;
        }
    }

    // Check if the new time slot is within the available range
    if (!isTimeSlotWithinRange(availableSlots, selectedBooking.timeSlot)) {
        cout << "The new time slot is not within the available range. Reverting to original booking details." << endl;
        selectedBooking.courtID = originalCourtID;
        selectedBooking.courtNumber = originalCourtNumber;
        selectedBooking.timeSlot = originalTimeSlot;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
        selectedBooking.hours = originalHours;
        return; // Exit the function
    }

    // Check if the new time slot is available
    if (!isCourtAvailable(selectedBooking.courtID, selectedBooking.day, selectedBooking.month, selectedBooking.year, selectedBooking.timeSlot)) {
        cout << "The new time slot or court is not available. Reverting to original booking details." << endl;
        selectedBooking.courtID = originalCourtID;
        selectedBooking.courtNumber = originalCourtNumber;
        selectedBooking.timeSlot = originalTimeSlot;
        selectedBooking.day = originalDay;
        selectedBooking.month = originalMonth;
        selectedBooking.year = originalYear;
        selectedBooking.hours = originalHours;
    } else {
        // Calculate additional charges if hours are increased
        double additionalCharges = 0.0;
        if (selectedBooking.hours > originalHours) {
            additionalCharges = (selectedBooking.hours - originalHours) * pricePerHour;
            cout << "Additional charges: RM " << fixed << setprecision(2) << additionalCharges << endl;
            cout << "The customer will be informed to pay the additional charges at the counter." << endl;
            this_thread::sleep_for(chrono::milliseconds(2500));
        }

        // Update court availability status to "Available" for the original booking details
        vector<Court> allCourts = readCourtsFromFile("courts.txt");
        for (Court& court : allCourts) {
            if (court.courtID == originalCourtID) {
                // Remove the old booking from booked_courts.txt
                ifstream bookedInfile("booked_courts.txt");
                ofstream tempOutfile("temp_booked_courts.txt");

                if (!bookedInfile.is_open() || !tempOutfile.is_open()) {
                    cout << "Error: Unable to open booked_courts.txt or temp_booked_courts.txt." << endl;
                    return;
                }

                string line;
                while (getline(bookedInfile, line)) {
                    stringstream ss(line);
                    int bookingID, bookedCourtID, bookedDay, bookedMonth, bookedYear, hours;
                    string bookedCourtNumber, bookedTimeSlot, name, contact, userName, courtType;
                    ss >> bookingID >> bookedCourtID >> bookedCourtNumber >> bookedDay >> bookedMonth >> bookedYear >> bookedTimeSlot >> hours;
                    ss >> ws;
                    getline(ss, name, ' ');
                    getline(ss, contact, ' ');
                    getline(ss, userName, ' ');
                    getline(ss, courtType);

                    if (bookingID != selectedBooking.bookingID) {
                        tempOutfile << line << endl;
                    }
                }

                bookedInfile.close();
                tempOutfile.close();

                remove("booked_courts.txt");
                rename("temp_booked_courts.txt", "booked_courts.txt");

                // Update the booking in booked_courts.txt
                ofstream bookedOutfile("booked_courts.txt", ios::app);
                if (!bookedOutfile.is_open()) {
                    cout << "Error: Unable to open booked_courts.txt for writing." << endl;
                    return;
                }

                bookedOutfile << selectedBooking.bookingID << " " << selectedBooking.courtID << " " << selectedBooking.courtNumber << " "
                              << selectedBooking.day << " " << selectedBooking.month << " " << selectedBooking.year << " "
                              << selectedBooking.timeSlot << " " << selectedBooking.hours << " " << selectedBooking.name << " "
                              << selectedBooking.contact << " " << selectedBooking.userName << " " << selectedBooking.courtType << endl;

                bookedOutfile.close();
                break;
            }
        }
    }

    cout << "Booking updated successfully." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    system("cls");
    loading();
    system("cls");
}

// Menu to Sort Courts
void sortingMenu()
{
    int choice;
    vector<Court> courts = readCourtsFromFile("courts.txt");

    cout << "\n****************************************\n";
    cout << "\tAdmin - Sorting Courts\n";
    cout << "****************************************\n";
    cout << "\n1. Sort courts by ID (Quicksort)";
    cout << "\n2. Sort courts by Type (Insertion Sort)";
    cout << "\n3. Sort courts by Price (Heapsort)";
    cout << "\n0. Back to Menu";
    cout << "\n\nEnter your choice: ";
    cin >> choice;

    switch(choice)
    {
        case 1:
            system("cls");
            loading();
            system("cls");
            sortCourtsByIDQuickSort(courts); // Now defined
            break;
        case 2:
            system("cls");
            loading();
            system("cls");
            insertionSortByType(courts);
            break;
        case 3:
            system("cls");
            loading();
            system("cls");
            heapSortByPrice(courts);
            break;
        case 0:
            system("cls");
            loading();
            system("cls");
            return; // Back to Admin Homepage
        default:
            cout << "Invalid choice" << endl;
            this_thread::sleep_for(chrono::milliseconds(1000)); // Pause before returning to menu
            system("cls");
            loading();
            system("cls");
            return; // Back to Admin Homepage
    }

    // Update the sorted courts back to file
    ofstream outfile("courts.txt");
    if (!outfile.is_open())
    {
        cout << "Failed to open the courts file for writing." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
        loading();
        system("cls");
        return;
    }
    for (const auto& court : courts)
    {
        outfile << court.courtID << " " << court.courtNumber << " " << court.pricePerHour << " " << court.bookingStatus << " " << court.courtType << " " << court.availableSlots << " " << court.courtLocation << endl;
    }
    outfile.close();

    cout << "Courts have been sorted and updated successfully.\n";
    cout << "Press '0' to return to the menu: ";
    string backInput;
    cin >> backInput;
    if (isBackCommand(backInput)) {
        system("cls");
        loading();
        system("cls");
    }
}

// Function to Register a New Admin
void adminRegister() {
    AdminClass adminClass;
    string adminsFilename = "admins.txt";
    vector<Admin> adminsList = LoadAdmins(adminsFilename);
    adminClass.registerAdmin(adminsList, adminsFilename);
}

// Function to Register a New User (Alternate Name for Clarity)
void userRegister() {
    UserClass userClass;
    string filenameUsers = "users.txt";
    vector<User> usersList = LoadUsers(filenameUsers);
    userClass.registerUser(usersList, filenameUsers);
}

void generateAdminReport(const vector<Court>& courts, vector<Booking>& bookings) {
    double totalEarnings = 0;
    for (const auto& booking : bookings) {
        totalEarnings += booking.pricePerHour * booking.hours;
    }

    int totalBookings = bookings.size();
    int totalCourts = courts.size();

    unordered_map<string, int> courtTypeBookings;

    // Function to filter bookings by month and year
    auto filterBookings = [&](int year, int month) {
        vector<Booking> filteredBookings;
        for (const auto& booking : bookings) {
            if (booking.year == year && (month == 0 || booking.month == month)) {
                filteredBookings.push_back(booking);
            }
        }
        return filteredBookings;
    };

    // Function to display report for given bookings
    auto displayReport = [&](const vector<Booking>& filteredBookings) {
        unordered_map<string, int> courtTypeBookings;
        double filteredTotalEarnings = 0;

        for (const auto& booking : filteredBookings) {
            string courtType = booking.courtType;

            if (courtType.empty()) {
                auto courtIt = find_if(courts.begin(), courts.end(), [&](const Court& c) {
                    return c.courtNumber == booking.courtNumber;
                });
                if (courtIt != courts.end()) {
                    courtType = courtIt->courtType;
                } else {
                    courtType = "unknown";
                }
            }

            transform(courtType.begin(), courtType.end(), courtType.begin(), ::tolower);
            courtTypeBookings[courtType]++;
            filteredTotalEarnings += booking.pricePerHour * booking.hours;
        }

        cout << "\n==================================\n";
        cout << "\tAdmin Report\n";
        cout << "==================================\n";
        cout << "Total Earnings: RM " << fixed << setprecision(2) << filteredTotalEarnings << endl;
        cout << "Total Bookings: " << filteredBookings.size() << endl;
        cout << "Total Courts: " << totalCourts << endl;

        cout << "\nBookings by Court Type:\n";
        if (courtTypeBookings.empty()) {
            cout << "No bookings found.\n";
        } else {
            for (const auto& pair : courtTypeBookings) {
                cout << pair.first << ": " << pair.second << " bookings" << endl;
            }
        }
    };

    // Main loop for admin report
    while (true) {
        cout << "\nAdmin Report Options:\n";
        cout << "1. Generate Report for All Bookings\n";
        cout << "2. Filter Report by Year\n";
        cout << "3. Filter Report by Year and Month\n";
        cout << "4. Search Bookings by Date\n";
        cout << "0. Back to Admin Menu\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            system("cls");
            loading();
            system("cls");
            displayReport(bookings);
        } else if (choice == 2) {
            system("cls");
            loading();
            system("cls");
            cout << "Enter the year to filter: ";
            int year;
            cin >> year;
            displayReport(filterBookings(year, 0)); // 0 for month means all months
        } else if (choice == 3) {
            system("cls");
            loading();
            system("cls");
            cout << "Enter the year to filter: ";
            int year;
            cin >> year;
            cout << "Enter the month to filter (1-12): ";
            int month;
            cin >> month;
            displayReport(filterBookings(year, month));
        } else if (choice == 4) {
            system("cls");
            loading();
            system("cls");
            cout << "Enter the year to search: ";
            int year;
            cin >> year;
            cout << "Enter the month to search (1-12): ";
            int month;
            cin >> month;
            cout << "Enter the day to search: ";
            int day;
            cin >> day;
        
            vector<Booking> searchedBookings;
            for (const auto& booking : bookings) {
                if (booking.year == year && booking.month == month && booking.day == day) {
                    searchedBookings.push_back(booking);
                }
            }
        
            if (searchedBookings.empty()) {
                cout << "No bookings found for the specified date.\n";
            } else {
                cout << "\nBookings on " << year << "-" << month << "-" << day << ":\n";
                for (const auto& booking : searchedBookings) {
                    // Display each booking detail
                    cout << "Booking ID: " << booking.bookingID << ", Court Number: " << booking.courtNumber
                         << ", Type: " << booking.courtType << ", Time: " << booking.timeSlot
                         << ", Hours: " << booking.hours << ", Customer: " << booking.name << "\n";
                }
            }
        } else if (choice == 0) {
            system("cls");
            loading();
            system("cls");
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }

        cout << "\nPress '0' to return to the menu: ";
        string backInput;
        cin >> backInput;
        if (isBackCommand(backInput)) {
            system("cls");
            loading();
            system("cls");
        }
    }
}

// Function to handle adding new court (Badminton or Pickleball)
void addNewCourt(vector<Court>& courts, const string& filename) {
    int choice;

    while (true) {
        cout << "\nSelect Court Type to Add:\n";
        cout << "1. Badminton Court\n";
        cout << "2. Pickleball Court\n";
        cout << "0. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system("cls");
                loading();
                system("cls");
                addCourt(courts, filename, "Badminton");
                return;
            case 2:
                system("cls");
                loading();
                system("cls");
                addCourt(courts, filename, "Pickleball");
                return;
            case 0:
                system("cls");
                loading();
                system("cls");
                return;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
}

int binarySearchCourtByID(const vector<Court>& courts, int targetID) {
    int low = 0;
    int high = courts.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Prevent potential overflow

        if (courts[mid].courtID == targetID) {
            return mid; // Court found at index mid
        } else if (courts[mid].courtID < targetID) {
            low = mid + 1; // Search in the right half
        } else {
            high = mid - 1; // Search in the left half
        }
    }

    return -1; // Court not found
}

// Admin Homepage with Enhanced Features
void AdminHomepage(const string& adminUsername) 
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);
    vector<Booking> bookings; 

    // Load bookings from file
    ifstream bookedFile("booked_courts.txt");
    if (bookedFile.is_open()) {
        string line;
        while (getline(bookedFile, line)) {
    Booking booking;
    stringstream ss(line);
    ss >> booking.bookingID >> booking.courtID >> booking.courtNumber >> booking.day >> booking.month >> booking.year >> booking.timeSlot >> booking.hours;
    ss >> ws; // Consume any leading whitespace
    
    // Use getline to read fields with spaces, but use a temporary variable for intermediate fields
    string tempName, tempContact, tempUserName;
    getline(ss, tempName, ' ');
    booking.name = tempName;
    getline(ss, tempContact, ' ');
    booking.contact = tempContact;
    getline(ss, tempUserName, ' ');
    booking.userName = tempUserName;

    // Read the rest of the line into courtType
    getline(ss, booking.courtType);
            
            // Find the court in the courts vector to get additional details
            auto courtIt = find_if(courts.begin(), courts.end(), [&](const Court& c) {
                return c.courtNumber == booking.courtNumber;
                });
            if (courtIt != courts.end()) {
                booking.pricePerHour = courtIt->pricePerHour;
                booking.courtType = courtIt->courtType;
                
            }

            bookings.push_back(booking);
        }
        bookedFile.close();
    }

    int choice;
    while (true)
    {
        cout << "\n****************************************\n";
        cout << "\tAdmin Panel\n";
        cout << "****************************************\n";
        cout << "\n1. Display Courts";
        cout << "\n2. Add New Court";
        cout << "\n3. Delete Courts";
        cout << "\n4. Edit Courts";
        cout << "\n5. Sort Courts";
        cout << "\n6. Search Courts";
        cout << "\n7. View Booking History";
        cout << "\n8. View Court Availability";
        cout << "\n9. Change Password"; 
        cout << "\n10. Generate Admin Report";
        cout << "\n11. Edit Customer Booking"; 
        cout << "\n0. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            loading();
            system("cls");
            displayCourts(courts);
            cout << "\nPress '0' to return to the menu: ";
            {
                string backInput;
                cin >> backInput;
                if (isBackCommand(backInput)) {
                    system("cls");
                    loading();
                    system("cls");
                }
            }
            break;
        case 2:
            system("cls");
            loading();
            system("cls");
            addNewCourt(courts, filename); // Call the function to add new court
            courts = readCourtsFromFile(filename); // Update the vector after adding new courts
            break;
        
        case 3: //Renumbered from 4 to 3
            system("cls");
            loading();
            system("cls");
            {
                int courtID;
                cout << "Enter Court No. to delete (or '0' to cancel): ";
                string idInput;
                cin >> idInput;
                if (isBackCommand(idInput)) {
                    cout << "Delete court cancelled. Returning to menu.\n";
                    cout << "Press '0' to return to the menu: ";
                    string backInput;
                    cin >> backInput;
                    if (isBackCommand(backInput)) {
                        system("cls");
                        loading();
                        system("cls");
                    }
                    break;
                }
                try {
                    courtID = stoi(idInput);
                    if (courtID < 0) {
                        throw invalid_argument("Negative ID");
                    }
                }
                catch (invalid_argument&) {
                    cout << "Invalid Court No. Returning to menu.\n";
                    cout << "Press '0' to return to the menu: ";
                    string backInput;
                    cin >> backInput;
                    if (isBackCommand(backInput)) {
                        system("cls");
                        loading();
                        system("cls");
                    }
                    break;
                }
                deleteCourtFriend(courts, courtID, filename);
            }
            break;
        case 4: //Renumbered from 5 to 4
            system("cls");
            loading();
            system("cls");
            editCourt(courts, filename);
            break;
        case 5: //Renumbered from 6 to 5
            system("cls");
            loading();
            system("cls");
            sortingMenu();
            courts = readCourtsFromFile(filename); // Update after sorting
            break;
    case 6: // Search Courts
{
    system("cls");
    loading();
    system("cls");
    cout << "\n****************************************\n";
    cout << "\tAdmin - Search Courts\n";
    cout << "****************************************\n";
    cout << "\n1. Search by Court ID";
    cout << "\n2. Search by Type";
    cout << "\n3. Search by Price";
    cout << "\n0. Back";
    cout << "\nEnter your choice: ";
    
    int searchChoice;
    cin >> searchChoice;

    switch (searchChoice) {
        case 1: { // Search by Court ID
            string input;
            int searchId;

            while (true) {
                cout << "\nEnter Court ID to search for (or '0' to cancel): ";
                cin >> input;

                if (isBackCommand(input)) {
                    cout << "Search cancelled. Returning to menu.\n";
                    break;
                }

                try {
                    searchId = stoi(input);

                    // Sort and search
                    sort(courts.begin(), courts.end(), compareCourtsByID);
                    int index = binarySearchCourtByID(courts, searchId);

                    if (index != -1) {
                        displayCourt(courts[index]);
                    } else {
                        cout << "Court with ID " << searchId << " not found.\n";
                    }
                } catch (invalid_argument&) {
                    cout << "Invalid input. Please enter a numeric Court ID.\n";
                }
            }
            break;
        }
        case 2: { // Search by Type
            unordered_map<string, vector<Court>> courtTypeMap = createCourtTypeHashMap(courts);

            string type;
            while (true) {
                cout << "\nEnter court type to search (Badminton/Pickleball) (or '0' to cancel): ";
                cin >> type;

                if (isBackCommand(type)) {
                    cout << "Search cancelled. Returning to menu.\n";
                    break;
                }

                if (type != "Badminton" && type != "Pickleball") {
                    cout << "Invalid type. Please enter either 'Badminton' or 'Pickleball'.\n";
                } else {
                    auto it = courtTypeMap.find(type);
                    if (it != courtTypeMap.end()) {
                        cout << "\nCourts found with type " << type << ":\n";
                        for (const auto& court : it->second) {
                            displayCourt(court);
                        }
                    } else {
                        cout << "\nNo courts with type " << type << " found.\n";
                    }
                    break;
                }
            }
            break;
        }
        case 3: { // Search by Price
            double price;
            while (true) {
                cout << "\nEnter price per hour to search (or '0' to cancel): RM ";
                string priceInput;
                cin >> priceInput;

                if (isBackCommand(priceInput)) {
                    cout << "Search cancelled. Returning to menu.\n";
                    break;
                }

                try {
                    price = stod(priceInput);
                    if (price < 0) throw invalid_argument("Negative price");

                    searchByPrice(courts, price);
                    break;
                } catch (invalid_argument&) {
                    cout << "Invalid input. Please enter a valid price.\n";
                }
            }
            break;
        }
        case 0: { // Back
            cout << "Returning to admin panel.\n";
            system("cls");
            loading();
            system("cls");
            break;
        }
        default:
            cout << "Invalid choice. Please select a valid option.\n";
    }

    cout << "\nPress '0' to return to the menu: ";
    string backInput;
    cin >> backInput;
    if (isBackCommand(backInput)) {
        system("cls");
        loading();
        system("cls");
    }
    break; // Break for case 6
}

        case 7: //Renumbered from 8 to 7
            system("cls");
            loading();
            system("cls");
            viewBookingHistory("admin", true); // Invoke Booking History for Admin
            break;
        case 8: //Renumbered from 9 to 8
            system("cls");
            loading();
            system("cls");
            viewCourtAvailability(); // Invoke Court Availability
            break;
      case 9: {
        system("cls");
        loading();
        system("cls");

        // No need to get the current admin's username from the file anymore
        // Use the adminUsername that was passed to the function
        changeAdminPassword(adminUsername);

    }
    break;
        case 10: //Renumbered from 11 to 10
            system("cls");
            loading();
            system("cls");
            generateAdminReport(courts, bookings); // Generate Admin Report
            break;
		case 11:
    		system("cls");
    		loading();
    		system("cls");
    		adminEditBooking();
    		break;
        case 0:
            system("cls");
            loading();
            system("cls");
            return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            break; // Continue the loop without recursion
        }
    }
}

// User Homepage with Enhanced Features
void UserHomepage(const string& username, const string& name, const string& phoneNumber)
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);

    int choice;
    while (true) {
        cout << "\n****************************************\n";
        cout << "Welcome to BP Court Booking Center\n";
        cout << "****************************************\n";
        cout << "\n1. Display Courts";
        cout << "\n2. Book Court";
        cout << "\n3. View Booking History";
        cout << "\n4. Edit Booking";
        cout << "\n5. View Court Availability";
        cout << "\n6. Sort Courts";
        cout << "\n7. Search Courts";
        cout << "\n8. Change Password";
        cout << "\n9. Delete Booking";
        cout << "\n0. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            loading();
            system("cls");
            displayCourts(courts);
            cout << "\nPress '0' to return to the menu: ";
            {
                string backInput;
                cin >> backInput;
                if (isBackCommand(backInput)) {
                    system("cls");
                    loading();
                    system("cls");
                }
            }
            break;

        case 2:
            system("cls");
            loading();
            system("cls");
            bookingMenu(username, name, phoneNumber);
            break;

        case 3:
            system("cls");
            loading();
            system("cls");
            viewBookingHistory(username, false);
            break;

        case 4:
            system("cls");
            loading();
            system("cls");
            editBooking(username, name, phoneNumber);
            break;

        case 5:
            system("cls");
            loading();
            system("cls");
            viewCourtAvailability();
            break;
         case 6:
            system("cls");
            loading();
            system("cls");
            sortingMenu();
            courts = readCourtsFromFile(filename); // Update after sorting
            break;
        case 7:
        {
            system("cls");
            loading();
            system("cls");
            cout << "\n****************************************\n";
            cout << "\tAdmin - Search Courts\n";
            cout << "****************************************\n";
            cout << "\n1. Search by Type";
            cout << "\n2. Search by Price";
            cout << "\n0. Back";
            cout << "\nEnter your choice: ";
            int searchChoice;
            cin >> searchChoice;

            if (searchChoice == 1)
            {
                string type;
                while (true) {
                    cout << "Enter court type to search (Badminton/Pickleball) (or '0' to cancel): ";
                    cin >> type;
                    if (isBackCommand(type)) {
                        cout << "Search cancelled. Returning to admin panel.\n";
                        cout << "Press '0' to return to the menu: ";
                        string backInput;
                        cin >> backInput;
if (isBackCommand(backInput)) {
                            system("cls");
                            loading();
                            system("cls");
                        }
                        break;
                    }
                    if (type != "Badminton" && type != "Pickleball") {
                        cout << "Invalid type. Please enter either 'Badminton' or 'Pickleball'.\n";
                    }
                    else {
                        searchByType(courts, type);
                        break;
                    }
                }
            }
            else if (searchChoice == 2)
            {
                double price;
                while (true) {
                    cout << "Enter price per hour to search (or '0' to cancel): RM ";
                    string priceInput;
                    cin >> priceInput;
                    if (isBackCommand(priceInput)) {
                        cout << "Search cancelled. Returning to admin panel.\n";
                        cout << "Press '0' to return to the menu: ";
                        string backInput;
                        cin >> backInput;
                        if (isBackCommand(backInput)) {
                            system("cls");
                            loading();
                            system("cls");
                        }
                        break;
                    }
                    try {
                        price = stod(priceInput);
                        if (price < 0) {
                            throw invalid_argument("Negative price");
                        }
                        searchByPrice(courts, price);
                        break;
                    }
                    catch (invalid_argument&) {
                        cout << "Invalid input. Please enter a valid price.\n";
                    }
                }
            }
            else if (searchChoice == 0)
            {
                cout << "Returning to admin panel.\n";
                system("cls");
                loading();
                system("cls");
                break;
            }
            else
            {
                cout << "Invalid choice." << endl;
            }
            cout << "\nPress '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            break;
        }
        case 8: {
                system("cls");
                loading();
                system("cls");

                // Display the current username
                cout << "\nLogged in as: " << username << endl;

                changeUserPassword(username);
                }
                break;
        
        case 9:
            system("cls");
            loading();
            system("cls");
            deleteBookingRecord(username);
            break;

        case 0:
            system("cls");
            loading();
            system("cls");
            return;

        default:
            cout << "\nInvalid choice. Please try again.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            break; // Continue the loop without recursion
        }
    }
}


// User Menu
void UserMenu()
{
    UserClass userClass; // Instantiate UserClass
    int choice;
    do {
        cout << "\n****************************************\n";
        cout << "\tWelcome to BP Court Booking Center\n";
        cout << "****************************************\n";
        cout << "\n1. Register";
        cout << "\n2. Login";
        cout << "\n0. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            loading();
            system("cls");
            userRegister(); // Register a new user
            break;
        case 2:
            system("cls");
            loading();
            system("cls");
            {
                string filenameUsers = "users.txt";
                vector<User> usersList = LoadUsers(filenameUsers);
                string loggedInName, loggedInPhone, loggedInUsername;
                if (userClass.loginUser(usersList, loggedInName, loggedInPhone, loggedInUsername)) {
                    UserHomepage(loggedInUsername, loggedInName, loggedInPhone);
                }
            }
            break;
        case 0:
            system("cls");
            loading();
            system("cls");
            return;
        default:
            cout << "\nInvalid choice. Please try again." << endl;
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            break; // Continue the loop without recursion
        }
    } while (choice != 0);
}

// Admin Menu
// Admin Menu
void AdminMenu()
{
    AdminClass adminClass; // Instantiate AdminClass
    int choice;
    do {
        cout << "\n****************************************\n";
        cout << "\tWelcome to BP Court Booking Center\n";
        cout << "****************************************\n";
        cout << "1. Login";
        cout << "\n2. Register New Admin";
        cout << "\n0. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            loading();
            system("cls");
            {
                string adminsFilename = "admins.txt";
                vector<Admin> adminsList = LoadAdmins(adminsFilename);
                string loggedInAdminUsername; // Store the logged-in admin's username here
                if (adminClass.loginAdmin(adminsList, loggedInAdminUsername)) { // Pass by reference
                    system("cls");
                    loading();
                    system("cls");
                    AdminHomepage(loggedInAdminUsername); // Pass the username to AdminHomepage
                }
            }
            break;
        case 2:
            system("cls");
            loading();
            system("cls");
            adminRegister(); // Register a new admin
            break;
        case 0:
            system("cls");
            loading();
            system("cls");
            return;
        default:
            cout << "\nInvalid choice. Please try again." << endl;
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            break; // Continue the loop without recursion
        }
    } while (choice != 0);
}
// Main Page Function
void MainPage()
{
    initializeFiles(); // Ensure all necessary files are initialized

    int choice;
    do
    {
        cout << "\n****************************************\n";
        cout << "\tWelcome to BP Court Booking Center\n";
        cout << "****************************************\n";
        cout << "1. User";
        cout << "\n2. Admin";
        cout << "\n0. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            loading();
            system("cls");
            UserMenu();
            break;
        case 2:
            system("cls");
            loading();
            system("cls");
            AdminMenu();
            break;
        case 0:
            cout << "\n>_< Thank you for using our system. See you soon! >_<" << endl;
            exit(0);
            break;
        default:
            cout << "\nInvalid choice. Please try again." << endl;
            cout << "Press '0' to exit or any other key to continue: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                cout << "\n>_< Thank you for using our system. See you soon! >_<" << endl;
                exit(0);
            }
            system("cls");
            loading();
            system("cls");
            break; // Continue the loop without recursion
        }
    } while (choice != 0);
}

// Function to Define bookingMenu
void bookingMenu(const string& username, const string& name, const string& phoneNumber) {
    // Display all courts
    vector<Court> courts = readCourtsFromFile("courts.txt");
    displayCourts(courts);

    // Ask user to choose number of courts to book
    int numberOfCourts;
    while (true) {
        cout << "Enter the number of courts you want to book (or '0' to cancel): ";
        string numInput;
        cin >> numInput;
        if (isBackCommand(numInput)) {
            cout << "Booking cancelled. Returning to menu.\n";
            cout << "Press '0' to return to the menu: ";
            string backInput;
            cin >> backInput;
            if (isBackCommand(backInput)) {
                system("cls");
                loading();
                system("cls");
            }
            return;
        }
        try {
            numberOfCourts = stoi(numInput);
            if (numberOfCourts <= 0) {
                throw invalid_argument("Non-positive number");
            }
            if (numberOfCourts > courts.size())
            {
                cout << " \n Sorry,We dont have enough court.\n";
                continue;
            }
            break;
        }
        catch (invalid_argument&) {
            cout << "Invalid input. Please enter a positive integer.\n";
        }
    }

    // Let user select multiple court IDs
    vector<int> selectedCourtIDs;
    for (int i = 0; i < numberOfCourts; ++i) {
        string courtNumber;
        while (true) {
            cout << "Enter Court Name " << (i + 1) << " to book (or '0' to cancel): ";
            
            cin >> courtNumber;
            if (isBackCommand(courtNumber)) {
                cout << "Booking cancelled. Returning to menu.\n";
                cout << "Press '0' to return to the menu: ";
                string backInput;
                cin >> backInput;
                if (isBackCommand(backInput)) {
                    system("cls");
                    loading();
                    system("cls");
                }
                return;
            }
            
            // Validate court number exists
            auto it = find_if(courts.begin(), courts.end(), [&](const Court& c) {
                return c.courtNumber == courtNumber;
                });
            if (it == courts.end()) {
                cout << "Court number does not exist. Please enter a valid Court number.\n";
            }
            else if (find(selectedCourtIDs.begin(), selectedCourtIDs.end(), it->courtID) != selectedCourtIDs.end()) {
                cout << "Court already selected. Please choose a different court.\n";
            }
            else {
                selectedCourtIDs.push_back(it->courtID);
                break;
            }
            
        }
    }

    // Proceed to input booking details
    customerInput(selectedCourtIDs, name, phoneNumber, username);
}

// Function to Define getPassword (Password Masking)
string getPassword() {
    string password = "";
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key pressed
        if (ch == '\b') { // Handle backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Remove '*' from console
            }
        }
        else if (ch == 3) { // Handle Ctrl+C
            exit(0);
        }
        else if (isprint(ch)) { // Only add printable characters
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

// Function to Read Integer with Validation
bool readInteger(int& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        return false;
    }
    return true;
}

// Function to Read Double with Validation
bool readDouble(double& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        return false;
    }
    return true;
}

// Function to Check if Input is '0' to Go Back
bool isBackCommand(const string& input) {
    return input == "0";
}

void changeUserPassword(const string& username) {
    vector<User> users = LoadUsers("users.txt");
    auto it = find_if(users.begin(), users.end(), [&](const User& u) {
        return u.username == username;
        });

    if (it == users.end()) {
        cout << "User not found." << endl;
        return;
    }

    UserClass userClass;

    // Prompt for the current password
    cout << "Enter current password for " << it->username << ": ";
    string currentPassword = getPassword();

    // Verify the current password
    if (currentPassword != it->password) {
        cout << "Incorrect current password. Password change aborted.\n";
        return;
    }

    // Prompt for the new password
    cout << "Enter new password: ";
    string newPassword = getPassword();

    // Check if new password is the same as the current password
    if (newPassword == currentPassword) {
        cout << "New password cannot be the same as the current password. Password change aborted.\n";
        return;
    }

    // Validate new password strength
    if (!userClass.isValidPassword(newPassword)) {
        cout << "Password does not meet the requirements:\n";
        cout << "- At least 8 characters\n";
        cout << "- At least one uppercase letter\n";
        cout << "- At least one lowercase letter\n";
        cout << "- At least one number\n";
        cout << "- At least one symbol\n";
        return;
    }

    it->password = newPassword;

    ofstream outfile("users.txt", ios::trunc);
    if (!outfile.is_open()) {
        cout << "Failed to update password." << endl;
        return;
    }

    for (const auto& user : users) {
        outfile << user.username << " " << user.password << " " << user.name << " " << user.phoneNumber << endl;
    }
    outfile.close();

    cout << "Password updated successfully.\n";
}


// Function to convert a string to lowercase for case-insensitive comparison
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to change admin password
void changeAdminPassword(const string& username) {
    vector<Admin> admins = LoadAdmins("admins.txt");

    string lowerUsername = toLower(trim(username));
    auto it = find_if(admins.begin(), admins.end(), [&](const Admin& a) {
        return toLower(trim(a.username)) == lowerUsername;
    });

    if (it == admins.end()) {
        cout << "Admin not found. Please ensure the username is correct.\n";
        return;
    }

    cout << "\nLogged in as: " << it->username << endl;

    // Prompt for the current password
    cout << "Enter current password for " << it->username << ": ";
    string currentPassword = getPassword();

    // Verify the current password
    if (currentPassword != it->password) {
        cout << "Incorrect current password. Password change aborted.\n";
        return;
    }

    // Prompt for the new password
    cout << "Enter new password for " << it->username << ": ";
    string newPassword = getPassword();

    // Check if new password is the same as the current password
    if (newPassword == currentPassword) {
        cout << "New password cannot be the same as the current password. Password change aborted.\n";
        return;
    }

    // Validate new password strength (if needed)
    if (newPassword.length() < 8) {
        cout << "Password must be at least 8 characters.\n";
        return;
    }

    // Update the password
    it->password = newPassword;

    // Write the updated list of admins back to the file
    ofstream outfile("admins.txt", ios::trunc);
    if (!outfile.is_open()) {
        cout << "Failed to update password.\n";
        return;
    }

    for (const auto& admin : admins) {
        outfile << admin.username << " " << admin.password << endl;
    }
    outfile.close();

    cout << "Password updated successfully for " << it->username << ".\n";
}

// Function to simulate admin login
string loginAdmin() {
    string username, password;
    bool loggedIn = false;
    vector<Admin> admins = LoadAdmins("admins.txt");

    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    // Check if username and password match
    for (const Admin& admin : admins) {
        if (admin.username == username && admin.password == password) {
            loggedIn = true;
            cout << "Login successful for: " << username << endl;
            return username; // Return the logged-in admin's username
        }
    }

    if (!loggedIn) {
        cout << "Invalid username or password.\n";
    }

    return ""; // Return an empty string if login fails
}

// Main function
int main() {
    MainPage(); // Uncomment if you have a function for admin page

    return 0;
}
        