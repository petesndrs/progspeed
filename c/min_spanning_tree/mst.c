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

struct graph {
    int numNodes;
    struct node* nodes;
    int numSubTrees;
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

void dumpGraph(struct graph g){
    printf("Graph subtrees %d\n",g.numSubTrees);
    for (int i = 0; i < g.numNodes; ++i){
        printf(" Node: %d, %s edges %d subtree %d\n", i, g.nodes[i].name, g.nodes[i].numEdges, g.nodes[i].subtree);
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            printf("  Edge: %s[%d] -> %s[%d] length=%0.1f used=%d\n",g.nodes[i].edges[j].origin, g.nodes[i].edges[j].originIndex,
                                                                     g.nodes[i].edges[j].destination, g.nodes[i].edges[j].destinationIndex,
                                                                     g.nodes[i].edges[j].length, g.nodes[i].edges[j].used);
        }
    }
}

struct graph createNodes(struct edge* edges, int numberOfEdges){

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

    struct graph g = {.numNodes=nodeNumber, .nodes=nodes, .numSubTrees=0};
    return g;
}


void setEdgeAndPartnerUsed(struct node* nodes, int node, int edge){

    nodes[node].edges[edge].used = 1;

    if (node == nodes[node].edges[edge].originIndex) {
        int otherNode = nodes[node].edges[edge].destinationIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].originIndex == node){
                nodes[otherNode].edges[j].used = 1;
            }
        }
    } else {
        int otherNode = nodes[node].edges[edge].originIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].destinationIndex == node){
                nodes[otherNode].edges[j].used = 1;
            }
        }
    }

}



void joinShortestEdgePerNode(struct node* nodes, int numNodes){

    for (int i = 0; i < numNodes; ++i){
        float minimumLength = nodes[i].edges[0].length;
        int minimumIndex = 0;
        for (int j = 0; j < nodes[i].numEdges; ++j){
            if (nodes[i].edges[j].length < minimumLength) {
                minimumLength = nodes[i].edges[j].length;
                minimumIndex = j;
            }
        }
        setEdgeAndPartnerUsed(nodes,i,minimumIndex);
    }

}

void assignSubTrees(struct graph* g){

    for (int i = 0; i < g->numNodes; ++i){
        g->nodes[i].subtree = 0;
    }

    int currentSubtree = 0;
    int finished = 0;
    while (finished == 0) {
        currentSubtree++;
        for (int i = 0; i < g->numNodes; ++i){
            if (g->nodes[i].subtree == 0){
                g->nodes[i].subtree = currentSubtree;
                break;
            }
            if ( i == g->numNodes-1) {
                finished = 1;
            }
        }
        int newnode = 1;
        while (newnode == 1){
            newnode = 0;
            for (int i = 0; i < g->numNodes; ++i){
                if (g->nodes[i].subtree == currentSubtree){
                    for (int j = 0; j < g->nodes[i].numEdges; ++j){
                        if (g->nodes[i].edges[j].used == 1) {
                            if (g->nodes[g->nodes[i].edges[j].destinationIndex].subtree == 0 ||
                                g->nodes[g->nodes[i].edges[j].destinationIndex].subtree == 0) newnode = 1;
                            g->nodes[g->nodes[i].edges[j].destinationIndex].subtree = currentSubtree;
                            g->nodes[g->nodes[i].edges[j].originIndex].subtree = currentSubtree;
                        }
                    }
                }
            }
        }
    }

    g->numSubTrees = currentSubtree-1;
}


void joinShortestEdgePerSubTrees(struct graph g){
    for (int tree = 1; tree <= g.numSubTrees; ++tree){
        float minimumLength;
        int minimumEdgeIndex;
        int minimumNodeIndex;
        int gotFirst = 0;
        for (int i = 0; i < g.numNodes; ++i){
            if (g.nodes[i].subtree == tree){
                for (int j = 0; j < g.nodes[i].numEdges; ++j){
                    if (g.nodes[i].edges[j].used == 0) {
                       if (gotFirst == 0) {
                           minimumLength = g.nodes[i].edges[j].length;
                           minimumEdgeIndex = j;
                           minimumNodeIndex = i;
                           gotFirst = 1;
                       }
                       if (g.nodes[i].edges[j].length < minimumLength) {
                           minimumLength = g.nodes[i].edges[j].length;
                           minimumEdgeIndex = j;
                           minimumNodeIndex = i;
                       }
                    }
                }
            }
        }
        g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].used = 2;
    }

    for (int i = 0; i < g.numNodes; ++i){
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            if (g.nodes[i].edges[j].used == 2){
                setEdgeAndPartnerUsed(g.nodes,i,j);
            }
        }
    }
}

void doAlgorithm1(struct graph g){


    joinShortestEdgePerNode(g.nodes, g.numNodes);

    dumpDotGraph(g.nodes, g.numNodes, 1);

    while (1) {
        assignSubTrees(&g);

        dumpDotGraph(g.nodes, g.numNodes, 1);

        if (g.numSubTrees == 1) {
            return;
        }

        joinShortestEdgePerSubTrees(g);

    }
}


int main() {

    struct edge allEdges[] = {
        {"start","stop",24.0,0},
        {"begin","end",99.9,0},
        {"begin","shop",11,0},
        {"A","start",22.2},
        {"A","begin",55.5},
        {"stop","end",100.1},
        {"stop","begin",40.0},
        {"A","B",77.7},
        {"B","C",3.3}
        };

    int numberOfEdges = sizeof(allEdges)/sizeof(struct edge);

    struct graph g = createNodes(allEdges, numberOfEdges);

    doAlgorithm1(g);
}

