// BANK MANAGEMENT SYSTEM (MULTIPLE USERS + UPDATE)
// Developer: Hitesh Sharma

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct account_detail
{
    long int Account_no;
    long int pin_no;
    char Name[50];
    long int Phone_no;
    char address[100];
    long int present_amount;
};

struct account_detail currentUser;

// Function declarations
void menu();
void create_account();
int login();
void deposit();
void withdraw();
void display_detail();
void update_account();

// Helper functions
void trim_newline(char *s);
void read_line(const char *prompt, char *buf, size_t size);
long int read_long(const char *prompt);

// ================= MAIN =================
int main()
{
    printf("______WELCOME TO BANK SYSTEM______\n");

    int loggedIn = 0;
    char userType[10];

    read_line("\nAre you NEW or OLD user: ", userType, sizeof(userType));

    if(strcmp(userType,"new")==0 || strcmp(userType,"NEW")==0)
    {
        create_account();
    }

    loggedIn = login();

    if(!loggedIn)
    {
        printf("\nPlease create account first.\n");
        create_account();
        loggedIn = login();
    }

    int choice;
    do{
        menu();
        choice = read_long("\nEnter choice: ");

        switch(choice)
        {
            case 1: deposit(); break;
            case 2: withdraw(); break;
            case 3: display_detail(); break;
            case 4: update_account(); break;
            case 5: create_account(); break;
            case 6: printf("\nThank You!\n"); break;
            default: printf("\nInvalid choice!\n");
        }

    }while(choice != 6);

    return 0;
}

// ================= MENU =================
void menu()
{
    printf("\n\n----- MENU -----\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Display Details\n");
    printf("4. Update Account\n");
    printf("5. Create New Account\n");
    printf("6. Exit\n");
}

// ================= CREATE ACCOUNT =================
void create_account()
{
    struct account_detail p;

    read_line("\nEnter Name: ", p.Name, sizeof p.Name);
    read_line("Enter Address: ", p.address, sizeof p.address);
    p.Account_no = read_long("Enter Account No: ");
    p.Phone_no = read_long("Enter Phone No: ");
    p.pin_no = read_long("Set PIN: ");
    p.present_amount = read_long("Enter Initial Balance: ");

    FILE *fp = fopen("Bankdetails.txt","a"); // append
    if(fp == NULL){
        printf("File error!\n");
        return;
    }

    fprintf(fp,"%s\n%s\n%ld\n%ld\n%ld\n%ld\n",
        p.Name, p.address, p.Account_no,
        p.Phone_no, p.present_amount, p.pin_no);

    fclose(fp);

    printf("\nAccount Created Successfully!\n");
}

// ================= LOGIN =================
int login()
{
    long int acc, pin;
    int found = 0;

    acc = read_long("\nEnter Account No: ");
    pin = read_long("Enter PIN: ");

    FILE *fp = fopen("Bankdetails.txt","r");
    if(fp == NULL){
        printf("No records found!\n");
        return 0;
    }

    struct account_detail temp;

    while(fgets(temp.Name, sizeof temp.Name, fp))
    {
        fgets(temp.address, sizeof temp.address, fp);

        fscanf(fp,"%ld\n%ld\n%ld\n%ld\n",
            &temp.Account_no,
            &temp.Phone_no,
            &temp.present_amount,
            &temp.pin_no);

        trim_newline(temp.Name);
        trim_newline(temp.address);

        if(temp.Account_no == acc && temp.pin_no == pin)
        {
            currentUser = temp;
            found = 1;
            break;
        }
    }

    fclose(fp);

    if(found)
        printf("\nLogin Successful!\n");
    else
        printf("\nInvalid Account or PIN!\n");

    return found;
}

// ================= DEPOSIT =================
void deposit()
{
    long int amount = read_long("\nEnter amount to deposit: ");

    FILE *fp = fopen("Bankdetails.txt","r");
    FILE *temp = fopen("temp.txt","w");

    struct account_detail p;

    while(fgets(p.Name, sizeof p.Name, fp))
    {
        fgets(p.address, sizeof p.address, fp);
        fscanf(fp,"%ld\n%ld\n%ld\n%ld\n",
            &p.Account_no,
            &p.Phone_no,
            &p.present_amount,
            &p.pin_no);

        trim_newline(p.Name);
        trim_newline(p.address);

        if(p.Account_no == currentUser.Account_no)
        {
            p.present_amount += amount;
            currentUser = p;
        }

        fprintf(temp,"%s\n%s\n%ld\n%ld\n%ld\n%ld\n",
            p.Name, p.address, p.Account_no,
            p.Phone_no, p.present_amount, p.pin_no);
    }

    fclose(fp);
    fclose(temp);

    remove("Bankdetails.txt");
    rename("temp.txt","Bankdetails.txt");

    printf("\nDeposit Successful!\n");
}

