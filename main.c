#include <stdio.h>
#include <string.h>

#define WORD_SIZE 15
#define WORD_COUNT 20
#define MAX_MISTAKES 5
#define LETTERS_NUMBER 26

// Prints the letters the user tried
void printLettersTried(char lettersTried[]) {
    printf("The letters that you already tried: ");
    int i = 1;
    if (lettersTried[0] != '\0')
        printf("%c", lettersTried[0]);
    while (lettersTried[i] != '\0') {
        printf(", %c", lettersTried[i]);
        i++;
    }
    printf("\nplease choose a letter:\n");
}

// Prints the hanged man until now
void printMan(int mistakes){
    printf(" _______________\n");
    switch (mistakes) {
        case 0:
            printf("|               |\n|               |\n|               |\n|               |\n|               |\n");
            break;
        case 1:
            printf("|               |\n|  **           |\n|  **           |\n|               |\n|               |\n");
            break;
        case 2:
            printf("|               |\n|  **       **  |\n|  **       **  |\n|               |\n|               |\n");
            break;
        case 3:
            printf("|  --           |\n|  **       **  |\n|  **       **  |\n|               |\n|               |\n");
            break;
        case 4:
            printf("|  --       --  |\n|  **       **  |\n|  **       **  |\n|               |\n|               |\n");
            break;
        case 5:
            printf("|  --       --  |\n|  **       **  |\n|  **       **  |\n|               |\n| \\/\\/\\/\\/\\/\\/\\ |\n");
            break;
        default:
            break;
    }
    printf("|_______________|\n");
}

// Checks if the letter was chosen before
int isChose(char lettersTried[], char c, int countTried) {
    for (int i = 0; i < countTried; i++)
        if (lettersTried[i] == c)
            return 1;
    return 0;
}

// Checks if the letter is in the word and update the letters the user found
int isRight(char word[], char found[], char c) {
    int i = 0, count_ = 0;
    while (word[i] != '\0') {
        if (word[i] == c)
            found[i] = c;
        if (found[i] == '_')
            count_++;
        i++;
    }
    return count_;
}

void game(char word[WORD_COUNT], char found[WORD_COUNT], char clue[WORD_COUNT], int count_) {
    int mistakes = 0, flag = 0, countTried = 0;
    char c, lettersTried[LETTERS_NUMBER] = "";

    // print the hanged man and get another letter until 5 mistakes or the user guessed the word
    while (mistakes != MAX_MISTAKES && count_ != 0) {
        printMan(mistakes);
        printf("%s\n", found);
        if (!flag)
            printf("do you want a clue? press -> *\n");
        printLettersTried(lettersTried);
        scanf(" %c", &c);

        // Print a clue to the screen
        if (c == '*') {
            printf("The clue is: %s\n", clue);
            flag = 1;
            continue;
        }

        // The letter was chosen before by the user
        if (isChose(lettersTried, c, countTried)) {
            printf("You've already tried that letter\n");
            continue;
        }

        // Add the new letter to the array of the letters we tried
        lettersTried[countTried] = c;
        countTried++;

        // Update the word with the new letter and the count of the letters remained
        int newCount_ = isRight(word, found, c);
        if (newCount_ != count_)
            printf("%s\n", found);
        else
            mistakes++;
        count_ = newCount_;

    }
    printMan(mistakes);

    // Print to the screen if the user won the game
    if(mistakes == MAX_MISTAKES)
        printf("The word is %s, GAME OVER!", word);
    if(count_ == 0)
        printf("The word is %s, good job!", word);
}

// The function swaps between the strings
void swap(char *a, char *b) {
    char temp[WORD_COUNT];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

// The function sorts the array of words in O(n^2) time
void bubbleSort(char words[WORD_SIZE][WORD_COUNT], int count) {
    int i, j;
    for (i = 1; i < count; i++)

        // Last i elements are already in place
        for (j = 1; j < count - i; j++)
            if (strcmp(words[j], words[j + 1]) > 0)
                swap(words[j], words[j + 1]);
}

int main() {
    char str[WORD_COUNT * WORD_SIZE], words[WORD_SIZE][WORD_COUNT], *token, found[WORD_COUNT];
    int count = 0, count_ = 0, i = 0, option;
    printf("Enter your words!\n");
    scanf("%[^\n]", str);

    // get the first token
    token = strtok(str, ":");

    // walk through other tokens
    while( token != NULL ) {
        strcpy(words[count], token);
        token = strtok(NULL, ",");
        count++;
    }

    // Sort the array and print it to the screen in lexicographic order
    bubbleSort(words, count);
    printf("Choose an option:\n");
    for(int i = 1; i < count; i++){
        printf("%d: %s\n",i , words[i]);
    }
    scanf("%d", &option);

    // Initialize the game by put underlines in the indexes where the user need to guess the letters
    while (words[option][i] != '\0') {
        if (words[option][i] == ' ') {
            found[i++] = ' ';
            continue;
        }
        found[i++] = '_';
        count_++;
    }
    found[i] = '\0';

    // Run the game
    game(words[option], found, words[0], count_);
    return 0;
}
