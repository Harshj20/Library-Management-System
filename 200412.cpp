#include <iostream>
#include <list>
#include <string.h>
using namespace std;

#define EXIT 0
#define Logout 1
#define Available 1
#define NotAvailalbe -1
#define NotFound 0

//to calcualte no of days between two days
const int monthDays[12] = { 31, 28, 31, 30, 31, 30,
       31, 31, 30, 31, 30, 31 };

struct date {
    int d;
    int m;
    int y;
};

//used in book database of a user
struct book_with_date{
    string bName;
    struct date dt;
};

//used as a model for user data base
struct user{
    char name[20];
    string id;
    char password[20];
    int fine_amount;
    list<struct book_with_date> books;
};

// used as a model for books in bookDatabase
struct book{
    string bName;
    struct user*ptr;
    struct date dt;
};

//funciton to calculate leapyears
int countLeapYears(date d)
{
    int years = d.y;
    if (d.m <= 2)
        years--;
    return years / 4
           - years / 100
           + years / 400;
}

//to calculate absolute value of a number
int mod(int a){
    return (a>0)?a:-a;
}

//this function calculates no. of days between  2 dates
int getDays(date dt1, date dt2){
   long int n1 = dt1.y * 365 + dt1.d;

    for (int i = 0; i < dt1.m - 1; i++)
        n1 += monthDays[i];

    n1 += countLeapYears(dt1);
 
    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);
 
    return mod(n2 - n1);
}

//this function checks whether an id is valid or not
//returns true if id is invalid
//else returns false
bool check(string id)
{
    if(id.length() != 6)
        return true;
    if(id[0] != 'P' && id[0] != 'L' && (id[0] < '0' || id[0] > '9'))
        return true;
    for(int i = 1; i <= 5; i++)
    {
        if(id[i] < '0' || id[i] > '9')
            return true;
    }
    return false;
}

//this function prints the message for correct id format
void message_for_id_info(){
    cout << "\n\n\t####### LOGIN #######\n\n## NoTE :\n1. ID of a student is his Roll number.(ex: 200550)\n2. ID of a Professor is \"P\" followed by 5 digits.(ex: P00550)\n3. ID of a Librarian is \"L\" followed by 5 digits.(ex: L12343)\n" << endl;
}

//An object of this class stores all the user data
//only one object of this class is created in the main function which stores all the data while the program is running
class UserDataBase{
    list<struct user> Student;
    list<struct user> Professor;
    list<struct user> Librarian;
    public:
        void addUser(struct user a){
            struct user*u = search(a.id); //search function returns the pointer to the user who is being searched
            if(u == NULL){                //if no such user is found then NULL is returned
            a.fine_amount = 0;
            if(a.id[0] == 'P')
                Professor.push_back(a);
            else if(a.id[0] == 'L')
                Librarian.push_back(a);
            else
                Student.push_back(a);
            cout << "\nUser Successfully added\n" << endl;
            }
            else
                cout << "\nUser with this id already Present!!!!\n" << endl;
        }
        void deleteUser(string id){
            list<struct user>*temp;
            if(id[0] == 'P')
                temp = &Professor;
            if(id[0] == 'L')
                temp = &Librarian;            
            else
                temp = &Student;
            for(auto i = temp->begin(); i != temp->end(); i++)
                {
                    if(i->id == id)
                    {
                        if(i->fine_amount > 0)
                        cout<<"\nPlease clear the dues before deleting the user\n"<<endl;
                        else if(i->books.size()>0)
                        cout <<"\nPlease return the borrowed books before deleting the user\n"<<endl;
                        else{
                        temp->erase(i);
                        cout << "\n User Deleted\n" << endl;
                        }
                        return;
                    }
                }
            cout << "\n################\nNo such user present" << endl;
        }
        //list iterator iterates through a list given
        void list_iterator(list<struct user> u){   
            int j = 1;
            for(auto i = u.begin(); i != u.end(); i++, j++)
                cout <<j<<". "<< i->name << " (" << i->id << ")"<< endl;
        }
        //this function calls the list_iterator function for all 3 user lists
        void listAllUsers(){
            cout << "\nLibrarians :" << endl;
            list_iterator(Librarian);
            cout << "\nProfessors :" << endl;
            list_iterator(Professor);
            cout << "\nStudents :" << endl;
            list_iterator(Student);
        }

        //returns NULL if no user is found
        struct user* search(string id){
            list<struct user>* temp;

