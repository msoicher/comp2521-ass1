#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "pagerank.h"
#include "readData.h"
// graph representation (adjacency matrix)

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Incorrect Input: Must be 3 arguments after ./pagerank\n");
        exit(1);
    }
    double d  = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    char **urlArray = GetCollection();
    
    Graph g = GetGraph(urlArray);
    int N = g->nV;
    // allocate meobtain the final pageRank lis
    float *pageRankList = malloc(g->nV * sizeof(float));
    pageRankList = calculatePageRank(g, d, diffPR, maxIterations, urlArray);
    printInOrder(pageRankList, N, urlArray, g);

    return 0;
}

// determine any given url's number of degrees, (outgoing links)
int findNumDegrees(char *urlName, Graph g, char **urlArray) { 

    // first, determine which element it belongs to in the graph
    int element = findUrlElement(urlArray, urlName);

    // Then, search that row in the graph, and find # 1's
    int degrees = 0;

    for (int i = 0; i < g->nV; i++) {
        // ignore self loops i.e. edges[5][5]
        if (g->edges[element][i] == 1 && i != element) {
            degrees++;
        }
    }
    return degrees;
}


// for each iteration, copy the calculated values into pageRank_t
void copyValues(float *pageRank_t, float *pageRank_t1, int N) {
    
    for (int i = 0; i < N; i++) {
        pageRank_t[i] = pageRank_t1[i];
    }
}

// Given a graph, and a vertex's index, determine the number of incoming links, ignoring self-loops
int numIncomingLinks(Graph g, Vertex index) {
    
    int numIncoming = 0;

    for (int i = 0; i < g->nV; i++) {
        if (g->edges[i][index] == 1 && i != index) {
            numIncoming++;
        } 
    }
    return numIncoming;
}
// Given a graph, and a vertex's index, determine the number of outgoing links, ignoring self-loops
int numOutgoingLinks(Graph g, Vertex index) {
    
    int numOutgoing = 0;

    for (int i = 0; i < g->nV; i++) {
        if (g->edges[index][i] == 1 && i != index) {
            numOutgoing++;
        } 
    }
    return numOutgoing;
}
// Given a graph G and a vertex's index, create an array of the index's outgoing links respective index
// i.e. index 5 has outward links to 3, 4: outGoingEdgesArray = {3,4}
// Find numOutgoingLinks first to determine sizeof array

int *outGoingLinkArray(Graph g, Vertex index) {
    
    int numElements = numOutgoingLinks(g, index);
    int arrayIndex = 0;
    int *outGoingArray = malloc(numElements * sizeof(int));

    for (int i = 0; i < g->nV; i++) {
        if (g->edges[index][i] == 1 && i != index) {
            *(outGoingArray + arrayIndex) = i;
            arrayIndex++;
        } 
    }
    return outGoingArray;
}

// Given a graph G and a vertex's index, create an array of the index's incoming links respective index
// i.e. vertex 5 has inward links from vertex 3 and 4: outGoingEdgesArray = {3, 4}
// Find numIncoming links first to determine sizeof array

int *incomingLinkArray(Graph g, Vertex index) {
 
    int numElements = numIncomingLinks(g, index);
    int arrayIndex = 0;
    int *incomingArray = malloc(numElements * sizeof(int));

    for (int i = 0; i < g->nV; i++) {
        if (g->edges[i][index] == 1 && i != index) {
            *(incomingArray + arrayIndex) = i;
            arrayIndex++;
        } 
    }
    return incomingArray;
}

double wIn(Graph g, Vertex pJ, Vertex pI) {

    // inCount of Vertex pI (numerator)
    double Iu = numIncomingLinks(g, pI);
    // R(v) = outgoing edges (reference pages) of pJ: array of the index's relating to pJ's outgoing edges
    int *Rv = outGoingLinkArray(g, pJ);
    // Ou = number of elements in the above array
    int numOutgoing = numOutgoingLinks(g, pJ);

    double Ip = 0;

    // for every given outgoing link of pJ, calculate its individual number of incoming links and add to sum
    for (int i = 0; i < numOutgoing; i++)  
        Ip += numIncomingLinks(g, Rv[i]); // 
    
    return Iu / Ip;
}

