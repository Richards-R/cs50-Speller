// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 265225;

int dict_count = 0;
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int slot_num = hash(word);

    node *next_word = NULL;

    if (table[slot_num] == NULL)
    {
        return false;
    }

    if (strcasecmp(table[slot_num]->word, word) == 0)
    {
        return true;
    }
    else
    {

        next_word = table[slot_num]->next;

        while (next_word != NULL)
        {
            if (strcasecmp(next_word->word, word) == 0)
            {
                return true;
            }
            else
            {
                next_word = next_word->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int word_sum = 0;
    int char_val = 0;
    int n = strlen(word);
    int new_sum = 0;

    for (int i = 0; i <= n - 1; i++)
    {
        if (i == 45)
        {
            break;
        }
        else
        {
            if (word[i] == '\'')
            {
                char_val = 25;
            }
            else
            {
                char_val = toupper(word[i]) - 'A';
            }
            word_sum += char_val;
        }
    }
    return word_sum * word_sum;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file

    FILE *source = fopen(dictionary, "r");

    if (source == NULL)
    {
        printf("ERROR: Failed to open dictionary");
        return false;
    }
    // Read each word in the file

    char c;
    char word_store[LENGTH + 1];
    for (int i = 0; i < LENGTH + 1; i++)
    {
        // clear word store
        word_store[i] = 0;
    }

    int letter_count = 0;
    node *list = NULL;

    while (fread(&c, sizeof(char), 1, source))
    {
        if (c != '\n')
        {
            word_store[letter_count] = toupper(c);
            letter_count++;
        }
        else
        {
            // Add each word to the hash table
            dict_count++;

            // Hash the word to obtain its hash value
            int slot_num = hash(word_store);

            // Find the list to add node to
            list = table[slot_num];

            // Create space for a new node
            node *head = malloc(sizeof(node));

            if (head == NULL)
            {
                printf("ERROR: Not enough memory for new hash table slot");
            }
            else
            {

                for (int i = 0; i < letter_count + 1; i++)
                {
                    // Copy the word into the new node
                    head->word[i] = word_store[i];
                    word_store[i] = 0;
                }
                head->next = list;

                list = head;

                // Insert the new node into the hash table (using the index specified by its hash value)
                table[slot_num] = list;

                letter_count = 0;
            }
        }
    }
    // Close the dictionary file
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dict_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = cursor;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
