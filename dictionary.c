// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// declare a function to free up memory space
void free_all(node *next);

node *root = NULL; // root of the trie
bool is_loaded = false; // indicator for loaded dictionary
int words = 0; // amount of words in dictionary

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *tmp = root; // temporary pointer to the root of trie
    int letter; // ascii number of the letter
    int index = 0; // calculated index of a letter in array

    // check each letter of the word for being in the trie
    for (int i = 0; word[i] != '\0'; i++)
    {
        // make any letter lowercase
        letter = tolower(word[i]);

        // calculate the index of a letter
        index = (letter != '\'') ? letter - 'a' : 26;

        // check if such element(structure) is in a trie
        if (tmp->children[index])
        {
            // change the value of temporary pointer to this element
            tmp = tmp->children[index];
        }
        else
        {
            // word is not in the dictionary
            return false;
        }
    }

    // if the last element(structure) is the end of the word, this word is properly spelled
    if (tmp->is_word)
    {
        return true;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // allocate memory for root structure
    root = calloc(1, sizeof(node));

    // open file for reading
    FILE *file = fopen(dictionary, "r");

    // if file fails opening
    if (!file)
    {
        return false;
    }

    int index = 0; // calculated index of a letter in array
    node *tmp = root; // temporary pointer to the root of trie

    // reaв each letter of the file
    for (int letter = fgetc(file); letter != EOF; letter = fgetc(file))
    {
        // if it is not the end of the word
        if (letter != '\n')
        {
            // calculate the index of a letter
            index = (letter != '\'') ? letter - 'a' : 26;

            // if such element(structure) doesnt exist, create a new one
            if (!tmp->children[index])
            {
                tmp->children[index] = calloc(1, sizeof(node));

                // change the value of temporary pointer to this element
                tmp = tmp->children[index];
            }
            else
            {
                // if element exists, change the value to it
                tmp = tmp->children[index];
            }
        }
        else
        {
            tmp->is_word = true; // the end of the word
            words++; // increase total amount of words
            tmp = root; // restart allocating memory for a new word
        }
    }


    // close file from reading
    fclose(file);
    is_loaded = true;

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (is_loaded)
    {
        // return total amount of words calculated in load function
        return words;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp = root; // temporary pointer to the root of trie
    free_all(tmp); // free up memory
    return true;
}

void free_all(node *next)
{
    // go down to the end of trie
    for (int i = 0; i < 27; i++)
    {
        // if child element exists
        if (next->children[i])
        {
            // recursive search of the bottom element
            free_all(next->children[i]);
        }
    }

    // delete element in chain
    free(next);
}