// ================= WITHDRAW =================
void withdraw()
{
    long int amount = read_long("\nEnter amount to withdraw: ");

    if(amount > currentUser.present_amount)
    {
        printf("\nInsufficient Balance!\n");
        return;
    }

    FILE *fp = fopen("Bankdetails.txt","r");
    FILE *temp = fopen("temp.txt","w");

    struct account_detail p;

    while(fgets(p.Name, sizeof p.Name, fp))
    {
        fgets(p.address, sizeof p.address, fp);
        fscanf(fp,"%ld\n%ld\n%ld\n%ld\n",
            &p.Account_no,
            &p.Phone_no,
            &p.present_amount,
            &p.pin_no);

        trim_newline(p.Name);
        trim_newline(p.address);

        if(p.Account_no == currentUser.Account_no)
        {
            p.present_amount -= amount;
            currentUser = p;
        }

        fprintf(temp,"%s\n%s\n%ld\n%ld\n%ld\n%ld\n",
            p.Name, p.address, p.Account_no,
            p.Phone_no, p.present_amount, p.pin_no);
    }

    fclose(fp);
    fclose(temp);

    remove("Bankdetails.txt");
    rename("temp.txt","Bankdetails.txt");

    printf("\nWithdraw Successful!\n");
}

// ================= UPDATE ACCOUNT =================
void update_account()
{
    struct account_detail p;

    printf("\n--- Update Your Details ---\n");

    read_line("Enter New Name: ", p.Name, sizeof p.Name);
    read_line("Enter New Address: ", p.address, sizeof p.address);
    p.Phone_no = read_long("Enter New Phone No: ");

    p.Account_no = currentUser.Account_no;
    p.pin_no = currentUser.pin_no;
    p.present_amount = currentUser.present_amount;

    FILE *fp = fopen("Bankdetails.txt","r");
    FILE *temp = fopen("temp.txt","w");

    struct account_detail tempUser;

    while(fgets(tempUser.Name, sizeof tempUser.Name, fp))
    {
        fgets(tempUser.address, sizeof tempUser.address, fp);
        fscanf(fp,"%ld\n%ld\n%ld\n%ld\n",
            &tempUser.Account_no,
            &tempUser.Phone_no,
            &tempUser.present_amount,
            &tempUser.pin_no);

        trim_newline(tempUser.Name);
        trim_newline(tempUser.address);

        if(tempUser.Account_no == currentUser.Account_no)
        {
            tempUser = p;
            currentUser = p;
        }

        fprintf(temp,"%s\n%s\n%ld\n%ld\n%ld\n%ld\n",
            tempUser.Name, tempUser.address,
            tempUser.Account_no,
            tempUser.Phone_no,
            tempUser.present_amount,
            tempUser.pin_no);
    }

    fclose(fp);
    fclose(temp);

    remove("Bankdetails.txt");
    rename("temp.txt","Bankdetails.txt");

    printf("\nAccount Updated Successfully!\n");
}

// ================= DISPLAY =================
void display_detail()
{
    printf("\n----- ACCOUNT DETAILS -----\n");
    printf("Name: %s\n", currentUser.Name);
    printf("Account No: %ld\n", currentUser.Account_no);
    printf("Phone: %ld\n", currentUser.Phone_no);
    printf("Balance: %ld\n", currentUser.present_amount);
}

// ================= HELPER FUNCTIONS =================
void trim_newline(char *s)
{
    size_t len = strlen(s);
    if(len > 0 && s[len-1] == '\n')
        s[len-1] = '\0';
}

void read_line(const char *prompt, char *buf, size_t size)
{
    printf("%s", prompt);
    fgets(buf, size, stdin);
    trim_newline(buf);
}

long int read_long(const char *prompt)
{
    char line[50];
    long int value;
    char *end;

    while(1)
    {
        read_line(prompt, line, sizeof line);
        value = strtol(line, &end, 10);

        if(end != line && *end == '\0')
            return value;

        printf("Invalid input! Try again.\n");
    }
}