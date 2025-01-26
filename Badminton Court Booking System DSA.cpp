#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <queue>
#include <windows.h>      // For Sleep()
#include <limits>         // For numeric_limits
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function Declarations
void MainPage();
void UserHomepage();
void AdminHomepage();
void bookingMenu();
void viewBookingHistory();
void viewCourtAvailability(); 
void bookingMenuAdmin();
void customerInput(int courtID);
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
};

// Admin Structure
struct Admin 
{
    string username;
    string password;
};

// Function to check if a file exists
bool fileExists(const string& filename)
{
    ifstream infile(filename);
    return infile.good();
}

// Receipt Class
class Receipt 
{
public:
    void generateGrandReceipt(const string& bookingDetails, string name, string contact, int day, int month, int year, string timeSlot) 
    {
        ofstream outputFile("grand_receipt.txt", ios::app);
        if (outputFile.is_open()) 
        {
            system("cls");
            loading();
            system("cls");
            outputFile << "\n>>>> GRAND RECEIPT <<<<" << endl;
            outputFile << "User Name: " << name << endl;
            outputFile << "User Contact: " << contact << endl;
            outputFile << "Date of Booking: " << day << "/" << month << "/" << year << endl;
            outputFile << "Time Slot: " << timeSlot << endl;
            outputFile << "============================================================================================================" << endl;
            outputFile << "\t\t\tCourt Details" << endl;
            outputFile << "============================================================================================================" << endl;
            outputFile << left << setw(15) << "Court ID" << setw(15) << "Type" << setw(15) << "Price/Hour" << endl;
            outputFile << "---------------------------------------------" << endl;

            // Parse bookingDetails and format as table
            istringstream iss(bookingDetails);
            string courtID, type, pricePerHour;
            while (iss >> courtID >> type >> pricePerHour) 
            {
                outputFile << left << " |" << setw(15) << courtID << setw(15) << type << setw(15) << pricePerHour << endl;
            }

            outputFile << endl;
            outputFile.close();
        } 
        else 
        {
            cout << "Failed to open the file for saving." << endl;
        }
    }

