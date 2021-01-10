#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct edge {
    char* origin;
    char* destination;
    float length;
    int used;
    int originIndex;
    int destinationIndex;
};

struct node {
    char* name;
    int numEdges;
    struct edge* edges;
    int subtree;
};

struct nodeInfo {
    int numNodes;
    struct node* nodes;
};

/*
struct node* findNodeByName(struct node* nodes, int numNodes, char* search){
    int i;
    for ( i = 0; i < numNodes; ++i){
        if (strcmp(nodes[i].name, search) == 0){
            return &nodes[i];
        }
    }
    return NULL;
} 
*/

int findNodeIndexByName(struct node* nodes, int numNodes, char* search){
    int i; 
    for ( i = 0; i < numNodes; ++i){
        if (strcmp(nodes[i].name, search) == 0){
            return i;
        }
    }
    return -1;
} 

/*
ptrdiff_t nodeIndex(struct node* nodes, struct node* node){
    return node-nodes;
}
*/

void dumpDotGraph(struct node* nodes, int numNodes, int withSubTree){
    puts("digraph G {");
    for (int i = 0; i < numNodes; ++i){
        //printf("%s %d\n",nodes[i].name, nodes[i].numEdges);
        for (int j = 0; j < nodes[i].numEdges; ++j){
            if (strcmp(nodes[i].name, nodes[i].edges[j].origin) == 0){
                char* linestyle;
                if (nodes[i].edges[j].used == 0){
                    linestyle="dashed";
                } else {
                    linestyle="bold";
                }
                if (withSubTree == 1){
                    printf("\"%s.%d\" -> \"%s.%d\" [dir=none, label=\"%0.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[nodes[i].edges[j].originIndex].subtree,
                                                                                            nodes[i].edges[j].destination,nodes[nodes[i].edges[j].destinationIndex].subtree,
                                                                                            nodes[i].edges[j].length,linestyle);
                } else {
                    printf("\"%s\" -> \"%s\" [dir=none, label=\"%0.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[i].edges[j].destination,nodes[i].edges[j].length,linestyle);
                }
            }
        }
    }
    puts("}");
}

void dumpGraph(struct node* nodes, int numNodes){
    for (int i = 0; i < numNodes; ++i){
        printf("%d, %s %d\n", i, nodes[i].name, nodes[i].numEdges);
        for (int j = 0; j < nodes[i].numEdges; ++j){
            printf(" %s[%d] -> %s[%d] length=%0.1f used=%d\n",nodes[i].edges[j].origin,nodes[i].edges[j].originIndex,nodes[i].edges[j].destination,nodes[i].edges[j].destinationIndex,nodes[i].edges[j].length,nodes[i].edges[j].used);
        }
    }
}

struct nodeInfo createNodes(struct edge* edges, int numberOfEdges){

    int nodeStorage = 10;
    int nodeNumber = 0;
    struct node* nodes = malloc(sizeof(struct node) * nodeStorage);    
  
    int i;
    for ( i = 0; i < numberOfEdges; ++i){
        int ni = findNodeIndexByName(nodes, nodeNumber, edges[i].origin);
        if (ni == -1) {
            nodes[nodeNumber].name = edges[i].origin;
            nodes[nodeNumber].numEdges = 1;
            nodes[nodeNumber].edges = NULL;
            nodeNumber++;
        } else {
            nodes[ni].numEdges++;
        }

        ni = findNodeIndexByName(nodes, nodeNumber, edges[i].destination);
        if (ni == -1) {
            nodes[nodeNumber].name = edges[i].destination;
            nodes[nodeNumber].numEdges = 1;
            nodes[nodeNumber].edges = NULL;
            nodeNumber++;
        } else {
            nodes[ni].numEdges++;
        }

    }

    for ( i = 0; i < nodeNumber; ++i){
        nodes[i].edges = malloc(sizeof(struct edge) * nodes[i].numEdges);
    }

    int* filledEdges = malloc(sizeof(struct node) * nodeNumber);
    memset(filledEdges, 0, sizeof(int) * nodeNumber);

    for ( i = 0; i < numberOfEdges; ++i){
        int originIndex = findNodeIndexByName(nodes, nodeNumber, edges[i].origin);
        int destinationIndex = findNodeIndexByName(nodes, nodeNumber, edges[i].destination);

        memcpy(&(nodes[originIndex].edges[filledEdges[originIndex]]), &edges[i], sizeof(struct edge));
        nodes[originIndex].edges[filledEdges[originIndex]].originIndex = originIndex;
        nodes[originIndex].edges[filledEdges[originIndex]].destinationIndex = destinationIndex;
        filledEdges[originIndex]++;

        memcpy(&(nodes[destinationIndex].edges[filledEdges[destinationIndex]]), &edges[i], sizeof(struct edge));
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].originIndex = originIndex;
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].destinationIndex = destinationIndex;
        filledEdges[destinationIndex]++;
    }
    free(filledEdges);

    dumpDotGraph(nodes, nodeNumber, 1);

    struct nodeInfo nInfo= {nodeNumber, nodes};
    return nInfo;
}

