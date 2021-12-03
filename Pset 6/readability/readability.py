from cs50 import get_string


def main():
    text = get_string("Text: ")

    # Calling functions to get the letter, word, and sentence counts
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sent(text)

    # Coleman-Liau index calculation
    L = (letters * 100) / words
    S = (sentences * 100) / words
    index = round((0.0588 * L) - (0.296 * S) - 15.8)

    # Printing the appropriate reading level for the text
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Defining funcion that inputs text and outputs the number of letters
def count_letters(string):
    let_count = 0
    for i in string:
        if i.isalpha():
            let_count += 1
    return let_count


# Defining funcion that inputs text and outputs the number of words
def count_words(string):
    word_count = 1
    for i in string:
        if i == " ":
            word_count += 1
    return word_count


# Defining funcion that inputs text and outputs the number of sentences
def count_sent(string):
    sent_count = 0
    for i in string:
        if i == "." or i == "!" or i == "?":
            sent_count += 1
    return sent_count


# Calling the main function to run the program
main()