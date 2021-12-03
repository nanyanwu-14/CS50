#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function Prototype
int check_key(string argument);

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
    string key = argv[1];
    
    // Check the entered key's validity
    if (check_key(key) == 1) // If the return value of the function is 1...
    {
        return 1; // Stop the execution of main function and return 1, if the key is invalid
    }
    
    // Get string from user to encipher
    string plain = get_string("plaintext: ");
    
    // Encipher user's entered text
    for (int j = 0, n = strlen(plain); j < n; j++)
    {
        if (islower(plain[j])) // If the character is lowercase...
        {
            plain[j] = tolower(key[plain[j] - 'a']); 
            // Convert it to a value from 0 to 25 and use this value to index the key. Return the lowercase form of the character from the key
        }
        else if (isupper(plain[j])) // If the character is uppercase...
        {
            plain[j] = toupper(key[plain[j] - 'A']);
            // Convert it to a value from 0 to 25 and use this value to index the key. Return the uppercase form of the character from the key
        }
        else
        {
            plain[j] = plain[j]; // If the character is not alphabetic, leave it the same
        }
    }
    printf("ciphertext: %s\n", plain);
    return 0;
}

int check_key(string argument)
{
    int m = strlen(argument); // Initialize variable m that is equal to the length of the user's entered key

    if (m != 26) // If the key is not 26 characters...
    { 
        printf("Key must be 26 characters.\n");
        return 1; // Stop the execution of this function and return 1
    }
        
    for (int i = 0; i < m; i++) // Loop through all the characters in the entered key
    {
        if (isalpha(argument[i]) == 0) // If any character is not alphabetic...    
        { 
            printf("Key must only contain alphabetic characters.\n");
            return 1; // Stop the execution of this function and return 1
        }
        // Reference: https://stackoverflow.com/questions/8199403/how-to-check-if-an-array-has-any-duplicates
        for (int j = i + 1; j < m; j++) // Loop through all the characters in the key after argument[i]
        {
            if (argument[i] == argument[j]) // If the current character, argument[i], is equal to any of the characters after it
            {
                printf("Key must not contain duplicated characters.\n");
                return 1; // Stop the execution of this function and return 1
            }
        }
    }
    return 0; // If the key is 26 alphabetic characters and has no duplicates are numbers, return 0
}