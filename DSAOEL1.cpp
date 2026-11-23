#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// PRODUCT NODE
struct Product
{
    int productID;
    string productName;
    string category;
    string size;
    string color;
    int quantity;
    float price;
    int reorderLevel;
    Product* next;
};

// TRANSACTION NODE
struct Transaction
{
    int transactionID;
    int productID;
    string transactionType;
    int quantity;
    int remainingStock;
    Transaction* next;
};

Product* head = NULL;
Transaction* transactionHead = NULL;

int nextTransactionID = 1;

int readInteger(string message, int minimum);
float readPrice(string message);
string readText(string message);

Product* searchByID(int id);
void addProduct();
void displayProducts();
void updateProduct();
void deleteProduct();
void searchProducts();
void sortInventory();
void bubbleSortByID();
void bubbleSortByName();
void bubbleSortByQuantity();

void stockIn();
void stockOut();
void addTransaction(int productID, string type,
                    int quantity, int remainingStock);
void displayTransactions();
void searchTransactions();

void inventoryReports();
void totalProducts();
void totalStock();
void lowStockReport();
void outOfStockReport();

void saveProducts();
void loadProducts();
void saveTransactions();
void loadTransactions();

void productMenu();
void searchMenu();
void sortMenu();
void stockMenu();
void reportMenu();
void transactionMenu();
void mainMenu();

