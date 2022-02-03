#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Word {
    char **translations;
    struct Word *next;
} Word;

typedef struct {
    char **languages;
    int numOfLanguages;
    Word *wordList;
} Dictionary;

Dictionary **New_Dictionary(Dictionary **dictionaries, int *numOfDictionaries);

void Add_Word(Dictionary **dictionaries, int numOfDictionaries);

void Del_Word(Dictionary **dictionaries, int numOfDictionaries);

void Find_Word(Dictionary **dictionaries, int numOfDictionaries);

Dictionary **Del_Dictionary(Dictionary **dictionaries, int *numOfDictionaries);

void freeWord(Word *word, int numOfTranslations);

void freeWordList(Word *word, int numOfTranslations);

void freeDictionary(Dictionary *dict);

void freeAll(Dictionary **dictionaries, int numOfDictionaries);

int printDictionaries(Dictionary **dictionaries, int numOfDictionaries);

char *getString();

int getNumOfWords(char *str);

int getWordSize(char *str);


int main() {
    int num, numOfDictionaries = 0;
    Dictionary **dictionaries;

    // print the menu until the user want to exit
    while (1) {
        printf("Welcome to the dictionaries manager!\nChoose an option:\n1. Create a new dictionary.\n"
               "2. Add a word to a dictionary.\n3. Delete a word from a dictionary.\n"
               "4. Find a word in a dictionary.\n5. Delete a dictionary.\n6. Exit.\n");
        scanf(" %d", &num);

        switch (num) {
            case 1:
                dictionaries = New_Dictionary(dictionaries, &numOfDictionaries);
                break;
            case 2:
                Add_Word(dictionaries, numOfDictionaries);
                break;
            case 3:
                Del_Word(dictionaries, numOfDictionaries);
                break;
            case 4:
                Find_Word(dictionaries, numOfDictionaries);
                break;
            case 5:
                dictionaries = Del_Dictionary(dictionaries, &numOfDictionaries);
                break;
            case 6:
                freeAll(dictionaries, numOfDictionaries);
                return 0;
            default:
                printf("Wrong option, try again:\n");
        }
    }
}

Dictionary **New_Dictionary(Dictionary **dictionaries, int *numOfDictionaries) {
    int numOfLanguages = 0;
    printf("Define a new dictionary:\n");

    // get the languages from the user
    char *pointToLanguages = getString(), *language;
    if (pointToLanguages == NULL) {
        printf("The creation of the dictionary has failed!\n");
        return dictionaries;
    }

    // allocate memory for the languages
    char **languages = (char **) malloc(sizeof(char *) * getNumOfWords(pointToLanguages));
    if (languages == NULL) {
        printf("The creation of the dictionary has failed!\n");
        return NULL;
    }
    char *token = strtok(pointToLanguages, ",");

    while (token != NULL) {
        // allocate memory for a language
        language = (char *) malloc(sizeof(char) * (getWordSize(token) + 1));
        if (language == NULL) {
            printf("The creation of the dictionary has failed!\n");
            return NULL;
        }

        // save the language in the array of languages
        strcpy(language, token);
        languages[numOfLanguages++] = language;
        token = strtok(NULL, ",");
    }

    // allocate memory for the dictionary
    Dictionary *dict = (Dictionary *) malloc(sizeof(Dictionary));
    if (dict == NULL) {
        printf("The creation of the dictionary has failed!\n");
        return NULL;
    }

    dict->numOfLanguages = numOfLanguages;
    dict->languages = languages;
    dict->wordList = NULL;

    if (*numOfDictionaries == 0)
        dictionaries = (Dictionary **) malloc(sizeof(Dictionary *));
    if (*numOfDictionaries > 0)
        dictionaries = (Dictionary **) realloc(dictionaries, sizeof(Dictionary *) * (*numOfDictionaries + 1));
    if (dictionaries == NULL) {
        printf("The creation of the dictionary has failed!\n");
        return NULL;
    }
    dictionaries[*numOfDictionaries] = dict;
    (*numOfDictionaries)++;
    printf("The dictionary has been created successfully!\n");
    return dictionaries;
}

