#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_wordlist.txt"

bool check_word(const char *word, hashmap_t hashtable[])
{
    // Get the lower case representation of the word to compare against.
    char *lower_case_word = (char *)malloc((strlen(word) + 1) * sizeof(char));
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        lower_case_word[i] = tolower(word[i]);
    }

    // Get the hash_value to identify the location in the hashmap.
    int hash_value = hash_function(lower_case_word);
    hashmap_t hash_value_entry = hashtable[hash_value];

    // Check the values in the hashmap at the index location.
    do
    {
        char *word_at_location = hash_value_entry->word;

        // We could potentially just use strcmp() here, but to
        // be on the safe side we use strcasecmp to leave case
        // out of the equation>
        if (strcasecmp(word, word_at_location) == 0)
        {
            free(lower_case_word);
            return true;
        }

        // check the next node
        hash_value_entry = hash_value_entry->next;

        // Repeat the loop until we no longer have nodes in the linked list
    } while (hash_value_entry->word);

    // allocated memory is let go of
    free(lower_case_word);

    // If we don't specifically check out as a correctly spelled
    // word, we return false
    return false;
}

int num_words = 0;

bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[]) 
{

    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    // file pointer for the dictionary file.
    FILE *dict_file = fopen(dictionary_file, "r");

    if (dict_file == NULL)
    {
        printf("Couldn't open file or file does not exists.");
        return false;
    }

    int bucket;
    char buffer[LENGTH + 1];

    while (fscanf(dict_file, "%s", buffer) > 0)
    {
        hashmap_t new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word, buffer);
        bucket = hash_function(new_node->word);

        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
        num_words++;
    }
    fclose(dict_file);
    return true;
};

