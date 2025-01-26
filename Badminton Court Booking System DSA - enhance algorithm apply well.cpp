#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <queue>
#include <windows.h>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <list>

using namespace std;

// Forward Declarations
class Court;
bool compareCourtsByID(const Court&, const Court&);
bool compareCourtsByType(const Court&, const Court&);
bool compareCourtsByPrice(const Court&, const Court&);
void displayCourt(const Court&);
void addCourt(vector<Court>&, const string&);
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
void AdminHomepage();
void bookingMenu(const string& username, const string& name, const string& phoneNumber);
void viewBookingHistory(const string& username, bool isAdmin);
void viewCourtAvailability();
void bookingMenuAdmin();
void customerInput(int courtID, const string& name, const string& contact);
void sortingMenu();
void adminRegister();
void userRegister();
void loading();
bool fileExists(const string& filename);

// Loading Animation
void loading() 
{
    cout << "Loading";
    for (int i = 0; i < 3; i++) 
    {
        cout << ".";
        Sleep(250); // Sleep for 250 milliseconds
    }
    cout << endl;
}

// User Structure
struct User 
{
    string username;
    string password;
    string name;
    string phoneNumber;
};

// Admin Structure
struct Admin 
{
    string username;
    string password;
};

// Booking Structure
struct Booking {
    int courtID;
    string type;
    double pricePerHour;
    string name;
    string contact;
    int day;
    int month;
    int year;
    string timeSlot;
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

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Person() {}
};

// Derived Class: UserClass
class UserClass : public Person {
public:
    UserClass() : Person() {}
    UserClass(string uname, string pwd, string nm, string phone) : Person(uname, pwd), name(nm), phoneNumber(phone) {}

    string name;
    string phoneNumber;

    void registerUser(vector<User>& users, const string& filenameUsers) {
        User newUser;
        cout << "\n****************************************\n";
        cout << "\tUser Register\n";
        cout << "****************************************\n";
        cout << "\nEnter username: ";
        cin >> newUser.username;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "\nEnter password: ";
        cin >> newUser.password;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        cout << "\nEnter your name: ";
        getline(cin, newUser.name);
        cout << "\nEnter your phone number: ";
        cin >> newUser.phoneNumber;

        // Save to file and vector
        users.push_back(newUser);
        ofstream file(filenameUsers.c_str(), ios::app);  // Use c_str() to convert string to const char*
        if (file.is_open()) 
        {
            file << newUser.username << " " << newUser.password << " " << newUser.name << " " << newUser.phoneNumber << endl;
            file.close();
        }
        else
        {
            cout << "Failed to open the users file for writing." << endl;
        }

        cout << "\nRegistration successful!" << endl;
    }

    bool loginUser(const vector<User>& usersList, string& loggedInName, string& loggedInPhone, string& loggedInUsername) {
        if (usersList.empty()) 
        {
            cout << "\nNo users registered. Please register first." << endl;
            return false;
        }

        string usernameInput, passwordInput;
        cout << "\n****************************************\n";
        cout << "\tUser Login\n";
        cout << "****************************************\n";
        cout << "\nEnter username: ";
        cin >> usernameInput;
        cout << "\nEnter password: ";
        cin >> passwordInput;

        for (const auto& user : usersList) 
        {
            if (user.username == usernameInput && user.password == passwordInput) 
            {
                cout << "\nLogin successful!" << endl;
                loggedInName = user.name;
                loggedInPhone = user.phoneNumber;
                loggedInUsername = user.username;
                return true;
            }
        }

        cout << "\nInvalid username or password." << endl;
        return false;
    }
};

// Derived Class: AdminClass
class AdminClass : public Person {
public:
    AdminClass() : Person() {}
    AdminClass(string uname, string pwd) : Person(uname, pwd) {}

    bool loginAdmin(const vector<Admin>& adminsList) {
        if (adminsList.empty()) 
        {
            cout << "\nNo admins registered." << endl;
            return false;
        }

        string usernameInput, passwordInput;
        cout << "\n****************************************\n";
        cout << "\tAdmin Login\n";
        cout << "****************************************\n";
        cout << "\nEnter admin username: ";
        cin >> usernameInput;
        cout << "\nEnter admin password: ";
        cin >> passwordInput;

        for (const auto& admin : adminsList) 
        {
            if (admin.username == usernameInput && admin.password == passwordInput) 
            {
                cout << "\nAdmin login successful!" << endl;
                return true;
            }
        }

        cout << "\nInvalid admin username or password." << endl;
        return false;
    }