            if(id[0] == 'P')
                temp = &Professor;
            else if(id[0] == 'L')
                temp = &Librarian;            
            else
                temp = &Student;
            for(auto i = temp->begin(); i != temp->end(); i++)
                {
                    if(i->id == id)
                    {
                        return &(*i);
                    }
                }
                return NULL;
        }     
        //returns the pointer to the user after authentication during login 
        struct user* authenticate(string id, char*password){
            list<struct user>* temp;

            if(id[0] == 'P')  
                temp = &Professor;
            else if(id[0] == 'L')
                temp = &Librarian;            
            else
                temp = &Student;
            for(auto i = temp->begin(); i != temp->end(); i++)
                {
                    if(i->id == id && !strcmp(i->password,password))
                    {
                        return &*i;
                    }
                }
                return NULL;
        }
            
};

class BookDataBase{
    list<struct book>Books;
    public:
        struct book b;
        struct book_with_date bdt;
        void addBook(string name){
            if(checkAvailability(name) == NotFound){
            b.bName = name;
            b.ptr = NULL;
            Books.push_back(b);
            }
            else
            cout << "\nBook Already Present\n" << endl;
        }
        void deleteBook(string name){
            for(auto i = Books.begin(); i != Books.end(); i++){
                if(i->bName == name)
                {
                    if(i->ptr){
                    cout << "\nThis book is issued to someone. To delete the book, first update its info\n" << endl;
                    return;
                }
                    else{
                        Books.erase(i);
                        cout << "\nBook deleted!!!!\n" << endl;
                        return;
                    }
                }
            } cout<< "\nBook not Found\n" << endl;
        }
        void Display(){
            int j = 1;
            for(auto i = Books.begin(); i != Books.end(); i++,j++)
            cout <<j<<". "<< i->bName << endl;
        }
         int search(string name){
            for(auto i = Books.begin(); i != Books.end(); i++){
                if(i->bName == name)
                {
                    cout << "\nThe book is issued to : " << endl;
                    if(i->ptr)
                    cout << i->ptr->name << "\n" <<endl;
                    else 
                    cout << "No one\n" << endl;
                    return Available;
                }
            }cout << "\nBook not found!!!!!!!!\n" << endl;
            return NotFound;
        }
        int checkAvailability(string name){
            for(auto i = Books.begin(); i != Books.end(); i++){
                if(i->bName == name){
                    if(i->ptr)
                        return NotAvailalbe;                    
                    else 
                        return Available;
                }
            }
            return NotFound;
        }
        //u is NULL if no user is found
        void update(struct user*u, string name){
            for(auto i = Books.begin(); i != Books.end(); i++){
                if(i->bName == name){
                    //if book is found in the data base
                    // u != NULL means u points to the user we are issuing book to and i->ptr points to the user the book is already issued to 
                    if(u != NULL && i->ptr != NULL)
                        cout << "\nThis Book is already issued to someone!!!!!!\n" << endl;
                    //this condition is true when an available book is being issued to somenone
                    else if(u != NULL){
                        //condition that a student can borrow atmost 5 books
                        if(u->id[0] >= '0' && u->id[0] <= '9' && u->books.size() >= 5)
                            cout << "\nStudent can issue at most 5 books\n"<<endl;
                        else{
                        cout << "Issue Date in the format date<space>month<space>year (eg: 2 1 2017): ";
                        cin >> i->dt.d >> i->dt.m >> i->dt.y;
                        i->ptr = u;
                        bdt.bName = name;
                        bdt.dt = i->dt;
                        u->books.push_back(bdt);
                        cout << "\nBooK \""<<name<<"\" issued to " << u->name<< "\n"<<endl;
                        }
                    }
                    //this condition is true when a book is being returned
                    else if(i->ptr != NULL){
                        struct date dt;
                        cout << "Return Date in the format date<space>month<space>year (eg: 2 1 2017): ";
                        cin >> dt.d >> dt.m >> dt.y;
                        int n = getDays(i->dt, dt);
                        if(i->ptr->id[0] == 'P') n = (n>60)?(n-60)*5:0;
                        else n = (n>30)?(n-30)*2:0;
                        i->ptr->fine_amount += n;
                        for(auto j = i->ptr->books.begin(); j != i->ptr->books.end(); j++){
                            if(j->bName == name){
                                i->ptr->books.erase(j);   
                                break;                     
                            }
                        }
                        i->ptr = NULL;
                        cout << "\nBook returned\n" << endl;
                    }
                    else{
                        cout<<"\nTask failed!!!\n"<<endl;
                    }
                    u = NULL;
                    return;
                }
            }
            cout << "\n######## please Enter Correct Book Name #########\n" << endl;
        }
        
};

