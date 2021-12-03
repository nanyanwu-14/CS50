from cs50 import get_int

# Prompting the user for a number
number = get_int("Number: ")

# Getting the length of the number for final checks
number_string = str(number)
length = len(number_string)

# Getting the first digit or first two digits for final checks
digits = number_string[0:2]

# Setting the allowed first digits for each card type
a_digits = ["34", "37"]
m_digits = ["51", "52", "53", "54", "55"]
v_digit = ["4"]

# Check Sum calculation
evensum = 0
oddsum = 0
position = 0

while position != length:
    position += 1
    if position % 2 == 1:  # Ex. if the loop has ran once, position = 1 and position % 2 == 1 indicating that we are on an odd numbered value
        odd = int(number_string[-position])
        oddsum += odd
    else:  # Ex. if the loop has ran twice, position = 2 and position % 2 == 0 indicating that we are on an even numbered value
        times2 = 2 * int(number_string[-position])
        if times2 >= 10:
            integer2 = times2 - 10
            evensum += 1 + integer2
        else:
            evensum += times2
            
checksum = oddsum + evensum

# Printing the card type or invalid
if checksum % 10 != 0:
    print("INVALID")
elif length == 15 and digits in a_digits:
    print("AMEX")
elif length == 16 and digits in m_digits:
    print("MASTERCARD")
elif (length == 16 or length == 13) and digits[0] in v_digit:
    print("VISA")
else:
    print("INVALID")