    void registerAdmin(vector<Admin>& adminsList, const string& filenameAdmins) {
        Admin newAdmin;
        cout << "\n****************************************\n";
        cout << "\tAdmin Register\n";
        cout << "****************************************\n";
        cout << "\nEnter new admin username: ";
        cin >> newAdmin.username;
        cout << "\nEnter new admin password: ";
        cin >> newAdmin.password;

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
        }

        cout << "\nAdmin registration successful!" << endl;
    }
};

// Derived Class: ManagerClass (Additional Inheritance)
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
    string type; // e.g., Singles, Doubles
    double pricePerHour;

    Court() : courtID(0), type(""), pricePerHour(0.0) {}

    Court(int id, string tp, double price) 
        : courtID(id), type(tp), pricePerHour(price) {}
    
    // Accessor methods
    int getCourtID() const { return courtID; }
    string getType() const { return type; }
    double getPricePerHour() const { return pricePerHour; }

    // Friend functions
    friend void displayCourt(const Court& court);
    friend void addCourt(vector<Court>& courts, const string& filename);
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
    return a.type < b.type;
}

bool compareCourtsByPrice(const Court& a, const Court& b) 
{
    return a.pricePerHour < b.pricePerHour;
}

// Friend Function to Display a Court's Details
void displayCourt(const Court& court) 
{
    cout << left << "| " << setw(10) << court.courtID 
         << "| " << setw(15) << court.type 
         << "| " << setw(15) << fixed << setprecision(2) << court.pricePerHour 
         << " |" << endl;
    cout << "----------------------------------------------" << endl;
}

// Friend Function to Add a Court
void addCourt(vector<Court>& courts, const string& filename) 
{
    int id;
    string type;
    double price;

    cout << "Enter details for the new court:" << endl;
    cout << "Court ID: ";
    cin >> id;

    // Validate court ID uniqueness
    bool unique = true;
    for (const auto& court : courts) 
    {
        if (court.courtID == id) 
        {
            unique = false;
            break;
        }
    }

    if (!unique) 
    {
        cout << "Court ID already exists. Cannot add court." << endl;
        return;
    }

    cout << "Type (Singles/Doubles): ";
    cin >> type;
    // Validate type input
    while (type != "Singles" && type != "Doubles") 
    {
        cout << "Invalid type. Please enter either 'Singles' or 'Doubles': ";
        cin >> type;
    }

    cout << "Price per Hour: RM ";
    cin >> price;

    Court newCourt(id, type, price);
    courts.push_back(newCourt);

    // Write to file
    ofstream outfile(filename.c_str(), ios::app);
    if (outfile.is_open()) 
    {
        outfile << newCourt.courtID << " " << newCourt.type << " " << newCourt.pricePerHour << endl;
        outfile.close();
        cout << "Court added successfully." << endl;
    } 
    else 
    {
        cout << "Failed to open the courts file for writing." << endl;
    }
}

// Friend Function to Edit a Court
void editCourt(vector<Court>& courts, const string& filename) 
{
    int courtID;
    cout << "Enter Court ID to edit: ";
    cin >> courtID;

    auto it = find_if(courts.begin(), courts.end(), [&](const Court& c) {
        return c.courtID == courtID;
    });

    if (it != courts.end()) 
    {
        cout << "Editing Court ID: " << it->courtID << endl;
        cout << "Current Type: " << it->type << endl;
        cout << "Current Price per Hour: RM " << fixed << setprecision(2) << it->pricePerHour << endl;

        string newType;
        double newPrice;

        cout << "Enter new Type (Singles/Doubles) or press Enter to keep current: ";
        cin.ignore(); // Clear buffer
        getline(cin, newType);
        if (!newType.empty()) 
        {
            while (newType != "Singles" && newType != "Doubles") 
            {
                cout << "Invalid type. Please enter either 'Singles' or 'Doubles': ";
                getline(cin, newType);
            }
            it->type = newType;
        }

        cout << "Enter new Price per Hour or press Enter to keep current: RM ";
        string priceInput;
        getline(cin, priceInput);
        if (!priceInput.empty()) 
        {
            try {
                newPrice = stod(priceInput);
                it->pricePerHour = newPrice;
            }
            catch (invalid_argument&) {
                cout << "Invalid price input. Keeping previous price." << endl;
            }
        }

        // Update the file
        ofstream outfile(filename, ios::trunc);
        if (outfile.is_open()) 
        {
            for (const auto& court : courts) 
            {
                outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
            }
            outfile.close();
            cout << "Court updated successfully." << endl;
        } 
        else 
        {
            cout << "Failed to open the courts file for writing." << endl;
        }
    } 
    else 
    {
        cout << "Court ID not found." << endl;
    }
}

