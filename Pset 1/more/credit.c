#include <cs50.h>
#include <stdio.h>
#include <math.h> // imported for the truncl function (found by checking through the manual)
#include <string.h> // imported for the strcmp function discussed in section

int main(void)
{
    // Get credit card number from user
    
    long number = get_long("Number: ");

    // Calculate the length of the number
    
    int length = 0;
    for (long j = 1; j <= number; j = j * 10)
    {
        length++;
    }

    // Convert the number to a character array and extract its first values.
    // Reference: https://stackoverflow.com/questions/5050067/simple-int-to-char-conversion/5050086
        
    char number_char[length];  
    sprintf(number_char, "%li", number); // Copy number (an integer) into a character array
    char digits[2]; // Declare a character array two units long
    sprintf(digits, "%c%c", number_char[0], number_char[1]); // Copy first two characters of numbers_char into new character array


    // Calculate the Checksum of the credit card number
    
    int evensum = 0; // Declare variable evensum to do the first part of the checksum process listed on the cs50 website
    int oddsum = 0; // Declare variable oddsum to do the second part of the checksum process
    int i = 0; // Declare variable i to mark whether an integer in the card number is odd or even when counting from right to left

    while (number > 0) // the number will be shortened with each iteration until it is equal to 0
    {
        i++;
        if (i % 2 == 1) // Ex. if the loop has ran once, i = 1 and i % 2 == 1 indicating that we are on an odd numbered value
        {
            int odd = (int)(number % 10); // set the odd value equal modulo
            oddsum = oddsum + odd; // add the new odd value to the past oddsum to get the new oddsum
            number = truncl(number / 10); // remove the odd value
        }

        else // Ex. if the loop has ran twice, i = 2 and i % 2 == 0 indicating that we are on an even numbered value
        {
            int times2 = (int)(2 * (number % 10)); // calculate the modulo and double this value
            if (times2 >= 10) // if 2 * modulo is a double digit number
            {
                int integer1 =  truncl(times2 / 10); // divide by 10 and truncate to 0
                int integer2 = (times2 % 10); // set the second integer equal to the modulo
                evensum = evensum + integer1 + integer2; // add both  integers to the previous evensum to get the new evensum
            }
            else // if 2 * modulo is a single digit number
            {
                evensum = evensum + times2; // add the value to the past evensum to get the new evensum
            }
            number = truncl(number / 10); // remove the even value
        }
    }

    int checksum = evensum + oddsum; // total both sums to get the checksum

    // Check the type of card using checksum, number of digits, and first few digits
    
    if (checksum % 10 == 0)
    {
        if ((length == 15) && (strcmp(digits, "34") == 0 || strcmp(digits, "37") == 0))
            // if the number is 15 integers long and starts with 34 or 37
        {
            printf("AMEX\n");
        }
        else if ((length == 16) && (strcmp(digits, "51") == 0 || strcmp(digits, "52") == 0 || strcmp(digits, "53") == 0
                                    || strcmp(digits, "54") == 0 || strcmp(digits, "55") == 0))
            // if the number is 16 integers long and starts with a number equal to or between 51 and 55
        { 
            printf("MASTERCARD\n");
        }
        else if ((length == 16 || length == 13) && (digits[0] == '4')) 
            // if the number is 16 or 13 integers long and starts with 4
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n"); // print INVALID if it does not meet length and initial digit requirements
        }
    }
    else
    {
        printf("INVALID\n"); // print INVALID if it does not meet checksum requirement
    }
}