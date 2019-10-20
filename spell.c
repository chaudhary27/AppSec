/*
Code skeleton adapted from https://github.com/bahalperin/spell-checker/blob/master/dictionary.c

*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "dictionary.c"


int main ()
{
    return 0;
}

bool check_word(const char *word, hashmap_t hashtable[])
{
    //Allocate memory for lowercase_word
    char *lowercase_word = (char *)malloc((strlen(word) + 1) * sizeof(char));

    for (int i = 0; i < strlen(word) + 1; i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }

    int bucket = hash_function(lowercase_word);
    hashmap_t cursor = hashtable[bucket];

    do
    {
        char *ptr = cursor->word;

        if (strcasecmp(word, ptr) == 0)
        {
            free(lowercase_word);
            return true;
        }

        cursor = cursor->next;

    } while (cursor->word);

    free(lowercase_word);
    return false;
}

int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[])
{
    bool long_word = false;
    int num_mispelled = 0;
    int i = 0;
    char word[LENGTH];

    while (fscanf(fp, "%45s", word) == 1)
    {
        int word_len = strlen(word);
        if (word_len == 45)
        {
            char ch = fgetc(fp);
            ungetc(ch, fp);
            if (!isspace(ch))
            {
                long_word = true;
                continue;
            }
        }

        if (long_word && word_len < LENGTH)
        {
            long_word = false;
            num_mispelled++;
            misspelled[i] = (char *)malloc(strlen(word));
            strcpy(misspelled[i++], word);
            continue;
        }

        char lst_ch = word[word_len - 1];
        if (ispunct(lst_ch))
        {
            word[word_len - 1] = '\0';
        }

        if (ispunct(word[0]))
        {
            memmove(word, word + 1, strlen(word) - 1);
        }

        if (!check_word(word, hashtable))
        {
            num_mispelled++;
            misspelled[i] = malloc(strlen(word) + 1);
            strcpy(misspelled[i++], word);
        }
    }
    return num_mispelled;
}

static bool add_hashmap(int bucket, char *word, hashmap_t hashtable[])
{
    hashmap_t new_node = malloc(sizeof(*new_node));
    strcpy(new_node->word, word);

    hashmap_t hash = hashtable[bucket];
    char *ptr = hash->word;

    if (ptr != NULL)
    {
        new_node->next = hash;
    }
    else
    {
        new_node->next = NULL;
    }

    hashtable[bucket] = new_node;
    return true;
}

bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[])
{
    int bucket;
    bool word_add;
    char word[LENGTH];
   
    // Open the dictionary file
    FILE *dict_file = fopen(dictionary_file, "r");

    if (dict_file == NULL)
    {
        return false;
    }

    while (fscanf(dict_file, "%45s", word) == 1)
    {
        //Allocate memory for lowercase_word
        char *lowercase_word = (char *)malloc((strlen(word) + 1) * sizeof(char));

        if (lowercase_word == NULL)
        {
            return false;
        }

        // Convert word to lowercase to accurately compare to hash table.
        for (int i = 0; i < (strlen(word) + 1); i++)
        {
            lowercase_word[i] = tolower(word[i]);
        }

        bucket = hash_function(lowercase_word);
        word_add = add_hashmap(bucket, lowercase_word, hashtable);

        if (!word_add)
        {
            return false;
        }

        free(lowercase_word);
    }

    fclose(dict_file);
    return true;
}