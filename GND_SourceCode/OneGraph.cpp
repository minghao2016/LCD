#include "OneGraph.h"

COneGraph::COneGraph(void)
{
	m_vsSeedNodesNameList.resize(0);
	m_vnSeedNodesIndexList.resize(0);
	m_bDoWeConsiderDCSKProblem = false;
}

COneGraph::~COneGraph(void)
{
}

void COneGraph::ReadNodesFile(string sNodesFileName)
{
	m_vaPPIAllNodes.resize(0);
	ifstream aOneFileInputStrm;
	aOneFileInputStrm.open(sNodesFileName.c_str(), ifstream::in);
	string sOneLineContent;
	string sOneNodeName;
	int nIdxOfLine = 0;
	while (std::getline(aOneFileInputStrm, sOneLineContent))
	{
		if (sOneLineContent.compare("") == 0){continue;} // if it is an empty line, continue;
		std::istringstream(sOneLineContent) >> sOneNodeName;
		sOneNodeName = sOneLineContent;
		COneNode aOneNode;
		aOneNode.SetNodeName(sOneNodeName);
		aOneNode.SetIndexInAllNodes(nIdxOfLine);
		m_vaPPIAllNodes.push_back(aOneNode);
		nIdxOfLine++;
	}
	aOneFileInputStrm.close();
	// The conceptual network contains the same set of nodes.
	m_vaGGIAllNodes.resize(0);
	int nNumOfNodes = m_vaPPIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode aOneNode = m_vaPPIAllNodes[nIdx];
		m_vaGGIAllNodes.push_back(aOneNode);
	}
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
		COneNode& aOneNodeRefer1 = m_vaPPIAllNodes[nIdxOfLine];
		aOneNodeRefer1.SetNodeWeight(dNodeWeight);
		COneNode& aOneNodeRefer2 = m_vaGGIAllNodes[nIdxOfLine];
		aOneNodeRefer2.SetNodeWeight(dNodeWeight);
		nIdxOfLine++;
	}
	aOneFileInputStrm.close();
};

void COneGraph::ReadEdgesFile(string sEdgesFileName)
{
	m_vaPPIAllEdges.resize(0);
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
		aOneEdge.SetEdgeWeight(1.0);
		m_vaPPIAllEdges.push_back(aOneEdge);
	}
	aOneFileInputStrm.close();
	// Copy these edges to GGI network
	m_vaGGIAllEdges.resize(0);
	int nNumOfEdges = m_vaPPIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++){
		COneEdge aOneEdge = m_vaPPIAllEdges[nIdx];
		m_vaGGIAllEdges.push_back(aOneEdge);
	}
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
	int nNumOfNodes = m_vaPPIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++){
		COneNode& aOneNodeRefer = m_vaPPIAllNodes[nIdx];
		string sThisNodeName = aOneNodeRefer.GetNodeName();
		if (sNodeName.compare(sThisNodeName) == 0){
			nNodeIndex = nIdx; break;
		}
	}
	return nNodeIndex;
};

void COneGraph::SetUpAdjacentNodesAndIncidentEdgesForEachNode()
{	// Set the adjacent list for PPI network.
	int nNumOfEdges = m_vaPPIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaPPIAllEdges[nIdx];
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		COneNode& aOneNode1Refer = m_vaPPIAllNodes[nIndexOfNode1];
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		COneNode& aOneNode2Refer = m_vaPPIAllNodes[nIndexOfNode2];
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		aOneNode1Refer.AddOneAdjacentNodeIndex(nIndexOfNode2);
		aOneNode2Refer.AddOneAdjacentNodeIndex(nIndexOfNode1);
		aOneNode1Refer.AddOneAdjacentNodeInducedEdgesIndex(nIdx);
		aOneNode2Refer.AddOneAdjacentNodeInducedEdgesIndex(nIdx);
		aOneNode1Refer.AddOneAdjacentEdgeWeight(dEdgeWeight);
		aOneNode2Refer.AddOneAdjacentEdgeWeight(dEdgeWeight);
	}
	// Set the adjacent list for GGI network.
	nNumOfEdges = m_vaGGIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaGGIAllEdges[nIdx];
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		COneNode& aOneNode1Refer = m_vaGGIAllNodes[nIndexOfNode1];
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		COneNode& aOneNode2Refer = m_vaGGIAllNodes[nIndexOfNode2];
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		aOneNode1Refer.AddOneAdjacentNodeIndex(nIndexOfNode2);
		aOneNode2Refer.AddOneAdjacentNodeIndex(nIndexOfNode1);
		aOneNode1Refer.AddOneAdjacentNodeInducedEdgesIndex(nIdx);
		aOneNode2Refer.AddOneAdjacentNodeInducedEdgesIndex(nIdx);
		aOneNode1Refer.AddOneAdjacentEdgeWeight(dEdgeWeight);
		aOneNode2Refer.AddOneAdjacentEdgeWeight(dEdgeWeight);
	}
};

