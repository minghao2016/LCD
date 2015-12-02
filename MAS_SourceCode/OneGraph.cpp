#include "OneGraph.h"

COneGraph::COneGraph(void)
{
}

COneGraph::~COneGraph(void)
{
}

void COneGraph::ReadNodesFile(string sNodesFileName)
{
	ifstream aOneFileInputStrm;
	aOneFileInputStrm.open(sNodesFileName.c_str(), ifstream::in);
	string sOneLineContent;
	string sOneNodeName;
	int nIdxOfLine = 0;
	while (std::getline(aOneFileInputStrm, sOneLineContent))
	{
		if (sOneLineContent.compare("") == 0){continue;} // if it is an empty line, continue;
//		std::istringstream(sOneLineContent) >> sOneNodeName;
		sOneNodeName = sOneLineContent;
		COneNode aOneNode;
		aOneNode.SetNodeName(sOneNodeName);
		aOneNode.SetNodeIndex(nIdxOfLine);
		m_vaAllNodes.push_back(aOneNode);
		nIdxOfLine++;
	}
	aOneFileInputStrm.close();

	int nNumOfNodes = m_vaAllNodes.size();
	string sOutputFileName = "C:/Yubao/39DBLP/DBLP01DataMining/NodeWeights.txt";
	ofstream aOutputStream;
	aOutputStream.open(sOutputFileName);
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++){
		aOutputStream << 1.0 << endl;
	}
	aOutputStream.close();
}

void COneGraph::ReadNodeWeightsFile(string sNodeWeightsFileName){
	ifstream aOneFileInputStrm;
	aOneFileInputStrm.open(sNodeWeightsFileName.c_str(), ifstream::in);
	string sOneLineContent;
	double dNodeWeight = 0.0;
	int nIdxOfLine = 0;
	while (std::getline(aOneFileInputStrm, sOneLineContent))
	{
		if (sOneLineContent.compare("") == 0){continue;} // if it is an empty line, continue;
		std::istringstream(sOneLineContent) >> dNodeWeight;
		COneNode& aOneNodeRefer = m_vaAllNodes[nIdxOfLine];
		aOneNodeRefer.SetNodeWeight(dNodeWeight);
		nIdxOfLine++;
	}
	aOneFileInputStrm.close();
};

void COneGraph::ReadEdgesFile(string sEdgesFileName)
{
	ifstream aOneFileInputStrm;
	aOneFileInputStrm.open(sEdgesFileName.c_str(), ifstream::in);
	string sOneLineContent;
	int nNode1Index = 0;
	int nNode2Index = 0;
	double dEdgeWeight = 0.0;
	while (std::getline(aOneFileInputStrm, sOneLineContent))
	{
		if (sOneLineContent.compare("") == 0){continue;} // if it is an empty line, continue;
		std::istringstream(sOneLineContent) >> nNode1Index >> nNode2Index >> dEdgeWeight;
		COneEdge aOneEdge;
		aOneEdge.SetIndexOfNode1(nNode1Index);
		aOneEdge.SetIndexOfNode2(nNode2Index);
		aOneEdge.SetEdgeWeight(dEdgeWeight);
		m_vaAllEdges.push_back(aOneEdge);
	}
	aOneFileInputStrm.close();
}

void COneGraph::ReadSeedNodeFile(string sSeedsFileName){
	m_vsSeedNodesNameList.resize(0);
	m_vnSeedNodesIndexList.resize(0);
	ifstream aOneFileInputStrm;
	aOneFileInputStrm.open(sSeedsFileName.c_str(), ifstream::in);
	string sOneLineContent;
	string sOneSeedNodeName;
	int nIdxOfLine = 0;
	while (std::getline(aOneFileInputStrm, sOneLineContent))
	{
		if (sOneLineContent.compare("") == 0){continue;} // if it is an empty line, continue;
//		std::istringstream(sOneLineContent) >> sOneNodeName;
		sOneSeedNodeName = sOneLineContent;
		m_vsSeedNodesNameList.push_back(sOneSeedNodeName);
		int nSeedNodeIndex = FindNodeIndexByNodeName(sOneSeedNodeName);
		m_vnSeedNodesIndexList.push_back(nSeedNodeIndex);
		nIdxOfLine++;
	}
	aOneFileInputStrm.close();
};