void Add_Word(Dictionary **dictionaries, int numOfDictionaries) {
    // check there is at least a single dictionary
    if (numOfDictionaries == 0) {
        printf("This option is not available right now, try again:\n");
        return;
    }
    int option = printDictionaries(dictionaries, numOfDictionaries) - 1, numOfTranslations = 0;

    // print the languages of the word
    printf("Enter a word in ");
    for (int i = 0; i < dictionaries[option]->numOfLanguages - 1; i++)
        printf("%s,", dictionaries[option]->languages[i]);
    printf("%s:\n", dictionaries[option]->languages[dictionaries[option]->numOfLanguages - 1]);

    // get the translations from the user
    char *translation, *pointToWords = getString();
    if (pointToWords == NULL) {
        printf("The addition of the word has failed!\n");
        return;
    }
    char *token = strtok(pointToWords, ",");

    // allocate memory for the translations
    char **translations = (char **) malloc(sizeof(char *) * getNumOfWords(pointToWords));
    if (translations == NULL) {
        printf("The addition of the word has failed!\n");
        return;
    }

    while (token != NULL) {
        // allocate memory for a translation
        translation = (char *) malloc(sizeof(char) * (getWordSize(token) + 1));
        if (translation == NULL) {
            printf("The addition of the word has failed!\n");
            return;
        }

        // save the translation in the array of translations
        strcpy(translation, token);
        translations[numOfTranslations++] = translation;
        token = strtok(NULL, ",");
    }

    // allocate a memory for the word
    Word *word = (Word *) malloc(sizeof(Word));
    if (word == NULL) {
        printf("The addition of the word has failed!\n");
        return;
    }

    word->translations = translations;
    word->next = dictionaries[option]->wordList;
    dictionaries[option]->wordList = word;
    printf("The word has been added successfully!\n");
}

void Del_Word(Dictionary **dictionaries, int numOfDictionaries) {
    // check there is at least a single dictionary
    if (numOfDictionaries == 0) {
        printf("This option is not available right now, try again:\n");
        return;
    }
    int option = printDictionaries(dictionaries, numOfDictionaries) - 1;
    printf("Enter a word in %s\n", dictionaries[option]->languages[0]);

    // get the word from the user
    char *word = getString(), c;
    if (word == NULL) {
        printf("The deletion of the word has failed!\n");
        return;
    }

    // check the user is sure about deleting the word
    printf("Are you sure? (y/n)\n");
    scanf(" %c", &c);
    if (c != 'y') {
        printf("The deletion of the word has been canceled.\n");
        return;
    }

    // check there is at least one word
    Word *node = dictionaries[option]->wordList;
    if (node == NULL) {
        printf("The deletion of the word has failed!\n");
        return;
    }

    // if the word we want to delete is the first one
    if (strcmp(node->translations[0], word) == 0) {
        Word *save = node->next;
        freeWord(node, dictionaries[option]->numOfLanguages);
        dictionaries[option]->wordList = save;
        printf("The word has been deleted successfully!\n");
        return;
    }

    // find in the linked list the word we want to delete
    while (node->next != NULL) {
        if (strcmp(node->next->translations[0], word) == 0)
            break;
        node = node->next;
    }

    // we didn't find the word
    if (node->next == NULL) {
        printf("The deletion of the word has failed!\n");
        return;
    }

    // save the next node and delete the node from the list
    Word *save = node->next->next;
    freeWord(node->next, dictionaries[option]->numOfLanguages);
    free(node->next);
    node->next = save;

    printf("The word has been deleted successfully!\n");
}

