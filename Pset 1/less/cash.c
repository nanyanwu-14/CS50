#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Declare a float variable named dollars
    float dollars;
    
    // Initiate a do loop requesting the user for their change and continue this loop until a positive number is entered
    do
    {
        dollars = get_float("Change owed: $");
    }
    while (dollars < 0);
    
    // Convert change from dollars to cents
    int cents = round(dollars * 100);
    
    // declaring variables for types of coins
    // could just keep track of the total coins using the coins variable, but it is useful for the cashier to know how many of each coin type is necessary
    int quarters = 0;
    int dimes = 0;
    int nickels = 0;
    int pennies = 0;
    
    // while loop that will subtract cents from the user's change until it is equal to 0, while keeping count of coins
    while (cents > 0)
    {
        if (cents >= 25) // if the amount of change, in cents, is greater or equal than 25, subtract 25 cents from it and count 1 quarter
        {
            cents = cents - 25;
            quarters++;
        }
        else if (cents >= 10) // if the amount of change, in cents, is greater or equal than 10, subtract 10 cents from it and count 1 dime
        {
            cents = cents - 10;
            dimes++;
        }
        else if (cents >= 5) // if the amount of change, in cents, is greater or equal than 5, subtract 5 cents from it and count 1 nickel
        {
            cents = cents - 5;
            nickels++;
        }
        else // if the amount of change, in cents, is greater or equal than 1, subtract 1 cent from it and count 1 penny
        {
            cents = cents - 1;
            pennies++;
        }
    }
    
    int coins = quarters + nickels + dimes + pennies; // sum all the different coins together
    
    printf("%i\n", coins); // print the maximal amount of coins for the user's change
}