from cs50 import get_string

# Get users name and store it as string (response)
response = get_string("What's your name?\n")

# Say hello to the user
print(f"hello, {response}")