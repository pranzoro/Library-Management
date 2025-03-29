#include <bits/stdc++.h>
using namespace std;
#define nl '\n' // It's easier for me to use this #define instead of writing '\n' each time.
class Book
{
private:
    int copiesAvailable;
    int totalCopies;
    string isbn;

public:
    string title;
    string author;
    // No - args constructor and Default Parameterized Constructor.
    Book() : copiesAvailable(0), totalCopies(5), isbn("ISBN"), title("UnknownTitle"), author("UnknownAuthor") {}
    Book(const int &avalcop, const int &totcop, const string &bkisbn, const string &bktitle, const string &bkauthor) : copiesAvailable(avalcop), totalCopies(totcop), isbn(bkisbn), title(bktitle), author(bkauthor) {}
    // Copy constructor taking object and isbn as input.
    Book(const Book &b1, const string &isbnNew) : copiesAvailable(b1.copiesAvailable), totalCopies(b1.totalCopies), isbn(isbnNew), title(b1.title), author(b1.author) {}
    string getISBN(void); // Returns isbn of the book.
    int getTotalCopies(void); // Returns total copies of the book.
    int getAvailableCopies(void); // Returns available copies of the book.
    void updateCopies(const int &count); // Adds the provided count to total copies and available copies of the book.
    bool borrowBook(void); // Returns true if the book can be borrow-able, false elsewise.
    bool returnBook(void); // Returns true if the book can be returned back, false elsewise.
    void printDetails(void);
};

class Member
{
private:
    string memberID;
    map<string, int> borrowedBooks; // Frequency map of borrowed books' isbn to their frequency.
    int borrowLimit;
    int borrowedCount;

public:
    string name;
    // No-args Constructor and Default Parameterized Constructors.
    Member() : borrowLimit(3), borrowedCount(0) {}
    Member(string memid, string memname, int limit) : memberID(memid), borrowLimit(limit), borrowedCount(0), name(memname) {}
    Member(string memid, string memname) : memberID(memid), borrowLimit(3), borrowedCount(0), name(memname) {}

    string getMemberID(void);            // Returns memberID.
    bool borrowBook(const string &isbn); // Checks if the member can borrow the book of the provided isbn.
    bool returnBook(const string &isbn); // Checks if the member can return back the book of the provided isbn.
    void printDetails(void);
};

class Library
{
private:
    vector<Book> books;
    vector<Member> members;

public:
    
    bool addBook(Book &newbk); // Adds the book into the vector of books if the isbn is not present.
    bool registerMember(Member &newmem); // Registers the member into the vector of members if the member with memberID is not present.
    Book &getBook(const string &isbn); // Returns a lvalue reference of the book with given isbn.
    Member &getMember(const string &memberID); // Returns a lvalue reference of the book with given memberID.
    bool borrowBook(const string &memberID, const string &isbn); // Checks first wrt book and then wrt member if the book can be borrow-able.
    bool returnBook(const string &memberID, const string &isbn); // Checks first wrt book and then wrt member if the book can be returned back.
    void printLibraryDetails(void);
};

string Book::getISBN(void)
{
    return isbn;
}

int Book::getTotalCopies(void)
{
    return totalCopies;
}

int Book::getAvailableCopies(void)
{
    return copiesAvailable;
}

void Book::updateCopies(const int &count)
{
    if (count + copiesAvailable >= 0 and count + totalCopies >= 0)
    {
        copiesAvailable += count;
        totalCopies += count;
        return;
    }
    cout << "Invalid request! Count becomes negative" << nl;
}

bool Book::borrowBook(void)
{
    if (copiesAvailable)
    {
        copiesAvailable--;
        return true;
    }
    cout << "Invalid request! Copy of book not available" << nl;
    return false;
}

bool Book::returnBook(void)
{
    if (copiesAvailable < totalCopies)
    {
        copiesAvailable++;
        return true;
    }
    cout << "Invalid request! Copy of book exceeds total copies" << nl;
    return false;
}

void Book::printDetails(void)
{
    cout << title << " " << author << nl;
}


string Member::getMemberID(void)
{
    return memberID;
}

bool Member::borrowBook(const string &isbn)
{
    if (borrowedCount < borrowLimit)
    {
        borrowedBooks[isbn]++;
        borrowedCount++;
        return true;
    }
    cout << "Invalid request! Borrow limit exceeded" << nl;
    return false;
}

bool Member::returnBook(const string &isbn)
{
    if (borrowedBooks[isbn])
    {
        borrowedBooks[isbn]--;
        borrowedCount--;
        return true;
    }
    cout << "Invalid request! Book not borrowed" << nl;
    return false;
}

