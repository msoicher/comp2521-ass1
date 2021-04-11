
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "readData.h"

// Implementation of Graph structure
// Code used from provided code in lab08, Graph.h and Graph.c, written by John Shepherd

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;


// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}


// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}
// insert an Edge
// - sets only (v,w) since it is directed
// unweighted so just setting it = 1

void insertEdge (Graph g, int v, int w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = 1;
	g->nE++;
}
// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// make an edge
Edge makeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

/* Create a	set	(list) of urls to process by reading data
from file collection.txt */

char **GetCollection() {

    // opening collection.txt for reading
    FILE *fptr = fopen("collection.txt", "r");

    // finding the number of urls in collection.txt
    char URL[100];
    int numFiles = 0;
    
    while (fscanf(fptr, "%s", URL) != EOF) {
        numFiles++;
    }

    fclose(fptr);

    // allocating memory for our urlList
    FILE *fpointer = fopen("collection.txt", "r");

    char **urlArray = malloc(numFiles * sizeof(char *));
    char url[100];

    // Scanning collection.txt and reading in each url individually, assigning it to char url
    int i = 0;
    while (fscanf(fpointer, "%s", url) != EOF) {
        urlArray[i] = malloc(strlen(url) +1);
        strcpy(urlArray[i], url);
        i++;
    }
    fclose(fpointer);

    return urlArray;
}


int findNumFiles() {

    // opening collection.txt for reading
    FILE *fptr = fopen("collection.txt", "r");

    // finding the number of urls in collection.txt
    char URL[100];
    int numFiles = 0;
    
    while (fscanf(fptr, "%s", URL) != EOF) {
        numFiles++;
    }

    fclose(fptr);
    return numFiles;
}

/* When accessing a url's contents, we have to find which element in the matrix it pertains to.
   It corresponds to which element it is in urlArray */

Vertex findUrlElement(char **urlArray, char *url) {

    int numFiles = findNumFiles();

    for (int i = 0; i < numFiles; i++)
        if (strcmp(url, urlArray[i]) == 0) {
            return i;
        }
    return 0;
}

/* Create empty graph for each url in the above list.
For each url in the above list, read <url>.txt ile and update
graph by adding a node and outgoing links */

Graph GetGraph(char **urlArray) {

    // number of files in the listofurls
    int numFiles = findNumFiles();

    // create new, empty graph
    Graph g = newGraph(numFiles);

    // read each element in the list line by line, and access its contents
    // update its links which are given

    // loop through the urlList
    char *url;
    url = malloc(sizeof(char));

    for (int element = 0; element < numFiles; element++) {

        url = malloc(strlen(urlArray[element]));
        strcpy(url, urlArray[element]);
        strcat(url, ".txt"); // add .txt suffix to access its info

        FILE *fpointer = fopen(url, "r"); // opening for reading

        char *currURL = malloc(sizeof(currURL));
        
        // accessing the contents of the current url, assigning word by word to currURL
        while (fscanf(fpointer, "%s", currURL) != EOF) {

            if (currURL[0] == 'u' && currURL[1] == 'r' && currURL[2] == 'l') {

                /* for each url, make a connection, but we need to know where the url connection
                   is in our graph so that we make the correct edge in the graph */
                int urlElement = findUrlElement(urlArray, currURL);

                // create a new edge
                makeEdge(g, element, urlElement);
                insertEdge(g, element, urlElement);
            }
        }
    }
    return g;
}