int COneGraph::FindNodeIndexByNodeName(string sNodeName){
	int nNodeIndex = 0;
	int nNumOfNodes = m_vaAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++){
		COneNode& aOneNodeRefer = m_vaAllNodes[nIdx];
		string sThisNodeName = aOneNodeRefer.GetNodeName();
		if (sNodeName.compare(sThisNodeName) == 0){
			nNodeIndex = nIdx; break;
		}
	}
	return nNodeIndex;
};

void COneGraph::SetUpAdjacentNodesAndInducedEdgeForEachNode()
{
	int nNumOfEdges = m_vaAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaAllEdges[nIdx];
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		COneNode& aOneNode1Refer = m_vaAllNodes[nIndexOfNode1];
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		COneNode& aOneNode2Refer = m_vaAllNodes[nIndexOfNode2];
		double dOneEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		aOneNode1Refer.AddOneOfAdjNodeIndex(nIndexOfNode2);
		aOneNode2Refer.AddOneOfAdjNodeIndex(nIndexOfNode1);
		aOneNode1Refer.AddOneOfAdjEdgeIndex(nIdx);
		aOneNode2Refer.AddOneOfAdjEdgeIndex(nIdx);
	}
};

string COneGraph::ConvertIntToString(int nNumber, int nWidth)
{
   stringstream sNumber;//create a stringstream
   sNumber << std::setfill('0') << std::setw(nWidth) << nNumber; //add number to the stream
   return sNumber.str();//return a string with the contents of the stream
}

void COneGraph::MaximumAdjacencySearch(){			// Kernel main function
	m_nIterationForMAExpansion = 0;
	// compute the GCS_SP; Grow the subgraph by maximum adjacency search. Iterate over these two steps.
	m_bHaveAllNodesBeenVisited = false;
	// Initialize all the nodes "m_nNodeRegion" with 0.
	int nNumOfNodes = m_vaAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode& aOneNodeRefer = m_vaAllNodes[nIdx];
		aOneNodeRefer.SetNodeRegion(0);
	}
	// After the convergence, we expand it further.
	m_vnIndexOfNodesInSubgraph.resize(0);
	int nNumOfSeedNodes = m_vnSeedNodesIndexList.size();
	for (int nIdx = 0; nIdx < nNumOfSeedNodes; nIdx++){
		int nIndexOfOneSeedNode = m_vnSeedNodesIndexList[nIdx];
		m_vnIndexOfNodesInSubgraph.push_back(nIndexOfOneSeedNode);
		COneNode& aSeedNodeRefer = m_vaAllNodes[nIndexOfOneSeedNode];
		aSeedNodeRefer.SetNodeRegion(3); // we label it as visited node.
	}
	m_vdDensityCurve.resize(0);
	for (int nIdxOfOneMANode = 0; nIdxOfOneMANode < m_nMaxNumOfNewNodesInMAS; nIdxOfOneMANode++)
	{	// We will expand the subgraph by one node at one time; and at most expand it by "m_nMaxNumOfNewNodesInMAS" times.
		double dDensity = ComputeDensityOfCurrentSubgraph();
		m_vdDensityCurve.push_back(dDensity);
		m_nIterationForMAExpansion++;
		FindTheNodeWithMaximumAdjacencyToVisitedNodes();
		if (m_bHaveAllNodesBeenVisited == true)
		{
			cout << "all nodes have been visited" << endl;
			break;
		}
		COneNode& aOneMaxAdjNodeRefer = m_vaAllNodes[m_nIndexOfNodeWithMaxAdj];
		aOneMaxAdjNodeRefer.SetNodeRegion(3); // we label it as visited node.
		m_vnIndexOfNodesInSubgraph.push_back(m_nIndexOfNodeWithMaxAdj);
	}
};

