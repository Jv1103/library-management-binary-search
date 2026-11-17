
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 500
#define DATA_FILE "books.txt"

typedef struct {
    int  id;
    char title[60];
    char author[40];
    int  total;      
    int  available;  
} Book;

static Book lib[MAX_BOOKS];   
static int  count = 0;

static int icmpn(const char *a, const char *b, int n) {
    int i = 0;
    while ((n < 0 || i < n) && *a && *b) {
        int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++; i++;
    }
    if (n >= 0 && i == n) return 0;
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

static void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (!fgets(buf, size, stdin)) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\n")] = '\0';
}

static int readInt(const char *prompt) {
    char buf[32];
    readLine(prompt, buf, sizeof buf);
    return atoi(buf);
}

static void printHeader(void) {
    printf("\n%-6s %-32s %-22s %5s %5s\n", "ID", "Title", "Author", "Total", "Avail");
    printf("---------------------------------------------------------------------------\n");
}

static void printBook(const Book *b) {
    printf("%-6d %-32.32s %-22.22s %5d %5d\n",
           b->id, b->title, b->author, b->total, b->available);
}



static int binarySearchById(int id) {
    int lo = 0, hi = count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (lib[mid].id == id) return mid;
        if (lib[mid].id < id)  lo = mid + 1;
        else                   hi = mid - 1;
    }
    return -1;
}


static int lowerBound(int id) {
    int lo = 0, hi = count;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (lib[mid].id < id) lo = mid + 1;
        else                  hi = mid;
    }
    return lo;
}


static int insertBook(Book b) {
    if (count >= MAX_BOOKS) return 2;
    int pos = lowerBound(b.id);
    if (pos < count && lib[pos].id == b.id) return 1;
    for (int i = count; i > pos; i--) lib[i] = lib[i - 1];  
    lib[pos] = b;
    count++;
    return 0;
}

static void addBook(void) {
    Book b;
    b.id = readInt("Book ID: ");
    if (b.id <= 0) { printf("Invalid ID.\n"); return; }
    readLine("Title : ", b.title, sizeof b.title);
    readLine("Author: ", b.author, sizeof b.author);
    b.total = readInt("Copies: ");
    if (b.total <= 0) { printf("Copies must be positive.\n"); return; }
    b.available = b.total;

    int r = insertBook(b);
    if (r == 0)      printf("Book added.\n");
    else if (r == 1) printf("A book with ID %d already exists.\n", b.id);
    else             printf("Library is full.\n");
}

static void deleteBook(void) {
    int id  = readInt("Book ID to delete: ");
    int pos = binarySearchById(id);
    if (pos < 0) { printf("Book not found.\n"); return; }
    if (lib[pos].available != lib[pos].total) {
        printf("Cannot delete: some copies are issued.\n");
        return;
    }
    for (int i = pos; i < count - 1; i++) lib[i] = lib[i + 1]; 
    count--;
    printf("Book deleted.\n");
}

static void searchById(void) {
    int id  = readInt("Book ID to search: ");
    int pos = binarySearchById(id);
    if (pos < 0) { printf("Book not found.\n"); return; }
    printHeader();
    printBook(&lib[pos]);
}

static int cmpTitle(const void *a, const void *b) {
    const Book *x = *(const Book *const *)a, *y = *(const Book *const *)b;
    return icmpn(x->title, y->title, -1);
}


static void searchByTitle(void) {
    char prefix[60];
    readLine("Title (or starting letters): ", prefix, sizeof prefix);
    int plen = (int)strlen(prefix);
    if (plen == 0 || count == 0) { printf("Nothing to search.\n"); return; }

    Book *idx[MAX_BOOKS];
    for (int i = 0; i < count; i++) idx[i] = &lib[i];
    qsort(idx, count, sizeof idx[0], cmpTitle);

    int lo = 0, hi = count;                    
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (icmpn(idx[mid]->title, prefix, plen) < 0) lo = mid + 1;
        else                                          hi = mid;
    }
    if (lo >= count || icmpn(idx[lo]->title, prefix, plen) != 0) {
        printf("No matching title.\n");
        return;
    }
    printHeader();
    for (int i = lo; i < count && icmpn(idx[i]->title, prefix, plen) == 0; i++)
        printBook(idx[i]);
}

static void issueBook(void) {
    int pos = binarySearchById(readInt("Book ID to issue: "));
    if (pos < 0) { printf("Book not found.\n"); return; }
    if (lib[pos].available == 0) { printf("No copies available right now.\n"); return; }
    lib[pos].available--;
    printf("Issued \"%s\". Copies left: %d\n", lib[pos].title, lib[pos].available);
}

static void returnBook(void) {
    int pos = binarySearchById(readInt("Book ID to return: "));
    if (pos < 0) { printf("Book not found.\n"); return; }
    if (lib[pos].available >= lib[pos].total) {
        printf("All copies are already in the library.\n");
        return;
    }
    lib[pos].available++;
    printf("Returned \"%s\". Copies available: %d\n", lib[pos].title, lib[pos].available);
}

static void displayAll(void) {
    if (count == 0) { printf("Library is empty.\n"); return; }
    printHeader();
    for (int i = 0; i < count; i++) printBook(&lib[i]);
    printf("Total titles: %d\n", count);
}


static void loadFromFile(void) {
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) return;
    Book b;
    while (fscanf(f, "%d|%59[^|]|%39[^|]|%d|%d\n",
                  &b.id, b.title, b.author, &b.total, &b.available) == 5)
        insertBook(b);         
    fclose(f);
}

static void saveToFile(void) {
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) { printf("Could not save data.\n"); return; }
    for (int i = 0; i < count; i++)
        fprintf(f, "%d|%s|%s|%d|%d\n", lib[i].id, lib[i].title,
                lib[i].author, lib[i].total, lib[i].available);
    fclose(f);
}


int main(void) {
    loadFromFile();
    int choice;
    do {
        printf("\n===== LIBRARY MANAGEMENT (Binary Search) =====\n"
               "1. Add book\n2. Search by ID\n3. Search by title\n"
               "4. Issue book\n5. Return book\n6. Delete book\n"
               "7. Display all books\n0. Save & exit\n");
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: addBook();      break;
            case 2: searchById();   break;
            case 3: searchByTitle();break;
            case 4: issueBook();    break;
            case 5: returnBook();   break;
            case 6: deleteBook();   break;
            case 7: displayAll();   break;
            case 0: saveToFile(); printf("Data saved. Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}
