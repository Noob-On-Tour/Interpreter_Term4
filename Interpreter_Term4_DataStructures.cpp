//============================================================================
// Name        : Term4_Project.cpp
// Author      : Ahmed Lotfey Siam && Sief El deen Samir
// Version     : V 1.1
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
#include "Stack_Postfix_To_Infix.h"

using namespace std;

// DEFINITIONS ===============================================================

#define len(s) s.length()
#define flush fflush(stdin);
#define Clear system("cls");
#define Pause cout<<"Press Any key"<<endl; getchar();
#define choiceEnd flush Pause Clear

// GLOBAL VARIABLES ==========================================================

FILE *f;
map<string, string> hashMap;
int equalIndex = 0;

// FUNCTIONS PROTOTYPES ======================================================

int newEntry(string name, string value);

int updateEntry(string name, string value);

int doesExist(string s);

int validateLHS(string s);

int validateRHS(string s);

int isInitialization(string s);

string replaceNameValue(string s);

int isOperator(char c);

string searchValue(string s);

int control();

int saveToFile();

int loadFromFile();

string removeSpaces(string s);

// MAIN ======================================================================

int main() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
//this is an example how to use hash maps
//	 hashMap["x"] = "10";
//	 hashMap["y"] = "20";
//	 cout << hashMap["x"] << endl;
//	 cout << hashMap["X"] << endl;
//	 hashMap["x"] = "1000";
//	 cout << hashMap["x"] << endl;
//	 if (hashMap.find("x") != hashMap.end()) {
//	 cout << "Key is found !";
//	 }
//
//	 map<string, string>::iterator i = hashMap.find("f");
//	 cout << i->first << endl << i->second;

    loadFromFile();
    control();
}

// FUNCTIONS IMPLEMENTATIONS =================================================
int isOperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        return 1;
    }
    return 0;
}

int isBracket(char c) {
    if (c == '(' || c == ')')
        return 1;
    return 0;
}

int saveToFile() {

    f = fopen("data.txt", "r+");
    if (f == NULL) {
        printf("Error 404 : File not found.\n");
        return 0;
    }
    for (map<string, string>::iterator i = hashMap.begin(); i != hashMap.end();
         i++)
        fprintf(f, "%s,%s\n", i->first.c_str(), i->second.c_str());
    fclose(f);
    printf("All data saved successfully !\n");

    return 1;
}

int loadFromFile() {
    char cName[100], cValue[100];
    f = fopen("data.txt", "r+");
    if (f == NULL) {
        printf("Error 404 : File not found.\n");
        f = fopen("data.txt", "w+");
        if (f == NULL) {
            printf("Can't Creat new file\n");
            return 0;
        } else {
            printf("New Clear File created\n"
                           "File directory Project Folder\n");
            return 1;
        }
    }

    while (fscanf(f, "%20[^,],%s\n", cName, cValue) != EOF) {
        stringstream Name, Value;
        Name << cName, Value << cValue;
        hashMap[Name.str()] = Value.str();
    }

    return 1;
}

string removeSpaces(string s) {
    string spaceLess;
    for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] == ' ')
            continue;
        spaceLess.push_back(s[i]);
    }
    return spaceLess;
}

string searchValue(string s) {
    if (hashMap.find(s) != hashMap.end()) {
        return hashMap[s];
    }
    return "\0";
}

int validateLHS(string s) {
    equalIndex = 0, equalIndex = s.find_first_of('=');
    int lastEqual = s.find_last_of('=');

    if (equalIndex == -1 || !equalIndex) {
        return 2; // equal not found
    }
    if (lastEqual != equalIndex) {
        return 4; // 2 equal signs found
    }

    for (int i = 0; i < equalIndex; i++) {
        if (!isalnum(s[i])) {
            return 3; //invalid name
        }
        if (isdigit(s[i]) || isOperator(s[i]) || s[i] == '.') {
            return 3; //invalid name
        }

    }

    return 1;
}

int validateRHS(string s) {
    equalIndex = 0, equalIndex = s.find_first_of('=');
    int bracketFlag = 0;

    for (int i = equalIndex + 1; i < len(s); i++) {
//x=(xxx)+yyy

        if (s[i] == '(') {
            bracketFlag = 1;
            continue;
        }

        if (isdigit(s[i])) {
            while (!isOperator(s[i]) && i < len(s)) {
                if (s[i] == ')' || s[i] == '(') {
                    break;
                }
                if (!isdigit(s[i]) && s[i] != '.') {
                    return 0;
                }

                i++;
            }
        }
        //x = y;
        //x = y.y
        if (isalpha(s[i])) {
            //x=(xxx)
            while (!isOperator(s[i]) && i < len(s)) {
                if (s[i] == ')' || s[i] == '(') {
                    break;
                }
                if (!isalnum(s[i])) {
                    return 0;
                }
                i++;
            }
        }

        if (isOperator(s[i])) {
            if (i + 1 <= len(s)) {
                if (isOperator(s[i + 1])) {
                    return 0;
                }
            } else if (i == len(s))
                return 0;
        }

        if (s[i] == ')') {
            if (bracketFlag == 1) {
                bracketFlag = 0;
            } else
                return 0;
        }
    }

    if (bracketFlag == 1) {
        return 0;
    }

    for (int i = 0; i < s.length(); i++) {
        // x = ( +xxx  )
        if (isOperator(s[i])) {
            if (i + 1 < len(s)) {
                if ((s[i + 1]) == ')') {
                    return 0;
                }
            }
        }
        // x= (+xxx)
        if (s[i] == '(') {
            if (i + 1 < s.length()) {
                if (isOperator(s[i + 1])) {
                    return 0;
                }
            }
        }
    }

    if (isOperator(s[s.length() - 1])) {
        return 0;
    }

    return 1;
}

