#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

struct edge {
    char* origin;
    char* destination;
    float length;
    int used;
    int flag;
    int originIndex;
    int destinationIndex;
};

struct node {
    char* name;
    int numEdges;
    struct edge* edges;
    int subtree;
    int used;
};

struct graph {
    int numNodes;
    struct node* nodes;
    int numSubTrees;
};

int findNodeIndexByName(struct node* nodes, int numNodes, char* search){
    int i; 
    for ( i = 0; i < numNodes; ++i){
        if (strcmp(nodes[i].name, search) == 0){
            return i;
        }
    }
    return -1;
} 

void dumpDotGraph(struct node* nodes, int numNodes, int withSubTree){
    puts("digraph G {");
    int maximumEdgeCount = 0;
    for (int i = 0; i < numNodes; ++i){
        int usedEdgeCount = 0;
        for (int j = 0; j < nodes[i].numEdges; ++j){
            if (nodes[i].edges[j].used == 1){
                usedEdgeCount++;
            }
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
        if (usedEdgeCount > maximumEdgeCount){
            maximumEdgeCount = usedEdgeCount;
        }
    }
    for (int i = 0; i < numNodes; ++i){
        int usedEdgeCount = 0;
        for (int j = 0; j < nodes[i].numEdges; ++j){
            if (nodes[i].edges[j].used == 1){
                usedEdgeCount++;
            }
        }
        if (usedEdgeCount == 1){
            printf("\"%s\" [style=filled, fillcolor = green]\n",nodes[i].name);
        }
        if (usedEdgeCount == maximumEdgeCount){
            printf("\"%s\" [style=filled, fillcolor = pink]\n",nodes[i].name);
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

    int nodeStorage = 2;
    int numNodes = 0;
    struct node* nodes = malloc(sizeof(struct node) * nodeStorage);    
  
    int i;
    for ( i = 0; i < numberOfEdges; ++i){
        int ni = findNodeIndexByName(nodes, numNodes, edges[i].origin);
        if (ni == -1) {
            if (numNodes == nodeStorage) {
                nodeStorage *= 2;
                nodes = realloc(nodes, sizeof(struct node) * nodeStorage);
            }
            nodes[numNodes].name = edges[i].origin;
            nodes[numNodes].numEdges = 1;
            nodes[numNodes].edges = NULL;
            nodes[numNodes].used = 0;
            nodes[numNodes].subtree = 0;
            numNodes++;
        } else {
            nodes[ni].numEdges++;
        }

        ni = findNodeIndexByName(nodes, numNodes, edges[i].destination);
        if (ni == -1) {
            if (numNodes == nodeStorage) {
                nodeStorage *= 2;
                nodes = realloc(nodes, sizeof(struct node) * nodeStorage);
            }
            nodes[numNodes].name = edges[i].destination;
            nodes[numNodes].numEdges = 1;
            nodes[numNodes].edges = NULL;
            nodes[numNodes].used = 0;
            nodes[numNodes].subtree = 0;
            numNodes++;
        } else {
            nodes[ni].numEdges++;
        }

    }

    for ( i = 0; i < numNodes; ++i){
        nodes[i].edges = malloc(sizeof(struct edge) * nodes[i].numEdges);
    }

    int* filledEdges = malloc(sizeof(struct node) * numNodes);
    memset(filledEdges, 0, sizeof(int) * numNodes);

    for ( i = 0; i < numberOfEdges; ++i){
        int originIndex = findNodeIndexByName(nodes, numNodes, edges[i].origin);
        int destinationIndex = findNodeIndexByName(nodes, numNodes, edges[i].destination);

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

    struct graph g = {.numNodes=numNodes, .nodes=nodes, .numSubTrees=0};
    return g;
}

void setEdgeAndPartnerUsed(struct node* nodes, int node, int edge, int used){

    nodes[node].edges[edge].used = used;

    if (node == nodes[node].edges[edge].originIndex) {
        int otherNode = nodes[node].edges[edge].destinationIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].originIndex == node){
                nodes[otherNode].edges[j].used = used;
            }
        }
    } else {
        int otherNode = nodes[node].edges[edge].originIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].destinationIndex == node){
                nodes[otherNode].edges[j].used = used;
            }
        }
    }

}

