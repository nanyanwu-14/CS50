#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

// Function Prototypes
int count_letters();
int count_words();
int count_sent();

int main(void)
{
    string s = get_string("Text: "); // Get a string from the user and save it as the variable s
    int let_count = count_letters(s); // Count the number of letters in the string s and save the result as let_count
    int word_count = count_words(s); // Count the number of words in the string s and save the result as word_count
    int sent_count = count_sent(s); // Count the number of sentences in the string s and save the result as sent_count
    
    float L = ((float) let_count * 100) / word_count; // Calculate the variable L in the Coleman-Liau index
    float S = ((float) sent_count * 100) / word_count; // Calculate the variable S in the Coleman-Liau index
    int index = round((0.0588 * L) - (0.296 * S) - 15.8); // Use L and S to calculate the index
    
    if (index < 1) // If the index result is less than 1, prunt out before Grade 1
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n"); // If the index result is greater than or equal to 16, prunt out Grade 16+
    }
    else
    {
        printf("Grade %i\n", index); // If the index is from 1 to 15, print out "Grade" followed by the index value
    }
}

int count_letters(char s[]) // Declare a funcion that inputs strings and outputs an integer
{
    int letters = 0; // Initialize a variable, letters, that keeps count of the number of letters in the string
    for (int i = 0, n = strlen(s); i < n; i++) // Run loop for as long as i is less than the length of the string
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) // If the character in s is a letter, add 1 to letters
        {
            letters++;
        }
    }
    return letters; // Output the final number of letters
}

int count_words(char s[]) // Declare a funcion that inputs strings and outputs an integer
{
    int words = 0; // Initialize a variable, words, that keeps count of the number of words in the string
    for (int i = 0, n = strlen(s); i < n; i++) // Run loop for as long as i is less than the length of the string
    {
        if (s[i] == ' ') // If the character in s is a space, add 1 to words
        {
            words++;
        }
    }
    words++; // Add an additional 1 to words to account for the first word in the string
    return words; // Output the final number of words
}

int count_sent(char s[]) // Declare a funcion that inputs strings and outputs an integer
{
    int sentences = 0; // Initialize a variable, sentences, that keeps count of the number of sentences in the string
    for (int i = 0, n = strlen(s); i < n; i++) // Run loop for as long as i is less than the length of the string
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?') // If the character in s is a typical sentence-ending character, add 1 to sentences
        {
            sentences++;
        }
    }
    return sentences; // Output the final number of sentences
}