bool MyCompare(stNodeWithAdjacencyToVisitedNodes& stFst, stNodeWithAdjacencyToVisitedNodes& stSnd){return stFst.m_dQueryBiasedAdjValue < stSnd.m_dQueryBiasedAdjValue;}; // ascending order
void COneGraph::FindTheNodeWithMaximumAdjacencyToVisitedNodes(){
	// Resize the adjacent nodes list
	m_vstNodeWithAdjacencyToVisitedNodes.resize(0);
	// Reset all the nodes' "m_bIsThisNodeInAdjNodesList"
	int nNumOfNodes = m_vaAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode & aOneNodeRefer = m_vaAllNodes[nIdx];
		aOneNodeRefer.SetIsThisNodeInAdjNodesList(false);
	}
	// Select one node with the maximum adjacency to the visited nodes.
	// the visited nodes are in the variable "m_vnIndexOfNodesInSubgraph".
	// All the neighbor nodes will be put in the variable "m_vstNodeWithAdjacencyToVisitedNodes" with their adjacency value to the visited nodes.
	int nNumOfVisitedNodes = m_vnIndexOfNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfVisitedNodes; nIdx++)
	{
		int nIndexOfOneVisitedNode = m_vnIndexOfNodesInSubgraph[nIdx];
		COneNode& aOneVisitedNodeRefer = m_vaAllNodes[nIndexOfOneVisitedNode];
		int nNumOfAdjacentNodes = aOneVisitedNodeRefer.GetNumOfAdjNodes();
		for (int nIdx2 = 0; nIdx2 < nNumOfAdjacentNodes; nIdx2++)
		{
			int nIndexOfOneAdjNode = aOneVisitedNodeRefer.GetOneOfAdjNodeIndex(nIdx2);
			COneNode& aOneAdjNodeRefer = m_vaAllNodes[nIndexOfOneAdjNode];
			int nNodeRegion = aOneAdjNodeRefer.GetNodeRegion();
			if (nNodeRegion == 3){
				continue; // if this node is a visited node, it is in the "m_vnIndexOfNodesInSpToSeedsSubgraph". Then, we need to skip it.
			}
			// Otherwise, we need to put this node into the variable "m_vstNodeWithAdjacencyToVisitedNodes".
			// Or, it already exists in "m_vstNodeWithAdjacencyToVisitedNodes", we need to update its degree.
			// --- Get the reference of the adjacent edge. and its edge weight.
			int nIndexOfAdjEdge = aOneVisitedNodeRefer.GetOneOfAdjEdgeIndex(nIdx2);
			COneEdge& aOneAdjEdgeRefer = m_vaAllEdges[nIndexOfAdjEdge];
			double dEdgeWeight = aOneAdjEdgeRefer.GetEdgeWeight();
			bool bIsThisNodeInAdjNodesList = aOneAdjNodeRefer.GetIsThisNodeInAdjNodesList();
			if (bIsThisNodeInAdjNodesList == true)
			{	// if this node is already in the adjacent node list.
				int nIndexOfAdjNodeInAdjNodesList = FindOneNodeIndexInAdjNodesList(nIndexOfOneAdjNode);
				stNodeWithAdjacencyToVisitedNodes& aOneAdjNodeStructRefer = m_vstNodeWithAdjacencyToVisitedNodes[nIndexOfAdjNodeInAdjNodesList];
				double dAdjNodeAdjacencyValue = aOneAdjNodeStructRefer.m_dNodeAdjacencyToVisitedNodes;
				// update the edge weight.
				dAdjNodeAdjacencyValue += dEdgeWeight;
				aOneAdjNodeStructRefer.m_dNodeAdjacencyToVisitedNodes = dAdjNodeAdjacencyValue;
				continue;
			}
			if (bIsThisNodeInAdjNodesList == false) // if not, we add this node.
			{	// it is neither in the visited nodes list, nor in the adjacency node list.
				// Add this node to the adjacency node list.
				stNodeWithAdjacencyToVisitedNodes aOneAdjNodeStruct;
				aOneAdjNodeStruct.m_nNodeIndex = nIndexOfOneAdjNode;
				// update the edge weight.
				aOneAdjNodeStruct.m_dNodeAdjacencyToVisitedNodes = dEdgeWeight;
				aOneAdjNodeRefer.SetIsThisNodeInAdjNodesList(true);
				m_vstNodeWithAdjacencyToVisitedNodes.push_back(aOneAdjNodeStruct);
			}
		}
	}
	// Divide each node's adjacency value by the node weight.
	int nNumOfAdjNodes = m_vstNodeWithAdjacencyToVisitedNodes.size();
	for (int nIdx = 0; nIdx < nNumOfAdjNodes; nIdx++){
		stNodeWithAdjacencyToVisitedNodes& aOneAdjNodeStructRefer = m_vstNodeWithAdjacencyToVisitedNodes[nIdx];
		int nIndexOfNode = aOneAdjNodeStructRefer.m_nNodeIndex;
		COneNode& aOneNodeRefer = m_vaAllNodes[nIndexOfNode];
		double dNodeWeight = aOneNodeRefer.GetNodeWeight();
		aOneAdjNodeStructRefer.m_dQueryBiasedAdjValue = aOneAdjNodeStructRefer.m_dNodeAdjacencyToVisitedNodes / dNodeWeight;
	}
	// Before selecting the node with the largest adjacency value, let's check whether it is empty.
	if (nNumOfAdjNodes == 0)
	{	// if there is 0 node in the list, it means all the nodes have been visited in the connected component which contains the seed nodes.
		m_bHaveAllNodesBeenVisited = true;
	} else {
		// OK. all the adjacent nodes have been put into the vector "m_vstNodeWithAdjacencyToVisitedNodes" with their adjacency value to the visited nodes.
		// Let's find the node with the maximum adjacency value.
		std::vector<stNodeWithAdjacencyToVisitedNodes>::iterator aTheIteratorWithMaxElement = std::max_element(m_vstNodeWithAdjacencyToVisitedNodes.begin(), m_vstNodeWithAdjacencyToVisitedNodes.end(), MyCompare); // ascending order
		int nIndexOfMaxElement = aTheIteratorWithMaxElement - m_vstNodeWithAdjacencyToVisitedNodes.begin();
		// Take the node with minimum adjacency value.
		stNodeWithAdjacencyToVisitedNodes aTheNodeWithMaxAdj = m_vstNodeWithAdjacencyToVisitedNodes[nIndexOfMaxElement]; // copy, instead of reference.
		m_nIndexOfNodeWithMaxAdj = aTheNodeWithMaxAdj.m_nNodeIndex; // store that node index. that node has the largest adjacency value.
	}
};

