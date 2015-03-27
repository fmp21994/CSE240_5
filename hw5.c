#include <stdio.h>
#include <string.h>
#include <ctype.h>
#pragma warning(disable: 4996)
#define max 100
#define myDatabase "myDatabase.txt"
typedef enum {diploma, bachelor, master, doctor} education;

struct person {				       // a node to hold personal details
    char name[30];
    char email[30];
    int phone;
    education degree;
};

struct person directory[max];	             // an array of structures, 100 entries
int tail = 0;					      // global variable

void flush();					      // forward declaration of functions
void branching(char c);
void sort(char* email, char* name, education level, int phone);
void saveFile(char* fileName);
void loadFile(char* fileName);
int insertion();
int print_person(int i);
int print_all();
int search_person();
int delete_person();


int main() {
    loadFile(myDatabase); // Loads from the file referenced in the parameter
    // print a menu for selection
    char ch = 'i';
    
    ungetc('\n', stdin); // inject the newline character into input buffer
    
    do {
        printf("Enter your selection\n");
        printf("\ti: insert a new entry\n");
        printf("\td: delete an entry\n");
        printf("\ts: search an entry\n");
        printf("\tp: print all entries\n");
        printf("\tq: quit \n");
        
        flush();	// flush the input buffer. To be discussed later
        ch = tolower(getchar());
        branching(ch);
    } while (ch != 113);
    saveFile(myDatabase); // Saves to the file referenced in the parameter
        return 0;
}
void saveFile(char* fileName)
{
    // writes the array into the file that's name is passed through to this function.
    FILE* fileBuffer;
    fileBuffer = fopen(fileName, "wb");
    if(fileBuffer != NULL)
    {
        fwrite(&tail, sizeof(tail), 1, fileBuffer);
        for (int i = 0; i < tail; i++) {
            fwrite(directory[i].name, sizeof(directory[i].name), 1, fileBuffer);
            fwrite(&directory[i].phone, sizeof(directory[i].phone), 1, fileBuffer);
            fwrite(directory[i].email, sizeof(directory[i].email), 1, fileBuffer);
            fwrite(&directory[i].degree, sizeof(directory[i].degree), 1, fileBuffer);
        }
        fclose(fileBuffer);
    }
    else
        printf ("ERROR: Could not open file for saving data !\n");
}

void loadFile(char* fileName)
{
    // Reads the array into the file that's name is passed through to this function.

    FILE* fileBuffer;
    fileBuffer = fopen(fileName, "rb");
    if (fileBuffer != NULL) {
        fread(&tail, sizeof(tail), 1, fileBuffer);
        for (int i = 0; i < tail; i++) {
            fread(directory[i].name, sizeof(directory[i].name), 1, fileBuffer);
            fread(&directory[i].phone, sizeof(directory[i].phone), 1, fileBuffer);
            fread(directory[i].email, sizeof(directory[i].email), 1, fileBuffer);
            fread(&directory[i].degree, sizeof(directory[i].degree), 1, fileBuffer);
        }
        fclose(fileBuffer);
    }
    else
        printf ("ERROR: Could not open file for reading data !\n");
}

void flush() {	// flush the input buffer. To be discussed later
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


void branching(char c) {    // branch to different tasks
    switch (c) {
        case 'i':
            insertion();
            break;
        case 's':
            search_person();
            break;
        case 'd':
            delete_person();
            break;
        case 'p':
            print_all();
            break;
        case 'q':
            break;
        default:
            printf("Invalid input\n");
    }
}


int print_person(int i) {
    // print all information one person in the directory
    printf("\n\nname = %s\n", directory[i].name);
    printf("email = %s\n", directory[i].email);
    printf("phone = %d\n", directory[i].phone);
    int *p = &directory[i].degree;
    switch (*p) {
        case (0):
            printf("Degree = %s\n", "Diploma");
            break;
        case (1):
            printf("Degree = %s\n", "Bachelor");
            break;
        case (2):
            printf("Degree = %s\n", "Master");
            break;
        case (3):
            printf("Degree = %s\n", "Doctor");
            break;
    }
    return 0;
}
int insertion() {    // insert a new entry at the end
    char email[30], name[30];
    int phone;
    education level = 0;
    if (tail == max) {
        printf("There are no more places to insert.\n");
        return -1;
    }
    else {
        printf("Enter name, phone, email, degree (0-3):\n");
        scanf("%s", name);
        // &directory[tail].name is an array. No "&" is needed
        scanf("%d", &phone, sizeof(directory[tail].phone));
        scanf("%s", email);
        scanf("%d", &level);
        sort(email, name, level, phone);
        tail++;
        printf("The number of entries = %d\n", tail);
        return 0;
    }
}

void sort(char* email, char* name, education level, int phone)
{
    int i = 0, j = 0;
    
    // Case 1: Empty List
    if (tail == 0)
    {
        strcpy(directory[tail].name, name);
        strcpy(directory[tail].email, email);
        directory[tail].phone = phone;
        directory[tail].degree = level;
        return;
    }
    
    while (i < tail)
    {
        // Case 2: Beginning or middle of list
        if (strcmp(name, directory[i].name) < 0)
        {
            j = tail;
            while (j > i)
            {
                strcpy(directory[j].name, directory[j - 1].name);
                strcpy(directory[j].email, directory[j - 1].email);
                directory[j].phone = directory[j - 1].phone;
                directory[j].degree = directory[j - 1].degree;
                j--;
            }
            strcpy(directory[i].name, name);
            strcpy(directory[i].email, email);
            directory[i].phone = phone;
            directory[i].degree = level;
            return;
        }
        i++;
    }
    
    // Case 3: End of list
    strcpy(directory[tail].name, name);
    strcpy(directory[tail].email, email);
    directory[tail].phone = phone;
    directory[tail].degree = level;
}

int print_all() {
    // print all information each person in the contactbook
    int i;
    
    if (tail == 0) {
        printf("No entries found.");
    }
    else {
        for (i = 0; i < tail; i++) {
            print_person(i);
        }
    }
    
    return 0;
}

int search_person() {      // print phone and email via name
    char sname[30]; int  i = 0;
    struct person *p = &directory[0], *pe = &directory[100];
    
    printf("Please enter the name to be searched for:\n");
    scanf("%s", sname); 	//sname is an array, no & needed
    
    while (p < pe) {
        if (strcmp(sname,p->name) == 0) {
            
            print_person(i);
            return i;
        }
        i++;
        p++;
    }
    printf("The name does not exist.\n");
    return -1;
}
int delete_person() {
    int i, k;
    k = search_person();
    if (k == -1) {
        printf("The name does not exist.\n"); return -1;
    }
    else {
        for (i = k; i<tail; i++) {
            strcpy(directory[i].name, directory[i + 1].name);
            directory[i].phone = directory[i + 1].phone;
            strcpy(directory[i].email, directory[i + 1].email);
            printf("The index deleted is: %d\n", k);
        }
        tail--;
        return k;
    }
}