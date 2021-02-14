package main

import "fmt"
import "math"

type edge struct{
    origin string
    destination string
    length float32
    used int
    flag int
    originIndex int
    destinationIndex int
}

type node struct{
    name string
    numEdges int
    edges []edge
    subtree int
    used int
}

type graph struct{
    anumNodes int
    nodes []node
    numSubTrees int
}

func findNodeIndexByName(nodes []node, search string) int {
    for i := 0; i < len(nodes); i++ {
        if nodes[i].name == search {
            return i
        }
    }
    return -1
}

func dumpDotGraph(nodes []node, withSubTree int){
    fmt.Println("digraph G {");
    maximumEdgeCount := 0;
    for i := 0; i < len(nodes); i++{
        usedEdgeCount := 0;
        for j := 0; j < len(nodes[i].edges); j++{
            if (nodes[i].edges[j].used == 1){
                usedEdgeCount++;
            }
            if (nodes[i].name == nodes[i].edges[j].origin){
                var linestyle string;
                if (nodes[i].edges[j].used == 0){
                    linestyle="dashed";
                } else {
                    linestyle="bold";
                }
                if (withSubTree == 1){
                    fmt.Printf("\"%s.%d\" -> \"%s.%d\" [dir=none, label=\"%0.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[nodes[i].edges[j].originIndex].subtree,
                                                                                            nodes[i].edges[j].destination,nodes[nodes[i].edges[j].destinationIndex].subtree,
                                                                                            nodes[i].edges[j].length,linestyle);
                } else {
                    fmt.Printf("\"%s\" -> \"%s\" [dir=none, label=\"%0.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[i].edges[j].destination,nodes[i].edges[j].length,linestyle);
                }
            }
        }
        if (usedEdgeCount > maximumEdgeCount){
            maximumEdgeCount = usedEdgeCount;
        }
    }
    for i := 0; i < len(nodes); i++ {
        usedEdgeCount := 0;
        for j := 0; j < len(nodes[i].edges); j++{
            if (nodes[i].edges[j].used == 1){
                usedEdgeCount++;
            }
        }
        if (usedEdgeCount == 1){
            fmt.Printf("\"%s\" [style=filled, fillcolor = green]\n",nodes[i].name);
        }
        if (usedEdgeCount == maximumEdgeCount){
            fmt.Printf("\"%s\" [style=filled, fillcolor = pink]\n",nodes[i].name);
        }
    }
    fmt.Println("}");
}
/*
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
*/
func createNodes(edges []edge, numberOfEdges int) graph {

    var numNodes int = 0;
    var nodes []node

    for i := 0; i < numberOfEdges; i++ {
        ni := findNodeIndexByName(nodes, edges[i].origin);
        if (ni == -1) {
		    var newnode node;
            newnode.name = edges[i].origin;
            newnode.numEdges = 1;
            //newnode.edges = NULL;
            newnode.used = 0;
            newnode.subtree = 0;
            numNodes++;
			nodes = append(nodes, newnode)
        } else {
            nodes[ni].numEdges++;
        }

        ni = findNodeIndexByName(nodes, edges[i].destination);
        if (ni == -1) {
		    var newnode node
            newnode.name = edges[i].destination;
            newnode.numEdges = 1;
            //nodes[numNodes].edges = NULL;
            newnode.used = 0;
            newnode.subtree = 0;
            numNodes++;
			nodes = append(nodes, newnode)
        } else {
            nodes[ni].numEdges++;
        }

    }

    for i := 0; i < numNodes; i++ {
        nodes[i].edges = make([]edge,nodes[i].numEdges);
    }
    filledEdges := make([]int,numNodes);

    for i := 0; i < numberOfEdges; i++ {
        originIndex := findNodeIndexByName(nodes, edges[i].origin);
        destinationIndex := findNodeIndexByName(nodes, edges[i].destination);

        nodes[originIndex].edges[filledEdges[originIndex]] = edges[i];
        nodes[originIndex].edges[filledEdges[originIndex]].originIndex = originIndex;
        nodes[originIndex].edges[filledEdges[originIndex]].destinationIndex = destinationIndex;
        filledEdges[originIndex]++;

        nodes[destinationIndex].edges[filledEdges[destinationIndex]] = edges[i];
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].originIndex = originIndex;
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].destinationIndex = destinationIndex;
        filledEdges[destinationIndex]++;
    }

    var g = graph{numNodes, nodes, 0};
    return g;
}