int isInitialization(string s) {
    equalIndex = 0, equalIndex = s.find_first_of('=');

    for (int i = equalIndex + 1; i < s.length(); i++) {
        if (!isdigit(s[i])) {
            if (!isOperator(s[i]) && s[i] != '.') {
                return 0;
            }
        }
    }
    return 1;
}

string replaceNameValue(string s) {

    equalIndex = 0, equalIndex = s.find_first_of('=');
    string final;

    unsigned int len = len(s);
    if (validateRHS(s) == 0) {
        return "RHS";
    }

    final.append(s.substr(0, (unsigned int) (equalIndex + 1)));

    for (int i = equalIndex + 1; i < len; i++) {
        if (isalpha(s[i])) {
            string tmpName, tmpValue;
            int j = i;
            while (!isOperator(s[j]) && j < len && !isBracket(s[j])) {
                tmpName.push_back(s[j++]);
            }
            tmpValue = searchValue(tmpName);
            if (!tmpValue.empty()) {
                final.append(tmpValue);
                i += len(tmpName) - 1;
                tmpValue.clear(), tmpName.clear();
            } else {
                return "NO";
            }
        } else
            final.push_back(s[i]);
    }

    return final;
}

int newEntry(string name, string value) {
    hashMap[name] = value;
    return 1;
}

int updateEntry(string name, string value) {
    hashMap[name] = value;
    return 1;
}

int doesExist(string s) {

    string name, value;
    name = s.substr(0, s.find('='));
    value = s.substr(s.find('=') + 1, s.length());

    if (hashMap.find(name) != hashMap.end()) {
        updateEntry(name, value);
        return 1;
    }
    newEntry(name, value);
    return 0;
}

int control() {

    Menu:
    cout << "******** INTERPRETER ********\n"
            "** 1 - Enter Initialization**\n"
            "** 2 - Enter Expression    **\n"
            "** 3 - Reset               **\n"
            "** 4 - Show Entries        **\n"
            "** 5 - Save To File        **\n"
            "** 6 - Exit                **\n"
            "*****************************\n";

    char choice[2];
    choice[1] = '\0';
    scanf("%c", &choice[0]);
    int x = atoi(choice);
    string s;

    switch (x) {
        case 1:
            cout << "Enter your Initialization : " << endl;
            flush
            getline(cin, s);
            s = removeSpaces(s);
            if (validateLHS(s) == 2) {
                cout << "Equal not Found !" << endl;
                choiceEnd
            } else if (validateLHS(s) == 3) {
                cout << "Invalid Name !" << endl;
                choiceEnd
                goto Menu;
            } else if (validateRHS(s) == 0) {
                cout << "Invalid !" << endl;
                choiceEnd
                goto Menu;
            }
            if (isInitialization(s)) {
                if (doesExist(s) == 1) {
                    cout << "Value Updated" << endl;
                    choiceEnd
                    goto Menu;
                } else
                    cout << "New Entry saved" << endl;
                choiceEnd
                goto Menu;
            } else {
                cout << "Invalid Initialization !" << endl;
                choiceEnd
                goto Menu;
            }

        case 2:
            cout << "Enter expression to evaluate it" << endl;
            flush
            getline(cin, s);
            s = removeSpaces(s);
            if (validateLHS(s) == 2) {
                cout << "Equal not Found !" << endl;
                choiceEnd
            } else if (validateLHS(s) == 3) {
                cout << "Invalid Name !" << endl;
                choiceEnd
                goto Menu;
            }
            if (isInitialization(s)) {
                if (doesExist(s)) {
                    cout << "New Entry saved" << endl;
                    choiceEnd
                    goto Menu;
                } else {
                    cout << "Value Updated" << endl;
                    choiceEnd
                    goto Menu;
                }
            } else {
                string replaceResult = replaceNameValue(s);
                if (replaceResult == "RHS") {
                    cout << "Invalid RHS" << endl;
                    choiceEnd
                    goto Menu;
                } else if (replaceResult == "NO") {
                    cout << "Variable not Found !" << endl;
                    choiceEnd
                    goto Menu;
                } else {
                    char infix[100];
                    strcpy(infix, replaceResult.c_str());
                    double dValue = evaluate_MultiDigitPostFix(
                            inFix_To_postFix(infix));
                    string name = replaceResult.substr(0, replaceResult.find('=')),
                            sValue;
                    ostringstream sstream;
                    sstream << dValue;
                    sValue = sstream.str();
                    cout << name << " = " << dValue << endl;
                    doesExist(name + "=" + sValue);
                    choiceEnd
                    goto Menu;
                }
            }

        case 3:
            hashMap.clear();
            cout << "Hash table has been reset." << endl;
            choiceEnd
            goto Menu;

        case 4:
            Clear
            cout << "Table Data : \n";
            for (map<string, string>::iterator i = hashMap.begin();
                 i != hashMap.end(); i++) {
                cout << i->first << " = " << i->second << endl;
            }
            cout << endl << "Hit Any Key to Continue..." << endl;
            flush
            getchar();
            goto Menu;

        case 5:
            saveToFile();
            choiceEnd
            goto Menu;
        case 6:
            Clear
            cout << "Interpreter Terminated !" << endl;
            return 1;
        default:
            cout << "Invalid Entry , retry !" << endl;
            choiceEnd
            goto Menu;
    }
}
