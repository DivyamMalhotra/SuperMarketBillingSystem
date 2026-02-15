#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
using namespace std;

#define GST 0.05

int generateBillNo() {
    ifstream in("billno.txt");
    int billNo = 1000;
    if (in) in >> billNo;
    in.close();

    ofstream out("billno.txt");
    out << billNo + 1;
    out.close();

    return billNo;
}

string currentDateTime() {
    time_t now = time(0);
    return string(ctime(&now));
}

class Item {
public:
    string name;
    double price;
    int quantity;
};

class SuperMarket {
private:
    string inventoryFile = "inventory.txt";

public:

    void adminLogin() {
        string user, pass;
        cout << "\nAdmin Login\n";
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        if (user == "admin" && pass == "1234")
            adminPanel();
        else
            cout << "Invalid Credentials!\n";
    }

    void adminPanel() {
        int choice;
        do {
            cout << "\n--- ADMIN PANEL ---\n";
            cout << "1. Add Item\n";
            cout << "2. View Inventory\n";
            cout << "3. View Sales Report\n";
            cout << "4. Back\n";
            cout << "Enter Choice: ";
            cin >> choice;

            if (choice == 1) addItem();
            else if (choice == 2) viewInventory();
            else if (choice == 3) viewSales();

        } while (choice != 4);
    }

    void addItem() {
        Item item;
        cout << "\nEnter Item Name: ";
        cin >> item.name;
        cout << "Enter Price: ";
        cin >> item.price;
        cout << "Enter Quantity: ";
        cin >> item.quantity;

        ofstream out(inventoryFile, ios::app);
        out << item.name << " "
            << item.price << " "
            << item.quantity << endl;
        out.close();

        cout << "Item Added Successfully!\n";
    }

    void viewInventory() {
        ifstream in(inventoryFile);
        Item item;

        cout << "\n--- INVENTORY ---\n";
        cout << left << setw(15) << "Item"
             << setw(10) << "Price"
             << setw(10) << "Qty" << endl;

        while (in >> item.name >> item.price >> item.quantity) {
            cout << left << setw(15) << item.name
                 << setw(10) << item.price
                 << setw(10) << item.quantity << endl;
        }
        in.close();
    }

    void billing() {
        string customerName, phone;
        cout << "\nCustomer Name: ";
        cin >> customerName;
        cout << "Phone Number: ";
        cin >> phone;

        vector<Item> cart;
        char choice;

        do {
            string itemName;
            int qty;
            cout << "\nEnter Item Name: ";
            cin >> itemName;
            cout << "Enter Quantity: ";
            cin >> qty;

            ifstream in(inventoryFile);
            ofstream temp("temp.txt");

            Item item;
            bool found = false;

            while (in >> item.name >> item.price >> item.quantity) {
                if (item.name == itemName) {
                    found = true;
                    if (qty <= item.quantity) {
                        item.quantity -= qty;

                        Item purchased;
                        purchased.name = item.name;
                        purchased.price = item.price;
                        purchased.quantity = qty;
                        cart.push_back(purchased);
                    } else {
                        cout << "Insufficient Stock!\n";
                    }
                }
                temp << item.name << " "
                     << item.price << " "
                     << item.quantity << endl;
            }

            in.close();
            temp.close();
            remove("inventory.txt");
            rename("temp.txt", "inventory.txt");

            if (!found)
                cout << "Item Not Found!\n";

            cout << "Add More Items? (y/n): ";
            cin >> choice;

        } while (choice == 'y');

        generateReceipt(cart, customerName, phone);
    }

    void generateReceipt(vector<Item> cart, string cname, string phone) {
        int billNo = generateBillNo();
        double subtotal = 0;

        string fileName = "Bill_" + to_string(billNo) + ".txt";
        ofstream bill(fileName, ios::app);
        ofstream sales("sales.txt", ios::app);

        bill << "=========== SUPER MARKET RECEIPT ==========\n";
        bill << "Bill No: " << billNo << endl;
        bill << "Date: " << currentDateTime();
        bill << "Customer: " << cname << endl;
        bill << "Phone: " << phone << endl;
        bill << "--------------------------------------------\n";
        bill << left << setw(15) << "Item"
             << setw(10) << "Price"
             << setw(10) << "Qty"
             << setw(10) << "Total" << endl;

        for (auto &item : cart) {
            double total = item.price * item.quantity;
            subtotal += total;

            bill << left << setw(15) << item.name
                 << setw(10) << item.price
                 << setw(10) << item.quantity
                 << setw(10) << total << endl;
        }

        double gstAmount = subtotal * GST;
        double discount = (subtotal > 1000) ? subtotal * 0.10 : 0;
        double finalTotal = subtotal + gstAmount - discount;

        bill << "--------------------------------------------\n";
        bill << "Subtotal: " << subtotal << endl;
        bill << "GST (5%): " << gstAmount << endl;
        bill << "Discount: " << discount << endl;
        bill << "Final Total: " << finalTotal << endl;
        bill << "============================================\n";
        bill << "Thank You For Shopping!\n";

        sales << "BillNo: " << billNo
              << " Customer: " << cname
              << " Total: " << finalTotal << endl;

        bill.close();
        sales.close();

        cout << "\nBill Generated Successfully!\n";
        cout << "Saved as: " << fileName << endl;
    }

    void viewSales() {
        ifstream in("sales.txt");
        string line;
        cout << "\n--- SALES REPORT ---\n";
        while (getline(in, line))
            cout << line << endl;
        in.close();
    }
};

int main() {
    SuperMarket market;
    int choice;

    do {
        cout << "\n====== SUPER MARKET BILLING ULTRA PRO ======\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Billing\n";
        cout << "3. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1) market.adminLogin();
        else if (choice == 2) market.billing();

    } while (choice != 3);

    return 0;
}