void flagEdges(struct node* nodes, int node, int edge, int flag){

    nodes[node].edges[edge].flag = flag;

    if (node == nodes[node].edges[edge].originIndex) {
        int otherNode = nodes[node].edges[edge].destinationIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].originIndex == node){
                nodes[otherNode].edges[j].flag = flag;
            }
        }
    } else {
        int otherNode = nodes[node].edges[edge].originIndex;
        for (int j = 0; j < nodes[otherNode].numEdges; ++j){
            if (nodes[otherNode].edges[j].destinationIndex == node){
                nodes[otherNode].edges[j].flag = flag;
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
        setEdgeAndPartnerUsed(nodes,i,minimumIndex,1);
        printf("Add node length %0.1f\n", minimumLength);
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
                            if (g->nodes[g->nodes[i].edges[j].destinationIndex].subtree == 0){
                                g->nodes[g->nodes[i].edges[j].destinationIndex].subtree = currentSubtree;
                                newnode = 1;
                            }
                            if (g->nodes[g->nodes[i].edges[j].originIndex].subtree == 0){ 
                                g->nodes[g->nodes[i].edges[j].originIndex].subtree = currentSubtree;
                                newnode = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    g->numSubTrees = currentSubtree-1;
}

int edgeConnectsDifferentSubtrees(struct node* nodes, struct edge e){
    if (nodes[e.originIndex].subtree != nodes[e.destinationIndex].subtree){
        return 1;
    }
    return 0;
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
                        if (edgeConnectsDifferentSubtrees(g.nodes, g.nodes[i].edges[j])){
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
        }
        g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].used = 2;
    }

    for (int i = 0; i < g.numNodes; ++i){
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            if (g.nodes[i].edges[j].used == 2){
                setEdgeAndPartnerUsed(g.nodes,i,j,1);
                printf("Add node length %0.1f\n", g.nodes[i].edges[j].length);
            }
        }
    }
}

int edgeConnectsUnusedNode(struct node* nodes, struct edge e){
    if (nodes[e.originIndex].used == 0 || nodes[e.destinationIndex].used == 0){
        return 1;
    }
    return 0;
}

void doBoruvkaAlgorithm(struct graph g){

    dumpDotGraph(g.nodes, g.numNodes, 0);

    joinShortestEdgePerNode(g.nodes, g.numNodes);

    while (1) {
        assignSubTrees(&g);

        printf("Number of subtrees = %d\n", g.numSubTrees);

        if (g.numSubTrees == 1) {
            dumpDotGraph(g.nodes, g.numNodes, 0);
            return;
        }

        joinShortestEdgePerSubTrees(g);
    }
}

void doPrimAlgorithm(struct graph g){

    dumpDotGraph(g.nodes, g.numNodes, 0);

    g.nodes[0].used = 1;

    int keepgoing = 1;

    while (keepgoing == 1) {

        int gotFirst = 0;
        float minimumLength;
        int minimumNodeIndex;
        int minimumEdgeIndex;
        for (int i = 0; i < g.numNodes; ++i){
            if (g.nodes[i].used == 1){
                for (int j = 0; j < g.nodes[i].numEdges; ++j){
                    if (g.nodes[i].edges[j].used == 0 && edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])) {
                        if (gotFirst == 0) {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                            gotFirst = 1;
                        }
                        if (g.nodes[i].edges[j].length < minimumLength) {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                        }
                    }
                }
            }
        }

        if (gotFirst == 1) {
            printf("Node \"%s\", Edge length %f\n", g.nodes[minimumNodeIndex].name, minimumLength);
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1;
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1;
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex,1);

        } else {
            keepgoing = 0;
        }
    }

    dumpDotGraph(g.nodes, g.numNodes, 0);

}

