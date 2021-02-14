
import copy
import math

class edge:
    def __init__(self, origin, destination, length, used, flag, originIndex, destinationIndex):
        self.origin = origin
        self.destination = destination
        self.length = length
        self.used = used
        self.flag = flag
        self.originIndex = originIndex
        self.destinationIndex = destinationIndex

class node:
    def __init__(self, name, numEdges, edges, subtree, used):
        self.name = name
        self.numEdges = numEdges
        self.edges = edges
        self.subtree = subtree
        self.used = used

class graph:
    def __init__(self, numNodes, nodes, numSubTrees):
        self.numNodes = numNodes
        self.nodes = nodes
        self.numSubTrees = numSubTrees

def findNodeIndexByName(nodes, numNodes, search):
    for i in range(len(nodes)):
        if (nodes[i].name == search):
            return i

    return -1


def dumpDotGraph(nodes, numNodes, withSubTree):
    print("digraph G {")
    maximumEdgeCount = 0
    for i in range(len(nodes)):
        usedEdgeCount = 0
        for j in range(len(nodes[i].edges)):
            if (nodes[i].edges[j].used == 1):
                usedEdgeCount+=1

            if (nodes[i].name == nodes[i].edges[j].origin):
                linestyle=""
                if (nodes[i].edges[j].used == 0):
                    linestyle="dashed"
                else:
                    linestyle="bold"

                if (withSubTree == 1):
                    print("\"{}.{}\" -> \"{}.{}\" [dir=none, label=\"{:0.1f}\", style={}]".format(nodes[i].edges[j].origin,nodes[nodes[i].edges[j].originIndex].subtree,
                                                                                            nodes[i].edges[j].destination,nodes[nodes[i].edges[j].destinationIndex].subtree,
                                                                                            nodes[i].edges[j].length,linestyle))
                else:
                    print("\"{}\" -> \"{}\" [dir=none, label=\"{:0.1f}\", style={}]".format(nodes[i].edges[j].origin,nodes[i].edges[j].destination,nodes[i].edges[j].length,linestyle))

        if (usedEdgeCount > maximumEdgeCount):
            maximumEdgeCount = usedEdgeCount

    for i in range(len(nodes)):
        usedEdgeCount = 0
        for j in range(len(nodes[i].edges)):
            if (nodes[i].edges[j].used == 1):
                usedEdgeCount+=1


        if (usedEdgeCount == 1):
            print("\"{}\" [style=filled, fillcolor = green]".format(nodes[i].name))

        if (usedEdgeCount == maximumEdgeCount):
            print("\"{}\" [style=filled, fillcolor = pink]".format(nodes[i].name))

    print("}")

def dumpGraph(g):
    print('Graph subtrees {}'.format(g.numSubTrees))
    for i in range(len(g.nodes)):
        print(' Node: {}, {} edges {} subtree {}'.format(i, g.nodes[i].name, g.nodes[i].numEdges, g.nodes[i].subtree))
        for j in range(len(g.nodes[i].edges)):
            print('  Edge: {}[{}] -> {}[{}] length={:0.1f} used={}'.format(g.nodes[i].edges[j].origin, g.nodes[i].edges[j].originIndex,
                                                                     g.nodes[i].edges[j].destination, g.nodes[i].edges[j].destinationIndex,
                                                                     g.nodes[i].edges[j].length, g.nodes[i].edges[j].used))