// Friend Function to Delete a Court
void deleteCourtFriend(vector<Court>& courts, int courtID, const string& filename) 
{
    auto it = find_if(courts.begin(), courts.end(), [&](const Court& c) {
        return c.courtID == courtID;
    });

    if (it != courts.end()) 
    {
        courts.erase(it);
        // Update the file
        ofstream outfile(filename, ios::trunc);
        if (outfile.is_open()) 
        {
            for (const auto& court : courts) 
            {
                outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
            }
            outfile.close();
            cout << "Court deleted successfully." << endl;
        } 
        else 
        {
            cout << "Failed to open the courts file for writing." << endl;
        }
    } 
    else 
    {
        cout << "Court ID not found." << endl;
    }
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
    void generateReceiptFromLastBooking() 
    {
        ifstream bookedFile("booked_courts.txt");
        if (!bookedFile.is_open()) 
        {
            cout << "Failed to open the \"booked_courts.txt\" file." << endl;
            return;
        }

        string line;
        Booking lastBooking;
        while (getline(bookedFile, line)) 
        {
            if (line.empty()) continue; // Skip empty lines
            stringstream ss(line);
            ss >> lastBooking.courtID >> lastBooking.day >> lastBooking.month >> lastBooking.year >> lastBooking.timeSlot;
            // Read the rest of the line for name and contact
            ss >> ws; // Consume any leading whitespace
            getline(ss, lastBooking.name, ' ');
            getline(ss, lastBooking.contact);
        }

        bookedFile.close();

        // Retrieve court details
        ifstream courtsFile("courts.txt");
        if (!courtsFile.is_open()) 
        {
            cout << "Failed to open the courts file." << endl;
            return;
        }

        string courtLine;
        bool courtFound = false;
        while (getline(courtsFile, courtLine)) 
        {
            stringstream ss(courtLine);
            int id;
            string type;
            double price;
            ss >> id >> type >> price;
            if (id == lastBooking.courtID) 
            {
                lastBooking.type = type;
                lastBooking.pricePerHour = price;
                courtFound = true;
                break;
            }
        }
        courtsFile.close();

        if (!courtFound) 
        {
            cout << "\nCourt with ID " << lastBooking.courtID << " was not found." << endl;
            cout << "Failed to generate receipt" << endl;
            return;
        }

        // Generate Grand Receipt
        ofstream grandReceiptFile("grand_receipt.txt", ios::app);
        if (grandReceiptFile.is_open()) 
        {
            system("cls");
            loading();
            system("cls");
            grandReceiptFile << "\n>>>> GRAND RECEIPT <<<<" << endl;
            grandReceiptFile << "User Name: " << lastBooking.name << endl;
            grandReceiptFile << "User Contact: " << lastBooking.contact << endl;
            grandReceiptFile << "Date of Booking: " << lastBooking.day << "/" << lastBooking.month << "/" << lastBooking.year << endl;
            grandReceiptFile << "Time Slot: " << lastBooking.timeSlot << endl;
            grandReceiptFile << "============================================================================================================" << endl;
            grandReceiptFile << "\t\t\tCourt Details" << endl;
            grandReceiptFile << "============================================================================================================" << endl;
            grandReceiptFile << left << setw(15) << "Court ID" << setw(15) << "Type" << setw(15) << "Price/Hour" << endl;
            grandReceiptFile << "---------------------------------------------" << endl;
            grandReceiptFile << left << " |" << setw(15) << lastBooking.courtID << setw(15) << lastBooking.type << setw(15) << fixed << setprecision(2) << lastBooking.pricePerHour << endl;
            grandReceiptFile << endl;
            grandReceiptFile.close();
        } 
        else 
        {
            cout << "Failed to open the grand_receipt.txt file for saving." << endl;
        }

        // Generate Regular Receipt
        ofstream receiptFile("receipt.txt");
        if (receiptFile.is_open()) 
        {
            receiptFile << "\n>>>> RECEIPT <<<<" << endl;
            receiptFile << "Customer Name: " << lastBooking.name << endl;
            receiptFile << "Customer Contact: " << lastBooking.contact << endl;
            receiptFile << "Date of Booking: " << lastBooking.day << "/" << lastBooking.month << "/" << lastBooking.year << endl;
            receiptFile << "Time Slot: " << lastBooking.timeSlot << endl;
            receiptFile << "===========================================================================" << endl;
            receiptFile << "\t\t\tCourt Details" << endl;
            receiptFile << "===========================================================================" << endl;
            receiptFile << left << setw(15) << "Court ID" << setw(15) << "Type" << setw(15) << "Price/Hour" << endl;
            receiptFile << "---------------------------------------------------------------------------" << endl;
            receiptFile << left << " |" << setw(15) << lastBooking.courtID << setw(15) << lastBooking.type << setw(15) << fixed << setprecision(2) << lastBooking.pricePerHour << endl;
            receiptFile << endl;
            receiptFile.close();
        } 
        else 
        {
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
        string type;
        double price;

        if (ss >> id >> type >> price) 
        {
            courts.emplace_back(id, type, price);
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
    cout << "----------------------------------------------" << endl;
    cout << left << "| " << setw(10) << "Court ID" 
         << "| " << setw(15) << "Type" 
         << "| " << setw(15) << "Price/Hour" << " |" << endl;
    cout << "----------------------------------------------" << endl;
    
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
        // Add a default admin
        adminFile << "admin admin123" << endl;
        adminFile.close();
    }

    // Initialize courts.txt
    if (!fileExists("courts.txt"))
    {
        ofstream courtsFile("courts.txt");
        // Populate courts.txt with 50 courts (25 Singles and 25 Doubles)
        for (int i = 1; i <= 25; ++i)
        {
            courtsFile << i << " Singles 50.00" << endl;
        }
        for (int i = 26; i <= 50; ++i)
        {
            courtsFile << i << " Doubles 70.00" << endl;
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

// Function to Load Admins from File
vector<Admin> LoadAdmins(const string& adminsFilename) 
{
    vector<Admin> adminsList;
    ifstream file(adminsFilename.c_str());
    if (!file.is_open()) 
    {
        cout << "Error: Unable to open file " << adminsFilename << endl;
        return adminsList;
    }
    Admin admin;
    while (file >> admin.username >> admin.password) 
    {
        adminsList.push_back(admin);
    }
    file.close();
    return adminsList;
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

// Function to Check Court Availability Based on Date
bool isCourtAvailable(int courtID, int day, int month, int year, const string& timeSlot)
{
    ifstream bookedFile("booked_courts.txt");
    if (!bookedFile.is_open())
    {
        cout << "Error: Unable to open booked_courts.txt" << endl;
        return false;
    }
    string line;
    while (getline(bookedFile, line))
    {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        int bookedID, bDay, bMonth, bYear;
        string bTimeSlot, name, contact;
        ss >> bookedID >> bDay >> bMonth >> bYear >> bTimeSlot;
        ss >> ws; // Consume any leading whitespace
        getline(ss, name, ' ');
        getline(ss, contact);
        if (bookedID == courtID && bDay == day && bMonth == month && bYear == year && bTimeSlot == timeSlot)
        {
            return false; // Court is already booked for this date and time slot
        }
    }
    return true; // Court is available
}

// Function to Input Customer Booking Details
void customerInput(int courtID, const string& name, const string& contact) 
{
    int day, month, year;
    string timeSlot;

    Receipt rp;  // Receipt class instance
    system("cls");
    loading();
    system("cls");
    cout << "==================================" << endl;
    cout << "User Details and Booking" << endl;
    cout << "==================================" << endl;
    // Name and contact are already provided, no need to input again

    bool validDate = false;
    cout << "Date of booking" << endl;
    cout << "==================================" << endl;
    do 
    {
        cout << "Enter the day: ";
        cin >> day;
        cout << "Enter the month: ";
        cin >> month;
        cout << "Enter the year: ";
        cin >> year;

        if (!isDateValid(day, month, year)) 
        {
            cout << "Invalid date! Please enter a future date." << endl;
        } 
        else 
        {
            validDate = true;
        }
    } while (!validDate);

    cout << "Enter Time Slot (e.g., 10:00-11:00): ";
    cin >> timeSlot;

    // Check court availability
    if (!isCourtAvailable(courtID, day, month, year, timeSlot))
    {
        cout << "\nSorry, the court is already booked for the selected date and time slot.\n";
        return;
    }

    cout << "Date entered: " << day << "/" << month << "/" << year << endl;
    cout << "Time Slot: " << timeSlot << endl;

    ifstream inputFile("courts.txt");
    if (inputFile.is_open()) 
    {
        string bookingDetails;
        bool found = false;

        string line;
        while (getline(inputFile, line)) 
        {
            stringstream ss(line);
            int currentCourtID;
            string type;
            double price;

            ss >> currentCourtID >> type >> price;

            // Check if the court ID matches
            if (currentCourtID == courtID) 
            {
                found = true;
                bookingDetails = to_string(currentCourtID) + " " + type + " " + to_string(price);
                break;
            }
        }

        inputFile.close();

        if (!found) 
        {
            cout << "\nCourt with ID " << courtID << " was not found." << endl;
            cout << "Failed to generate receipt" << endl;
            return;
        }

        // Save the booking to "booked_courts.txt" with user details
        ofstream bookedFile("booked_courts.txt", ios::app);
        if (bookedFile.is_open())
        {
            bookedFile << courtID << " " << day << " " << month << " " << year << " " << timeSlot << " " << name << " " << contact << endl;
            bookedFile.close();
        }
        else
        {
            cout << "Failed to open the \"booked_courts.txt\" file." << endl;
            return;
        }

        // Generate and display receipt automatically from the last booking
        rp.generateReceiptFromLastBooking();
        rp.displayReceipt();
        cout << "Receipt generated successfully." << endl;
    } 
    else 
    {
        cout << "Failed to open the courts file." << endl;
    }
}

// Function to View Booking History
void viewBookingHistory(const string& username, bool isAdmin)
{
    ifstream bookedFile("booked_courts.txt");
    if (!bookedFile.is_open())
    {
        cout << "Failed to open the \"booked_courts.txt\" file." << endl;
        return;
    }

    cout << "\n==================================\n";
    cout << "\tBooking History\n";
    cout << "==================================\n";

    string courtID, day, month, year, timeSlot, name, contact;

    // Display table header
    cout << left << setw(10) << "CourtID" 
         << setw(10) << "Day" 
         << setw(10) << "Month" 
         << setw(10) << "Year" 
         << setw(20) << "Time Slot" 
         << setw(20) << "Name" 
         << setw(15) << "Contact" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;

    // Read and display each booking
    while (bookedFile >> courtID >> day >> month >> year >> timeSlot)
    {
        // Since name and contact may contain spaces, use getline
        bookedFile >> ws; // Consume any leading whitespace
        getline(bookedFile, name, ' ');
        getline(bookedFile, contact);

        if (!isAdmin && name != username)
            continue; // Only show bookings for the logged-in user

        cout << left << setw(10) << courtID 
             << setw(10) << day 
             << setw(10) << month 
             << setw(10) << year 
             << setw(20) << timeSlot 
             << setw(20) << name 
             << setw(15) << contact << endl;
    }

    bookedFile.close();
}

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
    do 
    {
        cout << "Enter the day: ";
        cin >> day;
        cout << "Enter the month: ";
        cin >> month;
        cout << "Enter the year: ";
        cin >> year;

        if (!isDateValid(day, month, year)) 
        {
            cout << "Invalid date! Please enter a future date." << endl;
        } 
        else 
        {
            break;
        }
    } while (true);

    // Prompt for time slot
    cout << "Enter the Time Slot to check (e.g., 10:00-11:00): ";
    cin >> timeSlot;

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
        cout << "----------------------------------------------" << endl;
        cout << left << "| " << setw(10) << "Court ID" 
             << "| " << setw(15) << "Type" 
             << "| " << setw(15) << "Price/Hour" << " |" << endl;
        cout << "----------------------------------------------" << endl;
        for (const auto& court : availableCourts) 
        {
            displayCourt(court);
        }
    }
}

// Function to Search Courts by Type
void searchByType(vector<Court>& courts, const string& type) 
{
    vector<Court> filteredCourts;
    for (const auto& court : courts) 
    {
        if (court.type == type) 
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
        if (court.pricePerHour == price) 
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

// Selection Sort Implementation for Sorting Courts by ID
void selectionSortByID(vector<Court>& courts) 
{
    int n = courts.size();
    for (int i = 0; i < n - 1; i++) 
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) 
        {
            if (courts[j].courtID < courts[minIndex].courtID)
            {
                minIndex = j;
            }
        }
        if (minIndex != i) 
        {
            swap(courts[i], courts[minIndex]);
        }
    }
    cout << "\nCourts sorted by ID using Selection Sort successfully.\n";
}

// Overloaded Function to Sort Courts by Price Using Selection Sort
void sortCourtsByPriceSelection(vector<Court>& courts, bool byPrice) 
{
    if (!byPrice) return; // Simple overload example

    int n = courts.size();
    for (int i = 0; i < n - 1; i++) 
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) 
        {
            if (courts[j].pricePerHour < courts[minIndex].pricePerHour)
            {
                minIndex = j;
            }
        }
        if (minIndex != i) 
        {
            swap(courts[i], courts[minIndex]);
        }
    }
    cout << "\nCourts sorted by Price using Selection Sort successfully.\n";
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

    cout << "\nCourts sorted by Price using Heapsort successfully.\n";
}

// Insertion Sort Implementation for Sorting Courts by Type
void insertionSortByType(vector<Court>& courts) {
    int n = courts.size();
    for (int i = 1; i < n; ++i) {
        Court key = courts[i];
        int j = i - 1;

        while (j >= 0 && courts[j].type > key.type) {
            courts[j + 1] = courts[j];
            j = j - 1;
        }
        courts[j + 1] = key;
    }

    cout << "\nCourts sorted by Type using Insertion Sort successfully.\n";
}

// Additional Function: Implementing a Hash Table for Quick Search (Using unordered_map)
unordered_map<int, Court> createCourtHashMap(const vector<Court>& courts) {
    unordered_map<int, Court> courtMap;
    for (const auto& court : courts) {
        courtMap[court.courtID] = court;
    }
    return courtMap;
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

    quickSortCourtsByID(courts, 0, courts.size() - 1);
    cout << "\nCourts sorted by ID using Quicksort successfully.\n";
}

// Partition function for QuickSort
int partition(vector<Court>& courts, int low, int high) 
{
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
}

// Recursive QuickSort function
void quickSortCourtsByID(vector<Court>& courts, int low, int high) 
{
    if (low < high) 
    {
        // pi is partitioning index, courts[pi] is now at right place
        int pi = partition(courts, low, high);

        // Separately sort elements before and after partition
        quickSortCourtsByID(courts, low, pi - 1);
        quickSortCourtsByID(courts, pi + 1, high);
    }
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
    cout << "\n4. Sort courts by ID (Selection Sort) [Overloaded]";
    cout << "\n5. Sort courts by Price (Selection Sort) [Overloaded]";
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
        case 4:
            system("cls");
            loading();
            system("cls");
            selectionSortByID(courts);
            break;
        case 5:
            system("cls");
            loading();
            system("cls");
            sortCourtsByPriceSelection(courts, true);
            break;
        case 0:
            return; // Back to Admin Homepage
        default:
            cout << "Invalid choice" << endl;
            break; // Continue the loop without recursion
    }

    // Update the sorted courts back to file
    ofstream outfile("courts.txt");
    if (!outfile.is_open()) 
    {
        cout << "Failed to open the courts file for writing." << endl;
        return;
    }
    for (const auto& court : courts) 
    {
        outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
    }
    outfile.close();

    cout << "Courts have been sorted and updated successfully.\n";
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

// Admin Homepage with Enhanced Features
void AdminHomepage()
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);

    int choice;
    while (true) 
    {
        cout << "\n****************************************\n";
        cout << "\tAdmin Panel\n";
        cout << "****************************************\n";
        cout << "\n1. Display Courts";
        cout << "\n2. Add New Courts";
        cout << "\n3. Delete Courts";
        cout << "\n4. Edit Courts";
        cout << "\n5. Sort Courts";
        cout << "\n6. Search Courts";
        cout << "\n7. View Booking History";        // New Option
        cout << "\n8. View Court Availability";     // New Option
        cout << "\n9. Register New Admin";          // New Option
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
                break;
            case 2:
                system("cls");
                loading();
                system("cls");
                addCourt(courts, filename);
                courts = readCourtsFromFile(filename); // Update the vector after adding new courts
                break;
            case 3:
                system("cls");
                loading();
                system("cls");
                {
                    int courtID;
                    cout << "Enter Court ID to delete: ";
                    cin >> courtID;
                    deleteCourtFriend(courts, courtID, filename);
                }
                break;
            case 4:
                system("cls");
                loading();
                system("cls");
                editCourt(courts, filename);
                break;
            case 5:
                system("cls");
                loading();
                system("cls");
                sortingMenu();
                courts = readCourtsFromFile(filename); // Update after sorting
                break;
            case 6:
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
                    cout << "Enter court type to search (Singles/Doubles): ";
                    cin >> type;
                    searchByType(courts, type);
                }
                else if (searchChoice == 2)
                {
                    double price;
                    cout << "Enter price per hour to search: RM ";
                    cin >> price;
                    searchByPrice(courts, price);
                }
                else if (searchChoice == 0)
                {
                    break;
                }
                else
                {
                    cout << "Invalid choice." << endl;
                }
                break;
            }
            case 7:
                system("cls");
                loading();
                system("cls");
                viewBookingHistory("admin", true);           // Invoke Booking History for Admin
                break;
            case 8:
                system("cls");
                loading();
                system("cls");
                viewCourtAvailability();        // Invoke Court Availability
                break;
            case 9:
                system("cls");
                loading();
                system("cls");
                adminRegister();                // Register a new admin
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                AdminMenu();
                return;
            default:
                cout << "\nInvalid choice. Please try again.\n";
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
    while (true) 
    {
        cout << "\n****************************************\n";
        cout << "Welcome to Badminton Court Booking System\n";
        cout << "****************************************\n";
        cout << "\n1. Display Courts";
        cout << "\n2. Book Court";
        cout << "\n3. View Booking History";
        cout << "\n4. View Court Availability";
        cout << "\n5. Edit Booking";
        cout << "\n6. Delete Booking";
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
                viewCourtAvailability();
                break;
            case 5:
                system("cls");
                loading();
                system("cls");
                {
                    // Implement edit booking functionality using a stack
                    stack<Booking> bookingStack;
                    // Load bookings into stack
                    ifstream bookedFile("booked_courts.txt");
                    string line;
                    while (getline(bookedFile, line)) 
                    {
                        if (line.empty()) continue;
                        stringstream ss(line);
                        Booking booking;
                        ss >> booking.courtID >> booking.day >> booking.month >> booking.year >> booking.timeSlot;
                        ss >> ws;
                        getline(ss, booking.name, ' ');
                        getline(ss, booking.contact);
                        if (booking.name == name) 
                        {
                            bookingStack.push(booking);
                        }
                    }
                    bookedFile.close();

                    if (bookingStack.empty()) 
                    {
                        cout << "No bookings found to edit." << endl;
                        break;
                    }

                    Booking latestBooking = bookingStack.top();
                    bookingStack.pop();

                    cout << "Editing your latest booking:\n";
                    cout << "Court ID: " << latestBooking.courtID << endl;
                    cout << "Date: " << latestBooking.day << "/" << latestBooking.month << "/" << latestBooking.year << endl;
                    cout << "Time Slot: " << latestBooking.timeSlot << endl;

                    // Allow user to edit date and time slot
                    cout << "Enter new day (current: " << latestBooking.day << "): ";
                    cin >> latestBooking.day;
                    cout << "Enter new month (current: " << latestBooking.month << "): ";
                    cin >> latestBooking.month;
                    cout << "Enter new year (current: " << latestBooking.year << "): ";
                    cin >> latestBooking.year;
                    cout << "Enter new Time Slot (current: " << latestBooking.timeSlot << "): ";
                    cin >> latestBooking.timeSlot;

                    // Validate new date
                    if (!isDateValid(latestBooking.day, latestBooking.month, latestBooking.year)) 
                    {
                        cout << "Invalid date! Edit cancelled." << endl;
                        break;
                    }

                    // Check court availability
                    if (!isCourtAvailable(latestBooking.courtID, latestBooking.day, latestBooking.month, latestBooking.year, latestBooking.timeSlot))
                    {
                        cout << "\nSorry, the court is already booked for the selected date and time slot.\n";
                        break;
                    }

                    // Update booking in file
                    vector<Booking> allBookings;
                    ifstream bookedFileIn("booked_courts.txt");
                    while (getline(bookedFileIn, line)) 
                    {
                        if (line.empty()) continue;
                        stringstream ss(line);
                        Booking booking;
                        ss >> booking.courtID >> booking.day >> booking.month >> booking.year >> booking.timeSlot;
                        ss >> ws;
                        getline(ss, booking.name, ' ');
                        getline(ss, booking.contact);
                        if (booking.name == name && booking.courtID == latestBooking.courtID && booking.day == latestBooking.day && booking.month == latestBooking.month && booking.year == latestBooking.year && booking.timeSlot == latestBooking.timeSlot)
                        {
                            allBookings.push_back(latestBooking); // Update with edited booking
                        }
                        else
                        {
                            allBookings.push_back(booking);
                        }
                    }
                    bookedFileIn.close();

                    // Write updated bookings back to file
                    ofstream bookedFileOut("booked_courts.txt", ios::trunc);
                    for (const auto& booking : allBookings) 
                    {
                        bookedFileOut << booking.courtID << " " << booking.day << " " << booking.month << " " << booking.year << " " << booking.timeSlot << " " << booking.name << " " << booking.contact << endl;
                    }
                    bookedFileOut.close();

                    cout << "Booking updated successfully." << endl;
                }
                break;
            case 6:
                system("cls");
                loading();
                system("cls");
                {
                    // Implement delete booking functionality using a queue
                    queue<Booking> bookingQueue;
                    // Load bookings into queue
                    ifstream bookedFile("booked_courts.txt");
                    string line;
                    while (getline(bookedFile, line)) 
                    {
                        if (line.empty()) continue;
                        stringstream ss(line);
                        Booking booking;
                        ss >> booking.courtID >> booking.day >> booking.month >> booking.year >> booking.timeSlot;
                        ss >> ws;
                        getline(ss, booking.name, ' ');
                        getline(ss, booking.contact);
                        if (booking.name == name) 
                        {
                            bookingQueue.push(booking);
                        }
                    }
                    bookedFile.close();

                    if (bookingQueue.empty()) 
                    {
                        cout << "No bookings found to delete." << endl;
                        break;
                    }

                    Booking bookingToDelete = bookingQueue.front();
                    bookingQueue.pop();

                    cout << "Deleting your booking:\n";
                    cout << "Court ID: " << bookingToDelete.courtID << endl;
                    cout << "Date: " << bookingToDelete.day << "/" << bookingToDelete.month << "/" << bookingToDelete.year << endl;
                    cout << "Time Slot: " << bookingToDelete.timeSlot << endl;

                    cout << "Are you sure you want to delete this booking? [Y/N]: ";
                    char confirm;
                    cin >> confirm;
                    if (confirm == 'Y' || confirm == 'y') 
                    {
                        // Remove booking from file
                        vector<Booking> allBookings;
                        ifstream bookedFileIn("booked_courts.txt");
                        while (getline(bookedFileIn, line)) 
                        {
                            if (line.empty()) continue;
                            stringstream ss(line);
                            Booking booking;
                            ss >> booking.courtID >> booking.day >> booking.month >> booking.year >> booking.timeSlot;
                            ss >> ws;
                            getline(ss, booking.name, ' ');
                            getline(ss, booking.contact);
                            if (!(booking.name == bookingToDelete.name && booking.courtID == bookingToDelete.courtID && booking.day == bookingToDelete.day && booking.month == bookingToDelete.month && booking.year == bookingToDelete.year && booking.timeSlot == bookingToDelete.timeSlot))
                            {
                                allBookings.push_back(booking);
                            }
                        }
                        bookedFileIn.close();

                        // Write updated bookings back to file
                        ofstream bookedFileOut("booked_courts.txt", ios::trunc);
                        for (const auto& booking : allBookings) 
                        {
                            bookedFileOut << booking.courtID << " " << booking.day << " " << booking.month << " " << booking.year << " " << booking.timeSlot << " " << booking.name << " " << booking.contact << endl;
                        }
                        bookedFileOut.close();

                        cout << "Booking deleted successfully." << endl;
                    }
                    else 
                    {
                        cout << "Deletion cancelled." << endl;
                    }
                }
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                UserMenu();
                return;
            default:
                cout << "\nInvalid choice. Please try again.\n";
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
        cout << "\tWelcome to Badminton Court Booking\n";
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
                MainPage();
                return;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
                break; // Continue the loop without recursion
        }
    } while (choice != 0);
}