func setEdgeAndPartnerUsed(nodes []node, node int, edge int, used int){

    nodes[node].edges[edge].used = used;

    if (node == nodes[node].edges[edge].originIndex) {
        otherNode := nodes[node].edges[edge].destinationIndex;
        for j := 0; j < len(nodes[otherNode].edges); j++{
            if (nodes[otherNode].edges[j].originIndex == node){
                nodes[otherNode].edges[j].used = used;
            }
        }
    } else {
        otherNode := nodes[node].edges[edge].originIndex;
        for j := 0; j < len(nodes[otherNode].edges); j++{
            if (nodes[otherNode].edges[j].destinationIndex == node){
                nodes[otherNode].edges[j].used = used;
            }
        }
    }

}

func flagEdges(nodes []node, node int, edge int, flag int) {

    nodes[node].edges[edge].flag = flag;

    if (node == nodes[node].edges[edge].originIndex) {
        var otherNode int = nodes[node].edges[edge].destinationIndex;
        for j := 0; j < len(nodes[otherNode].edges); j++ {
            if (nodes[otherNode].edges[j].originIndex == node){
                nodes[otherNode].edges[j].flag = flag;
            }
        }
    } else {
        var otherNode int = nodes[node].edges[edge].originIndex;
        for j := 0; j < len(nodes[otherNode].edges); j++ {
            if (nodes[otherNode].edges[j].destinationIndex == node){
                nodes[otherNode].edges[j].flag = flag;
            }
        }
    }
}

func joinShortestEdgePerNode(nodes []node){

    for i := 0; i < len(nodes); i++{
        minimumLength := nodes[i].edges[0].length;
        minimumIndex := 0;
        for j := 0; j < len(nodes[i].edges); j++{
            if nodes[i].edges[j].length < minimumLength {
                minimumLength = nodes[i].edges[j].length;
                minimumIndex = j;
            }
        }
        setEdgeAndPartnerUsed(nodes,i,minimumIndex,1);
        fmt.Printf("Add node length %0.1f\n", minimumLength);
    }
}

func assignSubTrees(g *graph){

    for i := 0; i < len(g.nodes); i++ {
        g.nodes[i].subtree = 0;
    }

    currentSubtree := 0;
    keepgoing := true;
    for keepgoing {
        currentSubtree++;
        for i := 0; i < len(g.nodes); i++{
            if g.nodes[i].subtree == 0 {
                g.nodes[i].subtree = currentSubtree;
                break;
            }
            if i == len(g.nodes)-1 {
                keepgoing = false;
            }
        }
        newnode := true;
        for newnode {
            newnode = false;
            for i := 0; i < len(g.nodes); i++{
                if (g.nodes[i].subtree == currentSubtree){
                    for j := 0; j < len(g.nodes[i].edges); j++{
                        if (g.nodes[i].edges[j].used == 1) {
                            if (g.nodes[g.nodes[i].edges[j].destinationIndex].subtree == 0){
                                g.nodes[g.nodes[i].edges[j].destinationIndex].subtree = currentSubtree;
                                newnode = true;
                            }
                            if (g.nodes[g.nodes[i].edges[j].originIndex].subtree == 0){
                                g.nodes[g.nodes[i].edges[j].originIndex].subtree = currentSubtree;
                                newnode = true;
                            }
                        }
                    }
                }
            }
        }
    }

    g.numSubTrees = currentSubtree-1;
}

func edgeConnectsDifferentSubtrees(nodes []node, e edge) bool {
    if (nodes[e.originIndex].subtree != nodes[e.destinationIndex].subtree){
        return true;
    }
    return false;
}