void COneGraph::CalculateNodeDegreeInConceptualNetwork(){
	int nNumOfNodes = m_vaGGIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode & aOneNodeRefer = m_vaGGIAllNodes[nIdx];
		int nNumOfAdjNodes = aOneNodeRefer.GetNumberOfAdjacentNodes();
		double dSummationOfNodeWeight = 0.0;
		for (int nIdx2 = 0; nIdx2 < nNumOfAdjNodes; nIdx2++)
		{
			double dOneEdgeWeight = aOneNodeRefer.GetOneAdjacentEdgeWeight(nIdx2);
			dSummationOfNodeWeight += dOneEdgeWeight;
		}
		double dNodeWeight = aOneNodeRefer.GetNodeWeight();
		double dQueryBiasedNodeDegree = dSummationOfNodeWeight / dNodeWeight;
		aOneNodeRefer.SetNodeDegree(dQueryBiasedNodeDegree); // Yubao20151130
	}
};

void COneGraph::GreedyFindSubgraphConnectInPPIAndDenseInGGI(){
	// Initialize the subnetwork.
	// 1. Initialize the subnetwork "m_aSubnetwork4GreedyDeleteNodeWithMinWeightedDegree" with the original weighted PPI: nodes "vaAllNodes", edges "vaAllEdges".
	m_aSubNetworkForGreedyDeleteNode.ClearAllNodesAndEdges();
	int nNumOfNodes = m_vaPPIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{	// add all the nodes, even the nodes not in the largest CC.
		COneNode aOneNode = m_vaPPIAllNodes[nIdx];
		aOneNode.SetIndexInThisSubnetwork(nIdx);
		m_aSubNetworkForGreedyDeleteNode.AddOnePPINode(aOneNode);
	}
	int nNumOfEdges = m_vaPPIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge aOneEdge = m_vaPPIAllEdges[nIdx];
		m_aSubNetworkForGreedyDeleteNode.AddOnePPIEdge(aOneEdge);
	}
	int nNumOfGGINodes = m_vaGGIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{	// add all the nodes, even the nodes not in the largest CC.
		COneNode aOneNode = m_vaGGIAllNodes[nIdx];
		m_aSubNetworkForGreedyDeleteNode.AddOneGGINode(aOneNode);
	}
	int nNumOfGGIEdges = m_vaGGIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfGGIEdges; nIdx++)
	{
		COneEdge aOneEdge = m_vaGGIAllEdges[nIdx];
		m_aSubNetworkForGreedyDeleteNode.AddOneGGIEdge(aOneEdge);
	}
	m_aSubNetworkForGreedyDeleteNode.InitializeRemainNodesIndexAndRemainEdgeFlag();
	m_aSubNetworkForGreedyDeleteNode.InitializeTrackInfor4DeleteHistory();
	// 2. Enter the iteration, keep deleting the vertex with the minimum weighted degree.
	int nIdxOfIteration = 0;
	// Before enter the deletion iteration, we first calculate the density of the original network.
