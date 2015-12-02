#pragma once
#include "MyHeader.h"
#include "OneNode.h"
#include "OneEdge.h"
#include "OneSubnetwork.h"

class COneGraph
{
public:
	COneGraph(void);
	~COneGraph(void);
public:
	string m_sOutputFilesPath; // the output path for debug.
	double m_dGrammaThreshold; // the degree threshold in the greedy node deletion algorithm.
	int    m_nNumOfNodesInSubgraph; // the number of nodes in the DCS_k problem.
	bool   m_bDoWeConsiderDCSKProblem; // if yes, we find a DCS_k with "m_nNumOfNodesInSubgraph" number of nodes.
	vector<COneNode> m_vaPPIAllNodes; // the nodes list in PPI network.
	vector<COneEdge> m_vaPPIAllEdges; // the edges list in PPI network.
	vector<COneNode> m_vaGGIAllNodes; // the nodes list in GGI network. Same as "m_vaPPIAllNodes", including gene names, and order.
	vector<COneEdge> m_vaGGIAllEdges; // the edges list in GGI network.
	vector<string>	 m_vsSeedNodesNameList;  // seed nodes
	vector<int>		 m_vnSeedNodesIndexList; // seed nodes
	COneSubnetwork   m_aSubNetworkForGreedyDeleteNode; // the subnetwork used to implement the greedy deletion process.
	COneSimpleSubnetwork m_aDensestSubnetwork; // the densest subgraph, which is connected in PPI, and has maximum density in GGI.
public:
	inline void SetOutputFolder(string sOutputFilesPath){m_sOutputFilesPath = sOutputFilesPath;};
	inline void SetDegreeThresholdInNodeDeletion(double dGammaThreshold){m_dGrammaThreshold = dGammaThreshold;};
	inline void SetDCSKSizeConstraint(int nNumOfNodesInSubgraph){m_bDoWeConsiderDCSKProblem = true; m_nNumOfNodesInSubgraph = nNumOfNodesInSubgraph;};
public:
	void ReadNodesFile(string sNodesFileName);
	void ReadNodeWeightsFile(string sNodeWeightsFileName);
	void ReadEdgesFile(string sEdgesFileName);
	void ReadSeedNodeFile(string sSeedsFileName);
	int  FindNodeIndexByNodeName(string sNodeName);
public:
	void SetUpAdjacentNodesAndIncidentEdgesForEachNode();
	void CalculateNodeDegreeInConceptualNetwork();
public:
	void GreedyFindSubgraphConnectInPPIAndDenseInGGI();
	void GetTheDensestSubgraph();
	void OutputTheDensestSubgraph();
	void OutputSummaryOfDCS();
};