func joinShortestEdgePerSubTrees(g graph){
    for tree := 1; tree <= g.numSubTrees; tree++ {
        var minimumLength float32;
        var minimumEdgeIndex int;
        var minimumNodeIndex int;
        gotFirst := false;
        for i := 0; i < len(g.nodes); i++{
            if (g.nodes[i].subtree == tree){
                for j := 0; j < len(g.nodes[i].edges); j++ {
                    if (g.nodes[i].edges[j].used == 0) {
                        if (edgeConnectsDifferentSubtrees(g.nodes, g.nodes[i].edges[j])){
                            if (!gotFirst) {
                                minimumLength = g.nodes[i].edges[j].length;
                                minimumEdgeIndex = j;
                                minimumNodeIndex = i;
                                gotFirst = true;
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

    for i := 0; i < len(g.nodes); i++ {
        for j := 0; j < len(g.nodes[i].edges); j++ {
            if (g.nodes[i].edges[j].used == 2){
                setEdgeAndPartnerUsed(g.nodes,i,j,1);
                fmt.Printf("Add node length %0.1f\n", g.nodes[i].edges[j].length);
            }
        }
    }
}

func edgeConnectsUnusedNode(nodes []node, e edge) bool {
    if (nodes[e.originIndex].used == 0 || nodes[e.destinationIndex].used == 0){
        return true;
    }
    return false;
}

func doBoruvkaAlgorithm(g graph){

    joinShortestEdgePerNode(g.nodes);

    for true {
        assignSubTrees(&g);

        fmt.Printf("Number of subtrees = %d\n", g.numSubTrees);

        if (g.numSubTrees == 1) {
            return;
        }

        joinShortestEdgePerSubTrees(g);
    }
}

func doPrimAlgorithm(g graph){

    g.nodes[0].used = 1

    keepgoing := true

    for keepgoing {

        gotFirst := false;
        var minimumLength float32
        var minimumNodeIndex int
        var minimumEdgeIndex int
        for i := 0; i < len(g.nodes); i++ {
            if (g.nodes[i].used == 1){
                for j := 0; j < len(g.nodes[i].edges); j++ {
                    if (g.nodes[i].edges[j].used == 0 && edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])) {
                        if (!gotFirst) {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                            gotFirst = true;
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

        if gotFirst {
            fmt.Printf("Node \"%s\", Edge length %0.1f\n", g.nodes[minimumNodeIndex].name, minimumLength);
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1;
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1;
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex,1);

        } else {
            keepgoing = false
        }
    }

}

func isCyclic(g graph) bool {
    var keepgoing bool = true;
    var nodesLeft bool = false;
    for keepgoing {
		var removed bool = false;
		nodesLeft = false;
        for i := 0; i < len(g.nodes); i++ {
			if (g.nodes[i].used == 1){
				nodesLeft = true;
		        var numberEdgesInUse int = 0;
                for j := 0; j < len(g.nodes[i].edges); j++ {
				    if (g.nodes[i].edges[j].used == 1) {
						numberEdgesInUse++;
					}
				}
				if (numberEdgesInUse == 1) {
					removed = true;
					g.nodes[i].used = 2;
					for j := 0; j < len(g.nodes[i].edges); j++ {
						if (g.nodes[i].edges[j].used == 1) {
							setEdgeAndPartnerUsed(g.nodes, i, j, 2);
						}
					}
				}
				if (numberEdgesInUse == 0) {
					removed = true;
					g.nodes[i].used = 2;
				}
			}
		}
		if !removed {
		    keepgoing = false
	    }
    }

	for i := 0; i < len(g.nodes); i++ {
		if (g.nodes[i].used == 2){
			g.nodes[i].used = 1;
		}
		for j := 0; j < len(g.nodes[i].edges); j++ {
			if (g.nodes[i].edges[j].used == 2) {
				g.nodes[i].edges[j].used = 1;
			}
		}
	}

    if (nodesLeft) {
		return true
	}
	return false
}

func doKruskalAlgorithm(g graph){

    var keepgoing bool = true

    for keepgoing {
        var gotFirst bool = false;
        var minimumLength float32
        var minimumNodeIndex int
        var minimumEdgeIndex int
        for i := 0; i < len(g.nodes); i++ {
            for j := 0; j < g.nodes[i].numEdges; j++ {
				if (g.nodes[i].edges[j].used == 0) {
					var allowed bool = false;
					if (edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])) {
						allowed = true;
					} else {
						setEdgeAndPartnerUsed(g.nodes, i, j, 1);
						if !isCyclic(g) {
							allowed = true;
						}
						setEdgeAndPartnerUsed(g.nodes, i, j, 0);
					}
					if allowed {
                        if !gotFirst {
                            minimumLength = g.nodes[i].edges[j].length;
                            minimumNodeIndex = i;
                            minimumEdgeIndex = j;
                            gotFirst = true
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

        if gotFirst {
            fmt.Printf("Add edge length %0.1f\n", minimumLength);
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1;
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1;
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex, 1);
        } else {
            keepgoing = false;
        }
    }

}

func doReverseDeleteAlgorithm(g graph){

    var keepgoing bool = true;

    for keepgoing {
        var gotFirst bool = false;
        var maximumLength float32
        var maximumNodeIndex int
        var maximumEdgeIndex int
        for i := 0; i < len(g.nodes); i++ {
            for j := 0; j < len(g.nodes[i].edges); j++{
                if (g.nodes[i].edges[j].used == 1 && g.nodes[i].edges[j].flag == 0) {
                    if !gotFirst {
                        maximumLength = g.nodes[i].edges[j].length;
                        maximumNodeIndex = i;
                        maximumEdgeIndex = j;
                        gotFirst = true;
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
            fmt.Printf("Removed edge length %0.1f\n", maximumLength);
        } else {
            setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1);
            flagEdges(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1);
        }
        if !isCyclic(g) {
            keepgoing = false;
        }
    }

    for i := 0; i < len(g.nodes); i++ {
        for j := 0; j < len(g.nodes[i].edges); j++ {
            if (g.nodes[i].edges[j].used == 2) {
                g.nodes[i].edges[j].used = 1;
			}
        }
    }

}

func connectGraph(g graph){
    for i := 0; i < len(g.nodes); i++ {
        g.nodes[i].used = 1
        g.nodes[i].subtree = 1
        for j := 0; j < len(g.nodes[i].edges); j++ {
            g.nodes[i].edges[j].used = 1
            g.nodes[i].edges[j].flag = 0
        }
    }
}

func disconnectGraph(g graph){
    for i := 0; i < len(g.nodes); i++ {
        g.nodes[i].used = 0
        g.nodes[i].subtree = 0
        for j := 0; j < len(g.nodes[i].edges); j++ {
            g.nodes[i].edges[j].used = 0
            g.nodes[i].edges[j].flag = 0
        }
    }
}

func CalculateTotalEdgeLength(g graph) float32 {
    var total float32
    for i := 0; i < len(g.nodes); i++ {
        for j := 0; j < len(g.nodes[i].edges); j++ {
            if (g.nodes[i].edges[j].used == 1) {
                total += g.nodes[i].edges[j].length
            }
        }
    }
    fmt.Printf("Total length = %0.1f\n", total/2.0)
    return total/2.0
}

func main() {

    var allEdges = []edge{
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
        {"Swindon","Newbury",26.0,0,0,0,0},
    };

    numberOfEdges := len(allEdges)

    g := createNodes(allEdges, numberOfEdges);
    dumpDotGraph(g.nodes, 0)

    doBoruvkaAlgorithm(g);
    dumpDotGraph(g.nodes, 0)
    Blength := CalculateTotalEdgeLength(g);

    disconnectGraph(g);
    doPrimAlgorithm(g);
    dumpDotGraph(g.nodes, 0)
    Plength := CalculateTotalEdgeLength(g);

    disconnectGraph(g);
    doKruskalAlgorithm(g);
    dumpDotGraph(g.nodes, 0)
    Klength := CalculateTotalEdgeLength(g);

    connectGraph(g);
    doReverseDeleteAlgorithm(g);
    dumpDotGraph(g.nodes, 0)
    Rlength := CalculateTotalEdgeLength(g);

    if (math.Abs(float64(Blength - Plength)) < 1.0 &&
        math.Abs(float64(Plength - Klength)) < 1.0 &&
        math.Abs(float64(Klength - Rlength)) < 1.0	) {
        fmt.Println("Passed");
	} else {
        fmt.Println("Failed");
    }
}
