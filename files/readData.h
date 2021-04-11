#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
// Implementation of Graph structure
// Code used from provided code in lab08, Graph.h and Graph.c, written by John Shepherd

typedef struct GraphRep *Graph;
// vertices denoted by integers 0..N-1
typedef int Vertex;
int validV (Graph, Vertex); // validity check

// edges are pairs of vertices (end-points)
typedef struct Edge {
	Vertex v;
	Vertex w;
} Edge;

int validV (Graph g, Vertex v);
void insertEdge (Graph g, int v, int w);
Graph newGraph (int nV);
void dropGraph (Graph g);
void showGraph (Graph g, char **names);
char **GetCollection();
Vertex findNumFiles();
Vertex findUrlElement(char **urlArray, char *url);
Graph GetGraph(char **urlArray);

#endif
