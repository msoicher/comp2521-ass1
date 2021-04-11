#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "invertedIndex.h"

#define TRUE 1
#define FALSE 0

InvertedIndexBST createWord(char *word, char *fileName);
InvertedIndexBST InsertWord(InvertedIndexBST t, InvertedIndexBST newWord);
FileList createFileNode(char *fileName);
FileList insertFile(FileList L, FileList newFile);
InvertedIndexBST getWord(InvertedIndexBST t, char *word);
int wordExists(InvertedIndexBST t, char *word);
int fileExists(InvertedIndexBST t, char *fileName);
void printTree(InvertedIndexBST t, FILE *fp);

char *normaliseWord(char *str) {

    int strLength = strlen(str);

    // remove whitespace from end first;
    for (int i = strLength - 1; str[i] == ' '; i--) {
        str[i] = '\0';
        strLength--;
    }

    // remove from beginning
    while (str[0] == ' ') {
        for (int j = 0; j < strLength - 1; j++) {
            str[j] = str[j+1];
        }
        strLength--;
    }

    int lastChar = strLength - 1;

    // remove last punctuation mark
    if (str[lastChar] == '.' || str[lastChar] == ','
        || str[lastChar] == ';' || str[lastChar] == '?') {
        str[lastChar] = '\0';
    }
    strLength = strlen(str);
    // to lowercase
    for (int k = 0; k < strLength; k++) {
        str[k] = tolower(str[k]);
    }
    return str;
}


InvertedIndexBST generateInvertedIndex(char *collectionFilename) {

    /* The function needs to read a given file with collection of file names,
        read each of these files, generate inverted index as discussed
        earlier in the specs and return the inverted index.
        Do not modify invertedIndex.h file. */

    FILE *collectionFilePointer = fopen(collectionFilename, "r"); // opens up collectionFilename for reading

    char fileName[100];
    char wordPtr[100];
    char word[100];
    InvertedIndexBST t = NULL;

    while (fscanf(collectionFilePointer, "%s ", fileName) != EOF) { // scans the first .txt file to fileName

        FILE *fileNamePtr = fopen(fileName, "r"); // opening .txt file for reading

        while (fscanf(fileNamePtr, "%s", wordPtr) != EOF) { // scanning the words within the .txt file

            strcpy(word, normaliseWord(wordPtr));

            // If the word does not exist in the t, we want to create a new node and insert it
            if (wordExists(t, word) == FALSE) {

                InvertedIndexBST newWord = createWord(word, fileName);
                t = InsertWord(t, newWord);

            /* Otherwise, the word does exist in the tree, and we want to add the file if the file doesn't already exist */
            } else if (fileExists(getWord(t, word), fileName) == FALSE) {
                // Get the node where the word exists
                InvertedIndexBST requiredWord = getWord(t, word);
                // creates a new file node and adds it into the required words fileList
                FileList newFile = createFileNode(fileName);
                requiredWord->fileList = insertFile(requiredWord->fileList, newFile);
            }
        }
        fclose(fileNamePtr);
    }
    fclose(collectionFilePointer);
    return t;
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    return 0;
}

TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    return 0;
}


// recursively calling itself to print the tree In-Order
void printTree(InvertedIndexBST t, FILE *fp) {

    if (t == NULL) return;

    printTree(t->left, fp);

    fprintf(fp, "%s", t->word);

    while (t->fileList != NULL) {
        fprintf(fp, " %s", t->fileList->filename);
        t->fileList = t->fileList->next;
    }
    fprintf(fp, "\n");

    printTree(t->right, fp);
    return;
}
// Print the tree
void printInvertedIndex(InvertedIndexBST tree) {

    FILE * fp = fopen("invertedIndex.txt", "w");
    printTree(tree, fp);
    fclose(fp);
    }
// this function creates a new node in the tree if it doesn't already exist
// By creating this new Node, we need to create a newNode within fileList->fileName, using createFileNode

InvertedIndexBST createWord(char *word, char *fileName) {

    InvertedIndexBST newWord = malloc(sizeof(struct InvertedIndexNode));
    newWord->word = malloc(strlen(word) +1);
    strcpy(newWord->word, word);
    newWord->fileList = createFileNode(fileName);
    newWord->left = newWord->right = NULL;
    return newWord;
}
// inserts a new word into the tree: adapted from week03 lab

InvertedIndexBST InsertWord(InvertedIndexBST t, InvertedIndexBST newWord) {

    // If the tree is empty, insert the word at the position
    if (t == NULL) {
        return newWord;
    /* Otherwise, compare with the current word.
    If it is lexicographically smaller, go left, otherwise right */
    } else if (strcmp(newWord->word, t->word) < 0) {
        t->left = InsertWord(t->left, newWord);
    } else if (strcmp(newWord->word, t->word) > 0) {
        t->right = InsertWord(t->right, newWord);
    }
    return t;
}

// Create a new FileList Node: adapted from week01 lab
FileList createFileNode(char *fileName) {

    FileList newFile = malloc(sizeof(struct FileListNode));
    newFile->filename = malloc(strlen(fileName) +1);
    strcpy(newFile->filename, fileName);
    newFile->tf = 0;
    newFile->next = NULL;
    return newFile;
}
// Insert file into the linked list.
FileList insertFile(FileList L, FileList newFile) {

    // first in list
    if (strcmp(newFile->filename, L->filename) < 0) {
        newFile->next = L;
        return newFile;
    } else {
        FileList curr = L;
        while (curr->next != NULL && strcmp(curr->next->filename, newFile->filename) < 0) {
            curr = curr->next;
        }
        newFile->next = curr->next;
        curr->next = newFile;
        return L;
        free(curr);
    }
}
// This function returns the word in bst if it exists
InvertedIndexBST getWord(InvertedIndexBST t, char *word) {

    if (strcmp(word, t->word) < 0) {
        return getWord(t->left, word);
    } else if (strcmp(word, t->word) > 0) {
        return getWord(t->right, word);
    } else {
        return t;
    }
}

// Checking if a word exists in a tree. If it does, we need
// to find that position and add the fileName to the list
int wordExists(InvertedIndexBST t, char *word) {
    if (t == NULL) {
        return FALSE;
    } if (strcmp(word, t->word) < 0) {
        return wordExists(t->left, word);
    } else if (strcmp(word, t->word) > 0) {
        return wordExists(t->right, word);
    } else {
        return TRUE;
    }
}

int fileExists(InvertedIndexBST t, char *fileName) {

    FileList L = t->fileList;

    if (L == NULL) {
        return FALSE;
    }
    // Traversing while curr isn't null and while we haven't found if the file exists
    while (L != NULL && (strcmp(L->filename, fileName) != 0)) {
        L = L->next;
    }
    if (L == NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
