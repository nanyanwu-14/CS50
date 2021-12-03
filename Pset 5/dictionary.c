// Include libraries
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <strings.h>

#include <ctype.h>

// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 50000;

// Hash table
node *table[N];

// Initialize the counter for the number of words in the dictionary
int wordcount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int ind = hash(word);
    node *ptr = table[ind];
    while (ptr)
    {
        if (!strcasecmp(ptr -> word, word))
        {
            return true;
        }
        else
        {
            ptr = ptr -> next;
        }
    }
    return false;
}

// Hashes word to a number
// Referenced from: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        if (isupper(c))
        {
            hash = ((hash << 5) + hash) + (c + ('a' - 'A')); 
        }
        else
        {
            hash = ((hash << 5) + hash) + c;
        }
    }
    return (hash % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Load dictionary file
    FILE *dic = fopen(dictionary, "r");
    if (!dic)
    {
        return false;
    }
    
    // Import dictionary words into hash table
    
    // I structured my code like this instead of using strcpy to decrease the runtime. Strcpy uses a loop to copy characters so it is
    // faster to initialize a node before the loop and fscanf the first word into it.
    
    node *tmp = malloc(sizeof(node));
    if (!tmp)
    {
        printf("Could not store dictionary word.");
        return 1;
    }
        
    while (fscanf(dic, "%s", tmp -> word) != EOF)
    {
        int ind = hash(tmp -> word);
        tmp -> next = table[ind];
        table[ind] = tmp;
        tmp = malloc(sizeof(node));
        if (!tmp)
        {
            printf("Could not store dictionary word.");
            return 1;
        }

        // Keep track of the word count for the size function
        wordcount++;
    }
    free(tmp);
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr)
        {
            node *old = ptr;
            ptr = ptr -> next;
            free(old);
        }
    }
    return true;
}