class User{
    protected:
        char name[20];
        string id;
        char password[20];
    public:
        User(struct user _){
            strcpy(this->name, _.name);
            this->id = _.id;
            strcpy(this->password, _.password);
        }
};

class Professor:public User{
    int fine_amount;
    list<struct book_with_date>Books;

    public:
        Professor(struct user x):User(x){
            this->fine_amount = x.fine_amount;
            this->Books = x.books;
        }
        void Calculate_fine(){
            struct date dt;
            cout << "Enter current date in the format date<space>month<space>year (eg: 2 1 2017): ";
            cin >> dt.d >> dt.m >> dt.y;
            cout <<"\nPrevious amount is: "<<this->fine_amount<<endl;
            cout<<"\nCurrent fine amout on each book is:\n";
            int j = 1;
            for(auto i = this->Books.begin(); i != this->Books.end(); i++)
            {
                    int n = getDays(i->dt, dt);
                    int y = (n>60)?(n-60)*5:0;
                    if(y > 0)
                    cout<<j<<". "<<i->bName<<": "<<y<<"Rs due date was "<< (y/5) <<" days ago"<<endl;
                    else
                    cout<<j<<". "<<i->bName<<": "<<y<<"Rs due date is in "<< (60-n) <<" days"<<endl;
            }       
        }
        void display(){
            int j = 1;
            for(auto i = Books.begin(); i != Books.end(); i++, j++)
                cout <<j<<". "<< i->bName<<" , issued on "<<i->dt.d<<"/"<<i->dt.m<<"/"<<i->dt.y<< endl;
        } 
        void checkBook(BookDataBase*bdb){
            string name;
            cout << "Enter Book name (no spaces): ";
            cin >> name;
            int result = bdb->checkAvailability(name);
            if(result == NotFound)
                cout << "\nBook not found!!!\nEnter choice 2 to view list of books\n"<<endl;
            else if(result == NotAvailalbe)
                cout << "\nBook is not available\n" << endl;
            else
                cout << "\nBook is available\n" << endl;
        }
};

class Student:public User{
    int fine_amount;
    list<struct book_with_date> Books;

    public:
        Student(struct user x):User(x){
            this->fine_amount = x.fine_amount;
            this->Books = x.books;
        }
        int Calculate_fine(){
            struct date dt;
            cout << "Enter current date in the format date<space>month<space>year (eg: 2 1 2017): ";
            cin >> dt.d >> dt.m >> dt.y;
            cout <<"\nPrevious amount is: "<<this->fine_amount<<endl;
            cout<<"\nCurrent fine amout on each book is:\n";
            int j = 1;
            for(auto i = this->Books.begin(); i != this->Books.end(); i++)
            {
                    int n = getDays(i->dt, dt);
                    int y = (n>30)?(n-30)*2:0;
                    if(y>0)
                    cout<<j<<". "<<i->bName<<": "<<y<<"Rs due date was "<< (y/2) <<" days ago"<<endl;
                    else
                    cout<<j<<". "<<i->bName<<": "<<y<<"Rs due date is in "<< (30-n)<<" days"<<endl;
            }       
        }
        void display(){
            int j = 1;
            for(auto i = Books.begin(); i != Books.end(); i++, j++)
                cout <<j<<". "<< i->bName<<" , issued on "<<i->dt.d<<"/"<<i->dt.m<<"/"<<i->dt.y<< endl;
        } 
        void checkBook(BookDataBase*bdb){
            string name;
            cout << "Enter Book name (no spaces): ";
            cin >> name;
            int result = bdb->checkAvailability(name);
            if(result == NotFound)
                cout << "\nBook not found!!!\nEnter choice 2 to view list of books\n"<<endl;
            else if(result == NotAvailalbe)
                cout << "\nBook is not available\n" << endl;
            else
                cout << "\nBook is available\n" << endl;
        }
};