int COneGraph::FindOneNodeIndexInAdjNodesList(int nIndexOfOneNode){
	int nIndexOfOneNodeInAdjNodesList = -1;
	// Find the index value in "m_vstNodeWithAdjacencyToVisitedNodes" given the input node with index "nIndexOfOneNode".
	int nNumOfNodesInAdjNodeList = m_vstNodeWithAdjacencyToVisitedNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodesInAdjNodeList; nIdx++)
	{
		stNodeWithAdjacencyToVisitedNodes& aOneAdjNodeStructRefer = m_vstNodeWithAdjacencyToVisitedNodes[nIdx];
		int nThisNodeIndex = aOneAdjNodeStructRefer.m_nNodeIndex;
		if (nThisNodeIndex == nIndexOfOneNode)
		{
			nIndexOfOneNodeInAdjNodesList = nIdx;
			break;
		}
	}
	return nIndexOfOneNodeInAdjNodesList;
};

double COneGraph::ComputeDensityOfCurrentSubgraph(){
	double dSubgraphDensity = 0.0; // the density of the subgraph, whose nodes are in "m_vnIndexOfNodesInSubgraph".
	// We are not going to traverse all the edges in the original graph.
	// For each node in the subgraph, we will traverse its adjacency list, and if there is one adjacent node also exists in the subgraph, then we add one edge.
	double dSumOfEdgeWeights = 0.0;
	int nNumOfNodesInSubgraph = m_vnIndexOfNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodesInSubgraph; nIdx++)
	{
		int nIdxOfNodeInOriWholeGraph = m_vnIndexOfNodesInSubgraph[nIdx];
		COneNode& aTheNodeReferInOriWholeGraph = m_vaAllNodes[nIdxOfNodeInOriWholeGraph];
		// Examine its adjacent nodes list.
		int nNumOfAdjNodes = aTheNodeReferInOriWholeGraph.GetNumOfAdjNodes();
		for (int nIdx2 = 0; nIdx2 < nNumOfAdjNodes; nIdx2++)
		{
			int nOneAdjNodeIndex = aTheNodeReferInOriWholeGraph.GetOneOfAdjNodeIndex(nIdx2);
			int nIndexOfAdjNodeInSubgraph = -1;
			bool bIsThisNodeExistInSubgraph = FindWhetherThisNodeAlsoExistsInSubgraph(nOneAdjNodeIndex, nIndexOfAdjNodeInSubgraph);
			if (nIdx > nIndexOfAdjNodeInSubgraph)
			{	// this is to avoid the duplicate edges in the resulting subgraph.
				continue;
			}
			if (bIsThisNodeExistInSubgraph == true)
			{	// if this node exist in the subgraph also, then this edge exists in the subgraph
				// add one new edge to the subgraph
				int nIndexOfEdge = aTheNodeReferInOriWholeGraph.GetOneOfAdjEdgeIndex(nIdx2);
				COneEdge & aOneAdjEdgeRefer = m_vaAllEdges[nIndexOfEdge];
				double dEdgeWeight = aOneAdjEdgeRefer.GetEdgeWeight();
				dSumOfEdgeWeights += dEdgeWeight;
			}
		}
	}
	// compute the sum of node weights
	double dSumOfNodeWeights = 0.0;
	for (int nIdx = 0; nIdx < nNumOfNodesInSubgraph; nIdx++)
	{
		int nIdxOfNodeInOriWholeGraph = m_vnIndexOfNodesInSubgraph[nIdx];
		COneNode& aTheNodeReferInOriWholeGraph = m_vaAllNodes[nIdxOfNodeInOriWholeGraph];
		dSumOfNodeWeights += aTheNodeReferInOriWholeGraph.GetNodeWeight();
	}
	dSubgraphDensity = dSumOfEdgeWeights / dSumOfNodeWeights;
	return dSubgraphDensity;
};