def createNodes(edges):

    nodeStorage = 2
    numNodes = 0
    nodes = []

    for i in range(len(edges)):
        ni = findNodeIndexByName(nodes, numNodes, edges[i].origin)
        if (ni == -1):
            newnode = node(edges[i].origin, 1, [], 0, 0)
            numNodes+=1
            nodes.append(newnode)
        else:
            nodes[ni].numEdges+=1

        ni = findNodeIndexByName(nodes, numNodes, edges[i].destination)
        if (ni == -1):
            newnode = node(edges[i].destination, 1, [], 0, 0)
            numNodes+=1
            nodes.append(newnode)
        else:
            nodes[ni].numEdges+=1

    for i in range(len(nodes)):
        nodes[i].edges = [None]* nodes[i].numEdges

    filledEdges = [0] * numNodes

    for i in range(len(edges)):
        originIndex = findNodeIndexByName(nodes, numNodes, edges[i].origin)
        destinationIndex = findNodeIndexByName(nodes, numNodes, edges[i].destination)

        nodes[originIndex].edges[filledEdges[originIndex]] = copy.copy(edges[i])
        nodes[originIndex].edges[filledEdges[originIndex]].originIndex = originIndex
        nodes[originIndex].edges[filledEdges[originIndex]].destinationIndex = destinationIndex
        filledEdges[originIndex]+=1

        nodes[destinationIndex].edges[filledEdges[destinationIndex]] = copy.copy(edges[i])
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].originIndex = originIndex
        nodes[destinationIndex].edges[filledEdges[destinationIndex]].destinationIndex = destinationIndex
        filledEdges[destinationIndex]+=1

    g = graph(numNodes, nodes, 0)

    return g

def setEdgeAndPartnerUsed(nodes, node, edge, used):

    nodes[node].edges[edge].used = used

    if (node == nodes[node].edges[edge].originIndex):
        otherNode = nodes[node].edges[edge].destinationIndex
        for j in range(len(nodes[otherNode].edges)):
            if (nodes[otherNode].edges[j].originIndex == node):
                nodes[otherNode].edges[j].used = used
    else:
        otherNode = nodes[node].edges[edge].originIndex
        for j in range(len(nodes[otherNode].edges)):
            if (nodes[otherNode].edges[j].destinationIndex == node):
                nodes[otherNode].edges[j].used = used

def flagEdges(nodes, node, edge, flag):

    nodes[node].edges[edge].flag = flag

    if (node == nodes[node].edges[edge].originIndex):
        otherNode = nodes[node].edges[edge].destinationIndex
        for j in range(len(nodes[otherNode].edges)):
            if (nodes[otherNode].edges[j].originIndex == node):
                nodes[otherNode].edges[j].flag = flag
    else:
        otherNode = nodes[node].edges[edge].originIndex
        for j in range(len(nodes[otherNode].edges)):
            if (nodes[otherNode].edges[j].destinationIndex == node):
                nodes[otherNode].edges[j].flag = flag

def joinShortestEdgePerNode(nodes):

    for i in range(len(nodes)):
        minimumLength = nodes[i].edges[0].length
        minimumIndex = 0
        for j in range(len(nodes[i].edges)):
            if (nodes[i].edges[j].length < minimumLength):
                minimumLength = nodes[i].edges[j].length
                minimumIndex = j

        setEdgeAndPartnerUsed(nodes,i,minimumIndex,1)
        print('Add node length {:0.1f}'.format(minimumLength))


def assignSubTrees(g):

    for i in range(len(g.nodes)):
        g.nodes[i].subtree = 0

    currentSubtree = 0
    keepgoing = True
    while (keepgoing):
        currentSubtree+=1
        for i in range(len(g.nodes)):
            if (g.nodes[i].subtree == 0):
                g.nodes[i].subtree = currentSubtree
                break

            if ( i == g.numNodes-1):
                keepgoing = False

        newnode = True
        while (newnode):
            newnode = False
            for i in range(len(g.nodes)):
                if (g.nodes[i].subtree == currentSubtree):
                    for j in range(len(g.nodes[i].edges)):
                        if (g.nodes[i].edges[j].used == 1):
                            if (g.nodes[g.nodes[i].edges[j].destinationIndex].subtree == 0):
                                g.nodes[g.nodes[i].edges[j].destinationIndex].subtree = currentSubtree
                                newnode = True

                            if (g.nodes[g.nodes[i].edges[j].originIndex].subtree == 0):
                                g.nodes[g.nodes[i].edges[j].originIndex].subtree = currentSubtree
                                newnode = True

    g.numSubTrees = currentSubtree-1