class Librarian:public User{
    struct user a; //there are some temporary variables to facilitate operations
    struct book b;
    struct user*u;
    public:
        Librarian(struct user x):User(x){};
        void addUser(UserDataBase*udb){
            cout << "Enter Name (no Spaces): ";
            cin>> a.name;
            cout << "Enter User ID: ";
            cin >> a.id;
            cout << "Enter Password: ";
            cin >> a.password;
            if(check(a.id))
                message_for_id_info();
            else
                udb->addUser(a);
        }
        void deleteUser(UserDataBase*udb){
            cout << "Enter User ID: ";
            cin >> a.id;
            cout << "Enter your Password to confirm deletion: ";
            cin >> a.password;
            cout << this->id<<" "<<this->password<<endl;
            if(a.id == this->id)
                cout << "\nCannot delete your own account!!!\n" << endl;
            else if(strcmp(a.password, this->password))
                cout << "\nTask failed !!!!!\n";
            else
                udb->deleteUser(a.id);
        }
        void addBook(BookDataBase*bdb){
            cout << "Enter Book name (no spaces): ";
            cin >> b.bName;
            bdb->addBook(b.bName);
        }
        void deleteBook(BookDataBase*bdb){
            cout << "Enter Book name (no spaces): ";
            cin >> b.bName;
            cout << "Enter your password for confirmation: ";
            cin >> a.password;
            if(strcmp(a.password, this->password))
                cout << "\nTask failed !!!!!\n";
            else
                bdb->deleteBook((b.bName));
        }
        void issueBook(UserDataBase*udb, BookDataBase*bdb){
            cout << "Enter Book name (no spaces): ";
            cin >> b.bName;
            if(bdb->search(b.bName) == Available){
            cout << "Enter id of the person you want to assign it to (enter 0 if the Book is being returned): ";
            cin >> a.id;
            u = udb->search(a.id);
            bdb->update(u, b.bName);
            u = NULL;
            }
        }
        void ListBooksOfUser(UserDataBase*udb){
            cout << "Enter user ID : ";
            cin >> a.id;
            u = udb->search(a.id);
            if(u != NULL){
                int j = 1;
                for(auto i = u->books.begin(); i != u->books.end(); i++, j++)
                cout <<j<<". "<< i->bName<<" , issued on "<<i->dt.d<<"/"<<i->dt.m<<"/"<<i->dt.y<< endl;
            }
            else
            cout << "\n No such user\n" << endl;
            u = NULL;
        }
        void findOwner(BookDataBase*bdb){
            cout << "Enter Book name (no spaces) : ";
            cin >> b.bName;
            bdb->search(b.bName);
        }
        void clear_fine_amount(UserDataBase*udb){
            cout<<"Enter id of user whose fine is being cleared: ";
            cin>>a.id;
            u = udb->search(a.id);
            if(u != NULL){
            cout<<"Fine amount is: "<<u->fine_amount<<endl;
            cout<<"Amount paid is: ";
            cin>>a.fine_amount;
            if(u->fine_amount < a.fine_amount)
            cout<<"\n!!Amount being paid is greater than fine!!\n";
            else{
                u->fine_amount -= a.fine_amount;
                cout<<"fine due is: "<<u->fine_amount<<endl;
            }
            u = NULL;
            }
            else
            cout<<"\nUser Not Found!!!\n"<<endl;
        }

};