void joinShortestEdgePerNode(struct node* nodes, int numNodes){

    for (int i = 0; i < numNodes; ++i){
        float minimumLength = nodes[i].edges[0].length;
        int minimumIndex = 0;
        for (int j = 0; j < nodes[i].numEdges; ++j){
            if (nodes[i].edges[0].length < minimumLength) {
                minimumLength = nodes[i].edges[j].length;
                minimumIndex = j;
            }
        }
        nodes[i].edges[minimumIndex].used = 1;
        if (i == nodes[i].edges[minimumIndex].originIndex) {
            int otherNode = nodes[i].edges[minimumIndex].destinationIndex;
            for (int j = 0; j < nodes[otherNode].numEdges; ++j){
                if (nodes[otherNode].edges[j].length == minimumLength){
                    nodes[otherNode].edges[j].used = 1;
                }
            }
        } else {
            int otherNode = nodes[i].edges[minimumIndex].originIndex;
            for (int j = 0; j < nodes[otherNode].numEdges; ++j){
                if (nodes[otherNode].edges[j].length == minimumLength){
                    nodes[otherNode].edges[j].used = 1;
                }
            }
        }
    }

}

void assignSubTrees(struct node* nodes, int numNodes){
    int currentSubtree = 0;
    int finished = 0;
    while (finished == 0) {
    currentSubtree++;
    for (int i = 0; i < numNodes; ++i){
        if (nodes[i].subtree == 0){
            nodes[i].subtree = currentSubtree;
            break;
        }
        if ( i == numNodes-1) {
            finished = 1;
        }
    }
    int newnode = 1;
    while (newnode == 1){
        newnode = 0;
        for (int i = 0; i < numNodes; ++i){
            if (nodes[i].subtree == currentSubtree){
                for (int j = 0; j < nodes[i].numEdges; ++j){
                    if (nodes[i].edges[j].used == 1) {
                        if (nodes[nodes[i].edges[j].destinationIndex].subtree == 0 ||
                            nodes[nodes[i].edges[j].destinationIndex].subtree == 0) newnode = 1;
                        nodes[nodes[i].edges[j].destinationIndex].subtree = currentSubtree;
                        nodes[nodes[i].edges[j].originIndex].subtree = currentSubtree;
                    }
                }
            }
        }
    }
    }
}


void joinShortestEdgePerSubTrees(struct node* nodes, int numNodes){

}

void doAlgorithm1(struct nodeInfo ni){

    joinShortestEdgePerNode(ni.nodes, ni.numNodes);

    dumpDotGraph(ni.nodes, ni.numNodes, 1);

    assignSubTrees(ni.nodes, ni.numNodes);

    dumpDotGraph(ni.nodes, ni.numNodes, 1);

    joinShortestEdgePerSubTrees(ni.nodes, ni.numNodes);

    dumpDotGraph(ni.nodes, ni.numNodes, 1);

}


int main() {

    struct edge allEdges[] = {
        {"start","stop",24.0,0},
        {"begin","end",99.9,0},
        {"begin","shop",11,0},
        {"A","start",22.2},
        {"A","begin",55.5}
        };

    int numberOfEdges = sizeof(allEdges)/sizeof(struct edge);

    struct nodeInfo ni = createNodes(allEdges, numberOfEdges); 

    doAlgorithm1(ni);
}