// Admin Menu
void AdminMenu() 
{
    AdminClass adminClass; // Instantiate AdminClass
    int choice;
    do {
        cout << "\n****************************************\n";
        cout << "\tWelcome to Badminton Court Booking\n";
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
                    if (adminClass.loginAdmin(adminsList)) {
                        system("cls");
                        loading();
                        system("cls");
                        AdminHomepage();
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
                MainPage();
                return;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
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
        cout << "\tWelcome to Badminton Court Booking\n";
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
                break; // Continue the loop without recursion
        }
    } while (choice != 0);
}

// Function to Define bookingMenu
void bookingMenu(const string& username, const string& name, const string& phoneNumber) {
    // Display all courts
    vector<Court> courts = readCourtsFromFile("courts.txt");
    displayCourts(courts);

    // Ask user to choose a court by ID
    int courtID;
    cout << "Enter the Court ID you want to book: ";
    cin >> courtID;

    // Validate courtID exists
    bool courtExists = false;
    for (const auto& court : courts) {
        if (court.courtID == courtID) {
            courtExists = true;
            break;
        }
    }

    if (!courtExists) {
        cout << "Invalid Court ID. Returning to menu.\n";
        return;
    }

    // Proceed to input booking details
    customerInput(courtID, name, phoneNumber);
}

// Main Function
int main() 
{
    MainPage();
    return 0;
}
