#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get positive integer for height that is between or equal to 1 and 8
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    int hashtags = 0;

    // Run a for loop from 0 to the inputted height value and increment by 1 each loop
    for (int i = 0; i < height; i++)
    {
        hashtags++; // Add 1 to hashtag with each iteration (number of hashtags increases going down each row)

        // Print enough spaces to left align the hashtags
        for (int j = 0; j < (height - hashtags); j++)
        {
            printf(" ");
        }

        // Print the hashtags in the row for the first pyramid
        for (int k = 0; k < hashtags; k++)
        {
            printf("#");
        }

        // Print the space between the two pyramids
        printf("  ");

        // Print the hashtags in the row for the second pyramid
        for (int l = 0; l < hashtags; l++)
        {
            printf("#");
        }

        // Go to the next line
        printf("\n");
    }
}