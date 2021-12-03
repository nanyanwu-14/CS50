// Include libraries

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string response = get_string("What's your name?\n"); // Get users name and store it as string (response)
    printf("hello, %s\n", response); // Say hello to the user
}