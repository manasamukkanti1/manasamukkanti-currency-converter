
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CURRENCIES 5

// Currency Rate Structure
typedef struct {
    char code[4];
    float rate_to_usd;
} Currency;

// Conversion History Node
typedef struct HistoryNode {
    char from_currency[4];
    char to_currency[4];
    float amount;
    float result;
    struct HistoryNode* next;
} HistoryNode;

// Currency Rates Array
Currency currencies[MAX_CURRENCIES] = {
    {"USD", 1.0},
    {"EUR", 0.91},
    {"JPY", 155.12},
    {"INR", 83.33},
    {"GBP", 0.78}
};

// History Head Pointer
HistoryNode* history_head = NULL;

// Function to find currency rate
float get_rate(char code[]) {
    for (int i = 0; i < MAX_CURRENCIES; i++) {
        if (strcmp(currencies[i].code, code) == 0)
            return currencies[i].rate_to_usd;
    }
    return -1; // Not found
}

// Add to conversion history
void add_to_history(char from[], char to[], float amount, float result) {
    HistoryNode* new_node = (HistoryNode*)malloc(sizeof(HistoryNode));
    strcpy(new_node->from_currency, from);
    strcpy(new_node->to_currency, to);
    new_node->amount = amount;
    new_node->result = result;
    new_node->next = history_head;
    history_head = new_node;
}

// Display conversion history
void print_history() {
    HistoryNode* temp = history_head;
    printf("\nConversion History:\n");
    while (temp != NULL) {
        printf("%.2f %s = %.2f %s\n", temp->amount, temp->from_currency, temp->result, temp->to_currency);
        temp = temp->next;
    }
}

// Convert currency
float convert(char from[], char to[], float amount) {
    float from_rate = get_rate(from);
    float to_rate = get_rate(to);
    if (from_rate == -1 || to_rate == -1) {
        printf("Invalid currency code.\n");
        return -1;
    }
    float usd_amount = amount / from_rate;
    return usd_amount * to_rate;
}

// Free memory used by history
void free_history() {
    HistoryNode* temp;
    while (history_head != NULL) {
        temp = history_head;
        history_head = history_head->next;
        free(temp);
    }
}

// Main Program
int main() {
    char from[4], to[4];
    float amount, result;
    int choice;

    do {
        printf("\nCurrency Converter\n");
        printf("1. Convert Currency\n");
        printf("2. View Conversion History\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter from currency code (e.g., USD): ");
                scanf("%s", from);
                printf("Enter to currency code (e.g., EUR): ");
                scanf("%s", to);
                printf("Enter amount: ");
                scanf("%f", &amount);

                // Convert to uppercase
                for (int i = 0; from[i]; i++) from[i] = toupper(from[i]);
                for (int i = 0; to[i]; i++) to[i] = toupper(to[i]);

                result = convert(from, to, amount);
                if (result != -1) {
                    printf("Converted: %.2f %s = %.2f %s\n", amount, from, result, to);
                    add_to_history(from, to, amount, result);
                }
                break;

            case 2:
                print_history();
                break;

            case 3:
                printf("Exiting...\n");
                free_history();
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 3);

    return 0;
}
