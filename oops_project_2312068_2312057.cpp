#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <conio.h>  

using namespace std;

class User {
public:
    string name;       
    int age;
    virtual void showDetails() = 0; 
};

class Patient : public User {
public:                        
    string bloodGroup;
    int quantity;

    Patient(string n, int a, string bg, int q) {
        name = n;
        age = a;
        bloodGroup = bg;
        quantity = q;
    }

    void showDetails() override {   
        cout << "Patient: " << name << ", Age: " << age
             << ", Blood Group: " << bloodGroup
             << ", Bottles Required: " << quantity << endl;
    }
};

class Billing {
public:
    virtual int calculateCost(string bloodGroup, int quantity) {
        int rate = 0;

        if (bloodGroup == "O+" || bloodGroup == "A+") rate = 500;
        else if (bloodGroup == "B+" || bloodGroup == "AB+") rate = 600;
        else if (bloodGroup == "O-" || bloodGroup == "A-" || bloodGroup == "B-") rate = 700;
        else if (bloodGroup == "AB-") rate = 800;
        else rate = 550; 

        return rate * quantity;
    }
};

struct Donor {    
    string name;
    int age;
    string bloodGroup;
};

class BloodBank {
    map<string, int> bloodInventory;  
    vector<Donor> donors;  

    string adminUsername = "Somaresa";   
    string adminPassword = "12345";

public:
    BloodBank() {  
        bloodInventory["A+"] = 5;
        bloodInventory["A-"] = 3;
        bloodInventory["B+"] = 4;
        bloodInventory["B-"] = 2;
        bloodInventory["O+"] = 6;
        bloodInventory["O-"] = 1;
        bloodInventory["AB+"] = 1;
        bloodInventory["AB-"] = 0;
    }

    void registerDonor() {   
        Donor d;
        cout << "\n--- Donor Registration ---\n";
        cin.ignore();
        cout << "Enter Donor Name: ";
        getline(cin, d.name);
        cout << "Enter Donor Age: ";
        cin >> d.age;
        cout << "Enter Blood Group (A+, O-, etc.): ";
        cin >> d.bloodGroup;

        donors.push_back(d);
        cout << " Donor " << d.name << " registered successfully!\n";
    }

    void donateBlood() {
        string name;
        cin.ignore();
        cout << "\nEnter your name to donate blood: ";
        getline(cin, name);

        bool found = false; 
        string group;

        for (auto& d : donors) {
            if (d.name == name) {
                group = d.bloodGroup;
                found = true;
                break;
            }
        }

        if (found) {
            int qty;
            cout << "Enter number of bottles to donate: ";
            cin >> qty;
            if (qty > 0) {
                bloodInventory[group] += qty;
                cout << " Thank you! " << qty << " bottle(s) of " << group << " added to inventory.\n";
            } else {
                cout << " Invalid quantity.\n";
            }
        } else {
            cout << " Donor not found. Please register first.\n";
        }
    }

    void searchBlood() {
        string name, bloodGroup;
        int age, quantity;

        cout << "\n--- Blood Request ---\n";
        cin.ignore();
        cout << "Enter Patient Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Required Blood Group (A+, B-, etc.): ";
        cin >> bloodGroup;
        cout << "Enter Number of Bottles Required: ";
        cin >> quantity;

        if (bloodInventory.find(bloodGroup) != bloodInventory.end()) {
            int available = bloodInventory[bloodGroup];

            if (available >= quantity) {
                cout << "Request approved! " << quantity << " bottle(s) of " << bloodGroup << " will be issued.\n";
                bloodInventory[bloodGroup] -= quantity;
            } else if (available > 0) {
                cout << " Not enough bottles. Only " << available << " bottle(s) available.\n";
            } else {
                cout << " No bottles available for blood group " << bloodGroup << ".\n";
            }
        } else {
            cout << " Invalid blood group.\n";
        }
    }

    void purchaseBlood() {
        string name, bloodGroup;
        int age, quantity;

        cout << "\n--- Purchase Blood with Billing ---\n";
        cin.ignore();
        cout << "Enter Patient Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Required Blood Group (A+, B-, etc.): ";
        cin >> bloodGroup;
        cout << "Enter Number of Bottles Required: ";
        cin >> quantity;

        if (bloodInventory.find(bloodGroup) != bloodInventory.end()) {
            int available = bloodInventory[bloodGroup];

            if (available >= quantity) {
                Patient p(name, age, bloodGroup, quantity);
                p.showDetails();

                Billing* bill = new Billing();
                int total = bill->calculateCost(bloodGroup, quantity);
                cout << " Total Cost = ₹" << total << "\n";

                bloodInventory[bloodGroup] -= quantity;
                cout << " Blood issued successfully.\n";

                delete bill;
            } else {
                cout << " Only " << available << " bottle(s) available.\n";
            }
        } else {
            cout << " Invalid blood group.\n";
        }
    }

    bool adminLogin() {
        string username, password;
        char ch;

        cout << "\n--- Admin Login ---\n";
        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        password = "";

        while (true) {
            ch = _getch();  
            if (ch == 13) break;             
            else if (ch == 8) {
                if (!password.empty()) {
                    cout << "\b \b";
                    password.pop_back();
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;

        if (username == adminUsername && password == adminPassword) {
            cout << " Login successful!\n";
            return true;
        } else {
            cout << " Incorrect username or password!\n";
            return false;
        }
    }

    void showInventory() {
        cout << "\n--- Current Blood Inventory ---\n";
        for (auto& entry : bloodInventory) {
            cout << "Blood Group: " << entry.first
                 << " | Bottles Available: " << entry.second << endl;
        }
        cout << "--------------------------------\n";
    }

    void addBloodStock() {
        string group;
        int quantity;

        cout << "\nEnter Blood Group to Add: ";
        cin >> group;
        cout << "Enter Quantity to Add: ";
        cin >> quantity;

        if (quantity > 0) {
            bloodInventory[group] += quantity;
            cout << " " << quantity << " bottle(s) of " << group << " added.\n";
        } else {
            cout << " Invalid quantity.\n";
        }
    }
};

int main() {
    BloodBank bank;
    int mainChoice;

    do {
        cout << "\n========== Blood Bank System Menu ==========\n";
        cout << "1. Register as Donor\n";
        cout << "2. Donate Blood\n";
        cout << "3. Search for Blood\n";
        cout << "4. Purchase Blood (With Billing)\n";
        cout << "5. View Inventory (Admin Login Required)\n";
        cout << "6. Add Blood Stock (Admin Login Required)\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            bank.registerDonor();
            break;
        case 2:
            bank.donateBlood();
            break;
        case 3:
            bank.searchBlood();
            break;
        case 4:
            bank.purchaseBlood();
            break;
        case 5:
            if (bank.adminLogin())
                bank.showInventory();
            break;
        case 6:
            if (bank.adminLogin())
                bank.addBloodStock();
            break;
        case 7:
            cout << " Exiting... Thank you for using the system!\n";
            break;
        default:
            cout << " Invalid choice. Please try again.\n";
        }

    } while (mainChoice != 7);

    return 0;
}