import java.lang.Math.*;

class mst
{
	enum usedReason {
		NotUsed,
		IsUsed,
		IsUsedNotCyclic,
		IsUsedSubTree
	}

	public class edge
	{
		public String origin;
		public String destination;
		public float length;
		public usedReason used;
		public int flag;
		public int originIndex;
		public int destinationIndex;

		public edge(String a_origin, String a_destination, float a_length, usedReason a_used, int a_flag, int a_originIndex, int a_destinationIndex)
		{
			origin = a_origin;
			destination = a_destination;
			length = a_length;
			used = a_used;
			flag = a_flag;
			originIndex = a_originIndex;
			destinationIndex = a_destinationIndex;
		}

		public edge(edge init)
		{
			origin = init.origin;
			destination = init.destination;
			length = init.length;
			used = init.used;
			flag = init.flag;
			originIndex = init.originIndex;
			destinationIndex = init.destinationIndex;
		}
	}

	public class node
	{
		public String name;
		public int numEdges;
		public edge[] edges;
		public int subtree;
		public usedReason used;
	}

	public class graph
	{
		public node[] nodes;
		public int numSubTrees;
	}

	private int findNodeIndexByName(node[] nodes, int numNodes, String search)
	{
		int i;
		for ( i = 0; i < numNodes; ++i)
		{
			if (nodes[i].name.equals(search))
			{
				return i;
			}
		}
		return -1;
	}

	private void dumpDotGraph(node[] nodes, int withSubTree)
	{
		System.out.println("digraph G {");
		int maximumEdgeCount = 0;
			for (int i = 0; i < nodes.length; ++i)
		{
			int usedEdgeCount = 0;
			for (int j = 0; j < nodes[i].numEdges; ++j)
			{
				if (nodes[i].edges[j].used == usedReason.IsUsed)
				{
					usedEdgeCount++;
				}
				if (nodes[i].name.equals(nodes[i].edges[j].origin))
				{
					String linestyle;
					if (nodes[i].edges[j].used == usedReason.NotUsed)
					{
						linestyle="dashed";
					}
					else
					{
						linestyle="bold";
					}
					if (withSubTree == 1)
					{
						System.out.printf("\"%s.%d\" -> \"%s.%d\" [dir=none, label=\"%.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[nodes[i].edges[j].originIndex].subtree,
							nodes[i].edges[j].destination,nodes[nodes[i].edges[j].destinationIndex].subtree,
							nodes[i].edges[j].length,linestyle);
					}
					else
					{
						System.out.printf("\"%s\" -> \"%s\" [dir=none, label=\"%.1f\", style=%s]\n",nodes[i].edges[j].origin,nodes[i].edges[j].destination,nodes[i].edges[j].length,linestyle);
					}
				}
			}
			if (usedEdgeCount > maximumEdgeCount)
			{
				maximumEdgeCount = usedEdgeCount;
			}
		}
		for (int i = 0; i < nodes.length; ++i)
		{
			int usedEdgeCount = 0;
			for (int j = 0; j < nodes[i].numEdges; ++j)
			{
				if (nodes[i].edges[j].used == usedReason.IsUsed)
				{
					usedEdgeCount++;
				}
			}
			if (usedEdgeCount == 1)
			{
				System.out.printf("\"%s\" [style=filled, fillcolor = green]\n",nodes[i].name);
			}
			if (usedEdgeCount == maximumEdgeCount)
			{
				System.out.printf("\"%s\" [style=filled, fillcolor = pink]\n",nodes[i].name);
			}
		}
		System.out.println("}");
	}

