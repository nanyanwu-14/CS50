from cs50 import get_float

# Initiate a do loop requesting the user for their change and continue this loop until a positive number is entered
while True:
    dollars = get_float("Change owed: $")
    if dollars >= 0:
        break

# Convert dollars to cents
cents = round(dollars * 100)

# Declaring counter for number of coins
coins = 0

# if we divide the change owed by the value of the coin, the integer part of the result will be the number of those coins we need
# the remainder of this divsion will be the left over cents
coins += int(cents/25)
cents = cents % 25

coins += int(cents/10)
cents = cents % 10

coins += int(cents/5) + cents % 5

# print the maximal amount of coins for the user's change
print(coins)