def edgeConnectsDifferentSubtrees(nodes, e):
    if (nodes[e.originIndex].subtree != nodes[e.destinationIndex].subtree):
        return True

    return False

def joinShortestEdgePerSubTrees(g):
    for tree in range(1, g.numSubTrees+1):
        minimumLength = 0.0
        minimumEdgeIndex = 0
        minimumNodeIndex = 0
        gotFirst = False
        for i in range(len(g.nodes)):
            if (g.nodes[i].subtree == tree):
                for j in range(len(g.nodes[i].edges)):
                    if (g.nodes[i].edges[j].used == 0):
                        if (edgeConnectsDifferentSubtrees(g.nodes, g.nodes[i].edges[j])):
                            if (not gotFirst):
                                minimumLength = g.nodes[i].edges[j].length
                                minimumEdgeIndex = j
                                minimumNodeIndex = i
                                gotFirst = True

                            if (g.nodes[i].edges[j].length < minimumLength):
                                minimumLength = g.nodes[i].edges[j].length
                                minimumEdgeIndex = j
                                minimumNodeIndex = i

        g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].used = 2

    for i in range(len(g.nodes)):
        for j in range(len(g.nodes[i].edges)):
            if (g.nodes[i].edges[j].used == 2):
                setEdgeAndPartnerUsed(g.nodes,i,j,1)
                print('Add node length {:0.1f}'.format(g.nodes[i].edges[j].length))

def edgeConnectsUnusedNode(nodes, e):
    if (nodes[e.originIndex].used == 0 or nodes[e.destinationIndex].used == 0):
        return True

    return False

def doBoruvkaAlgorithm(g):

    joinShortestEdgePerNode(g.nodes)

    while (True):
        assignSubTrees(g)

        print('Number of subtrees = {}'.format(g.numSubTrees))

        if (g.numSubTrees == 1):
            return

        joinShortestEdgePerSubTrees(g)

def doPrimAlgorithm(g):

    g.nodes[0].used = 1

    keepgoing = True

    while (keepgoing):

        gotFirst = False
        minimumLength = 0.0
        minimumNodeIndex = 0
        minimumEdgeIndex = 0
        for i in range(len(g.nodes)):
            if (g.nodes[i].used == 1):
                for j in range(len(g.nodes[i].edges)):
                    if (g.nodes[i].edges[j].used == 0 and edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])):
                        if (not gotFirst):
                            minimumLength = g.nodes[i].edges[j].length
                            minimumNodeIndex = i
                            minimumEdgeIndex = j
                            gotFirst = True

                        if (g.nodes[i].edges[j].length < minimumLength):
                            minimumLength = g.nodes[i].edges[j].length
                            minimumNodeIndex = i
                            minimumEdgeIndex = j

        if (gotFirst):
            print('Node \"{}\", Edge length {:0.1f}'.format(g.nodes[minimumNodeIndex].name, minimumLength))
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex,1)
        else:
            keepgoing = False

def isCyclic(g):
    keepgoing = True
    nodesLeft = False
    while (keepgoing):
        removed = False
        nodesLeft = False
        for i in range(len(g.nodes)):
            if (g.nodes[i].used == 1):
                nodesLeft = True
                numberEdgesInUse = 0
                for j in range(len(g.nodes[i].edges)):
                    if (g.nodes[i].edges[j].used == 1):
                        numberEdgesInUse+=1

                if (numberEdgesInUse == 1):
                    removed = True
                    g.nodes[i].used = 2
                    for j in range(len(g.nodes[i].edges)):
                        if (g.nodes[i].edges[j].used == 1):
                            setEdgeAndPartnerUsed(g.nodes, i, j, 2)

                if (numberEdgesInUse == 0):
                    removed = True
                    g.nodes[i].used = 2

        if (not removed):
            keepgoing = False

    for i in range(len(g.nodes)):
        if (g.nodes[i].used == 2):
            g.nodes[i].used = 1

        for j in range(len(g.nodes[i].edges)):
            if (g.nodes[i].edges[j].used == 2):
                g.nodes[i].edges[j].used = 1

    if (nodesLeft):
        return True

    return False

