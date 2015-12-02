#include "OneSubgraph.h"

COneSubgraph::COneSubgraph(void)
{
}

COneSubgraph::~COneSubgraph(void)
{
}

void COneSubgraph::FindTheNodeNewIndex(int nIndexOfNode1, int & nIndexOfNode1New){
	int nNumOfNodes = m_vaAllNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNodeInSubgraph & aOneNodeRefer = m_vaAllNodesInSubgraph[nIdx];
		int nIndexInAllNodes = aOneNodeRefer.GetNodeIndex(); // "nIndexInAllNodes" is index in "std::vector<COneNode> m_vaAllNodes;" in COneGraph.
		if (nIndexInAllNodes == nIndexOfNode1)
		{
			nIndexOfNode1New = nIdx;
		}
	}
}

void COneSubgraph::TryToAddOneNodeToTheSubgraph(int nOneNewNodeIndex, vector<COneNode>& vaAllNodesInOriWholeGraph){
	// we will try to add this node to this subgraph: "vector<COneNode> m_vaAllNodes;"
	bool bIsThisNodeExist = false;
	int nNumOfExistingNodes = m_vaAllNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfExistingNodes; nIdx++)
	{
		COneNodeInSubgraph & aOneNodeRefer = m_vaAllNodesInSubgraph[nIdx];
		int nOneExistingNodeIndex = aOneNodeRefer.GetNodeIndex();
		if (nOneExistingNodeIndex == nOneNewNodeIndex)
		{
			bIsThisNodeExist = true;
			break;
		}
	}
	// Check whether this node exists in the subgraph
	if (bIsThisNodeExist == false)
	{	// we need to add this node to the new subgraph.
		COneNode& aNodeReferInOriWholeGraph = vaAllNodesInOriWholeGraph[nOneNewNodeIndex];
		string sOneNewNodeName = aNodeReferInOriWholeGraph.GetNodeName();
		double dOneNewNodeWeight = aNodeReferInOriWholeGraph.GetNodeWeight();
		COneNodeInSubgraph aOneNewNodeToSubgraph;
		aOneNewNodeToSubgraph.SetNodeName(sOneNewNodeName);
		aOneNewNodeToSubgraph.SetNodeIndex(nOneNewNodeIndex);
		aOneNewNodeToSubgraph.SetNodeWeight(dOneNewNodeWeight);
		aOneNewNodeToSubgraph.SetIndexOfNodeInSubgraph(m_vaAllNodesInSubgraph.size());
		m_vaAllNodesInSubgraph.push_back(aOneNewNodeToSubgraph);
	} // if bIsThisNodeExist == true, then do nothing, just return.
};

bool COneSubgraph::FindWhetherThisNodeExistInSubgraph(int nOneAdjNodeIndex, int & nIndexOfAdjNodeInSubgraph){
	bool bIsThisNodeExistInSubgraph = false;
	int nNumOfNodes = m_vaAllNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNodeInSubgraph & aOneNodeRefer = m_vaAllNodesInSubgraph[nIdx];
		int nIdxOfNodeInOriWholeGraph = aOneNodeRefer.GetNodeIndex();
		if (nIdxOfNodeInOriWholeGraph == nOneAdjNodeIndex)
		{
			bIsThisNodeExistInSubgraph = true;
			nIndexOfAdjNodeInSubgraph = nIdx;
			break;
		}
	}
	return bIsThisNodeExistInSubgraph;
};

void COneSubgraph::AddOneNewEdgeInSubgraph(int nIdxOfNode1InSubgraph, int nIdxOfNode2InSubgraph, COneEdge& aOneEdgeReferInOriWholeGraph){
	COneEdge aOneNewEdgeInSubgraph;
	aOneNewEdgeInSubgraph.SetIndexOfNode1(nIdxOfNode1InSubgraph);
	aOneNewEdgeInSubgraph.SetIndexOfNode2(nIdxOfNode2InSubgraph);
	double dEdgeWeight = aOneEdgeReferInOriWholeGraph.GetEdgeWeight();
	aOneNewEdgeInSubgraph.SetEdgeWeight(dEdgeWeight);
	m_vaAllEdgesInSubgraph.push_back(aOneNewEdgeInSubgraph);
};

void COneSubgraph::ComputeSubgraphDensity(){
	int nNumOfNodes = m_vaAllNodesInSubgraph.size();
	int nNumOfEdges = m_vaAllEdgesInSubgraph.size();
	double dSumOfAllEdgeWeight = 0.0;
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaAllEdgesInSubgraph[nIdx];
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		dSumOfAllEdgeWeight += dEdgeWeight;
	}
	double dSumOfAllNodeWeight = 0.0;
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode& aOneNodeRefer = m_vaAllNodesInSubgraph[nIdx];
		double dNodeWeight = aOneNodeRefer.GetNodeWeight();
		dSumOfAllNodeWeight += dNodeWeight;
	}
	// Compute the density.
	m_dSubgraphDensity = dSumOfAllEdgeWeight / dSumOfAllNodeWeight;
};

void COneSubgraph::OutputTheSubgraph(string sOutputFileNameNodes, string sOutputFileNameEdges){
	ofstream aOutputStream;
	// Output the nodes in the subgraph
	aOutputStream.open(sOutputFileNameNodes);
	int nNumOfNodes = m_vaAllNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNodeInSubgraph& aOneNodeReferInSubgraph = m_vaAllNodesInSubgraph[nIdx];
		string sNodeName = aOneNodeReferInSubgraph.GetNodeName();
		double dNodeWeight = aOneNodeReferInSubgraph.GetNodeWeight();
		aOutputStream << sNodeName << " " << dNodeWeight << endl;
	}
	aOutputStream.close();
	// output the edges in the subgraph
	aOutputStream.open(sOutputFileNameEdges);
	int nNumOfEdges = m_vaAllEdgesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeReferInSubgraph = m_vaAllEdgesInSubgraph[nIdx];
		int nIndexOfNode1 = aOneEdgeReferInSubgraph.GetIndexOfNode1();
		int nIndexOfNode2 = aOneEdgeReferInSubgraph.GetIndexOfNode2();
		double dEdgeWeight = aOneEdgeReferInSubgraph.GetEdgeWeight();
		aOutputStream << nIndexOfNode1 << " " << nIndexOfNode2 << " " << dEdgeWeight << endl;
	}
	aOutputStream.close();
};

void COneSubgraph::ClearAllMemberVariables(){
	m_vaAllNodesInSubgraph.resize(0);
	m_vaAllEdgesInSubgraph.resize(0);
	m_dSubgraphDensity = 0.0;
};
