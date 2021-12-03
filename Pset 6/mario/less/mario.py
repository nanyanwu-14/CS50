from cs50 import get_int

# Prompt user for height until they input a value between 1 and 8, inclusive
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

# Print pyramid
for size in range(1, height + 1):
    print(" " * (height - size) + "#" * size)