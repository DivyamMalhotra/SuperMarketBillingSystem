#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void clearScreen() {
    cout << string(50, '\n');
}

class Bill {
private:
    string Item;
    int Rate, Quantity;

public:
    Bill() : Item(""), Rate(0), Quantity(0) {}

    void setItem(string item) { Item = item; }
    void setRate(int rate) { Rate = rate; }
    void setQuant(int quant) { Quantity = quant; }

    string getItem() { return Item; }
    int getRate() { return Rate; }
    int getQuant() { return Quantity; }
};

void addItem(Bill &b) {
    bool close = false;

    while (!close) {
        int choice;
        cout << "\n\t1. Add Item";
        cout << "\n\t2. Close";
        cout << "\n\tEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            clearScreen();

            string item;
            int rate, quant;

            cout << "\tEnter Item Name: ";
            cin >> item;
            b.setItem(item);

            cout << "\tEnter Rate Of Item: ";
            cin >> rate;
            b.setRate(rate);

            cout << "\tEnter Quantity Of Item: ";
            cin >> quant;
            b.setQuant(quant);

            ofstream out("Bill.txt", ios::app);
            if (!out) {
                cout << "\tError: File Can't Open!\n";
            } else {
                out << b.getItem() << " "
                    << b.getRate() << " "
                    << b.getQuant() << endl;
                cout << "\tItem Added Successfully!\n";
            }
            out.close();
        }

        else if (choice == 2) {
            close = true;
            cout << "\tBack To Main Menu!\n";
        }
    }
}

void printBill() {
    clearScreen();
    int total = 0;
    bool close = false;

    while (!close) {
        int choice;
        cout << "\n\t1. Add To Bill";
        cout << "\n\t2. Close Session";
        cout << "\n\tEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            string item;
            int quant;

            cout << "\tEnter Item: ";
            cin >> item;
            cout << "\tEnter Quantity: ";
            cin >> quant;

            ifstream in("Bill.txt");
            ofstream out("Temp.txt");

            if (!in) {
                cout << "\tFile Not Found!\n";
                return;
            }

            string itemName;
            int itemRate, itemQuant;
            bool found = false;

            while (in >> itemName >> itemRate >> itemQuant) {
                if (item == itemName) {
                    found = true;

                    if (quant <= itemQuant) {
                        int amount = itemRate * quant;
                        cout << "\n\tItem | Rate | Qty | Amount\n";
                        cout << "\t" << itemName << " | "
                             << itemRate << " | "
                             << quant << " | "
                             << amount << endl;

                        total += amount;
                        itemQuant -= quant;
                    } else {
                        cout << "\tNot Enough Stock!\n";
                    }
                }

                out << itemName << " "
                    << itemRate << " "
                    << itemQuant << endl;
            }

            if (!found) {
                cout << "\tItem Not Available!\n";
            }

            in.close();
            out.close();

            remove("Bill.txt");
            rename("Temp.txt", "Bill.txt");
        }

        else if (choice == 2) {
            close = true;
        }
    }

    clearScreen();
    cout << "\n\tTotal Bill: " << total << endl;
    cout << "\tThanks For Shopping!\n";
}

int main() {
    Bill b;
    bool exitProgram = false;

    while (!exitProgram) {
        clearScreen();

        int val;
        cout << "\tWelcome To Super Market Billing System\n";
        cout << "\t**************************************\n";
        cout << "\t1. Add Item\n";
        cout << "\t2. Print Bill\n";
        cout << "\t3. Exit\n";
        cout << "\tEnter Choice: ";
        cin >> val;

        if (val == 1) {
            addItem(b);
        }
        else if (val == 2) {
            printBill();
        }
        else if (val == 3) {
            exitProgram = true;
            cout << "\tGood Luck!\n";
        }
    }

    return 0;
}