int isCyclic(struct graph g){
    int keepgoing = 1;
    int nodesLeft = 0;
    while (keepgoing == 1) {
		int removed = 0;
		nodesLeft = 0;
        for (int i = 0; i < g.numNodes; ++i){
			if (g.nodes[i].used == 1){
				nodesLeft++;
		        int numberEdgesInUse = 0;
                for (int j = 0; j < g.nodes[i].numEdges; ++j){
				    if (g.nodes[i].edges[j].used == 1) {
						numberEdgesInUse++;
					}
				}
				if (numberEdgesInUse == 1) {
					removed = 1;
					g.nodes[i].used = 2;
					for (int j = 0; j < g.nodes[i].numEdges; ++j){
						if (g.nodes[i].edges[j].used == 1) {
							setEdgeAndPartnerUsed(g.nodes, i, j, 2);
						}
					}
				}
				if (numberEdgesInUse == 0) {
					removed = 1;
					g.nodes[i].used = 2;
				}
			}
		}
		if (removed == 0) {
		    keepgoing = 0;
	    }
    }

	for (int i = 0; i < g.numNodes; ++i){
		if (g.nodes[i].used == 2){
			g.nodes[i].used = 1;
		}
		for (int j = 0; j < g.nodes[i].numEdges; ++j){
			if (g.nodes[i].edges[j].used == 2) {
				g.nodes[i].edges[j].used = 1;
			}
		}
	}

    if (nodesLeft > 0) {
		return 1;
	}
	return 0;
}

void doKruskalAlgorithm(struct graph g){

    dumpDotGraph(g.nodes, g.numNodes, 0);

    int keepgoing = 1;

    while (keepgoing == 1) {
        int gotFirst = 0;
        float minimumLength;
        int minimumNodeIndex;
        int minimumEdgeIndex;
        for (int i = 0; i < g.numNodes; ++i){
            for (int j = 0; j < g.nodes[i].numEdges; ++j){
				if (g.nodes[i].edges[j].used == 0) {
					int allowed = 0;
					if (edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])) {
						allowed = 1;
					} else {
						setEdgeAndPartnerUsed(g.nodes, i, j, 1);
						if (isCyclic(g) == 0){
							allowed = 1;
						}
						setEdgeAndPartnerUsed(g.nodes, i, j, 0);
					}
					if (allowed == 1) {
                        if (gotFirst == 0) {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                            gotFirst = 1;
                        }
                        if (g.nodes[i].edges[j].length < minimumLength) {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                        }
                    }
                }
			}
        }

        if (gotFirst == 1) {
            printf("Add edge length %f\n", minimumLength);
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1;
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1;
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex, 1);
        } else {
            keepgoing = 0;
        }
    }

    dumpDotGraph(g.nodes, g.numNodes, 0);
}

void doReverseDeleteAlgorithm(struct graph g){

    dumpDotGraph(g.nodes, g.numNodes, 0);

    int keepgoing = 1;

    while (keepgoing == 1) {
        int gotFirst = 0;
        float maximumLength;
        int maximumNodeIndex;
        int maximumEdgeIndex;
        for (int i = 0; i < g.numNodes; ++i){
            for (int j = 0; j < g.nodes[i].numEdges; ++j){
                if (g.nodes[i].edges[j].used == 1 && g.nodes[i].edges[j].flag == 0) {
                    if (gotFirst == 0) {
                        maximumLength = g.nodes[i].edges[j].length;
                        maximumNodeIndex = i;
                        maximumEdgeIndex = j;
                        gotFirst = 1;
                    }
                    if (g.nodes[i].edges[j].length > maximumLength) {
                        maximumLength = g.nodes[i].edges[j].length;
                        maximumNodeIndex = i;
                        maximumEdgeIndex = j;
                    }
                }
            }
        }
        setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, 0);
        assignSubTrees(&g);
        if (g.numSubTrees == 1) {
            printf("Removed edge length %f\n", maximumLength);
        } else {
            setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1);
            flagEdges(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1);
        }
        if (isCyclic(g) == 0){
            keepgoing = 0;
        }
    }

    for (int i = 0; i < g.numNodes; ++i){
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            if (g.nodes[i].edges[j].used == 2) {
                g.nodes[i].edges[j].used = 1;
			}
        }
    }
    dumpDotGraph(g.nodes, g.numNodes, 0);
}

void connectGraph(struct graph g){
    for (int i = 0; i < g.numNodes; ++i){
        g.nodes[i].used = 1;
        g.nodes[i].subtree = 1;
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            g.nodes[i].edges[j].used = 1;
            g.nodes[i].edges[j].flag = 0;
        }
    }
}