void Find_Word(Dictionary **dictionaries, int numOfDictionaries) {
    // check there is at least a single dictionary
    if (numOfDictionaries == 0) {
        printf("This option is not available right now, try again:\n");
        return;
    }
    int option = printDictionaries(dictionaries, numOfDictionaries) - 1;
    printf("Enter a word in %s\n", dictionaries[option]->languages[0]);

    // get the word from the user
    char *word = getString();
    if (word == NULL) {
        printf("The search has failed successfully!\n");
        return;
    }
    Word *node = dictionaries[option]->wordList;

    // find the word in the linked list
    while (node != NULL) {
        if (strcmp(node->translations[0], word) == 0)
            break;
        node = node->next;
    }

    // we didn't find the word
    if (node == NULL) {
        printf("There are no translations for %s in this dictionary.\n", word);
        return;
    }

    // print all the translations of the word
    int numOfTranslations = dictionaries[option]->numOfLanguages;
    printf("The translations are:\n");
    for (int i = 1; i < numOfTranslations - 1; ++i)
        printf("%s: %s, ", dictionaries[option]->languages[i], node->translations[i]);
    printf("%s: %s\n", dictionaries[option]->languages[numOfTranslations - 1],
           node->translations[numOfTranslations - 1]);
}

Dictionary **Del_Dictionary(Dictionary **dictionaries, int *numOfDictionaries) {
    // check there is at least a single dictionary
    if (*numOfDictionaries == 0) {
        printf("This option is not available right now, try again:\n");
        return dictionaries;
    }
    int option = printDictionaries(dictionaries, *numOfDictionaries) - 1;
    char c;

    // check the user is sure about deleting the dictionary
    printf("Are you sure? (y/n)\n");
    scanf(" %c", &c);
    if (c != 'y') {
        printf("The deletion of the dictionary has been canceled.\n");
        return dictionaries;
    }

    // deallocate all the memory of the dictionary
    freeDictionary(dictionaries[option]);
    for (int i = option; i < *numOfDictionaries - 1; ++i)
        dictionaries[i] = dictionaries[i + 1];

    if (*numOfDictionaries == 1)
        free(dictionaries);

    if (*numOfDictionaries > 1)
        dictionaries = (Dictionary **) realloc(dictionaries,
                                               sizeof(Dictionary *) * (*numOfDictionaries - 1));
    (*numOfDictionaries)--;
    printf("The dictionary has been deleted successfully!\n");

    return dictionaries;
}

void freeDictionary(Dictionary *dict) {
    for (int i = 0; i < dict->numOfLanguages; ++i)
        free(dict->languages[i]);
    free(dict->languages);
    freeWordList(dict->wordList, dict->numOfLanguages);
    free(dict);
}

void freeWord(Word *word, int numOfTranslations) {
    for (int i = 0; i < numOfTranslations; ++i)
        free(word->translations[i]);
    free(word->translations);
    free(word);
}

void freeWordList(Word *word, int numOfTranslations) {
    if (word != NULL) {
        freeWordList(word->next, numOfTranslations);
        freeWord(word, numOfTranslations);
    }
}

void freeAll(Dictionary **dictionaries, int numOfDictionaries) {
    for (int i = 0; i < numOfDictionaries; ++i)
        freeDictionary(dictionaries[i]);
    if (numOfDictionaries > 0)
        free(dictionaries);
    printf("Bye!\n");
}

int getWordSize(char *str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

int getNumOfWords(char *str) {
    int i = 0, count = 1;
    while (str[i] != '\0')
        if (str[i++] == ',')
            count++;
    return count;
}

char *getString() {
    // allocate memory for one char
    char *str = (char *) malloc(sizeof(char)), c;
    if (str == NULL)
        return NULL;

    int counter = 1;
    scanf(" %c", &c);
    while (c != '\n') {
        // allocate memory for another char
        str = (char *) realloc(str, ((counter + 1) * sizeof(char)));
        if (str == NULL)
            return NULL;

        // put the new char in the string and get another one
        str[counter - 1] = c;
        counter++;
        scanf("%c", &c);
    }

    // put in the end of the string null
    str[counter - 1] = '\0';
    return str;
}

int printDictionaries(Dictionary **dictionaries, int numOfDictionaries) {
    printf("Choose a dictionary:\n");
    for (int i = 0; i < numOfDictionaries; i++) {
        printf("%d. ", i + 1);
        for (int j = 0; j < dictionaries[i]->numOfLanguages - 1; j++)
            printf("%s,", dictionaries[i]->languages[j]);
        printf("%s\n", dictionaries[i]->languages[dictionaries[i]->numOfLanguages - 1]);
    }
    int option;
    scanf(" %d", &option);
    return option;
}
