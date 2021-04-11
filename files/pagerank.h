#ifndef PAGE_H
#define PAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "readData.h"



int findNumDegrees(char *urlName, Graph g, char **urlArray);

void copyValues(float *pageRank_t, float *pageRank_t1, int N);

int numIncomingLinks(Graph g, Vertex index);

int numOutgoingLinks(Graph g, Vertex index);

int *outGoingLinkArray(Graph g, Vertex index);

int *incomingLinkArray(Graph g, Vertex index);

double wIn(Graph g, Vertex pJ, Vertex pI);

double wOut(Graph g, Vertex pJ, Vertex pI);

float hugeSumFormula(Graph g, Vertex pI, float *pageRank_t, float d);

float absValueCalculation(Graph g, float *pageRank_t, float *pageRank_t1);

float *calculatePageRank(Graph g, float d, float diffPR, int maxIterations, char **urlArray);

void *printInOrder(float *pageRank_t1, int N, char **urlArray, Graph g);

#endif