bool COneGraph::FindWhetherThisNodeAlsoExistsInSubgraph(int nOneNodeIndex, int & nIndexOfAdjNodeInSubgraph){
	bool bDoesThisNodeExistInSubgraph = false;
	int nNumOfNodesInSubgraph = m_vnIndexOfNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodesInSubgraph; nIdx++)
	{
		int nOneNodeIndexInSubgraph = m_vnIndexOfNodesInSubgraph[nIdx];
		if (nOneNodeIndexInSubgraph == nOneNodeIndex){
			bDoesThisNodeExistInSubgraph = true;
			nIndexOfAdjNodeInSubgraph = nIdx; break;
		}
	}
	return bDoesThisNodeExistInSubgraph;
};

void COneGraph::ExtractAndStoreTheSubgraph(){
	// Find the maximum desity value
	std::vector<double>::iterator aIndexOfMaxDensity = std::max_element(m_vdDensityCurve.begin(), m_vdDensityCurve.end()); // ascending order
	int nNumOfNewNodes = aIndexOfMaxDensity - m_vdDensityCurve.begin();
	int nNumOfNodesInTheSubgraph = nNumOfNewNodes + m_vnSeedNodesIndexList.size();
	// the nodes is the first "nNumOfNodesInTheSubgraph" nodes in vector "m_vnIndexOfNodesInSubgraph"
	// 0. Before any work, clear the subgraph first.
	m_aTheSubgraphWithMaximumDensity.ClearAllMemberVariables();
	// extract a subgraph. whenever the node appear in "m_vnIndexOfNodesInSpToSeedsSubgraph", put it in the subgraph "m_aSpToSeedsSubgraph".
	// 1) Add nodes to the subgraph
	int nNumOfNodes = m_vnIndexOfNodesInSubgraph.size();
	for (int nIdx = 0; nIdx < nNumOfNodesInTheSubgraph; nIdx++)
	{
		int nOneNodeIndex = m_vnIndexOfNodesInSubgraph[nIdx];
		m_aTheSubgraphWithMaximumDensity.TryToAddOneNodeToTheSubgraph(nOneNodeIndex, m_vaAllNodes);
	}
	// 2) Add edges to the subgraph.
	// We are not going to traverse all the edges in the original graph.
	// For each node in the subgraph, we will traverse its adjacency list, and if there is one adjacent node also exists in the subgraph, then we add one edge.
	int nNumOfNodesInSubgraph = m_aTheSubgraphWithMaximumDensity.GetNumOfNodes();
	for (int nIdx = 0; nIdx < nNumOfNodesInSubgraph; nIdx++)
	{
		COneNodeInSubgraph& aOneNodeReferInSubgraph = m_aTheSubgraphWithMaximumDensity.GetOneNodeRefer(nIdx);
		int nIdxOfNodeInOriWholeGraph = aOneNodeReferInSubgraph.GetNodeIndex();
		COneNode& aTheNodeReferInOriWholeGraph = m_vaAllNodes[nIdxOfNodeInOriWholeGraph];
		// Examine its adjacent nodes list.
		int nNumOfAdjNodes = aTheNodeReferInOriWholeGraph.GetNumOfAdjNodes();
		for (int nIdx2 = 0; nIdx2 < nNumOfAdjNodes; nIdx2++)
		{
			int nOneAdjNodeIndex = aTheNodeReferInOriWholeGraph.GetOneOfAdjNodeIndex(nIdx2);
			int nIndexOfAdjNodeInSubgraph = -1;
			bool bIsThisNodeExistInSubgraph = m_aTheSubgraphWithMaximumDensity.FindWhetherThisNodeExistInSubgraph(nOneAdjNodeIndex, nIndexOfAdjNodeInSubgraph);
			if (nIdx > nIndexOfAdjNodeInSubgraph)
			{	// this is to avoid the duplicate edges in the resulting subgraph.
				continue;
			}
			if (bIsThisNodeExistInSubgraph == true)
			{	// if this node exist in the subgraph also, then this edge exists in the subgraph
				// add one new edge to the subgraph
				int nIndexOfEdge = aTheNodeReferInOriWholeGraph.GetOneOfAdjEdgeIndex(nIdx2);
				COneEdge & aOneAdjEdgeRefer = m_vaAllEdges[nIndexOfEdge];
				m_aTheSubgraphWithMaximumDensity.AddOneNewEdgeInSubgraph(nIdx,nIndexOfAdjNodeInSubgraph,aOneAdjEdgeRefer);
			}
		}
	}
	// OK. We are done. The subgraph has been extracted.
	// 3) compute the density of the resulting subgraph;
	m_aTheSubgraphWithMaximumDensity.ComputeSubgraphDensity();
};

