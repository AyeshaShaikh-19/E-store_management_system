#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

// Struct Definitions
struct Stock {
    int Icode;
    string Iname;
    int Iprice;
    int Iqtt;
    char Icat;
};

struct Employee {
    int Emp_ID;
    string Emp_Name;
    string Emp_Pass;
};

struct Customer {
    string User_Name;
    string Password;
    int Balance;
};

// Global Variables
vector<Stock> stockItems;
vector<Employee> employees;
vector<Customer> customers;

// Function Declarations
void loadStock();
void loadEmployees();
void loadCustomers();
void saveStock();
void saveCustomers();

void printStock();
void printEmployees();
void printCustomers();

string employeeLogin();
void customerLogin();
void customerSignup();

void addStock();
void updateStock();
void buyItem();
void viewBalance(const string &username);
void depositMoney(const string &username);
void withdrawMoney(const string &username);

// Helper Functions
bool isValidCustomer(const string &username, const string &password, Customer &customer);
void updateCustomerBalance(const string &username, int newBalance);
void updateStockQuantity(int code, int newQuantity);
void clearScreen() { system("cls"); }

// Main Function
int main() {
    loadStock();
    loadEmployees();
    loadCustomers();

    while (true) {
        clearScreen();
        cout << "------------------------------------\n";
        cout << "Welcome to E-Store Management System\n";
        cout << "------------------------------------\n";
        cout << "1. Employee Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Customer Signup\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting the program. Goodbye!\n";
            break;
        } else if (choice == 1) {
            string empName = employeeLogin();
            if (!empName.empty()) {
                cout << "Welcome, " << empName << "!\n";
                cout << "1. View Stock\n";
                cout << "2. View Customers\n";
                cout << "Enter your choice: ";

                int empChoice;
                cin >> empChoice;

                if (empChoice == 1) {
                    printStock();
                    cout << "1. Add Stock\n";
                    cout << "2. Update Stock\n";
                    cout << "Enter your choice: ";
                    int stockChoice;
                    cin >> stockChoice;

                    if (stockChoice == 1) {
                        addStock();
                    } else if (stockChoice == 2) {
                        updateStock();
                    }
                } else if (empChoice == 2) {
                    printCustomers();
                }
            }
        } else if (choice == 2) {
            customerLogin();
        } else if (choice == 3) {
            customerSignup();
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}

// Load Functions
void loadStock() {
    ifstream file("Stock.csv");
    string line;
    stockItems.clear();

    while (getline(file, line)) {
        Stock item;
        stringstream ss(line);
        string temp;
        getline(ss, temp, ',');
        item.Icode = stoi(temp);
        getline(ss, item.Iname, ',');
        getline(ss, temp, ',');
        item.Iprice = stoi(temp);
        getline(ss, temp, ',');
        item.Iqtt = stoi(temp);
        getline(ss, temp, ',');
        item.Icat = temp[0];

        stockItems.push_back(item);
    }

    file.close();
}

void loadEmployees() {
    ifstream file("Employee.csv");
    string line;
    employees.clear();

    while (getline(file, line)) {
        Employee emp;
        stringstream ss(line);
        string temp;
        getline(ss, temp, ',');
        emp.Emp_ID = stoi(temp);
        getline(ss, emp.Emp_Name, ',');
        getline(ss, emp.Emp_Pass, ',');

        employees.push_back(emp);
    }

    file.close();
}

void loadCustomers() {
    ifstream file("Customer.csv");
    string line;
    customers.clear();

    while (getline(file, line)) {
        Customer cust;
        stringstream ss(line);
        string temp;
        getline(ss, cust.User_Name, ',');
        getline(ss, cust.Password, ',');
        getline(ss, temp, ',');
        cust.Balance = stoi(temp);

        customers.push_back(cust);
    }

    file.close();
}

// Save Functions
void saveStock() {
    ofstream file("Stock.csv");

    for (const auto &item : stockItems) {
        file << item.Icode << "," << item.Iname << "," << item.Iprice << "," << item.Iqtt << "," << item.Icat << "\n";
    }

    file.close();
}

void saveCustomers() {
    ofstream file("Customer.csv");

    for (const auto &cust : customers) {
        file << cust.User_Name << "," << cust.Password << "," << cust.Balance << "\n";
    }

    file.close();
}

// Print Functions
void printStock() {
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(10) << "Category" << "\n";
    for (const auto &item : stockItems) {
        cout << setw(10) << item.Icode << setw(20) << item.Iname << setw(10) << item.Iprice << setw(10) << item.Iqtt << setw(10) << item.Icat << "\n";
    }
}

void printEmployees() {
    cout << setw(10) << "ID" << setw(20) << "Name" << "\n";
    for (const auto &emp : employees) {
        cout << setw(10) << emp.Emp_ID << setw(20) << emp.Emp_Name << "\n";
    }
}

void printCustomers() {
    cout << setw(20) << "Username" << setw(20) << "Balance" << "\n";
    for (const auto &cust : customers) {
        cout << setw(20) << cust.User_Name << setw(20) << cust.Balance << "\n";
    }
}

// Functionalities
string employeeLogin() {
    int empID;
    string password;

    cout << "Enter Employee ID: ";
    cin >> empID;

    for (const auto &emp : employees) {
        if (emp.Emp_ID == empID) {
            cout << "Enter Password: ";
            cin >> password;

            if (emp.Emp_Pass == password) {
                return emp.Emp_Name;
            } else {
                cout << "Invalid password!\n";
                return "";
            }
        }
    }

    cout << "Invalid Employee ID!\n";
    return "";
}

void customerLogin() {
    string username, password;
    Customer loggedInCustomer;

    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (isValidCustomer(username, password, loggedInCustomer)) {
        cout << "Welcome, " << username << "!\n";
        cout << "1. View Stock\n";
        cout << "2. Buy Item\n";
        cout << "3. View Balance\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            printStock();
        } else if (choice == 2) {
            buyItem();
        } else if (choice == 3) {
            viewBalance(username);
        } else if (choice == 4) {
            depositMoney(username);
        } else if (choice == 5) {
            withdrawMoney(username);
        } else {
            cout << "Invalid choice!\n";
        }
    } else {
        cout << "Invalid credentials!\n";
    }
}