void Member::printDetails(void)
{
    for (auto &book : borrowedBooks)
    {
        if(book.second)
            cout << memberID << " " << name << " " << book.first << " " << book.second << nl;
    }
}


bool Library::addBook(Book &newbk)
{
    for (auto &book : books)
    {
        if (book.getISBN() == newbk.getISBN())
        {
            cout << "Invalid request! Book with same isbn already exists" << nl;
            return false;
        }
    }
    books.push_back(newbk);
    return true;
}

bool Library::registerMember(Member &newmem)
{
    for (auto &mem : members)
    {
        if (newmem.getMemberID() == mem.getMemberID())
        {
            cout << "Invalid request! Member with same id already exists" << nl;
            return false;
        }
    }
    members.push_back(newmem);
    return true;
}

Book &Library::getBook(const string &isbn)
{
    for (auto &book : books)
    {
        if (book.getISBN() == isbn)
            return book;
    }
    static Book dummy;
    return dummy;
}

Member &Library::getMember(const string &memberID)
{
    for (auto &mem : members)
    {
        if (mem.getMemberID() == memberID)
            return mem;
    }
    static Member dummy;
    return dummy;
}

bool Library::borrowBook(const string &memberID, const string &isbn)
{
    Book &reqBook = getBook(isbn);
    if (reqBook.borrowBook())
    {
        Member &reqMem = getMember(memberID);
        if (reqMem.borrowBook(isbn))
        {
            return true;
        }
        reqBook.returnBook();
        return false;
    }
    return false;
}

bool Library::returnBook(const string &memberID, const string &isbn)
{
    Book &reqBook = getBook(isbn);
    if (reqBook.returnBook())
    {
        Member &reqMember = getMember(memberID);
        if (reqMember.returnBook(isbn))
        {
            return true;
        }
        reqBook.borrowBook();
        return false;
    }
    return false;
}

void Library::printLibraryDetails(void)
{
    for (auto &book : books)
    {
        cout << book.title << " " << book.author << " " << book.getAvailableCopies() << nl;
    }

    for (auto &mem : members)
    {
        cout << mem.getMemberID() << " " << mem.name << nl;
    }
}


int main(void)
{
    Library lib;
    string command;
    cin >> command;
    while (command != "Done")
    {
        if (command == "Book")
        {
            string s1;
            cin >> s1;
            if (s1 == "None")
            {
                Book o1;
                lib.addBook(o1);
            }
            else if (s1 == "ExistingBook")
            {
                string isbnOld, isbnNew;
                cin >> isbnOld >> isbnNew;
                Book o1(lib.getBook(isbnOld), isbnNew);
                lib.addBook(o1);
            }
            else
            {
                // s1 is title
                string authorInput, isbnInput;
                int copiesAvailableInput, totalCopiesInput;
                cin >> authorInput >> isbnInput >> copiesAvailableInput >> totalCopiesInput;
                Book o1(copiesAvailableInput, totalCopiesInput, isbnInput, s1, authorInput);
                lib.addBook(o1);
            }
        }
        else if (command == "UpdateCopiesCount")
        {
            string isbnInput;
            int newCount;
            cin >> isbnInput >> newCount;
            Book &o1 = lib.getBook(isbnInput);
            o1.updateCopies(newCount);
        }
        else if (command == "Member")
        {
            string s1;
            cin >> s1;
            if (s1 == "NoBorrowLimit")
            {
                string memberIDInput, nameInput;
                cin >> memberIDInput >> nameInput;
                Member m1(memberIDInput, nameInput);
                lib.registerMember(m1);
            }
            else
            {
                // s1 is memberid
                string nameInput;
                int borrowLimitInput;
                cin >> nameInput >> borrowLimitInput;
                Member m1(s1, nameInput, borrowLimitInput);
                lib.registerMember(m1);
            }
        }
        else if (command == "Borrow")
        {
            string memberIDInput, isbnInput;
            cin >> memberIDInput >> isbnInput;
            lib.borrowBook(memberIDInput, isbnInput);
        }
        else if (command == "Return")
        {
            string memberIDInput, isbnInput;
            cin >> memberIDInput >> isbnInput;
            lib.returnBook(memberIDInput, isbnInput);
        }
        else if (command == "PrintBook")
        {
            string isbnInput;
            cin >> isbnInput;
            Book &o1 = lib.getBook(isbnInput);
            o1.printDetails();
        }
        else if (command == "PrintMember")
        {
            string memberIDInput;
            cin >> memberIDInput;
            Member &m1 = lib.getMember(memberIDInput);
            m1.printDetails();
        }
        else if (command == "PrintLibrary")
        {
            lib.printLibraryDetails();
        }
        cin >> command;
    }
    return 0;
}