    void generateReceipt(const string& bookingDetails, string name, string contact, int day, int month, int year, string timeSlot) 
    {
        ofstream outputFile("receipt.txt");
        if (outputFile.is_open()) 
        {
            outputFile << "\n>>>> RECEIPT <<<<" << endl;
            outputFile << "Customer Name: " << name << endl;
            outputFile << "Customer Contact: " << contact << endl;
            outputFile << "Date of Booking: " << day << "/" << month << "/" << year << endl;
            outputFile << "Time Slot: " << timeSlot << endl;
            outputFile << "===========================================================================" << endl;
            outputFile << "\t\t\tCourt Details" << endl;
            outputFile << "===========================================================================" << endl;
            outputFile << left << setw(15) << "Court ID" << setw(15) << "Type" << setw(15) << "Price/Hour" << endl;
            outputFile << "---------------------------------------------------------------------------" << endl;

            // Parse bookingDetails and format as table
            istringstream iss(bookingDetails);
            string courtID, type, pricePerHour;
            while (iss >> courtID >> type >> pricePerHour) 
            {
                outputFile << left << " |" << setw(15) << courtID << setw(15) << type << setw(15) << pricePerHour << endl;
            }

            outputFile << endl;
            outputFile.close();
        } 
        else 
        {
            cout << "Failed to open the file for saving." << endl;
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

// Court Class
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

    // Friend function to display court details
    friend void displayCourt(const Court& court);
};

// Comparator Function for Sorting Courts by courtID
bool compareCourtsByID(const Court& a, const Court& b) 
{
    return a.courtID < b.courtID;
}

// Comparator Function for Sorting Courts by Type
bool compareCourtsByType(const Court& a, const Court& b) 
{
    return a.type < b.type;
}

// Comparator Function for Sorting Courts by Price
bool compareCourtsByPrice(const Court& a, const Court& b) 
{
    return a.pricePerHour < b.pricePerHour;
}

// Function to Sort Courts Using a Custom Comparator
void sortCourts(std::vector<Court>& courts, bool (*compare)(const Court&, const Court&)) 
{
    std::sort(courts.begin(), courts.end(), compare);
}

// Function to Display a Court's Details
void displayCourt(const Court& court) 
{
    cout << left << "| " << setw(10) << court.courtID 
         << "| " << setw(15) << court.type 
         << "| " << setw(15) << fixed << setprecision(2) << court.pricePerHour 
         << " |" << endl;
    cout << "----------------------------------------------" << endl;
}

// Function to Read Courts from a Text File
vector<Court> readCourtsFromFile(const string& filename) 
{
    vector<Court> courts;
    ifstream infile(filename.c_str());
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

// Function to Write a Court to a Text File
void writeCourtToFile(const string& filename, const Court& court) 
{
    ofstream outfile(filename.c_str(), ios::app);
    outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
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

// Function to Input Courts into the System
void inputCourts(const string& filename) 
{
    vector<Court> courts = readCourtsFromFile(filename); // Read the existing courts
    int count;
    cout << "\n****************************************\n";
    cout << "\tAdmin - Add New Courts\n";
    cout << "****************************************\n";
    cout << "Enter the number of courts to add: ";
    cin >> count;

    for (int i = 0; i < count; i++) 
    {
        int id;
        string type;
        double price;

        // Validate court ID
        while (true) 
        {
            cout << "Enter details for court " << (i + 1) << ":" << endl;
            cout << "\nCourt ID: ";
            cin >> id;

            if(cin.fail()) 
            {
                cin.clear(); // clear the error flag
                cout << "Invalid input. Please enter a valid integer for Court ID.\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore invalid input
                continue;
            }

            if (validateCourtID(courts, id)) 
            {
                break;
            } 
            else 
            {
                cout << "Court ID already exists. Please enter a unique Court ID.\n";
            }
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
        writeCourtToFile(filename, newCourt);
    }
}

// Function to Delete a Court
void deleteCourt(vector<Court>& courts, int courtID) 
{
    bool found = false;
    for (auto it = courts.begin(); it != courts.end(); ++it) 
    {
        if (it->getCourtID() == courtID) 
        {
            courts.erase(it); 

            // Update the courts file
            ofstream outfile("courts.txt");
            for (const auto& court : courts) 
            {
                outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
            }
            outfile.close();
            
            found = true;
            cout << "\nCourt with ID " << courtID << " has been deleted.\n";
            break;
        }
    }
    if (!found) 
    {
        cout << "\nCourt with ID " << courtID << " not found.\n";
    }
}

// Menu to Delete a Court
void deleteCourtMenu(vector<Court>& courts) 
{
    int courtID;
    displayCourts(courts);
    cout << "Enter Court ID of the court to delete: ";
    cin >> courtID;

    deleteCourt(courts, courtID); 
}

// Quicksort Implementation for Sorting Courts by ID
int partitionByID(vector<Court>& courts, int low, int high) {
    int pivot = courts[high].courtID;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (courts[j].courtID < pivot) {
            i++;
            swap(courts[i], courts[j]);
        }
    }
    swap(courts[i + 1], courts[high]);
    return i + 1;
}

void quickSortByID(vector<Court>& courts, int low, int high) {
    if (low < high) {
        int pi = partitionByID(courts, low, high);
        quickSortByID(courts, low, pi - 1);
        quickSortByID(courts, pi + 1, high);
    }
}

void sortCourtsByIDQuickSort(vector<Court>& courts) {
    if (!courts.empty()) {
        quickSortByID(courts, 0, courts.size() - 1);
        cout << "\nCourts sorted by ID successfully.\n";
    }
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

    cout << "\nCourts sorted by Price successfully.\n";
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

    cout << "\nCourts sorted by Type successfully.\n";
}

// Updated Sorting Menu for Admin
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
            sortCourtsByIDQuickSort(courts);
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
            MainPage();
            return; // Return instead of recursive call
        default:
            cout << "Invalid choice" << endl;
            sortingMenu(); // Allow retry for invalid choice
            return;
    }

    // Update the sorted courts back to file
    ofstream outfile("courts.txt");
    for (const auto& court : courts) 
    {
        outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
    }
    outfile.close();

    cout << "Courts have been sorted and updated successfully.\n";

    cout << "Enter Court ID to choose: ";
    int courtID;
    cin >> courtID;

    cout << "Do you want to book court ID[" << courtID << "]? [Y/N]: ";
    char c;
    cin >> c;

    if (c == 'Y' || c == 'y') 
    {
        customerInput(courtID);  // Pass courtID to customerInput function
    } 
    else 
    {
        sortingMenu();
    }
}

// Meta Binary Search on Courts by courtID
int metaBinarySearch(const vector<Court>& courts, int courtID) 
{
    int left = 0;
    int right = courts.size() - 1;
    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (courts[mid].courtID == courtID)
            return mid;
        if (courts[mid].courtID < courtID)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
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

// Function to Sort Courts by Price Using Selection Sort (Overloaded)
void sortCourtsByPriceSelection(vector<Court>& courts) 
{
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
}

// Overloaded Function to Sort Courts by ID Using Selection Sort
void sortCourtsByIDSelection(vector<Court>& courts, bool byID) 
{
    if (!byID) return; // Simple overload example

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
}

// Function to Sort Courts by Type Using Selection Sort (Overloaded)
void sortCourtsByTypeSelection(vector<Court>& courts, string type) 
{
    // Overloaded function example: additional parameter not used
    // Sorting by type is already handled by sortCourts function
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

// Function to Sort Courts and Update File (Overloaded)
void sortCourtsAndUpdateFile(vector<Court>& courts, bool (*compare)(const Court&, const Court&), const string& filename) 
{
    sort(courts.begin(), courts.end(), compare);
    ofstream outfile(filename);
    for (const auto& court : courts) 
    {
        outfile << court.courtID << " " << court.type << " " << court.pricePerHour << endl;
    }
    outfile.close();
    cout << "\nCourts sorted and file updated successfully.\n";
}

// Function to Initialize Files if Not Exist
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
        // Add some default courts
        courtsFile << "1 Singles 50.00" << endl;
        courtsFile << "2 Doubles 70.00" << endl;
        courtsFile << "3 Singles 50.00" << endl;
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
}

// Queue for Users
queue<User> users;
const string filenameUsers = "users.txt";

// Load Users from File
void LoadUsers() 
{
    ifstream file(filenameUsers.c_str());  // Use c_str() to convert string to const char*
    if (file.is_open()) 
    {
        User user;
        while (file >> user.username >> user.password) 
        {
            users.push(user);
        }
        file.close();
    }
}

// Save User to File
void SaveUser(const User& user) 
{
    ofstream file(filenameUsers.c_str(), ios::app);  // Use c_str() to convert string to const char*
    if (file.is_open()) 
    {
        file << user.username << " " << user.password << endl;
        file.close();
    }
}

// User Registration
void Register() 
{
    User newUser;
    cout << "\n****************************************\n";
    cout << "\tUser Register\n";
    cout << "****************************************\n";
    cout << "\nEnter username: ";
    cin >> newUser.username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    cout << "\nEnter password: ";
    cin >> newUser.password;

    // Save to file and queue
    users.push(newUser);
    SaveUser(newUser);

    cout << "\nRegistration successful!" << endl;
}

// User Login
void Login() 
{
    if (users.empty()) 
    {
        cout << "\nNo users registered. Please register first." << endl;
        return;
    }

    string username, password;
    cout << "\n****************************************\n";
    cout << "\tUser Login\n";
    cout << "****************************************\n";
    cout << "\nEnter username: ";
    cin >> username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    cout << "\nEnter password: ";
    cin >> password;

    queue<User> tempQueue = users;
    bool loggedIn = false;
    while (!tempQueue.empty()) 
    {
        User currentUser = tempQueue.front();
        tempQueue.pop();
        if (currentUser.username == username && currentUser.password == password) 
        {
            cout << "\nLogin successful!" << endl;
            loggedIn = true;
            system("cls");
            loading();
            system("cls");
            UserHomepage();
            break;
        }
    }

    if (!loggedIn) 
    {
        cout << "\nInvalid username or password." << endl;
    }
}

// User Menu
void UserMenu() 
{
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
                Register();
                break;
            case 2:
                system("cls");
                loading();
                system("cls");
                Login();
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                MainPage();
                break;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

// Queue for Admins
queue<Admin> admins;
const string adminsFilename = "admins.txt";  // Unique filename for admins

// Load Admins from File
void LoadAdmins() 
{
    ifstream file(adminsFilename.c_str());
    if (file.is_open()) 
    {
        Admin admin;
        while (file >> admin.username >> admin.password) 
        {
            admins.push(admin);
        }
        file.close();
    }
}

// Admin Login
void AdminLogin() 
{
    if (admins.empty()) 
    {
        cout << "\nNo admins registered." << endl;
        return;
    }

    string username, password;
    cout << "\n****************************************\n";
    cout << "\tAdmin Login\n";
    cout << "****************************************\n";
    cout << "\nEnter admin username: ";
    cin >> username;
    cout << "\nEnter admin password: ";
    cin >> password;

    queue<Admin> tempQueue = admins;
    bool loggedIn = false;
    while (!tempQueue.empty()) 
    {
        Admin currentAdmin = tempQueue.front();
        tempQueue.pop();
        if (currentAdmin.username == username && currentAdmin.password == password) 
        {
            cout << "\nAdmin login successful!" << endl;
            loggedIn = true;
            system("cls");
            loading();
            system("cls");
            AdminHomepage();
            break;
        }
    }

    if (!loggedIn) 
    {
        cout << "\nInvalid admin username or password." << endl;
    }
}

// Admin Menu
void AdminMenu() 
{
    int choice;
    do {
        cout << "\n****************************************\n";
        cout << "\tWelcome to Badminton Court Booking\n";
        cout << "****************************************\n";
        cout << "1. Login"; 
        cout << "\n0. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                system("cls");
                loading();
                system("cls");
                AdminLogin();
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                MainPage();
                break;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

// Admin Homepage
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
        cout << "\n4. Sort Courts";
        cout << "\n5. Search Courts";
        cout << "\n6. View Booking History";        // New Option
        cout << "\n7. View Court Availability";     // New Option
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
                inputCourts(filename);
                courts = readCourtsFromFile(filename); // Update the vector after adding new courts
                break;
            case 3:
                system("cls");
                loading();
                system("cls");
                deleteCourtMenu(courts);
                break;
            case 4:
                system("cls");
                loading();
                system("cls");
                sortingMenu();
                courts = readCourtsFromFile(filename); // Update after sorting
                break;
            case 5:
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
            case 6:
                system("cls");
                loading();
                system("cls");
                viewBookingHistory();           // Invoke Booking History
                break;
            case 7:
                system("cls");
                loading();
                system("cls");
                viewCourtAvailability();        // Invoke Court Availability
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                AdminMenu();
                return;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
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
    string line;
    while (getline(bookedFile, line))
    {
        int bookedID, bDay, bMonth, bYear;
        string bTimeSlot;
        stringstream ss(line);
        ss >> bookedID >> bDay >> bMonth >> bYear >> bTimeSlot;
        if (bookedID == courtID && bDay == day && bMonth == month && bYear == year && bTimeSlot == timeSlot)
        {
            return false; // Court is already booked for this date and time slot
        }
    }
    return true; // Court is available
}

// Function to Input Customer Booking Details
void customerInput(int courtID) 
{
    string name;
    string contact;
    int day, month, year;
    string timeSlot;

    Receipt rp;  // Receipt class instance
    system("cls");
    loading();
    system("cls");
    cout << "==================================" << endl;
    cout << "User Details and Booking" << endl;
    cout << "==================================" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your contact number: ";
    cin >> contact;
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

        // Generate and display receipt
        rp.generateGrandReceipt(bookingDetails, name, contact, day, month, year, timeSlot);
        rp.generateReceipt(bookingDetails, name, contact, day, month, year, timeSlot);
        rp.displayReceipt();
        cout << "Receipt generated successfully." << endl;
    } 
    else 
    {
        cout << "Failed to open the courts file." << endl;
    }
}

// Function to View Booking History
void viewBookingHistory()
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
        getline(bookedFile, name, ' ');
        getline(bookedFile, contact);

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
        displayCourts(availableCourts);
    }
}

// Function to Search Available Courts Based on Date and Time
vector<Court> getAvailableCourts(int day, int month, int year, const string& timeSlot)
{
    vector<Court> allCourts = readCourtsFromFile("courts.txt");
    vector<Court> availableCourts;

    for (const auto& court : allCourts)
    {
        if (isCourtAvailable(court.courtID, day, month, year, timeSlot))
        {
            availableCourts.push_back(court);
        }
    }

    return availableCourts;
}

// Booking Menu for Users
void bookingMenu() 
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);
    char c;
    string courtType;
    int courtID;
    bool courtFound = false;
    bool validCourtType = false;

