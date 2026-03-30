#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// ================= ELEMENT STRUCTURE =================
// Structure to store element symbol and atomic number.
// Used for encoding letters into numbers and decoding numbers back to letters.
typedef struct {
    char symbol[3];      // 1 or 2 letter element symbol (plus null terminator)
    int atomic_number;   // Atomic number (1–99)
} Element;

// ================= ELEMENT TABLE =================
// Periodic elements from atomic number 1 to 99
// This table is used for both encoding and decoding
Element elements[] = {
    {"H",1},{"HE",2},{"LI",3},{"BE",4},{"B",5},{"C",6},{"N",7},{"O",8},{"F",9},{"NE",10},
    {"NA",11},{"MG",12},{"AL",13},{"SI",14},{"P",15},{"S",16},{"CL",17},{"AR",18},
    {"K",19},{"CA",20},{"SC",21},{"TI",22},{"V",23},{"CR",24},{"MN",25},{"FE",26},
    {"CO",27},{"NI",28},{"CU",29},{"ZN",30},{"GA",31},{"GE",32},{"AS",33},{"SE",34},
    {"BR",35},{"KR",36},{"RB",37},{"SR",38},{"Y",39},{"ZR",40},{"NB",41},{"MO",42},
    {"TC",43},{"RU",44},{"RH",45},{"PD",46},{"AG",47},{"CD",48},{"IN",49},{"SN",50},
    {"SB",51},{"TE",52},{"I",53},{"XE",54},{"CS",55},{"BA",56},{"LA",57},{"CE",58},
    {"PR",59},{"ND",60},{"PM",61},{"SM",62},{"EU",63},{"GD",64},{"TB",65},{"DY",66},
    {"HO",67},{"ER",68},{"TM",69},{"YB",70},{"LU",71},{"HF",72},{"TA",73},{"W",74},
    {"RE",75},{"OS",76},{"IR",77},{"PT",78},{"AU",79},{"HG",80},{"TL",81},{"PB",82},
    {"BI",83},{"PO",84},{"AT",85},{"RN",86},{"FR",87},{"RA",88},{"AC",89},{"TH",90},
    {"PA",91},{"U",92},{"NP",93},{"PU",94},{"AM",95},{"CM",96},{"BK",97},{"CF",98},{"ES",99}
};

int element_count = sizeof(elements) / sizeof(elements[0]);

// ================= LOOKUP FUNCTIONS =================

// find_element:
//  - Input: symbol (1 or 2 letters, uppercase)
//  - Output: atomic number if found, -1 if not found
//  - Description: searches the element table to convert a symbol to its atomic number
int find_element(const char *sym) {
    for (int i = 0; i < element_count; i++) {
        if (strcmp(sym, elements[i].symbol) == 0) {
            return elements[i].atomic_number;
        }
    }
    return -1; // not found
}

// find_symbol:
//  - Input: atomic number (1–99)
//  - Output: pointer to element symbol if found, NULL if not found
//  - Description: searches the element table to convert a number back into its symbol
const char* find_symbol(int num) {
    for (int i = 0; i < element_count; i++) {
        if (elements[i].atomic_number == num) {
            return elements[i].symbol;
        }
    }
    return NULL; // not found
}

// ================= ENCODER =================

// encode:
//  - Input: string (all caps, no spaces, letters only)
//  - Output: prints encoded string
//  - Description:
//      Reads input string left-to-right.
//      Tries to match 2-letter symbols first, then 1-letter symbols.
//      If a match is found, replaces it with atomic number in 2-digit format (01–99).
//      If no match, leaves the character as-is.
//      Builds the full output string in a buffer, then prints at the end.
void encode(char *input) {
    char output[1024] = "";  // buffer to store the full encoded string
    int out_index = 0;
    int len = strlen(input);

    for (int i = 0; i < len;) {
        char two[3] = {0}; // temporary 2-letter symbol
        char one[2] = {0}; // temporary 1-letter symbol

        // Try to match 2-letter symbol first
        if (i + 1 < len) {
            two[0] = input[i];
            two[1] = input[i + 1];
            int num = find_element(two);
            if (num != -1) {
                out_index += sprintf(output + out_index, "%02d", num); // encode as 2 digits
                i += 2; // move past matched letters
                continue;
            }
        }

        // Try to match 1-letter symbol
        one[0] = input[i];
        int num = find_element(one);
        if (num != -1) {
            out_index += sprintf(output + out_index, "%02d", num); // encode as 2 digits
        } else {
            // No match: copy character as-is
            output[out_index++] = input[i];
            output[out_index] = '\0';
        }

        i++;
    }

    printf("Encoded: %s\n", output);
}

// ================= DECODER =================

// decode:
//  - Input: string (numbers and letters, encoded format)
//  - Output: prints decoded string
//  - Description:
//      Reads input string left-to-right.
//      Reads 2 characters at a time to form numbers (01–99).
//      Converts number back to symbol using element table.
//      If next 2 characters are not digits, copies character as-is.
//      Builds full output string in buffer, then prints at the end.
void decode(char *input) {
    char output[1024] = ""; // buffer for decoded string
    int out_index = 0;
    int len = strlen(input);

    for (int i = 0; i < len;) {
        // Check if next 2 chars are digits
        if (isdigit(input[i]) && isdigit(input[i+1])) {
            char num_str[3];
            num_str[0] = input[i];
            num_str[1] = input[i+1];
            num_str[2] = '\0';

            int num = atoi(num_str); // convert 2-digit string to number
            const char *sym = find_symbol(num); // look up symbol

            if (sym != NULL) {
                out_index += sprintf(output + out_index, "%s", sym); // append symbol
                i += 2; // move past the number
                continue;
            }
        }

        // Not a valid number: copy character as-is
        output[out_index++] = input[i];
        output[out_index] = '\0';
        i++;
    }

    printf("Decoded: %s\n", output);
}

// ================= MAIN MENU LOOP =================

int main() {
    int choice;
    char input[256];

    while (1) {
        // Display user menu
        printf("\n=== MENU ===\n");
        printf("1. Encode\n");
        printf("2. Decode\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        // Exit the program
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }

        // Get input string from user
        printf("Enter string: ");
        scanf("%255s", input);

        // Call encode or decode based on user choice
        if (choice == 1) {
            encode(input);
        } else if (choice == 2) {
            decode(input);
        } else {
            printf("Invalid choice. Please select 1, 2, or 3.\n");
        }
    }

    return 0;
}
