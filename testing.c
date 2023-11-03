#include <stdio.h>
#include <stdlib.h>

int main() {
    char numberString[20];  // Assuming a number could be within 20 characters
    printf("Enter a number with or without sign: ");
    scanf("%s", numberString);

    // Check if the first character is a '+' or '-'
    if (numberString[0] == '+' || numberString[0] == '-') {
        // If it is a valid sign, convert the rest of the string to a double
        double number = atof(&numberString[1]);  // atof() converts a string to a double
        if (numberString[0] == '-') {
            number = -number;  // If the sign was '-', make the number negative
        }
        printf("The number entered is: %lf\n", number);
    } else {
        // If there's no sign, the string is directly converted to a double
        double number = atof(numberString);
        printf("The number entered is: %lf\n", number);
    }

    return 0;
}
