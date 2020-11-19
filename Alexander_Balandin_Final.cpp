//NAME: Alexander Balandin
//ID: 132145194
//EMAIL: abalandin@myseneca.ca
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include <string>
#include <occi.h>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

//Employee structure:
struct Employee {
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char email[100];
    char phone[50];
    char extension[10];
    // FN and LN of employee who is the manager
    char reportsTo[100];
    char jobTitle[50];
    char city[50];

};
//insert employee
void insertEmployee(Connection* conn, struct Employee emp);
//update employee
void updateEmployee(Connection* conn, int employeeNumber);
void displayAllEmployees(Connection *conn);
//menu function
int menu(void);
//display
void displayEmployee(Connection *conn, struct Employee emp);
//find employee function
int findEmployee(Connection *conn, int employeeNumber, struct Employee* emp);
//delete employee
void deleteEmployee(Connection* conn, int employeeNumber);
int main(void) {
   
    Environment* env = nullptr;
    Connection* conn = nullptr;
    
    string str;
    // login
    string usr = "dbs211_202e04";
    // password
    string pass = "22395257";
    // connection
    string srv = "myoracle12c.senecacollege.ca:1521/oracle12c/";
    try {
    env = Environment::createEnvironment(Environment::DEFAULT);
    //creating connection
    conn = env->createConnection(usr, pass, srv);
    // creating environment
    Statement* stmt = conn->createStatement();
   
    // variables in used in the switch
    int option = 0;
    int num = 0;
    Employee emp = { 0 };
    string input;


        while (option != 6)
        {
            //prompting the user to select an option
            option = menu();
            switch (option)
            {
              // display an employee
            case 1:
                displayEmployee(conn, emp);
                break;
            case 2:
                //display all employees
                displayAllEmployees(conn);
                break;
                // add employee
            case 3:
                // users input
                cout << endl << "***Adding Employee***" << endl;
                cout << "Enter employees's number: ";
                cin >> emp.employeeNumber;
                cout << "Enter employees's last name: ";
                cin >> emp.lastName;
                cout << "Enter employees's first name: ";
                cin >> emp.firstName;
                cout << "Enter employees's email: ";
                cin >> emp.email;
                cout << "Enter employees's extension: ";
                cin >> emp.extension;
                cin.clear();
                cin.ignore(2000, '\n');
                cout << "Enter employees's job title: ";
                getline(cin, input);
                strcpy(emp.jobTitle, input.c_str());
                
               

                insertEmployee(conn, emp);
                break;
            case 4:
                //update an employee
                cout << endl << "***Updating Employee***" << endl;
                cout << "Enter an employee's number: ";
                
                cin >> num;
                updateEmployee(conn, num);
                break;
            case 5:
                cout << "Enter an employee's number: ";
                cin >> num;
                deleteEmployee(conn, num);
            default:
                break;
            }



        }
        // close connection
        conn->terminateStatement(stmt);
    }
    //cathc errors
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}


void insertEmployee(Connection* conn, Employee emp)
{
    //if not found
    if (!findEmployee(conn, emp.employeeNumber, &emp))
    {
        string quotes = "', '";
        string convert(emp.lastName);
        string query = "INSERT INTO employees(employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle) VALUES(" + to_string(emp.employeeNumber) + ",'";
        // adding values to the query
        query += convert + quotes;
        convert = emp.firstName;
        query += convert + quotes;
        convert = emp.extension;
        query += convert + quotes;
        convert = emp.email;
        query += convert + quotes + "1" + quotes + "1002" + quotes;
        convert = emp.jobTitle;
       
        query += convert + "')";
        // doing the query
        Statement* stmt = conn->createStatement();
        ResultSet* t1 = stmt->executeQuery(query);
        conn->terminateStatement(stmt);

        cout << "The new employee is added successfully." << endl;
    }
    //if found
    else
    {
        cout << endl << "An employee with the same employee number exists." << endl << endl;
    }
}

void updateEmployee(Connection* conn, int employeeNumber)
{
    Statement* stmt = conn->createStatement();
    Employee emp = { 0 };
    // if found
    if (findEmployee(conn, employeeNumber, &emp))
    {
        string ext;
        cout  << "Enter a new extension: ";
        cin >> ext;
        //creating a query
        string query = "UPDATE employees SET extension = '" + ext + "' WHERE employeenumber = " + to_string(employeeNumber);
       //perfome the query
        ResultSet* t1 = stmt->executeQuery(query);
        cout << " The employee's exstension is updated." << endl;
        
    }
    else
    {
        cout << "Entered employee not found." << endl;
    }

    //close connection
    conn->terminateStatement(stmt);
}