def doKruskalAlgorithm(g):

    keepgoing = True

    while (keepgoing):
        gotFirst = False
        minimumLength = 0.0
        minimumNodeIndex = 0
        minimumEdgeIndex = 0
        for i in range(len(g.nodes)):
            for j in range(len(g.nodes[i].edges)):
                if (g.nodes[i].edges[j].used == 0):
                    allowed = False
                    if (edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j])):
                        allowed = True
                    else:
                        setEdgeAndPartnerUsed(g.nodes, i, j, 1)
                        if (not isCyclic(g)):
                            allowed = True
                        setEdgeAndPartnerUsed(g.nodes, i, j, 0)

                    if (allowed):
                        if (not gotFirst):
                            minimumLength = g.nodes[i].edges[j].length
                            minimumNodeIndex = i
                            minimumEdgeIndex = j
                            gotFirst = True

                        if (g.nodes[i].edges[j].length < minimumLength):
                            minimumLength = g.nodes[i].edges[j].length
                            minimumNodeIndex = i
                            minimumEdgeIndex = j

        if (gotFirst):
            print('Add edge length {:0.1f}'.format(minimumLength))
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = 1
            g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = 1
            setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex, 1)
        else:
            keepgoing = False

def doReverseDeleteAlgorithm(g):

    keepgoing = True

    while (keepgoing):
        gotFirst = False
        maximumLength = 0.0
        maximumNodeIndex = 0
        maximumEdgeIndex = 0
        for i in range(len(g.nodes)):
            for j in range(len(g.nodes[i].edges)):
                if (g.nodes[i].edges[j].used == 1 and g.nodes[i].edges[j].flag == 0):
                    if (not gotFirst):
                        maximumLength = g.nodes[i].edges[j].length
                        maximumNodeIndex = i
                        maximumEdgeIndex = j
                        gotFirst = True

                    if (g.nodes[i].edges[j].length > maximumLength):
                        maximumLength = g.nodes[i].edges[j].length
                        maximumNodeIndex = i
                        maximumEdgeIndex = j

        setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, 0)
        assignSubTrees(g)
        if (g.numSubTrees == 1):
            print('Removed edge length {:0.1f}'.format(maximumLength))
        else:
            setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1)
            flagEdges(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1)

        if (not isCyclic(g)):
            keepgoing = False

    for i in range(len(g.nodes)):
        for j in range(len(g.nodes[i].edges)):
            if (g.nodes[i].edges[j].used == 2):
                g.nodes[i].edges[j].used = 1


def connectGraph(g):
    for i in range(len(g.nodes)):
        g.nodes[i].used = 1
        g.nodes[i].subtree = 1
        for j in range(len(g.nodes[i].edges)):
            g.nodes[i].edges[j].used = 1
            g.nodes[i].edges[j].flag = 0

def disconnectGraph(g):
    for i in range(len(g.nodes)):
        g.nodes[i].used = 0
        g.nodes[i].subtree = 0
        for j in range(len(g.nodes[i].edges)):
            g.nodes[i].edges[j].used = 0
            g.nodes[i].edges[j].flag = 0

def CalculateTotalEdgeLength(g):
    total = 0.0
    for i in range(len(g.nodes)):
        for j in range(len(g.nodes[i].edges)):
            if (g.nodes[i].edges[j].used == 1):
                total += g.nodes[i].edges[j].length

    print("Total length = {:0.1f}".format(total/2.0))
    return total/2.0