double wOut(Graph g, Vertex pJ, Vertex pI) {

    // outCount of Vertex pI (numerator)
    double Ou = numOutgoingLinks(g, pI);
    // R(v) = outgoing edges of v: array of the index's relating to v's outgoing edges
    int *Rv = outGoingLinkArray(g, pJ);
    // Op = number of outgoing links in the above array
    int numOutgoing = numOutgoingLinks(g, pJ);

    double Op = 0;

    // for every outgoing link of v, calculate its individual number of outgoing links and add to sum
    for (int i = 0; i < numOutgoing; i++) 
        if (numOutgoingLinks(g, Rv[i]) == 0) {
            Op += 0.5;
        } else {
            Op += numOutgoingLinks(g, Rv[i]);
        }
    
    if (Ou == 0) { Ou = 0.5; }
    return Ou / Op; 
}

float hugeSumFormula(Graph g, Vertex pI, float *pageRank_t, float d) {
    
    // Create array of incoming edges to Vertex i e.g.: M(pI)
    int *Rv = incomingLinkArray(g, pI);
    int numIncoming = numIncomingLinks(g, pI);

    float num = 0.0f;
    Vertex pJ;

    // for each node pJ in the inLit of pI e.g. pJ in set of M(pI)

    for (int i = 0; i < numIncoming; i++) {
        // pJ needs to obtain the index in the graph we want to look at. Thus, we just need the number itself, 
        // which in the inComingLink Array, the number simply refers to this index

        pJ = Rv[i]; // iterate through the list of incoming edges

        num += pageRank_t[pJ] * wIn(g, pJ, pI) * wOut(g, pJ, pI);
    }
    num *= d;
    return num;
}

float absValueCalculation(Graph g, float *pageRank_t, float *pageRank_t1) {
    
    float diff = 0.0f;

    // add the sum of the differences between each elements newPageRank and oldPageRank
    for (int i = 0; i < g->nV; i++) {
        
        diff += fabs(pageRank_t1[i] - pageRank_t[i]);
    }
    return diff;
}

// this program creates an array of each page with #outgoing links, and respective pagerank
// each index of the array refers to ther respective index/position in charArray and graph
float *calculatePageRank(Graph g, float d, float diffPR, int maxIterations, char **urlArray) {

    int N = g->nV; // number of urls in the collection
    
    // create a current iteration (pi; t), size num vertices - beginning at 1/N - this array will store the pageRanks
    float *pageRank_t = malloc(N * sizeof(float));

    for (int j = 0; j < N; j++) {
        pageRank_t[j] = 1.0/N;
    }
    // create a next iteration pageRank (t+1), will be dependant on the calculations below
    float *pageRank_t1 = malloc(N * sizeof(float));

    int iteration = 0;
    float diff = diffPR;
    
    while (iteration < maxIterations && diff >= diffPR) {
        // loop through the collection: i refers to the index we are at in the graph
        for (int i = 0; i < N; i++) {            
            pageRank_t1[i] = ((1.0 - d) / N) + hugeSumFormula(g, i, pageRank_t, d);
        }
        diff = absValueCalculation(g, pageRank_t, pageRank_t1);

        // copy all values of next iteration into current iteration, for next loop
        copyValues(pageRank_t, pageRank_t1, N);
        iteration++;
    }
    return pageRank_t1;
}

// this function sorts the pageRank array into descending order and makes sure to print the corresponding url name and degree.
void *printInOrder(float *pageRank_t1, int N, char **urlArray, Graph g) {

    float largest = 0;
    int index = 0;
    int degrees = 0;
    char *urlName;
    // Open up required file  to write to - make sure to change

    FILE *fPointer = fopen("pagerankList.txt", "w");

    for (int i = 0; i < N; i++) {

        largest = 0;
        for (int j = 0; j < N; j++) {
            if (pageRank_t1[j] > largest) {
                largest = pageRank_t1[j];
                index = j;
            }
        }
        pageRank_t1[index] = -1; // set to -1 to not find again
        urlName = urlArray[index];
        degrees = findNumDegrees(urlName, g, urlArray);
        fprintf(fPointer, "%s, %d, %.7lf\n", urlName, degrees, largest);
    }
    fclose(fPointer);
    return 0;
} 