//	m_aSubNetworkForGreedyDeleteNode.CalculateTheDensityOfTheSubnetwork(); // 20151201
	m_aSubNetworkForGreedyDeleteNode.AddOneIndexOfDeletedNode(-1); // the first one is the whole network, no node is deleted.
	m_aSubNetworkForGreedyDeleteNode.AddOneNumOfCriticalNodesEncounteredWhenDelete(-1); // the first one is the whole network, no critical node is encountered.
	while (1)
	{
		cout << "\r          \r" << nIdxOfIteration;
		nIdxOfIteration++;
		// 2.1 Calculate the weighted degree of each node.
		m_aSubNetworkForGreedyDeleteNode.CalculateNodesWeightedDegree();
		// Then, call the density of the remaining subnetwork.
		double dDensityOfRemainingSubgraph = m_aSubNetworkForGreedyDeleteNode.CalculateTheDensityOfTheSubnetwork();
		double dNodeDegreeThreshold = dDensityOfRemainingSubgraph * m_dGrammaThreshold;
		// 2.2 Sort the Remain Nodes by their weighted degree, and select the node with minimum weighted degree, which is not candidate nodes.
		bool bIsThereAnyNodeCanBeDeleted = m_aSubNetworkForGreedyDeleteNode.SortRemainNodesAndSelectIndependentNonArticLowDegreeNodes(dNodeDegreeThreshold, m_vnSeedNodesIndexList);
		if (bIsThereAnyNodeCanBeDeleted == true)
		{	// Delete the nodes.
			m_aSubNetworkForGreedyDeleteNode.DeleteIndepdtLowDegreeNodesAndUpdateRelatedEdges(dNodeDegreeThreshold);
		} else {	break;	}
	}
	cout << endl;
}

void COneGraph::GetTheDensestSubgraph()
{	// Get the densest subgraph, which is stored in "m_aDensestSubnetwork".
	m_aSubNetworkForGreedyDeleteNode.ExtractTheDensestSubgraph(m_aDensestSubnetwork, m_bDoWeConsiderDCSKProblem, m_nNumOfNodesInSubgraph);
	// Analyze the approximation ratio after get the density of densest subgraph
	m_aSubNetworkForGreedyDeleteNode.ApproximationRatioAnalysis();
};

void COneGraph::OutputTheDensestSubgraph()
{
	string sOutputFileName1 = m_sOutputFilesPath + "01DenseSubgraphNodes.txt";
	string sOutputFileName2 = m_sOutputFilesPath + "02DenseSubgraphEdges.txt";
	ofstream aOutputStream;
	// Output the nodes
	aOutputStream.open(sOutputFileName1, ofstream::out);
	int nNumOfNodes = m_aDensestSubnetwork.GetNumOfNodes();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode& aOneNodeRefer = m_aDensestSubnetwork.GetOneNodeRefer(nIdx);
		string sNodeName = aOneNodeRefer.GetNodeName();
		aOutputStream << sNodeName << endl;
	}
	aOutputStream.close();
	// Output the edges in the network
	aOutputStream.open(sOutputFileName2, ofstream::out);
	int nNumOfEdges = m_aDensestSubnetwork.GetNumOfGGIEdges();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_aDensestSubnetwork.GetOneGGIEdgeRefer(nIdx);
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		aOutputStream << nIndexOfNode1 << " " << nIndexOfNode2 << " " << dEdgeWeight << endl;
	}
	aOutputStream.close();
};

void COneGraph::OutputSummaryOfDCS(){
	string sOutputFileName1 = m_sOutputFilesPath + "03SummaryOfTheLocalCommunity.txt";
	ofstream aOutputStream;
	// Output PPI subnetwork
	aOutputStream.open(sOutputFileName1, ofstream::out);
	aOutputStream << "Number of Nodes : " << m_aDensestSubnetwork.GetNumOfNodes() << endl;
	aOutputStream << "Number of Edges : " << m_aDensestSubnetwork.GetNumOfGGIEdges() << endl;
	aOutputStream << "Density of the Subgraph : " << m_aSubNetworkForGreedyDeleteNode.GetDensityOfDensestSubgraph() << endl;
	aOutputStream << "Maximum Assigned Edge Weight : " << m_aSubNetworkForGreedyDeleteNode.GetMaximumAssignedEdgeWeight() << endl;
	aOutputStream << "Approximation Ratio : " << m_aSubNetworkForGreedyDeleteNode.GetApproximationRatio() << endl;
	aOutputStream.close();
};
