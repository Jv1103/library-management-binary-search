# library-management-binary-search
Library Management System using Binary Search

A console-based mini-project in C for the Data Structures course (CIC-209). It manages a library's books and uses binary search as the core searching technique.

Objectives
Apply binary search to a real-world problem (finding books quickly).
Understand why keeping data sorted makes searching O(log n) instead of O(n).
Practice arrays, structures, sorting, and file handling in C.
Features
#	Feature	How binary search is used
1	Add a book	Binary search (lower bound) finds the correct sorted position; rejects duplicate IDs
2	Search by Book ID	Classic iterative binary search
3	Search by title / starting letters	Binary search on a title-sorted index (case-insensitive prefix match)
4	Issue a book	Locates the book by ID, reduces available copies
5	Return a book	Locates the book by ID, increases available copies
6	Delete a book	Locates the book by ID (blocked if copies are issued)
7	Display all books	Always in sorted ID order
0	Save & exit	Data persists in books.txt between runs
Data Structures Used
Array of structures (Book lib[]) kept permanently sorted by Book ID.
Structure (Book): id, title, author, total copies, available copies.
Array of pointers (Book *idx[]) sorted by title with qsort, used for title search without disturbing the main array.
File (text) for persistent storage; records are loaded via sorted insertion.
Algorithms and Complexity
Operation	Time complexity
Search by ID (binary search)	O(log n)
Find insert position (lower bound)	O(log n)
Insert / Delete (shifting elements)	O(n)
Search by title (sort index + binary search)	O(n log n)
Display all	O(n)

Space complexity: O(n).

How to Run
bash
gcc -o library library.c
./library          # on Windows: library.exe

Sample data is provided in books.txt (format: id|title|author|total|available).

Project Structure
library-management-binary-search/
├── library.c     # complete source code
├── books.txt     # sample data (auto-updated on exit)
└── README.md
Sample Run
===== LIBRARY MANAGEMENT (Binary Search) =====
Choice: 2
Book ID to search: 110

ID     Title                            Author                 Total Avail
110    The C Programming Language       Kernighan and Ritchie      4     4
Limitations and Future Scope
Fixed capacity of 500 books (could use dynamic memory with realloc).
Insert/delete cost O(n); a balanced BST or B-tree would make these O(log n).
Could add member records, due dates, and fines.
Author

Jagratt, B.Tech IT, MAIT (GGSIPU), Delhi