    // Display court type options
    cout << "-------------------------------" << endl;
    cout << "1. Singles" << endl;
    cout << "2. Doubles" << endl;
    cout << "-------------------------------" << endl;

    // Prompt for court type with validation
    while (!validCourtType) 
    {
        cout << "Enter Court Type to search [Singles or Doubles]: ";
        cin >> courtType;

        if (courtType == "Singles" || courtType == "Doubles") 
        {
            validCourtType = true;
        } 
        else 
        {
            cout << "\nInvalid court type. Please enter either 'Singles' or 'Doubles'.\n";
        }
    }

    // Prompt for booking date and time
    int day, month, year;
    string timeSlot;
    cout << "\nEnter booking date:" << endl;
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

    cout << "Enter Time Slot (e.g., 10:00-11:00): ";
    cin >> timeSlot;

    // Get available courts based on type, date, and time
    vector<Court> availableCourts;
    for (const auto& court : courts)
    {
        if (court.type == courtType && isCourtAvailable(court.courtID, day, month, year, timeSlot))
        {
            availableCourts.push_back(court);
        }
    }

    if (availableCourts.empty()) 
    {
        cout << "\nNo available courts found for the selected type, date, and time slot.\n";
        return;
    } 
    else 
    {
        // Display available courts
        cout << "\nAvailable Courts:\n";
        cout << "----------------------------------------------" << endl;
        cout << left << "| " << setw(10) << "Court ID" 
             << "| " << setw(15) << "Type" 
             << "| " << setw(15) << "Price/Hour" << " |" << endl;
        cout << "----------------------------------------------" << endl;
        for (const auto& court : availableCourts) 
        {
            displayCourt(court);
        }

        // Prompt for court ID
        while (true) 
        {
            cout << "Enter Court ID to choose: ";
            cin >> courtID;

            // Search for the court in availableCourts
            auto it = find_if(availableCourts.begin(), availableCourts.end(), [&](const Court& c) {
                return c.courtID == courtID;
            });

            if (it != availableCourts.end()) 
            {
                cout << "\nCourt found:\n";
                displayCourt(*it);
                courtFound = true;
                break; 
            } 
            else
            {
                cout << "\nCourt with ID " << courtID << " not found or unavailable. Please try again.\n";
            }
        }
    }

