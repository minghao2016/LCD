#pragma once
#include "MyHeader.h"
#include "OneNode.h"
#include "OneEdge.h"
#include "OneSubgraph.h"

typedef struct {
	int    m_nNodeIndex;
	double m_dNodeAdjacencyToVisitedNodes;
	double m_dQueryBiasedAdjValue;
} stNodeWithAdjacencyToVisitedNodes;

class COneGraph
{
public:
	COneGraph(void);
	~COneGraph(void);
public:
	string m_sOutputFolder;
	std::vector<COneNode> m_vaAllNodes;
	std::vector<COneEdge> m_vaAllEdges;
	vector<string> m_vsSeedNodesNameList;
	vector<int>    m_vnSeedNodesIndexList;
	int  m_nMaxNumOfNewNodesInMAS; // To avoid visiting too many nodes (make sure the algorithm will terminate.)
public: // Part 1. Reading the files.
	void ReadNodesFile(string sNodesFileName);
	void ReadNodeWeightsFile(string sNodeWeightsFileName);
	void ReadEdgesFile(string sEdgesFileName);
	void ReadSeedNodeFile(string sSeedsFileName);
	int  FindNodeIndexByNodeName(string sNodeName);
	inline void SetOutputFileFolder(string sOutputFolder){m_sOutputFolder = sOutputFolder;};
	inline void SetNumOfIterK(int nMaxNumOfNewNodesInMAS){m_nMaxNumOfNewNodesInMAS = nMaxNumOfNewNodesInMAS;};
	void SetUpAdjacentNodesAndInducedEdgeForEachNode();
	string ConvertIntToString(int nNumber, int nWidth);
public: // Part 4. GCS and MA process.
	int  m_nIterationForMAExpansion; // the index of the iteration for the maximum adjacency expansion process.
	vector<stNodeWithAdjacencyToVisitedNodes> m_vstNodeWithAdjacencyToVisitedNodes; // node degree (adjacency values to the visited nodes).
	int  m_nIndexOfNodeWithMaxAdj; // we will expand this node to the subgraph in the next iteration.
	bool m_bHaveAllNodesBeenVisited; // we may have visited all the nodes in the whole graph.
	vector<int> m_vnIndexOfNodesInSubgraph; // this variable stores the nodes in the subgraph, where all the new nodes' shortest path are also in the subgraph.
	vector<int> m_vnNewNodesIndex_Buffer1; // they will be eventually added to the subgraph, i.e., m_vnIndexOfNodesInSpToSeedsSubgraph
	vector<double> m_vdDensityCurve; // the list of all the density values.
	COneSubgraph m_aTheSubgraphWithMaximumDensity; // The subgraph with the maximum density will be stored in this variable.
public:
	void MaximumAdjacencySearch(); // maximum adjacency search algorithm
	void FindTheNodeWithMaximumAdjacencyToVisitedNodes(); // the final goal is to find the node "m_nIndexOfNodeWithMaxAdj". If all nodes have been visited, then set up m_bHaveAllNodesBeenVisited as true.
	int  FindOneNodeIndexInAdjNodesList(int nIndexOfOneNode); // auxiliary function. Given a node, find whether it is in the list "m_vstNodeWithAdjacencyToVisitedNodes".
	double ComputeDensityOfCurrentSubgraph(); // compute the density of current subgraph during the maximum adjacency search process.
	bool FindWhetherThisNodeAlsoExistsInSubgraph(int nOneNodeIndex, int & nIndexOfAdjNodeInSubgraph);
	void ExtractAndStoreTheSubgraph(); // extract the densest subgraph
	void OutputTheLocalCommunity(); // output the local community (the densest subgraph).
};