void COneGraph::OutputTheLocalCommunity(){
	// output the densest subgraph
	// output the node file
	string sOutputFileName = m_sOutputFolder + "01Nodes.txt";
	ofstream aOutputStream;
	aOutputStream.open(sOutputFileName);
	int nNumOfNodes = m_aTheSubgraphWithMaximumDensity.GetNumOfNodes();
	for (int nIdx1 = 0; nIdx1 < nNumOfNodes; nIdx1++)
	{
		COneNodeInSubgraph& aOneNodeRefer = m_aTheSubgraphWithMaximumDensity.GetOneNodeRefer(nIdx1);
		string sNodeName = aOneNodeRefer.GetNodeName();
		aOutputStream << sNodeName << endl;
	}
	aOutputStream.close();
	// output the edge file
	sOutputFileName = m_sOutputFolder + "02Edges.txt";
	aOutputStream.open(sOutputFileName);
	int nNumOfEdges = m_aTheSubgraphWithMaximumDensity.GetNumOfEdges();
	for (int nIdx2 = 0; nIdx2 < nNumOfEdges; nIdx2++)
	{
		COneEdge& aOneEdgeRefer = m_aTheSubgraphWithMaximumDensity.GetOneEdgeRefer(nIdx2);
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		aOutputStream << nIndexOfNode1 << " " << nIndexOfNode2 << " " << dEdgeWeight << endl;
	}
	aOutputStream.close();
};