    if (!courtFound) 
    {
        cout << "\nCourt with ID " << courtID << " not found.\n";
    } 
    else 
    {
        cout << "Do you want to book court ID[" << courtID << "]? [Y/N]: ";
        cin >> c;

        if (c == 'Y' || c == 'y') 
        {
            customerInput(courtID);  // Pass courtID to customerInput function
        } 
        else 
        {
            bookingMenu();
        }
    }
}

// Main Booking Menu for Admin
void bookingMenuAdmin() // Renamed to prevent conflict
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);

    int choice;
    while (true) 
    {
        cout << "\n****************************************\n";
        cout << "Welcome to Badminton Court Booking System\n";
        cout << "****************************************\n";
        cout << "\n1. Search Court by Type";
        cout << "\n2. Sort Courts";
        cout << "\n0. Back to Menu";
        cout << "\n\nEnter your choice: ";
        cin >> choice;
        
        switch (choice) 
        {
            case 1: 
            { 
                system("cls");
                loading();
                system("cls");
                // Implement search functionality
                cout << "Search functionality not implemented here.\n";
                break;
            }
            case 2: 
            {
                system("cls");
                loading();
                system("cls");
                sortingMenu(); 
                break;
            }
            case 0:
            {
                system("cls");
                loading();
                system("cls");
                UserHomepage();
                break;
            }
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }   
    }
}

// User Homepage
void UserHomepage()
{
    string filename = "courts.txt";
    vector<Court> courts = readCourtsFromFile(filename);
    // vector<string> bookingHistoryDetails; // Placeholder for booking history

    int choice;
    while (true) 
    {
        cout << "\n****************************************\n";
        cout << "Welcome to Badminton Court Booking System\n";
        cout << "****************************************\n";
        cout << "\n1. Display Courts";
        cout << "\n2. Book Court";
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
                bookingMenu();
                break;
            case 0:
                system("cls");
                loading();
                system("cls");
                UserMenu();
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
}

// Main Page Function
void MainPage()
{
    initializeFiles(); // Ensure all necessary files are initialized
    LoadUsers();
    LoadAdmins();
    
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
        }
    } while (choice != 0);
}


// Main Function
int main() 
{
    MainPage();
    return 0;
}