void pauseScreen();
void freeMemory();
void swapProductData(Product* first, Product* second);
int readInteger(string message, int minimum)
{
    string input;
    int value;
    char extra;
    while(true)
    {
        cout << message;
        getline(cin, input);
        stringstream ss(input);
        if(ss >> value && !(ss >> extra) && value >= minimum)
        {
            return value;
        }
        cout << "Invalid input! Enter a valid number.\n";
    }
}
float readPrice(string message)
{
    string input;
    float value;
    char extra;
    while(true)
    {
        cout << message;
        getline(cin, input);
        stringstream ss(input);
        if(ss >> value && !(ss >> extra) &&
           value >= 0 && value == value)
        {
            return value;
        }
        cout << "Invalid price! Enter a non-negative number.\n";
    }
}
string readText(string message)
{
    string value;
    while(true)
    {
        cout << message;
        getline(cin, value);
        if(value.length() > 0 && value.find('|') == string::npos)
        {
            return value;
        }
        cout << "Input cannot be empty or contain |. Try again.\n";
    }
}
// SEARCH PRODUCT BY ID
Product* searchByID(int id)
{
    Product* temp = head;
    while(temp != NULL)
    {
        if(temp->productID == id)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
// ADD PRODUCT
void addProduct()
{
    cout << "\n========== ADD PRODUCT ==========\n";
    int id = readInteger("Enter Product ID: ", 1);
    if(searchByID(id) != NULL)
    {
        cout << "Error! Product ID already exists.\n";
        return;
    }
    string name = readText("Enter Product Name: ");
    string category = readText("Enter Category: ");
    string size = readText("Enter Size: ");
    string color = readText("Enter Color: ");
    int quantity = readInteger("Enter Quantity: ", 0);
    float price = readPrice("Enter Unit Price: ");
    int reorder = readInteger("Enter Reorder Level: ", 0);
    Product* newNode = new Product;
    newNode->productID = id;
    newNode->productName = name;
    newNode->category = category;
    newNode->size = size;
    newNode->color = color;
    newNode->quantity = quantity;
    newNode->price = price;
    newNode->reorderLevel = reorder;
    newNode->next = NULL;
    if(head == NULL)
    {
        head = newNode;
    }
    else
    {
        Product* temp = head;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    saveProducts();
    cout << "\nProduct added successfully!\n";
}
// DISPLAY PRODUCTS
void displayProducts()
{
    cout << "\n========================== INVENTORY ==========================\n";
    if(head == NULL)
    {
        cout << "No products available.\n";
        return;
    }
    Product* temp = head;
    cout << left
         << setw(8) << "ID"
         << setw(18) << "Name"
         << setw(15) << "Category"
         << setw(10) << "Size"
         << setw(12) << "Color"
         << setw(10) << "Quantity"
         << setw(12) << "Price"
         << setw(12) << "Reorder"
         << "Status\n";
    cout << string(105, '-') << "\n";
    while(temp != NULL)
    {
        cout << left
             << setw(8) << temp->productID
             << setw(18) << temp->productName
             << setw(15) << temp->category
             << setw(10) << temp->size
             << setw(12) << temp->color
             << setw(10) << temp->quantity
             << setw(12) << fixed << setprecision(2) << temp->price
             << setw(12) << temp->reorderLevel;
        if(temp->quantity == 0)
        {
            cout << "Out of Stock";
        }
        else if(temp->quantity <= temp->reorderLevel)
        {
            cout << "Low Stock";
        }
        else
        {
            cout << "Normal";
        }
        cout << endl;
        temp = temp->next;
    }
    cout << string(105, '-') << "\n";
}
// UPDATE PRODUCT
void updateProduct()
{
    cout << "\n========== UPDATE PRODUCT ==========\n";
    int id = readInteger("Enter Product ID: ", 1);
    Product* temp = searchByID(id);
    if(temp == NULL)
    {
        cout << "Product not found.\n";
        return;
    }
    cout << "\nCurrent Product: " << temp->productName << "\n";
    cout << "1. Update Product Name\n";
    cout << "2. Update Category\n";
    cout << "3. Update Size\n";
    cout << "4. Update Color\n";
    cout << "5. Update Price\n";
    cout << "6. Update Reorder Level\n";
    int choice = readInteger("Enter choice: ", 1);
    switch(choice)
    {
        case 1:
            temp->productName = readText("Enter New Name: ");
            break;
        case 2:
            temp->category = readText("Enter New Category: ");
            break;
        case 3:
            temp->size = readText("Enter New Size: ");
            break;
        case 4:
            temp->color = readText("Enter New Color: ");
            break;
        case 5:
            temp->price = readPrice("Enter New Price: ");
            break;
        case 6:
            temp->reorderLevel =
                readInteger("Enter New Reorder Level: ", 0);
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }
    saveProducts();
    cout << "Product updated successfully.\n";
}
// DELETE PRODUCT
void deleteProduct()
{
    cout << "\n========== DELETE PRODUCT ==========\n";
    if(head == NULL)
    {
        cout << "Inventory is empty.\n";
        return;
    }
    int id = readInteger("Enter Product ID to delete: ", 1);
    Product* temp = head;
    Product* previous = NULL;

    while(temp != NULL && temp->productID != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if(temp == NULL)
    {
        cout << "Product not found.\n";
        return;
    }

    if(previous == NULL)
    {
        head = temp->next;
    }
    else
    {
        previous->next = temp->next;
    }
    delete temp;
    saveProducts();
    cout << "Product deleted successfully.\n";
}
// SEARCH PRODUCTS
void searchProducts()
{
    cout << "\n========== SEARCH PRODUCTS ==========\n";
    cout << "1. Search by Product ID\n";
    cout << "2. Search by Product Name\n";
    cout << "3. Search by Quantity\n";
    int choice = readInteger("Enter choice: ", 1);
    if(choice < 1 || choice > 3)
    {
        cout << "Invalid choice.\n";
        return;
    }
    if(head == NULL)
    {
        cout << "Inventory is empty.\n";
        return;
    }
    Product* temp = head;
    bool found = false;
    if(choice == 1)
    {
        int id = readInteger("Enter Product ID: ", 1);
        temp = searchByID(id);
        if(temp != NULL)
        {
            found = true;
            cout << "\nProduct Found: " << temp->productName
                 << ", Quantity: " << temp->quantity << "\n";
        }
    }
    else if(choice == 2)
    {
        string name = readText("Enter Product Name: ");
        while(temp != NULL)
        {
            if(temp->productName == name)
            {
                cout << "\nID: " << temp->productID
                     << "\nName: " << temp->productName
                     << "\nCategory: " << temp->category
                     << "\nSize: " << temp->size
                     << "\nColor: " << temp->color
                     << "\nQuantity: " << temp->quantity
                     << "\nPrice: " << temp->price << "\n";
                found = true;
            }
            temp = temp->next;
        }
    }
    else
    {
        int quantity = readInteger("Enter Quantity: ", 0);

        while(temp != NULL)
        {
            if(temp->quantity == quantity)
            {
                cout << "\nID: " << temp->productID
                     << " | Name: " << temp->productName
                     << " | Quantity: " << temp->quantity << "\n";

                found = true;
            }
            temp = temp->next;
        }
    }
    if(!found)
    {
        cout << "No matching product found.\n";
    }
}
// SWAP PRODUCT DATA FOR BUBBLE SORT
void swapProductData(Product* first, Product* second)
{
    int tempInt;
    float tempFloat;
    string tempString;
    tempInt = first->productID;
    first->productID = second->productID;
    second->productID = tempInt;
    tempString = first->productName;
    first->productName = second->productName;
    second->productName = tempString;
    tempString = first->category;
    first->category = second->category;
    second->category = tempString;
    tempString = first->size;
    first->size = second->size;
    second->size = tempString;
    tempString = first->color;
    first->color = second->color;
    second->color = tempString;
    tempInt = first->quantity;
    first->quantity = second->quantity;
    second->quantity = tempInt;
    tempFloat = first->price;
    first->price = second->price;
    second->price = tempFloat;
    tempInt = first->reorderLevel;
    first->reorderLevel = second->reorderLevel;
    second->reorderLevel = tempInt;
}
// BUBBLE SORT BY ID
void bubbleSortByID()
{
    if(head == NULL || head->next == NULL)
    {
        cout << "Not enough products to sort.\n";
        return;
    }
    bool swapped;
    do
    {
        swapped = false;
        Product* temp = head;
        while(temp->next != NULL)
        {
            if(temp->productID > temp->next->productID)
            {
                swapProductData(temp, temp->next);
                swapped = true;
            }
            temp = temp->next;
        }
    }while(swapped);
    saveProducts();
    cout << "Products sorted by ID successfully.\n";
}
// BUBBLE SORT BY NAME
void bubbleSortByName()
{
    if(head == NULL || head->next == NULL)
    {
        cout << "Not enough products to sort.\n";
        return;
    }
    bool swapped;
    do
    {
        swapped = false;
        Product* temp = head;
        while(temp->next != NULL)
        {
            if(temp->productName > temp->next->productName)
            {
                swapProductData(temp, temp->next);
                swapped = true;
            }
            temp = temp->next;
        }
    }while(swapped);
    saveProducts();
    cout << "Products sorted by Name successfully.\n";
}
// BUBBLE SORT BY QUANTITY
void bubbleSortByQuantity()
{
    if(head == NULL || head->next == NULL)
    {
        cout << "Not enough products to sort.\n";
        return;
    }
    bool swapped;
    do
    {
        swapped = false;
        Product* temp = head;
        while(temp->next != NULL)
        {
            if(temp->quantity > temp->next->quantity)
            {
                swapProductData(temp, temp->next);
                swapped = true;
            }
            temp = temp->next;
        }
    }while(swapped);
    saveProducts();
    cout << "Products sorted by Quantity successfully.\n";
}
// SORT MENU
void sortInventory()
{
    cout << "\n========== SORT INVENTORY ==========\n";
    cout << "1. Sort by Product ID\n";
    cout << "2. Sort by Product Name\n";
    cout << "3. Sort by Quantity\n";
    int choice = readInteger("Enter choice: ", 1);
    switch(choice)
    {
        case 1:
            bubbleSortByID();
            break;
        case 2:
            bubbleSortByName();
            break;
        case 3:
            bubbleSortByQuantity();
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }
    displayProducts();
}
// ADD TRANSACTION
void addTransaction(int productID, string type,
                    int quantity, int remainingStock)
{
    Transaction* newNode = new Transaction;
    newNode->transactionID = nextTransactionID++;
    newNode->productID = productID;
    newNode->transactionType = type;
    newNode->quantity = quantity;
    newNode->remainingStock = remainingStock;
    newNode->next = NULL;
    if(transactionHead == NULL)
    {
        transactionHead = newNode;
    }
    else
    {
        Transaction* temp = transactionHead;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    saveTransactions();
}
// STOCK IN
void stockIn()
{
    cout << "\n========== STOCK IN ==========\n";
    int id = readInteger("Enter Product ID: ", 1);
    Product* temp = searchByID(id);
    if(temp == NULL)
    {
        cout << "Product not found.\n";
        return;
    }
    cout << "Current Stock: " << temp->quantity << "\n";
    int amount = readInteger("Enter Quantity to Add: ", 1);
    if(amount > 1000000000 - temp->quantity)
    {
        cout << "Stock limit exceeded.\n";
        return;
    }
    temp->quantity += amount;
    addTransaction(id, "STOCK-IN", amount, temp->quantity);
    saveProducts();
    cout << "Stock added successfully.\n";
    cout << "New Stock: " << temp->quantity << "\n";
}
// STOCK OUT
void stockOut()
{
    cout << "\n========== STOCK OUT ==========\n";
    int id = readInteger("Enter Product ID: ", 1);
    Product* temp = searchByID(id);
    if(temp == NULL)
    {
        cout << "Product not found.\n";
        return;
    }
    cout << "Available Stock: " << temp->quantity << "\n";
    int amount = readInteger("Enter Quantity to Remove: ", 1);
    if(amount > temp->quantity)
    {
        cout << "ERROR: Insufficient stock!\n";
        cout << "Transaction not completed.\n";
        return;
    }
    temp->quantity -= amount;
    addTransaction(id, "STOCK-OUT", amount, temp->quantity);
    saveProducts();
    cout << "Stock removed successfully.\n";
    cout << "Remaining Stock: " << temp->quantity << "\n";
}
// DISPLAY TRANSACTIONS
void displayTransactions()
{
    cout << "\n================ TRANSACTION HISTORY ================\n";
    if(transactionHead == NULL)
    {
        cout << "No transactions available.\n";
        return;
    }
    Transaction* temp = transactionHead;
    cout << left
         << setw(15) << "Transaction ID"
         << setw(15) << "Product ID"
         << setw(18) << "Type"
         << setw(12) << "Quantity"
         << "Remaining Stock\n";
    cout << string(75, '-') << "\n";
    while(temp != NULL)
    {
        cout << left
             << setw(15) << temp->transactionID
             << setw(15) << temp->productID
             << setw(18) << temp->transactionType
             << setw(12) << temp->quantity
             << temp->remainingStock << "\n";
        temp = temp->next;
    }
}
// SEARCH TRANSACTIONS BY PRODUCT ID
void searchTransactions()
{
    if(transactionHead == NULL)
    {
        cout << "No transactions available.\n";
        return;
    }
    int id = readInteger("Enter Product ID: ", 1);
    Transaction* temp = transactionHead;
    bool found = false;
    while(temp != NULL)
    {
        if(temp->productID == id)
        {
            cout << "\nTransaction ID: " << temp->transactionID
                 << "\nProduct ID: " << temp->productID
                 << "\nType: " << temp->transactionType
                 << "\nQuantity: " << temp->quantity
                 << "\nRemaining Stock: " << temp->remainingStock
                 << "\n";
            found = true;
        }
        temp = temp->next;
    }
    if(!found)
    {
        cout << "No transactions found for this product.\n";
    }
}
// TOTAL PRODUCTS
void totalProducts()
{
    int count = 0;
    Product* temp = head;
    while(temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    cout << "Total Number of Products: " << count << "\n";
}
// TOTAL AVAILABLE STOCK
void totalStock()
{
    int total = 0;
    Product* temp = head;
    while(temp != NULL)
    {
        total += temp->quantity;
        temp = temp->next;
    }
    cout << "Total Available Stock Units: " << total << "\n";
}
// LOW STOCK REPORT
void lowStockReport()
{
    cout << "\n========== LOW STOCK REPORT ==========\n";
    Product* temp = head;
    bool found = false;
    while(temp != NULL)
    {
        if(temp->quantity > 0 &&
           temp->quantity <= temp->reorderLevel)
        {
            cout << "ID: " << temp->productID
                 << " | Product: " << temp->productName
                 << " | Quantity: " << temp->quantity
                 << " | Reorder Level: " << temp->reorderLevel
                 << "\n";
            found = true;
        }
        temp = temp->next;
    }
    if(!found)
    {
        cout << "No low-stock products.\n";
    }
}
// OUT OF STOCK REPORT
void outOfStockReport()
{
    cout << "\n========== OUT OF STOCK REPORT ==========\n";
    Product* temp = head;
    bool found = false;
    while(temp != NULL)
    {
        if(temp->quantity == 0)
        {
            cout << "ID: " << temp->productID
                 << " | Product: " << temp->productName << "\n";
            found = true;
        }
        temp = temp->next;
    }
    if(!found)
    {
        cout << "No out-of-stock products.\n";
    }
}
// INVENTORY REPORTS
void inventoryReports()
{
    cout << "\n========== INVENTORY REPORTS ==========\n";
    if(head == NULL)
    {
        cout << "Inventory is empty.\n";
        return;
    }
    totalProducts();
    totalStock();
    lowStockReport();
    outOfStockReport();
}
// SAVE PRODUCTS TO FILE
void saveProducts()
{
    ofstream file("products.txt");
    if(!file.is_open())
    {
        cout << "Error saving products file.\n";
        return;
    }
    Product* temp = head;
    while(temp != NULL)
    {
        file << temp->productID << "|"
             << temp->productName << "|"
             << temp->category << "|"
             << temp->size << "|"
             << temp->color << "|"
             << temp->quantity << "|"
             << temp->price << "|"
             << temp->reorderLevel << "\n";
        temp = temp->next;
    }
    file.close();
}
// LOAD PRODUCTS FROM FILE
void loadProducts()
{
    ifstream file("products.txt");
    if(!file.is_open())
    {
        return;
    }
    string line;
    while(getline(file, line))
    {
        string fields[8];
        stringstream ss(line);
        bool valid = true;
        for(int i = 0; i < 8; i++)
        {
            if(!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }
        if(!valid)
        {
            continue;
        }
        stringstream a(fields[0]);
        stringstream b(fields[5]);
        stringstream c(fields[6]);
        stringstream d(fields[7]);
        int id, quantity, reorder;
        float price;
        char extra;
        if(!(a >> id) || (a >> extra) ||
           !(b >> quantity) || (b >> extra) ||
           !(c >> price) || (c >> extra) ||
           !(d >> reorder) || (d >> extra))
        {
            continue;
        }
        if(id <= 0 || quantity < 0 || price < 0 ||
           price != price || reorder < 0 ||
           searchByID(id) != NULL)
        {
            continue;
        }
        Product* newNode = new Product;
        newNode->productID = id;
        newNode->productName = fields[1];
        newNode->category = fields[2];
        newNode->size = fields[3];
        newNode->color = fields[4];
        newNode->quantity = quantity;
        newNode->price = price;
        newNode->reorderLevel = reorder;
        newNode->next = NULL;
        if(head == NULL)
        {
            head = newNode;
        }
        else
        {
            Product* temp = head;
            while(temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    file.close();
}
// SAVE TRANSACTIONS TO FILE
void saveTransactions()
{
    ofstream file("transactions.txt");
    if(!file.is_open())
    {
        cout << "Error saving transactions file.\n";
        return;
    }
    Transaction* temp = transactionHead;
    while(temp != NULL)
    {
        file << temp->transactionID << "|"
             << temp->productID << "|"
             << temp->transactionType << "|"
             << temp->quantity << "|"
             << temp->remainingStock << "\n";

        temp = temp->next;
    }
    file.close();
}
// LOAD TRANSACTIONS FROM FILE
void loadTransactions()
{
    ifstream file("transactions.txt");
    if(!file.is_open())
    {
        return;
    }
    string line;
    while(getline(file, line))
    {
        string fields[5];
        stringstream ss(line);
        bool valid = true;
        for(int i = 0; i < 5; i++)
        {
            if(!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }
        if(!valid)
        {
            continue;
        }
        stringstream a(fields[0]);
        stringstream b(fields[1]);
        stringstream c(fields[3]);
        stringstream d(fields[4]);
        int tid, pid, quantity, remaining;
        char extra;
        if(!(a >> tid) || (a >> extra) ||
           !(b >> pid) || (b >> extra) ||
           !(c >> quantity) || (c >> extra) ||
           !(d >> remaining) || (d >> extra))
        {
            continue;
        }
        if(tid <= 0 || pid <= 0 ||
           quantity <= 0 || remaining < 0 ||
           (fields[2] != "STOCK-IN" &&
            fields[2] != "STOCK-OUT"))
        {
            continue;
        }
        Transaction* newNode = new Transaction;
        newNode->transactionID = tid;
        newNode->productID = pid;
        newNode->transactionType = fields[2];
        newNode->quantity = quantity;
        newNode->remainingStock = remaining;
        newNode->next = NULL;
        if(transactionHead == NULL)
        {
            transactionHead = newNode;
        }
        else
        {
            Transaction* temp = transactionHead;
            while(temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        if(tid >= nextTransactionID)
        {
            nextTransactionID = tid + 1;
        }
    }
    file.close();
}
// PAUSE SCREEN
void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    string input;
    getline(cin, input);
}
// PRODUCT MENU
void productMenu()
{
    int choice;
    do
    {
        cout << "\n========== PRODUCT MANAGEMENT ==========\n";
        cout << "1. Add Product\n";
        cout << "2. Update Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Display All Products\n";
        cout << "5. Back to Main Menu\n";
        choice = readInteger("Enter choice: ", 1);
        switch(choice)
        {
            case 1: addProduct(); break;
            case 2: updateProduct(); break;
            case 3: deleteProduct(); break;
            case 4: displayProducts(); break;
            case 5: break;
            default: cout << "Invalid choice.\n";
        }
        if(choice != 5)
        {
            pauseScreen();
        }
    }while(choice != 5);
}
// SEARCH MENU
void searchMenu()
{
    searchProducts();
    pauseScreen();
}
// SORT MENU
void sortMenu()
{
    sortInventory();
    pauseScreen();
}
// STOCK MENU
void stockMenu()
{
    int choice;
    do
    {
        cout << "\n========== STOCK MANAGEMENT ==========\n";
        cout << "1. Stock-In\n";
        cout << "2. Stock-Out\n";
        cout << "3. Back to Main Menu\n";
        choice = readInteger("Enter choice: ", 1);
        switch(choice)
        {
            case 1: stockIn(); break;
            case 2: stockOut(); break;
            case 3: break;
            default: cout << "Invalid choice.\n";
        }
        if(choice != 3)
        {
            pauseScreen();
        }
    }while(choice != 3);
}
// REPORT MENU
void reportMenu()
{
    int choice;
    do
    {
        cout << "\n========== INVENTORY REPORTS ==========\n";
        cout << "1. Complete Inventory\n";
        cout << "2. Total Number of Products\n";
        cout << "3. Total Available Stock\n";
        cout << "4. Low Stock Products\n";
        cout << "5. Out of Stock Products\n";
        cout << "6. Back to Main Menu\n";
        choice = readInteger("Enter choice: ", 1);
        switch(choice)
        {
            case 1: displayProducts(); break;
            case 2: totalProducts(); break;
            case 3: totalStock(); break;
            case 4: lowStockReport(); break;
            case 5: outOfStockReport(); break;
            case 6: break;
            default: cout << "Invalid choice.\n";
        }
        if(choice != 6)
        {
            pauseScreen();
        }
    }while(choice != 6);
}
// TRANSACTION MENU
void transactionMenu()
{
    int choice;
    do
    {
        cout << "\n========== TRANSACTION HISTORY ==========\n";
        cout << "1. Display All Transactions\n";
        cout << "2. Search Transactions by Product ID\n";
        cout << "3. Back to Main Menu\n";
        choice = readInteger("Enter choice: ", 1);
        switch(choice)
        {
            case 1: displayTransactions(); break;
            case 2: searchTransactions(); break;
            case 3: break;
            default: cout << "Invalid choice.\n";
        }
        if(choice != 3)
        {
            pauseScreen();
        }
    }while(choice != 3);
}
// FREE MEMORY
void freeMemory()
{
    Product* temp;

    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
    Transaction* tempTransaction;
    while(transactionHead != NULL)
    {
        tempTransaction = transactionHead;
        transactionHead = transactionHead->next;
        delete tempTransaction;
    }
}
// MAIN MENU
void mainMenu()
{
    int choice;

    do
    {
        cout << "\n============================================\n";
        cout << "       STYLESTOCK INVENTORY SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Product Management\n";
        cout << "2. Search Products\n";
        cout << "3. Sort Inventory\n";
        cout << "4. Stock Management\n";
        cout << "5. Inventory Reports\n";
        cout << "6. Transaction History\n";
        cout << "7. Display All Products\n";
        cout << "8. Exit\n";
        choice = readInteger("\nEnter your choice: ", 1);
        switch(choice)
        {
            case 1:
                productMenu();
                break;

            case 2:
                searchMenu();
                break;

            case 3:
                sortMenu();
                break;

            case 4:
                stockMenu();
                break;

            case 5:
                reportMenu();
                break;

            case 6:
                transactionMenu();
                break;

            case 7:
                displayProducts();
                pauseScreen();
                break;

            case 8:
                saveProducts();
                saveTransactions();

                cout << "\nAll records saved successfully.\n";
                cout << "Thank you for using StyleStock!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
                pauseScreen();
        }

    }while(choice != 8);
}
// MAIN FUNCTION
int main()
{
    loadProducts();
    loadTransactions();

    cout << "\nWelcome to StyleStock!\n";
    cout << "Saved records loaded successfully.\n";

    mainMenu();

    freeMemory();

    return 0;
}

