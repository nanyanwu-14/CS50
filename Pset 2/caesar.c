#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function Prototype
int check_key(string input); 

// Takes in the number of arguments enteed after the program's name and stores these arguments in a string array
int main(int argc, string argv[]) 
{
    // Check that there are not more or less than 2 arguments
    if (argc != 2) 
    {
        printf("Enter in a key!\n");
        return 1; // Stop the execution of main function and return 1
    }
    
    // Store the entered key as a string
    string input = argv[1]; 
    
    // Check the entered key's validity
    if (check_key(input) == 1) // If the return value of the function is 1...
    {
        return 1; // Stop the execution of main function and return 1, if the key is invalid
    }
    
    // Transfer user's key from char to int variable. Modulo is used so that the code wraps back to 'a' if the entered key is greater than 26
    const int KEY = atoi(input) % 26;
    
    // Get string from user to encipher
    string plain = get_string("plaintext: ");

    // Encipher user's entered text
    for (int j = 0, n = strlen(plain); j < n; j++) // Loop through all the characters in the user's input
    {
        if (islower(plain[j])) // If the character is lowercase...
        {
            plain[j] =  'a' + (plain[j] - 'a' + KEY) % 26; 
            // Convert it to a value from 0 to 25 and add the key value. Convert back to ASCII value for ciphered alphabetic character
        }
        else if (isupper(plain[j])) // If the character is uppercase...
        {
            plain[j] =  'A' + (plain[j] - 'A' + KEY) % 26; 
            // Convert it to a value from 0 to 25 and add the key value. Convert back to ASCII value for ciphered alphabetic character
        }
        else
        {
            plain[j] = plain[j]; // If the character is not alphabetic, leave it the same
        }
    }
    printf("ciphertext: %s\n", plain);
    return 0;
}

int check_key(string input)
{
    for (int i = 0, m = strlen(input); i < m; i++) // Loop through all the characters in the key
    {
        if (isdigit(input[i]) == 0) // If the character is not a number
        {
            printf("Usage: ./caesar key\n"); // Print error message
            return 1; // Stop the execution of this function and return 1
        }
    }
    return 0; // If all the characters are numbers, return 0
}