void disconnectGraph(struct graph g){
    for (int i = 0; i < g.numNodes; ++i){
        g.nodes[i].used = 0;
        g.nodes[i].subtree = 0;
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            g.nodes[i].edges[j].used = 0;
            g.nodes[i].edges[j].flag = 0;
        }
    }
}

float CalculateTotalEdgeLength(struct graph g){
    float total = 0.0;
    for (int i = 0; i < g.numNodes; ++i){
        for (int j = 0; j < g.nodes[i].numEdges; ++j){
            if (g.nodes[i].edges[j].used == 1) {
                total += g.nodes[i].edges[j].length;
            }
        }
    }
    printf("Total length = %0.1f\n", total/2.0);
    return total/2.0;
}

int main() {

    struct edge allEdges[] = {
        {"Bath","Bristol",12.9,0,0,0,0},
        {"Derby","Nottingham",14.9,0,0,0,0},
        {"Liverpool","Warrington",18.4,0,0,0,0},
        {"Chester","Liverpool",18.8,0,0,0,0},
        {"Portsmouth","Southampton",19.5,0,0,0,0},
        {"Manchester","Warrington",21.0,0,0,0,0},
        {"Chester","Warrington",22.6,0,0,0,0},
        {"Salisbury","Southampton",23.6,0,0,0,0},
        {"Leicester","Nottingham",27.9,0,0,0,0},
        {"Leeds","York",28.2,0,0,0,0},
        {"Preston","Warrington",30.5,0,0,0,0},
        {"Oxford","Swindon",30.6,0,0,0,0},
        {"Derby","Leicester",32.2,0,0,0,0},
        {"Leeds","Sheffield",35.5,0,0,0,0},
        {"Derby","Stoke",36.2,0,0,0,0},
        {"Hull","York",37.6,0,0,0,0},
        {"Chester","Stoke",37.7,0,0,0,0},
        {"Nottingham","Sheffield",37.8,0,0,0,0},
        {"Stoke","Warrington",39.1,0,0,0,0},
        {"Bath","Salisbury",39.4,0,0,0,0},
        {"Bristol","Gloucester",39.6,0,0,0,0},
        {"Bristol","Swindon",40.2,0,0,0,0},
        {"Manchester","Sheffield",40.9,0,0,0,0},
        {"Derby","Sheffield",41.5,0,0,0,0},
        {"Cardiff","Swansea",41.6,0,0,0,0},
        {"Birmingham","Derby",41.8,0,0,0,0},
        {"Leicester","Peterborough",42.1,0,0,0,0},
        {"Birmingham","Leicester",42.7,0,0,0,0},
        {"Cambridge","Peterborough",43.1,0,0,0,0},
        {"Bristol","Cardiff",44.1,0,0,0,0},
        {"Leeds","Manchester",44.7,0,0,0,0},
        {"Exeter","Plymouth",44.8,0,0,0,0},
        {"Birmingham","Stoke",44.9,0,0,0,0},
        {"Ipswich","Norwich",45.0,0,0,0,0},
        {"Edinburgh","Glasgow",47.5,0,0,0,0},
        {"Brighton","Portsmouth",50.0,0,0,0,0},
        {"Brighton","London",53.3,0,0,0,0},
        {"London","Oxford",55.9,0,0,0,0},
        {"Carlisle","Newcastle",59.1,0,0,0,0},
        {"Cambridge","Norwich",63.9,0,0,0,0},
        {"Cambridge","London",64.0,0,0,0,0},
        {"Hull","Leeds",64.4,0,0,0,0},
        {"London","Portsmouth",74.7,0,0,0,0},
        {"Birmingham","Oxford",78.5,0,0,0,0},
        {"London","Southampton",79.2,0,0,0,0},
        {"Bristol","Exeter",80.7,0,0,0,0},
        {"Ipswich","London",81.9,0,0,0,0},
        {"London","Peterborough",85.4,0,0,0,0},
        {"Carlisle","Preston",88.3,0,0,0,0},
        {"Exeter","Salisbury",91.6,0,0,0,0},
        {"Carlisle","Glasgow",96.8,0,0,0,0},
        {"Edinburgh","Newcastle",104.0,0,0,0,0},
        {"Canterbury","Dover",17.1,0,0,0,0},
        {"Canterbury","London",61.9,0,0,0,0},
        {"Hull","Lincoln",48.2,0,0,0,0},
        {"Lincoln","Nottingham",39.5,0,0,0,0},
        {"Lincoln","Sheffield",46.8,0,0,0,0},
        {"Lincoln","Peterborough",51.9,0,0,0,0},
        {"Bangor","Chester",60.9,0,0,0,0},
        {"Blackpool","Preston",16.0,0,0,0,0},
        {"Edinburgh","Perth",43.4,0,0,0,0},
        {"Dundee","Perth",22.4,0,0,0,0},
        {"Inverness","Perth",112.0,0,0,0,0},
        {"Aberdeen","Dundee",66.0,0,0,0,0},
        {"Aberdeen","Inverness",104.0,0,0,0,0},
        {"Chester","Wrexham",13.4,0,0,0,0},
        {"Birmingham","Shrewsbury",47.2,0,0,0,0},
        {"Shrewsbury","Wrexham",31.7,0,0,0,0},
        {"Birmingham","Worcester",40.6,0,0,0,0},
        {"Gloucester","Worcester",30.2,0,0,0,0},
        {"Bournemouth","Exeter",84.6,0,0,0,0},
        {"Bournemouth","Southampton",33.2,0,0,0,0},
        {"Darlington","Leeds",61.3,0,0,0,0},
        {"Darlington","Newcastle",37.3,0,0,0,0},
        {"Darlington","Middlesbrough",16.4,0,0,0,0},
        {"Burnley","Leeds",35.7,0,0,0,0},
        {"Burnley","Manchester",29.0,0,0,0,0},
        {"Burnley","Preston",21.5,0,0,0,0},
        {"Cambridge","Kings Lynn",45.1,0,0,0,0},
        {"Norwich","Kings Lynn",43.6,0,0,0,0},
        {"Peterborough","Kings Lynn",36.4,0,0,0,0},
        {"Scarborough","York",41.2,0,0,0,0},
        {"Middlesbrough","Scarborough",48.9,0,0,0,0},
        {"Hull","Scarborough",42.5,0,0,0,0},
        {"Aberystwyth","Shrewsbury",76.0,0,0,0,0},
        {"Aberystwyth","Cardigan",38.3,0,0,0,0},
        {"Cardigan","Carmarthen",26.1,0,0,0,0},
        {"Carmarthen","Swansea",28.0,0,0,0,0},
        {"Carlisle","Kendal",46.1,0,0,0,0},
        {"Kendal","Preston",42.7,0,0,0,0},
        {"Darlington","Kendal",63.3,0,0,0,0},
        {"Barrow","Kendal",34.7,0,0,0,0},
        {"Cambridge","Luton",41.0,0,0,0,0},
        {"Leicester","Luton",69.5,0,0,0,0},
        {"London","Luton",34.3,0,0,0,0},
        {"Luton","Oxford",43.5,0,0,0,0},
        {"London","Newbury",60.8,0,0,0,0},
        {"Newbury","Oxford",27.2,0,0,0,0},
        {"Newbury","Southampton",38.1,0,0,0,0},
        {"Swindon","Newbury",26.0,0,0,0,0},\
    };

    int numberOfEdges = sizeof(allEdges)/sizeof(struct edge);

    struct graph g = createNodes(allEdges, numberOfEdges);

    doBoruvkaAlgorithm(g);
    float Blength = CalculateTotalEdgeLength(g);

    disconnectGraph(g);
    doPrimAlgorithm(g);
    float Plength = CalculateTotalEdgeLength(g);

    disconnectGraph(g);
    doKruskalAlgorithm(g);
    float Klength = CalculateTotalEdgeLength(g);

    connectGraph(g);
    doReverseDeleteAlgorithm(g);
    float Rlength = CalculateTotalEdgeLength(g);

    if (fabs(Blength - Plength) < 1.0 &&
        fabs(Plength - Klength) < 1.0 &&
        fabs(Klength - Rlength) < 1.0) {
        puts("Passed");
    } else {
        puts("Failed");
    }
}

