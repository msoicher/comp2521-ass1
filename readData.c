#include <stdio.h>
#include <string.h>

Graph newGraph (int nV);
static Edge makeEdge(Graph g, Vertex v, Vertex w);
void insertEdge (Graph g, Vertex v, Vertex w);
int findUrlElement(char *urlList, char url);

/* Create a	set	(list) of urls to process by reading data
from file collection.txt */

List_of_Urls GetCollection() {

    // opening collection.txt for reading
    FILE *fpointer = fopen("collection.txt", "r");
    FILE *fptr = fopen("collection.txt", "r");

    // finding the number of urls in collection.txt
    char *URL;
    while (fscanf(fptr, "%s", URL)) {
        numFiles++;
    }

    fclose(fptr);

    // allocating memory for our urlList
    char *urlList = calloc(numFiles, sizeof(char));
    char url[100];

    // Scanning collection.txt and reading in each url individually, assigning it to char url
    while (fscanf(fpointer, "%s", url) != EOF) {
        strcat(urlList, url);
        strcat(urlList, "\n");
    }
    fclose(fpointer);
    return urlList;
}

/* Create empty graph for each url in the above list.
For each url in the above list, read <url>.txt ile and update
graph by adding a node and outgoing links */

Graph GetGraph(char List_of_Urls) {
// taken from lab08

    typedef struct GraphRep {
        int nV;		 // #vertices
        int nE;		 // #edges
        int **edges; // matrix of connections
    } GraphRep;

    typedef struct {
	    Vertex v;
	    Vertex w;
    } Edge;

    // number of files in the listofurls
    int numFiles = sizeof(List_of_Urls) / sizeof(List_of_Urls[0]);

    // create new, empty graph

    Graph g = newGraph(numFiles);

    // read each element in the list line by line, and access its contents
    // update its links which are given

    // loop through the urlList

    for (int element = 0; element < numFiles; element++) {

        char url[100] = List_of_Urls[element];
        strcat(url, ".txt") // add .txt suffix to access its info

        FILE *fpointer= fopen(url, "r"); // opening for reading

        char currURL;
        // accessing the contents of the current url, assigning word by word to currURL
        while (fscanf(fpointer, "%s" currURL)) {

            if (currURL[0] == 'u' && currURL[1] == 'r' && currURL[2] == 'l') {

                // for each url, make a connection, but we need the element to insert
                // the correct connection in the graph

                int urlElement = findUrlElement(List_of_Urls, currURL);

                // create a new edge l
                edge = makeEdge(g, element, urlElement);
                insertEdge(g, element, urlElement);
            }

        }
    }
    return g;
}

// taken from week08 lab, graph.c

Graph newGraph (int nV) {
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

static Edge makeEdge(Graph g, Vertex v, Vertex w) {
    Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
// unweighted so just setting it = 1

void insertEdge (Graph g, Vertex v, Vertex w) {

	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0)
		// an edge already exists; do nothing.
		return;

    // we want a directed graph, so only in one spot
	g->edges[v][w] = 1;
	g->nE++;
}
/* When accessing a url's contents, we have to find which element in the matrix it pertains to.
It corresponds to which element it is in urlList */

int findUrlElement(char *urlList, char url) {

    int numFiles = sizeof(list) / sizeof(list[0]);

    for (int i = 0; i < numFiles; i++)
        if (strcmp(url, urlList[i]) == 0) {
            return i;
        }
}