void displayAllEmployees(Connection* conn)
{
    //query
    string query = "SELECT lastname || \' \' || firstname as fullname FROM employees WHERE employeenumber like ";
    string num, queryNum;
    Statement* stmt = conn->createStatement();
    Statement* stmt2 = conn->createStatement();
    //display a header
    cout  << setw(2)  << "E " << setw(23) << "Employee Name " << setw(20) << "Email" << setw(30) << "Phone" << setw(15) << "Ext" << setw(12) << "Manager" << setw(10) << endl;
    cout.fill('-');
    cout << setw(120);
    cout << "--" << endl;
    
    cout.fill(' ');
    //display the table
    ResultSet* t1 = stmt->executeQuery("SELECT employees.employeenumber, employees.lastname, employees.firstname, employees.email , offices.phone , employees.extension, employees.reportsto FROM employees LEFT OUTER JOIN offices ON employees.officecode = offices.officecode");
    while (t1->next())
    {
        num = to_string(t1->getInt(7));
        queryNum = query + num;
        
        cout << left << setw(10) << t1->getInt(1) << setw(10) << t1->getString(2) << setw(15) << left << t1->getString(3) << setw(35) << left<< t1->getString(4) << setw(10) << left << t1->getString(5) << left << setw(8) << right << t1->getString(6) << "\t" << left ;
        if (t1->getInt(7) != 0) {
            ResultSet* t2 = stmt2->executeQuery(queryNum);
            t2->next();

            cout  << left << t2->getString(1) << endl;
        }
        else {
            cout << "0" << endl;
        }
       
    }
    //close connection
    conn->terminateStatement(stmt);
}

int menu(void)
{
    int option;
    cout << "********************* HR Menu *********************" << endl;
    //printing the menu
    cout  << "1) Find Employee" << endl << left <<"2) Employees Report" << endl << "3) Add Employee" << endl << "4) Update Employee" << endl << "5) Remove Employee" << endl << "6) Exit" << endl;
    //prompting the user to select an option
    
    cout << "> ";
    cin >> option;
    //error message
    while (option < 1 || option > 6)
    {
        cout << "Invalid option, please try again: ";
        cin >> option;
    }
    return option;
}

void displayEmployee(Connection* conn, Employee emp)
{
   
    int empNum = 0;
    //prompting
    cout << "Please enter an employee number: ";
    cin >> empNum;
    
     //call find employee function
    if (findEmployee(conn, empNum, &emp))
    {
        //display the employee information
        cout << endl << setw(25) << right << "employeeNumber = " << emp.employeeNumber << endl;
        cout << setw(25) << right << "lastName = " << emp.lastName << endl;
        cout << setw(25) << right << "firstName = " << emp.firstName << endl;
        cout << setw(25) << right << "email = " << emp.email << endl;
        cout << setw(25) << right << "phone = " << emp.phone << endl;
        cout << setw(25) << right << "extension = " << emp.extension << endl;
        cout << setw(25) << right << "reportsTo = " << emp.reportsTo << endl;
        cout << setw(25) << right << "jobTitle = " << emp.jobTitle << endl;
        cout << setw(25) << right << "city = " << emp.city << endl;
    }
    else
    {
        //if not exist
        cout << "Employee " << empNum << " does not exist." << endl;
    }
    
}

int findEmployee(Connection* conn, int employeeNumber, Employee* emp)
{
    int reportsTo = 0;
    bool found = false;
    //sql query
    string query = "SELECT employees.employeenumber, employees.lastname, employees.firstname, employees.email, offices.phone, employees.extension, employees.reportsto, employees.jobtitle, offices.city  FROM employees LEFT OUTER JOIN offices ON employees.officecode = offices.officecode WHERE employees.employeenumber = " + to_string(employeeNumber);
    
    Statement* stmt = conn->createStatement();
    ResultSet* t1 = stmt->executeQuery(query);
    //if is not empty

    if (t1->next())
    {
        //copying data from the data base 
        emp->employeeNumber = t1->getInt(1);
        strcpy(emp->lastName, t1->getString(2).c_str());
        strcpy(emp->firstName, t1->getString(3).c_str());
        strcpy(emp->email, t1->getString(4).c_str());
        strcpy(emp->phone, t1->getString(5).c_str());
        strcpy(emp->extension, t1->getString(6).c_str());
        strcpy(emp->jobTitle, t1->getString(8).c_str());
        strcpy(emp->city, t1->getString(9).c_str());
        //storing reports to of the employee to the variable
        reportsTo = t1->getInt(7);
        //if is not empty
        if (reportsTo != 0)
        {
            query = "SELECT lastname || \' \' || firstname as fullname FROM employees WHERE employeenumber like " + to_string(reportsTo);
            t1 = stmt->executeQuery(query);
            t1->next();
           
           
            //add numName to a reportsTo
            strcpy(emp->reportsTo, t1->getString(1).c_str());
        }
        else
        {
            emp->reportsTo[0] = 0;
        }
        
        found = true;
    }
    conn->terminateStatement(stmt);
    return found;
}

void deleteEmployee(Connection* conn, int employeeNumber)
{
    Statement* stmt = conn->createStatement();
    
    Employee emp;
    //if found
    if (findEmployee(conn, employeeNumber, &emp))
    {
        //sql query
        string query = "DELETE FROM employees WHERE employeenumber = " + to_string(employeeNumber);
        //perfome the query
        ResultSet* t1 = stmt->executeQuery(query);
        //output for the user
        cout << "The employee is deleted." << endl;
    }
    //if not found
    else
    {
        cout << "The employee does not exist." << endl;
    }
    conn->terminateStatement(stmt);
}