def main():

    allEdges = [
        edge("Bath","Bristol",12.9,0,0,0,0),
        edge("Derby","Nottingham",14.9,0,0,0,0),
        edge("Liverpool","Warrington",18.4,0,0,0,0),
        edge("Chester","Liverpool",18.8,0,0,0,0),
        edge("Portsmouth","Southampton",19.5,0,0,0,0),
        edge("Manchester","Warrington",21.0,0,0,0,0),
        edge("Chester","Warrington",22.6,0,0,0,0),
        edge("Salisbury","Southampton",23.6,0,0,0,0),
        edge("Leicester","Nottingham",27.9,0,0,0,0),
        edge("Leeds","York",28.2,0,0,0,0),
        edge("Preston","Warrington",30.5,0,0,0,0),
        edge("Oxford","Swindon",30.6,0,0,0,0),
        edge("Derby","Leicester",32.2,0,0,0,0),
        edge("Leeds","Sheffield",35.5,0,0,0,0),
        edge("Derby","Stoke",36.2,0,0,0,0),
        edge("Hull","York",37.6,0,0,0,0),
        edge("Chester","Stoke",37.7,0,0,0,0),
        edge("Nottingham","Sheffield",37.8,0,0,0,0),
        edge("Stoke","Warrington",39.1,0,0,0,0),
        edge("Bath","Salisbury",39.4,0,0,0,0),
        edge("Bristol","Gloucester",39.6,0,0,0,0),
        edge("Bristol","Swindon",40.2,0,0,0,0),
        edge("Manchester","Sheffield",40.9,0,0,0,0),
        edge("Derby","Sheffield",41.5,0,0,0,0),
        edge("Cardiff","Swansea",41.6,0,0,0,0),
        edge("Birmingham","Derby",41.8,0,0,0,0),
        edge("Leicester","Peterborough",42.1,0,0,0,0),
        edge("Birmingham","Leicester",42.7,0,0,0,0),
        edge("Cambridge","Peterborough",43.1,0,0,0,0),
        edge("Bristol","Cardiff",44.1,0,0,0,0),
        edge("Leeds","Manchester",44.7,0,0,0,0),
        edge("Exeter","Plymouth",44.8,0,0,0,0),
        edge("Birmingham","Stoke",44.9,0,0,0,0),
        edge("Ipswich","Norwich",45.0,0,0,0,0),
        edge("Edinburgh","Glasgow",47.5,0,0,0,0),
        edge("Brighton","Portsmouth",50.0,0,0,0,0),
        edge("Brighton","London",53.3,0,0,0,0),
        edge("London","Oxford",55.9,0,0,0,0),
        edge("Carlisle","Newcastle",59.1,0,0,0,0),
        edge("Cambridge","Norwich",63.9,0,0,0,0),
        edge("Cambridge","London",64.0,0,0,0,0),
        edge("Hull","Leeds",64.4,0,0,0,0),
        edge("London","Portsmouth",74.7,0,0,0,0),
        edge("Birmingham","Oxford",78.5,0,0,0,0),
        edge("London","Southampton",79.2,0,0,0,0),
        edge("Bristol","Exeter",80.7,0,0,0,0),
        edge("Ipswich","London",81.9,0,0,0,0),
        edge("London","Peterborough",85.4,0,0,0,0),
        edge("Carlisle","Preston",88.3,0,0,0,0),
        edge("Exeter","Salisbury",91.6,0,0,0,0),
        edge("Carlisle","Glasgow",96.8,0,0,0,0),
        edge("Edinburgh","Newcastle",104.0,0,0,0,0),
        edge("Canterbury","Dover",17.1,0,0,0,0),
        edge("Canterbury","London",61.9,0,0,0,0),
        edge("Hull","Lincoln",48.2,0,0,0,0),
        edge("Lincoln","Nottingham",39.5,0,0,0,0),
        edge("Lincoln","Sheffield",46.8,0,0,0,0),
        edge("Lincoln","Peterborough",51.9,0,0,0,0),
        edge("Bangor","Chester",60.9,0,0,0,0),
        edge("Blackpool","Preston",16.0,0,0,0,0),
        edge("Edinburgh","Perth",43.4,0,0,0,0),
        edge("Dundee","Perth",22.4,0,0,0,0),
        edge("Inverness","Perth",112.0,0,0,0,0),
        edge("Aberdeen","Dundee",66.0,0,0,0,0),
        edge("Aberdeen","Inverness",104.0,0,0,0,0),
        edge("Chester","Wrexham",13.4,0,0,0,0),
        edge("Birmingham","Shrewsbury",47.2,0,0,0,0),
        edge("Shrewsbury","Wrexham",31.7,0,0,0,0),
        edge("Birmingham","Worcester",40.6,0,0,0,0),
        edge("Gloucester","Worcester",30.2,0,0,0,0),
        edge("Bournemouth","Exeter",84.6,0,0,0,0),
        edge("Bournemouth","Southampton",33.2,0,0,0,0),
        edge("Darlington","Leeds",61.3,0,0,0,0),
        edge("Darlington","Newcastle",37.3,0,0,0,0),
        edge("Darlington","Middlesbrough",16.4,0,0,0,0),
        edge("Burnley","Leeds",35.7,0,0,0,0),
        edge("Burnley","Manchester",29.0,0,0,0,0),
        edge("Burnley","Preston",21.5,0,0,0,0),
        edge("Cambridge","Kings Lynn",45.1,0,0,0,0),
        edge("Norwich","Kings Lynn",43.6,0,0,0,0),
        edge("Peterborough","Kings Lynn",36.4,0,0,0,0),
        edge("Scarborough","York",41.2,0,0,0,0),
        edge("Middlesbrough","Scarborough",48.9,0,0,0,0),
        edge("Hull","Scarborough",42.5,0,0,0,0),
        edge("Aberystwyth","Shrewsbury",76.0,0,0,0,0),
        edge("Aberystwyth","Cardigan",38.3,0,0,0,0),
        edge("Cardigan","Carmarthen",26.1,0,0,0,0),
        edge("Carmarthen","Swansea",28.0,0,0,0,0),
        edge("Carlisle","Kendal",46.1,0,0,0,0),
        edge("Kendal","Preston",42.7,0,0,0,0),
        edge("Darlington","Kendal",63.3,0,0,0,0),
        edge("Barrow","Kendal",34.7,0,0,0,0),
        edge("Cambridge","Luton",41.0,0,0,0,0),
        edge("Leicester","Luton",69.5,0,0,0,0),
        edge("London","Luton",34.3,0,0,0,0),
        edge("Luton","Oxford",43.5,0,0,0,0),
        edge("London","Newbury",60.8,0,0,0,0),
        edge("Newbury","Oxford",27.2,0,0,0,0),
        edge("Newbury","Southampton",38.1,0,0,0,0),
        edge("Swindon","Newbury",26.0,0,0,0,0),
    ]

    g = createNodes(allEdges)
    dumpDotGraph(g.nodes, g.numNodes, 0)

    doBoruvkaAlgorithm(g)
    dumpDotGraph(g.nodes, g.numNodes, 0)
    Blength = CalculateTotalEdgeLength(g)

    disconnectGraph(g)
    doPrimAlgorithm(g)
    dumpDotGraph(g.nodes, g.numNodes, 0)
    Plength = CalculateTotalEdgeLength(g)

    disconnectGraph(g)
    doKruskalAlgorithm(g)
    dumpDotGraph(g.nodes, g.numNodes, 0)
    Klength = CalculateTotalEdgeLength(g)

    connectGraph(g)
    doReverseDeleteAlgorithm(g)
    dumpDotGraph(g.nodes, g.numNodes, 0)
    Rlength = CalculateTotalEdgeLength(g)

    if (math.fabs(Blength - Plength) < 1.0 and
        math.fabs(Plength - Klength) < 1.0 and
        math.fabs(Klength - Rlength) < 1.0):
        print("Passed")
    else:
        print("Failed")

if __name__ == "__main__":
    main()