int WelcomeProfessor(struct user*u, BookDataBase*bdb){
    int choice = 0;
    Professor P(*u);
    do{
    cout << "\nEnter the item number to select that choice:" <<endl;
    cout << "0. EXIT the program" <<endl;
    cout << "1. LOGOUT" <<endl;
    cout << "2. View List of Books" << endl;
    cout << "3. View My Books" << endl;
    cout << "4. Check Availability of a book" << endl;
    cout << "5. Calculate fine Amount"<<endl; 
    cout << "\nEnter your Choice: " <<endl;
    cin >> choice;
    switch(choice){
        case 0:
            return EXIT;
        case 1:
            return Logout;
        case 2:
            bdb->Display();
            break;
        case 3:
            P.display();
            break;
        case 4:
            P.checkBook(bdb);
            break;
        case 5:
            P.Calculate_fine();
            break;
        default:
            cout << "\nPlease Enter correct choice number" << endl;
    }
    }while(1);
}
int WelcomeLibrarian(struct user*u, UserDataBase*udb, BookDataBase*bdb){
    int choice = 0;
    Librarian L(*u);
    do{
    cout << "\nEnter the item number to select that choice:" <<endl;
    cout << "0. EXIT the program" <<endl;
    cout << "1. LOGOUT" <<endl;
    cout << "2. ADD a user " <<endl;
    cout << "3. DELETE a user" <<endl;
    cout << "4. Clear fine amount" <<endl;
    cout << "5. ADD a book" <<endl;
    cout << "6. DELETE a book" <<endl;
    cout << "7. UPDATE book info" <<endl;
    cout << "8. List Down All users" <<endl;
    cout << "9. List Down all Books" <<endl;
    cout << "10. List Down Books issued to a user" <<endl;
    cout << "11. See which Book is issued to which user\n\nEnter your Choice: " <<endl;
    cin >> choice;
    switch(choice){
        case 0:
            return EXIT;
        case 1:
            return Logout;
        case 2:
            L.addUser(udb);
            break;
        case 3:
            L.deleteUser(udb);
            break;
        case 4:
            L.clear_fine_amount(udb);
            break;
        case 5:
            L.addBook(bdb);
            break;
        case 6:
            L.deleteBook(bdb);
            break;
        case 7:
            L.issueBook(udb, bdb);
            break;
        case 8:
            udb->listAllUsers();
            break;
        case 9:
            bdb->Display();
            break;
        case 10:
            L.ListBooksOfUser(udb);
            break;
        case 11:
            L.findOwner(bdb);
            break;
           default:
              cout << "Please Enter correct choice number" << endl;
    }
    }while(1);
}
int WelcomeStudent(struct user*u, BookDataBase*bdb){
    int choice = 0;
    Student S(*u);
    do{
    cout << "\nEnter the item number to select that choice:" <<endl;
    cout << "0. EXIT the program" <<endl;
    cout << "1. LOGOUT" <<endl;
    cout << "2. View List of Books" << endl;
    cout << "3. View My Books" << endl;
    cout << "4. Check Availability of a book" << endl;
    cout << "5. Calculate fine Amount"<<endl; 
    cout << "\nEnter your Choice: " <<endl;
    cin >> choice;
    switch(choice){
        case 0:
            return EXIT;
        case 1:
            return Logout;
        case 2:
            bdb->Display();
            break;
        case 3:
            S.display();
            break;
        case 4:
            S.checkBook(bdb);
            break;
        case 5:
            S.Calculate_fine();
            break;
        default:
            cout << "\nPlease Enter correct choice number" << endl;
    }
    }while(1);
}


int Login(UserDataBase*udb, BookDataBase*bdb){
    string id;
    int choice;
    char password[20];
    cout << "Enter 0 to terminate the program.\nEnter 1 to go to Login Page\nPLEASE NOTE : The database contain only one user - \"Librarian\" \nYou can login as a librarian to create more users and add books and then logout to login again as a different user\nThe Login credentials of the Librarian is :\nName : Mr.X\nUser ID : L11111\nPassword : helloworld\nEnter your choice:"<< endl;
    cin >> choice;
    if(choice == 0)
    return 0;

    message_for_id_info();

    do{
        if(id != ""){
            cout << "\nPlease enter a valid id of 6 characters." << endl; 
        }
    cout << "Enter User ID: " << endl;
    cin >> id;
    }while(check(id));
    cout << "Enter Password: " << endl;
    cin >> password;

    
    // Authenticate
    struct user*s  = udb->authenticate(id, password);
    if(s == NULL)
    return -1;

    cout << "##########################\n\tWELCOME " << s->name << "!!!" << endl;

    if(id[0] == 'L')
        return WelcomeLibrarian(s, udb, bdb);
    if(id[0] == 'P')
        return WelcomeProfessor(s, bdb);  
    return WelcomeStudent(s, bdb);
    
}

int main()
{
    struct user u = {
        "Mr. X",
        "L11111",
        "helloworld"
    };
    UserDataBase udb;
    udb.addUser(u);
    BookDataBase bdb;
    int choice = 0;
    // choice = 0 -> EXIt the program
    // choice = -1 -> Wrong credentials | Login again
    // choice = 1 -> Logout

    // struct user*s = udb.search("L11111");
    // cout << s->name;
    // strcpy(s->name ,"hi there");
    // cout << s->name;
    // s = udb.search("L11111");
    // cout << s->name;
    // int j = 1;
    // for(auto i = s->books.begin(); i != s->books.end(); i++)
    // cout<<j<<". " << *i <<endl;
    do{
    if(choice == -1)
        cout << "\n#############\nUser NOT FOUND\nPLease Enter Correct Credentials\n" << endl;
    if(choice == 1)
        cout << "\n SUCCESSFULLY LOGGED OUT \n" << endl;
    choice = Login(&udb, &bdb);
    if(choice == 0)
        break;
    }while(choice == -1 || choice == 1);
    return 0;
}