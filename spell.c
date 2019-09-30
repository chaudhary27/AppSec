#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Code skeleton adapted from https://github.com/bahalperin/spell-checker/blob/master/dictionary.c

bool check_word(const char *word, hashmap_t hashtable[])
{
    
    int word_length = strlen(word);
    char lower_word[LENGTH+1];

    // Convert word to lowercase
    for (int i = 0; i < word_length; i++)
    {
        // If character is uppercase, make it lowercase.
        if(isupper(word[i]))
        {
            lower_word[i] = tolower(word[i]) ;
        }
        // Otherwise it's already lowercase or it's not a letter.
        else
        {
            lower_word[i] = word[i];
        }
    }
    // Add null character to end of char array... not sure why this is needed?
    lower_word[word_length] = '\0';

    int bucket = hash_function(word);

    hashmap_t cursor = hashtable[bucket];

    while(cursor != NULL){
        if(strcmp(word, cursor->word) == 0){
            return true;
        }
        else if(strcmp(tolower(word), cursor->word) == 0){
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}



bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[]) 
{ 
    // Initialize all values in hash table to NULL
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    // Open dict_file from path stored in dictionary.
    FILE* dict_file = fopen(dictionary_file, "r");

    if (dict_file == NULL)
    {
        printf("no such file...");
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

    }
    fclose(dict_file);
    return true;
};


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
    int num_misspelled = 0;

    // While line in fp is not EOF (end of file):
    while (fscanf(fp, "%s", buffer) > 0)
    {


    }
        Read the line.
        Split the line on spaces.
        For each word in line:
            Remove punctuation from beginning and end of word.
            If not check_word(word):
                Append word to misspelled.
                Increment num_misspelled.
                
    return num_misspelled;
};