void customerSignup() {
    Customer newCustomer;
    cout << "Enter Username: ";
    cin >> newCustomer.User_Name;
    cout << "Enter Password: ";
    cin >> newCustomer.Password;
    cout << "Enter Initial Balance: ";
    cin >> newCustomer.Balance;

    customers.push_back(newCustomer);
    saveCustomers();

    cout << "Signup successful! You can now login.\n";
}

void addStock() {
    Stock newStock;
    cout << "Enter Stock Code: ";
    cin >> newStock.Icode;
    cout << "Enter Stock Name: ";
    cin >> newStock.Iname;
    cout << "Enter Stock Price: ";
    cin >> newStock.Iprice;
    cout << "Enter Stock Quantity: ";
    cin >> newStock.Iqtt;
    cout << "Enter Stock Category: ";
    cin >> newStock.Icat;

    stockItems.push_back(newStock);
    saveStock();

    cout << "Stock added successfully!\n";
}

void updateStock() {
    int code, quantity;
    cout << "Enter Stock Code to Update: ";
    cin >> code;

    for (auto &item : stockItems) {
        if (item.Icode == code) {
            cout << "Enter New Quantity: ";
            cin >> quantity;
            item.Iqtt = quantity;
            saveStock();
            cout << "Stock updated successfully!\n";
            return;
        }
    }

    cout << "Stock not found!\n";
}

void buyItem() {
    string username, itemName;
    int quantity;

    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Item Name: ";
    cin >> itemName;
    cout << "Enter Quantity: ";
    cin >> quantity;

    for (auto &cust : customers) {
        if (cust.User_Name == username) {
            for (auto &item : stockItems) {
                if (item.Iname == itemName) {
                    int totalCost = quantity * item.Iprice;

                    if (cust.Balance >= totalCost && item.Iqtt >= quantity) {
                        cust.Balance -= totalCost;
                        item.Iqtt -= quantity;

                        saveCustomers();
                        saveStock();

                        cout << "Purchase successful!\n";
                        return;
                    } else {
                        cout << "Insufficient balance or stock!\n";
                        return;
                    }
                }
            }

            cout << "Item not found!\n";
            return;
        }
    }

    cout << "Customer not found!\n";
}

void viewBalance(const string &username) {
    for (const auto &cust : customers) {
        if (cust.User_Name == username) {
            cout << "Your Balance: " << cust.Balance << "\n";
            return;
        }
    }

    cout << "Customer not found!\n";
}

void depositMoney(const string &username) {
    int amount;
    cout << "Enter Amount to Deposit: ";
    cin >> amount;

    for (auto &cust : customers) {
        if (cust.User_Name == username) {
            cust.Balance += amount;
            saveCustomers();
            cout << "Deposit successful! New Balance: " << cust.Balance << "\n";
            return;
        }
    }

    cout << "Customer not found!\n";
}

void withdrawMoney(const string &username) {
    int amount;
    cout << "Enter Amount to Withdraw: ";
    cin >> amount;

    for (auto &cust : customers) {
        if (cust.User_Name == username) {
            if (cust.Balance >= amount) {
                cust.Balance -= amount;
                saveCustomers();
                cout << "Withdrawal successful! New Balance: " << cust.Balance << "\n";
                return;
            } else {
                cout << "Insufficient balance!\n";
                return;
            }
        }
    }

    cout << "Customer not found!\n";
}

// Helper Functions
bool isValidCustomer(const string &username, const string &password, Customer &customer) {
    for (const auto &cust : customers) {
        if (cust.User_Name == username && cust.Password == password) {
            customer = cust;
            return true;
        }
    }
    return false;
}