	public void dumpGraph(graph g)
	{
		System.out.printf("Graph subtrees %d\n",g.numSubTrees);
		for (int i = 0; i < g.nodes.length; ++i)
		{
			System.out.printf(" Node: %d, %s edges %d subtree %d\n", i, g.nodes[i].name, g.nodes[i].numEdges, g.nodes[i].subtree);
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				System.out.printf("  Edge: %s[%d] -> %s[%d] length=%0.1f used=%d\n",g.nodes[i].edges[j].origin, g.nodes[i].edges[j].originIndex,
					g.nodes[i].edges[j].destination, g.nodes[i].edges[j].destinationIndex,
					g.nodes[i].edges[j].length, g.nodes[i].edges[j].used);
			}
		}
	}

	private graph createNodes(edge[] edges, int numberOfEdges)
	{
		int nodeStorage = 2;
		int numNodes = 0;
		node[] nodes = new node[nodeStorage];

		int i;
		for ( i = 0; i < numberOfEdges; ++i)
		{
			int ni = findNodeIndexByName(nodes, numNodes, edges[i].origin);
			if (ni == -1)
			{
				if (numNodes == nodeStorage)
	 			{
					nodeStorage *= 2;
					node[] new_nodes = new node[nodeStorage];
					System.arraycopy(nodes, 0, new_nodes, 0, nodes.length);
					nodes = new_nodes;
				}
				nodes[numNodes] = new node();
				nodes[numNodes].name = edges[i].origin;
				nodes[numNodes].numEdges = 1;
				nodes[numNodes].edges = null;
				nodes[numNodes].used = usedReason.NotUsed;
				nodes[numNodes].subtree = 0;
				numNodes++;
			}
			else
			{
				nodes[ni].numEdges++;
			}

			ni = findNodeIndexByName(nodes, numNodes, edges[i].destination);
			if (ni == -1)
			{
				if (numNodes == nodeStorage)
				{
					nodeStorage *= 2;
					node[] new_nodes = new node[nodeStorage];
					System.arraycopy(nodes, 0, new_nodes, 0, nodes.length);
					nodes = new_nodes;
				}
				nodes[numNodes] = new node();
				nodes[numNodes].name = edges[i].destination;
				nodes[numNodes].numEdges = 1;
				nodes[numNodes].edges = null;
				nodes[numNodes].used = usedReason.NotUsed;
				nodes[numNodes].subtree = 0;
				numNodes++;
			}
			else
			{
				nodes[ni].numEdges++;
			}

		}

		for ( i = 0; i < numNodes; ++i)
		{
			nodes[i].edges = new edge[nodes[i].numEdges];
		}

		int[] filledEdges = new int[numNodes];

		for ( i = 0; i < numberOfEdges; ++i)
		{
			int originIndex = findNodeIndexByName(nodes, numNodes, edges[i].origin);
			int destinationIndex = findNodeIndexByName(nodes, numNodes, edges[i].destination);

			nodes[originIndex].edges[filledEdges[originIndex]] = new edge(edges[i]);
			nodes[originIndex].edges[filledEdges[originIndex]].originIndex = originIndex;
			nodes[originIndex].edges[filledEdges[originIndex]].destinationIndex = destinationIndex;
			filledEdges[originIndex]++;

			nodes[destinationIndex].edges[filledEdges[destinationIndex]] = new edge(edges[i]);
			nodes[destinationIndex].edges[filledEdges[destinationIndex]].originIndex = originIndex;
			nodes[destinationIndex].edges[filledEdges[destinationIndex]].destinationIndex = destinationIndex;
			filledEdges[destinationIndex]++;
		}

		// Shrink array to size
		node[] new_nodes = new node[numNodes];
		System.arraycopy(nodes, 0, new_nodes, 0, numNodes);
		nodes = new_nodes;

		graph g = new graph();
		g.nodes = nodes;
		g.numSubTrees = 0;
		return g;
	}

	private void setEdgeAndPartnerUsed(node[] nodes, int node, int edge, usedReason used)
	{

		nodes[node].edges[edge].used = used;

		if (node == nodes[node].edges[edge].originIndex)
		{
			int otherNode = nodes[node].edges[edge].destinationIndex;
			for (int j = 0; j < nodes[otherNode].numEdges; ++j)
			{
				if (nodes[otherNode].edges[j].originIndex == node)
				{
					nodes[otherNode].edges[j].used = used;
				}
			}
		}
		else
		{
			int otherNode = nodes[node].edges[edge].originIndex;
			for (int j = 0; j < nodes[otherNode].numEdges; ++j)
			{
				if (nodes[otherNode].edges[j].destinationIndex == node)
				{
					nodes[otherNode].edges[j].used = used;
				}
			}
		}

	}

	private void flagEdges(node[] nodes, int node, int edge, int flag)
	{

		nodes[node].edges[edge].flag = flag;

		if (node == nodes[node].edges[edge].originIndex)
			{
				int otherNode = nodes[node].edges[edge].destinationIndex;
				for (int j = 0; j < nodes[otherNode].numEdges; ++j)
				{
					if (nodes[otherNode].edges[j].originIndex == node)
					{
						nodes[otherNode].edges[j].flag = flag;
					}
				}
			}
			else
			{
				int otherNode = nodes[node].edges[edge].originIndex;
				for (int j = 0; j < nodes[otherNode].numEdges; ++j)
				{
					if (nodes[otherNode].edges[j].destinationIndex == node)
					{
						nodes[otherNode].edges[j].flag = flag;
					}
				}
			}
	}

	private void joinShortestEdgePerNode(node[] nodes, int numNodes)
	{
	    for (int i = 0; i < numNodes; ++i)
			{
	        float minimumLength = nodes[i].edges[0].length;
	        int minimumIndex = 0;
	        for (int j = 0; j < nodes[i].numEdges; ++j)
					{
	            if (nodes[i].edges[j].length < minimumLength)
							{
	                minimumLength = nodes[i].edges[j].length;
	                minimumIndex = j;
	            }
	        }
	        setEdgeAndPartnerUsed(nodes,i,minimumIndex,usedReason.IsUsed);
	        System.out.printf("Add node length %.1f\n", minimumLength);
	    }
	}

	private void assignSubTrees(graph g)
	{
		for (int i = 0; i < g.nodes.length; ++i)
		{
			g.nodes[i].subtree = 0;
		}

		int currentSubtree = 0;
		boolean keepgoing = true;
		while (keepgoing)
		{
			currentSubtree++;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				if (g.nodes[i].subtree == 0)
				{
					g.nodes[i].subtree = currentSubtree;
					break;
				}
				if ( i == g.nodes.length-1)
				{
					keepgoing = false;
				}
			}
			boolean newnode = true;
			while (newnode)
			{
				newnode = false;
				for (int i = 0; i < g.nodes.length; ++i)
				{
					if (g.nodes[i].subtree == currentSubtree)
					{
						for (int j = 0; j < g.nodes[i].numEdges; ++j)
						{
							if (g.nodes[i].edges[j].used == usedReason.IsUsed)
							{
								if (g.nodes[g.nodes[i].edges[j].destinationIndex].subtree == 0)
								{
									g.nodes[g.nodes[i].edges[j].destinationIndex].subtree = currentSubtree;
									newnode = true;
								}
								if (g.nodes[g.nodes[i].edges[j].originIndex].subtree == 0)
								{
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

	private boolean edgeConnectsDifferentSubtrees(node[] nodes, edge e)
	{
		if (nodes[e.originIndex].subtree != nodes[e.destinationIndex].subtree)
		{
			return true;
		}
		return false;
	}

	private void joinShortestEdgePerSubTrees(graph g)
	{
		for (int tree = 1; tree <= g.numSubTrees; ++tree)
		{
			float minimumLength = 0.0f;
			int minimumEdgeIndex = 0;
			int minimumNodeIndex = 0;
			boolean gotFirst = false;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				if (g.nodes[i].subtree == tree)
				{
					for (int j = 0; j < g.nodes[i].numEdges; ++j)
					{
						if (g.nodes[i].edges[j].used == usedReason.NotUsed)
						{
							if (edgeConnectsDifferentSubtrees(g.nodes, g.nodes[i].edges[j]))
							{
								if (!gotFirst)
								{
									minimumLength = g.nodes[i].edges[j].length;
									minimumEdgeIndex = j;
									minimumNodeIndex = i;
									gotFirst = true;
								}
								if (g.nodes[i].edges[j].length < minimumLength)
								{
									minimumLength = g.nodes[i].edges[j].length;
									minimumEdgeIndex = j;
									minimumNodeIndex = i;
								}
							}
						}
					}
				}
			}
			g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].used = usedReason.IsUsedSubTree;
		}

		for (int i = 0; i < g.nodes.length; ++i)
		{
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				if (g.nodes[i].edges[j].used == usedReason.IsUsedSubTree)
				{
					setEdgeAndPartnerUsed(g.nodes,i,j,usedReason.IsUsed);
					System.out.printf("Add node length %.1f\n", g.nodes[i].edges[j].length);
				}
			}
		}
	}

	private boolean edgeConnectsUnusedNode(node[] nodes, edge e)
	{
		if (nodes[e.originIndex].used == usedReason.NotUsed || nodes[e.destinationIndex].used == usedReason.NotUsed)
		{
			return true;
		}
		return false;
	}

	private void doBoruvkaAlgorithm(graph g)
	{

		joinShortestEdgePerNode(g.nodes, g.nodes.length);

		while (true)
		{
			assignSubTrees(g);

			System.out.printf("Number of subtrees = %d\n", g.numSubTrees);

			if (g.numSubTrees == 1)
			{
				return;
			}

			joinShortestEdgePerSubTrees(g);
		}
	}

	private void doPrimAlgorithm(graph g)
	{

		g.nodes[0].used = usedReason.IsUsed;

		boolean keepgoing = true;

		while (keepgoing)
		{

			boolean gotFirst = false;
			float minimumLength = 0.0f;
			int minimumNodeIndex = 0;
			int minimumEdgeIndex = 0;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				if (g.nodes[i].used == usedReason.IsUsed)
				{
					for (int j = 0; j < g.nodes[i].numEdges; ++j)
					{
						if (g.nodes[i].edges[j].used == usedReason.NotUsed && edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j]))
						{
							if (!gotFirst)
							{
								minimumLength = g.nodes[i].edges[j].length;
								minimumNodeIndex = i;
								minimumEdgeIndex = j;
								gotFirst = true;
							}
							if (g.nodes[i].edges[j].length < minimumLength)
							{
								minimumLength = g.nodes[i].edges[j].length;
								minimumNodeIndex = i;
								minimumEdgeIndex = j;
							}
						}
					}
				}
			}

			if (gotFirst)
			{
				System.out.printf("Node \"%s\", Edge length %.1f\n", g.nodes[minimumNodeIndex].name, minimumLength);
				g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = usedReason.IsUsed;
				g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = usedReason.IsUsed;
				setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex,usedReason.IsUsed);
			}
			else
			{
				keepgoing = false;
			}
		}

	}

	private boolean isCyclic(graph g)
	{
		boolean keepgoing = true;
		boolean nodesLeft = false;
		while (keepgoing)
		{
			boolean removed = false;
			nodesLeft = false;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				if (g.nodes[i].used == usedReason.IsUsed)
				{
					nodesLeft = true;
					int numberEdgesInUse = 0;
					for (int j = 0; j < g.nodes[i].numEdges; ++j)
					{
						if (g.nodes[i].edges[j].used == usedReason.IsUsed)
						{
							numberEdgesInUse++;
						}
					}
					if (numberEdgesInUse == 1)
					{
						removed = true;
						g.nodes[i].used = usedReason.IsUsedNotCyclic;
						for (int j = 0; j < g.nodes[i].numEdges; ++j)
						{
							if (g.nodes[i].edges[j].used == usedReason.IsUsed)
							{
								setEdgeAndPartnerUsed(g.nodes, i, j, usedReason.IsUsedNotCyclic);
							}
						}
					}
					if (numberEdgesInUse == 0)
					{
						removed = true;
						g.nodes[i].used = usedReason.IsUsedNotCyclic;
					}
				}
			}
			if (!removed)
			{
				keepgoing = false;
			}
		}

		for (int i = 0; i < g.nodes.length; ++i)
		{
			if (g.nodes[i].used == usedReason.IsUsedNotCyclic)
			{
				g.nodes[i].used = usedReason.IsUsed;
			}
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				if (g.nodes[i].edges[j].used == usedReason.IsUsedNotCyclic)
				{
					g.nodes[i].edges[j].used = usedReason.IsUsed;
				}
			}
		}

		if (nodesLeft)
		{
			return true;
		}
		return false;
	}

	private void doKruskalAlgorithm(graph g)
	{

		boolean keepgoing = true;

		while (keepgoing)
		{
			boolean gotFirst = false;
			float minimumLength = 0.0f;
			int minimumNodeIndex = 0;
			int minimumEdgeIndex = 0;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				for (int j = 0; j < g.nodes[i].numEdges; ++j)
				{
					if (g.nodes[i].edges[j].used == usedReason.NotUsed)
					{
						boolean allowed = false;
						if (edgeConnectsUnusedNode(g.nodes, g.nodes[i].edges[j]))
						{
							allowed = true;
						}
						else
						{
							setEdgeAndPartnerUsed(g.nodes, i, j, usedReason.IsUsed);
							if (!isCyclic(g))
							{
								allowed = true;
							}
							setEdgeAndPartnerUsed(g.nodes, i, j, usedReason.NotUsed);
						}
						if (allowed)
						{
							if (!gotFirst)
							{
								minimumLength = g.nodes[i].edges[j].length;
								minimumNodeIndex = i;
								minimumEdgeIndex = j;
								gotFirst = true;
							}
							if (g.nodes[i].edges[j].length < minimumLength)
							{
								minimumLength = g.nodes[i].edges[j].length;
								minimumNodeIndex = i;
								minimumEdgeIndex = j;
							}
						}
					}
				}
			}

			if (gotFirst)
			{
				System.out.printf("Add edge length %.1f\n", minimumLength);
				g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].originIndex].used = usedReason.IsUsed;
				g.nodes[g.nodes[minimumNodeIndex].edges[minimumEdgeIndex].destinationIndex].used = usedReason.IsUsed;
				setEdgeAndPartnerUsed(g.nodes, minimumNodeIndex, minimumEdgeIndex, usedReason.IsUsed);
			}
			else
			{
				keepgoing = false;
			}
		}

	}

	public void doReverseDeleteAlgorithm(graph g)
	{

		boolean keepgoing = true;

		while (keepgoing)
		{
			boolean gotFirst = false;
			float maximumLength = 0.0f;
			int maximumNodeIndex = 0;
			int maximumEdgeIndex = 0;
			for (int i = 0; i < g.nodes.length; ++i)
			{
				for (int j = 0; j < g.nodes[i].numEdges; ++j)
				{
					if (g.nodes[i].edges[j].used == usedReason.IsUsed && g.nodes[i].edges[j].flag == 0)
					{
						if (!gotFirst)
						{
							maximumLength = g.nodes[i].edges[j].length;
							maximumNodeIndex = i;
							maximumEdgeIndex = j;
							gotFirst = true;
						}
						if (g.nodes[i].edges[j].length > maximumLength)
						{
							maximumLength = g.nodes[i].edges[j].length;
							maximumNodeIndex = i;
							maximumEdgeIndex = j;
						}
					}
				}
			}
			setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, usedReason.NotUsed);
			assignSubTrees(g);
			if (g.numSubTrees == 1)
			{
				System.out.printf("Removed edge length %.1f\n", maximumLength);
			}
			else
			{
				setEdgeAndPartnerUsed(g.nodes, maximumNodeIndex, maximumEdgeIndex, usedReason.IsUsed);
				flagEdges(g.nodes, maximumNodeIndex, maximumEdgeIndex, 1);
			}
			if (!isCyclic(g))
			{
				keepgoing = false;
			}
		}
	}

	private void connectGraph(graph g)
	{
		for (int i = 0; i < g.nodes.length; ++i)
		{
			g.nodes[i].used = usedReason.IsUsed;
			g.nodes[i].subtree = 1;
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				g.nodes[i].edges[j].used = usedReason.IsUsed;
				g.nodes[i].edges[j].flag = 0;
			}
		}
	}

	private void disconnectGraph(graph g)
	{
		for (int i = 0; i < g.nodes.length; ++i)
		{
			g.nodes[i].used = usedReason.NotUsed;
			g.nodes[i].subtree = 0;
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				g.nodes[i].edges[j].used = usedReason.NotUsed;
				g.nodes[i].edges[j].flag = 0;
			}
		}
	}

	private float CalculateTotalEdgeLength(graph g)
	{
		float total = 0.0f;
		for (int i = 0; i < g.nodes.length; ++i)
		{
			for (int j = 0; j < g.nodes[i].numEdges; ++j)
			{
				if (g.nodes[i].edges[j].used == usedReason.IsUsed)
				{
					total += g.nodes[i].edges[j].length;
				}
			}
		}
		System.out.printf("Total length = %.1f\n", total/2.0f);
		return total/2.0f;
	}

	public void run()
	{
		final int NUMBER_OF_EDGES = 200;
		final edge[] allEdges = new edge[NUMBER_OF_EDGES];
		int i = 0;
		allEdges[i++] = new edge ("Bath","Bristol",12.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Derby","Nottingham",14.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Liverpool","Warrington",18.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Chester","Liverpool",18.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Portsmouth","Southampton",19.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Manchester","Warrington",21.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Chester","Warrington",22.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Salisbury","Southampton",23.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leicester","Nottingham",27.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leeds","York",28.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Preston","Warrington",30.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Oxford","Swindon",30.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Derby","Leicester",32.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leeds","Sheffield",35.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Derby","Stoke",36.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Hull","York",37.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Chester","Stoke",37.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Nottingham","Sheffield",37.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Stoke","Warrington",39.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bath","Salisbury",39.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bristol","Gloucester",39.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bristol","Swindon",40.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Manchester","Sheffield",40.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Derby","Sheffield",41.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cardiff","Swansea",41.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Derby",41.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leicester","Peterborough",42.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Leicester",42.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cambridge","Peterborough",43.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bristol","Cardiff",44.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leeds","Manchester",44.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Exeter","Plymouth",44.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Stoke",44.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Ipswich","Norwich",45.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Edinburgh","Glasgow",47.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Brighton","Portsmouth",50.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Brighton","London",53.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Oxford",55.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Carlisle","Newcastle",59.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cambridge","Norwich",63.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cambridge","London",64.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Hull","Leeds",64.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Portsmouth",74.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Oxford",78.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Southampton",79.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bristol","Exeter",80.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Ipswich","London",81.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Peterborough",85.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Carlisle","Preston",88.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Exeter","Salisbury",91.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Carlisle","Glasgow",96.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Edinburgh","Newcastle",104.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Canterbury","Dover",17.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Canterbury","London",61.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Hull","Lincoln",48.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Lincoln","Nottingham",39.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Lincoln","Sheffield",46.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Lincoln","Peterborough",51.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bangor","Chester",60.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Blackpool","Preston",16.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Edinburgh","Perth",43.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Dundee","Perth",22.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Inverness","Perth",112.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Aberdeen","Dundee",66.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Aberdeen","Inverness",104.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Chester","Wrexham",13.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Shrewsbury",47.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Shrewsbury","Wrexham",31.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Birmingham","Worcester",40.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Gloucester","Worcester",30.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bournemouth","Exeter",84.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Bournemouth","Southampton",33.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Darlington","Leeds",61.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Darlington","Newcastle",37.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Darlington","Middlesbrough",16.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Burnley","Leeds",35.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Burnley","Manchester",29.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Burnley","Preston",21.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cambridge","Kings Lynn",45.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Norwich","Kings Lynn",43.6f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Peterborough","Kings Lynn",36.4f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Scarborough","York",41.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Middlesbrough","Scarborough",48.9f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Hull","Scarborough",42.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Aberystwyth","Shrewsbury",76.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Aberystwyth","Cardigan",38.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cardigan","Carmarthen",26.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Carmarthen","Swansea",28.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Carlisle","Kendal",46.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Kendal","Preston",42.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Darlington","Kendal",63.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Barrow","Kendal",34.7f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Cambridge","Luton",41.0f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Leicester","Luton",69.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Luton",34.3f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Luton","Oxford",43.5f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("London","Newbury",60.8f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Newbury","Oxford",27.2f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Newbury","Southampton",38.1f,usedReason.NotUsed,0,0,0);
		allEdges[i++] = new edge ("Swindon","Newbury",26.0f,usedReason.NotUsed,0,0,0);

		final graph g = createNodes(allEdges, i);
		dumpDotGraph(g.nodes, 0);

		doBoruvkaAlgorithm(g);
		dumpDotGraph(g.nodes, 0);
		float Blength = CalculateTotalEdgeLength(g);

		disconnectGraph(g);
		doPrimAlgorithm(g);
		dumpDotGraph(g.nodes, 0);
		float Plength = CalculateTotalEdgeLength(g);

		disconnectGraph(g);
		doKruskalAlgorithm(g);
		dumpDotGraph(g.nodes, 0);
		float Klength = CalculateTotalEdgeLength(g);

		connectGraph(g);
		doReverseDeleteAlgorithm(g);
		dumpDotGraph(g.nodes, 0);
		float Rlength = CalculateTotalEdgeLength(g);

		if (Math.abs(Blength - Plength) < 1.0f &&
		Math.abs(Plength - Klength) < 1.0f &&
		Math.abs(Klength - Rlength) < 1.0f)
		{
			System.out.println("Passed");
		}
		else
		{
			System.out.println("Failed");
		}
	}

	public static void main(String args[])
	{
		mst myClass = new mst();
		myClass.run();
	}

}
