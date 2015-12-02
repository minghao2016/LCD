#include "OneSubnetwork.h"

COneSimpleSubnetwork::COneSimpleSubnetwork(void)
{
};
COneSimpleSubnetwork::~COneSimpleSubnetwork(void)
{
};

void COneSimpleSubnetwork::FindTheNodeNewIndex(int nIndexOfNode1, int & nIndexOfNode1New){
	int nNumOfNodes = m_vaAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode & aOneNodeRefer = m_vaAllNodes[nIdx];
		int nIndexInAllNodes = aOneNodeRefer.GetIndexInAllNodes();
		if (nIndexInAllNodes == nIndexOfNode1)
		{
			nIndexOfNode1New = nIdx;
		}
	}
}

COneSubnetwork::COneSubnetwork(void)
{
}

COneSubnetwork::~COneSubnetwork(void)
{
}

void COneSubnetwork::ClearAllNodesAndEdges()
{	// clear all the nodes and edges in this subnetwork.
	m_vaPPIAllNodes.resize(0);
	m_vaPPIAllEdges.resize(0);
	m_vaGGIAllNodes.resize(0);
	m_vaGGIAllEdges.resize(0);
};

void COneSubnetwork::InitializeRemainNodesIndexAndRemainEdgeFlag()
{ // Initialize "m_vnRemainNodesIndex" and "m_vbAllEdgesRemainFlag".
	// 1. Remain Nodes Flag.
	int nNumOfNodes = m_vaPPIAllNodes.size();
	m_vbPPIAllNodesRemainFlag.resize(nNumOfNodes);
	m_vbGGIAllNodesRemainFlag.resize(nNumOfNodes);
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		m_vbPPIAllNodesRemainFlag[nIdx] = true;
		m_vbGGIAllNodesRemainFlag[nIdx] = true;
	}
	// 2. Remain Nodes Index
	m_vstRemainNodesIndexAndWeighDegree.resize(nNumOfNodes);
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		stRemainNode& stOneRemainNode = m_vstRemainNodesIndexAndWeighDegree[nIdx];
		COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIdx];
		stOneRemainNode.m_nRemainNodeIndex = aOneNodeRefer.GetIndexInAllNodes();
		stOneRemainNode.m_dRemainNodeQueryBiasedDegree = 0.0;
		stOneRemainNode.m_bCriticalNodeFlag = false;
	}
	// 3. Remain Edges Flag
	int nNumOfPPIEdges = m_vaPPIAllEdges.size();
	m_vbPPIAllEdgesRemainFlag.resize(nNumOfPPIEdges);
	for (int nIdx = 0; nIdx < nNumOfPPIEdges; nIdx++)
	{
		COneEdge & aOneEdgeRefer = m_vaPPIAllEdges[nIdx];
		m_vbPPIAllEdgesRemainFlag[nIdx] = true;
	}
	int nNumOfGGIEdges = m_vaGGIAllEdges.size();
	m_vbGGIAllEdgesRemainFlag.resize(nNumOfGGIEdges);
	for (int nIdx = 0; nIdx < nNumOfGGIEdges; nIdx++)
	{
		COneEdge & aOneEdgeRefer = m_vaGGIAllEdges[nIdx];
		m_vbGGIAllEdgesRemainFlag[nIdx] = true;
	}
};

void COneSubnetwork::CalculateNodesWeightedDegree()
{	// calculate the weighted degree of each node.
	int nNumOfRemainNodes = m_vstRemainNodesIndexAndWeighDegree.size();
	for (int nIdx = 0; nIdx < nNumOfRemainNodes; nIdx++)
	{
		stRemainNode& stOneRemainNode = m_vstRemainNodesIndexAndWeighDegree[nIdx];
		int nIndexOfRemainNode = stOneRemainNode.m_nRemainNodeIndex;
		COneNode& aOneRemainGGINodeRefer = m_vaGGIAllNodes[nIndexOfRemainNode];
		int nNumOfRemainEdges = aOneRemainGGINodeRefer.GetNumberOfAdjacentNodesInducedEdges();
		// Begin to calculate the edge weight
		int	   nNumOfRemainEdgesRelateToThisNode = 0; // to count
		double dSumOfWeightOnRelatedEdgesToThisNode = 0.0;
		for (int nIdx2 = 0; nIdx2 < nNumOfRemainEdges; nIdx2++)
		{
			int nIndexOfOneRemainEdge = aOneRemainGGINodeRefer.GetOneAdjacentNodeInducedEdgesIndex(nIdx2);
			COneEdge& aOneRemainEdge = m_vaGGIAllEdges[nIndexOfOneRemainEdge];
			bool bIsThisEdgeRemain = m_vbGGIAllEdgesRemainFlag[nIndexOfOneRemainEdge];
			if (bIsThisEdgeRemain == true)
			{
				double dThisEdgeWeight = aOneRemainEdge.GetEdgeWeight();
				dSumOfWeightOnRelatedEdgesToThisNode += dThisEdgeWeight;
				nNumOfRemainEdgesRelateToThisNode++;
			}
		}
		// Store this weighted degree "dSumOfWeightOnRelatedEdgesToThisNode". Not average degree.
		double dNodeWeight = aOneRemainGGINodeRefer.GetNodeWeight();
		stOneRemainNode.m_dRemainNodeQueryBiasedDegree = dSumOfWeightOnRelatedEdgesToThisNode / dNodeWeight;
	}
};

void COneSubnetwork::InitializeTrackInfor4DeleteHistory()
{ // "m_vnNumOfDeletedNodes", "m_vdRemainNetworkDensity", "m_vnNumOfCandidateNodesToDelete" will be initialized.
	m_vnIndexOfDeletedNode.resize(0);
	m_vdRemainNetworkDensity.resize(0);
	m_vnNumOfCriticalNodesEncounteredWhenDelete.resize(0);
};

bool MyCompare(stRemainNode& stFst, stRemainNode& stSnd){return stFst.m_dRemainNodeQueryBiasedDegree > stSnd.m_dRemainNodeQueryBiasedDegree;}; // descending order
bool COneSubnetwork::SortRemainNodesAndSelectIndependentNonArticLowDegreeNodes(double dNodeDegreeThreshold, vector<int> & vnSeedNodesList)
{	// "nIndexOfMinNode" is the index in vector "m_vstRemainNodesIndexAndWeighDegree", this should be sorted vector.
	m_vnIdxOfIndependentNonArticLowDegreeNodes.resize(0);
	m_vnIdxOfExistingBlocksIndex.resize(0);
	// count the number of nodes, which can be deleted. It means the deletion of one node will not disconnect the subnetwork in PPI.
	bool bIsThereAnyNonCriticalNodeRemain = false;
	std::sort(m_vstRemainNodesIndexAndWeighDegree.begin(), m_vstRemainNodesIndexAndWeighDegree.end(), MyCompare); // descending order
	ReCalculateArticulationNodes();	// 20130807 Recalculate the Flag of Articulation Nodes.
	int nNumOfRemainNodes = m_vstRemainNodesIndexAndWeighDegree.size();
	int nIdxOfLastNode = nNumOfRemainNodes - 1;
	int nNumOfCriticalNodesEncountered = 0; // "critical node" means the nodes, whose deletion will disconnect the subnetwork.
	for (int nIdx = nIdxOfLastNode; nIdx >= 0; nIdx--)
	{
		stRemainNode& stOneRemainNode = m_vstRemainNodesIndexAndWeighDegree[nIdx];
		int nIndexOfOneRemainNode = stOneRemainNode.m_nRemainNodeIndex;
		bool bFoundWhetherThisNodeIsSeed = CheckWhetherThisNodeIsSeed(nIndexOfOneRemainNode, vnSeedNodesList);
		if (bFoundWhetherThisNodeIsSeed == true){	continue;	} // If this node is a seed node, then we do nothing and continue to check the next node.
		COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIndexOfOneRemainNode];
		bool bIsThisNodeArticulationNode = aOneNodeRefer.GetArticulationNodeFlag();
		stOneRemainNode.m_bCriticalNodeFlag = bIsThisNodeArticulationNode;
		if (bIsThisNodeArticulationNode == false) // Not articulation node.
		{
			bIsThereAnyNonCriticalNodeRemain = true;
			// Try to add this node to "m_vnIdxOfIndependentNonArticLowDegreeNodes".
			double dNodeDegree = stOneRemainNode.m_dRemainNodeQueryBiasedDegree;
			if (dNodeDegree <= dNodeDegreeThreshold)
			{
				int  nNodeIdxOfBiconnectedComponent = aOneNodeRefer.GetOneIdxOfBiconnectedComponents(0); // if it is non-articulation node, it should only have one "index of biconnected component".
				bool bFoundThisIdxOfBiconnectedComponent = false;
				int  nNumExistingIdxOfBlocks = m_vnIdxOfExistingBlocksIndex.size();
				for (int nIdx2 = 0; nIdx2 < nNumExistingIdxOfBlocks; nIdx2++)
				{
					int nIdxOfOneExistBlock = m_vnIdxOfExistingBlocksIndex[nIdx2];
					if (nIdxOfOneExistBlock == nNodeIdxOfBiconnectedComponent)
					{
						bFoundThisIdxOfBiconnectedComponent = true; break;
					}
				}
				// If we did not find this biconnected component before, we can delete it.
				if (bFoundThisIdxOfBiconnectedComponent == false)
				{
					m_vnIdxOfIndependentNonArticLowDegreeNodes.push_back(nIdx);
					m_vnIdxOfExistingBlocksIndex.push_back(nNodeIdxOfBiconnectedComponent);
				}
			} else {
				break; // if we find one node with larger node degree, we can stop.
			}
		} else {
			nNumOfCriticalNodesEncountered++;
		}
	}
	m_vnNumOfCriticalNodesEncounteredWhenDelete.push_back(nNumOfCriticalNodesEncountered);
	int nNumOfNodesDeleted = m_vnIdxOfIndependentNonArticLowDegreeNodes.size();
	m_vnNumOfIndptNonArticLowDegreeNodesAtEachIter.push_back(nNumOfNodesDeleted);
	int nNumOfNodesToBeDeleted = m_vnIdxOfIndependentNonArticLowDegreeNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodesToBeDeleted; nIdx++){
		int nIdxOfOneNode = m_vnIdxOfIndependentNonArticLowDegreeNodes[nIdx];
		stRemainNode& stOneRemainNode = m_vstRemainNodesIndexAndWeighDegree[nIdxOfOneNode];
		int nIdxOfOneNOdeGlobal = stOneRemainNode.m_nRemainNodeIndex;
	}

	// If the number of nodes to be deleted is 0, then we need to add the node with minimum node degree.
	if (nNumOfNodesDeleted == 0) {
		for (int nIdx = nIdxOfLastNode; nIdx >= 0; nIdx--)
		{
			stRemainNode& stOneRemainNode = m_vstRemainNodesIndexAndWeighDegree[nIdx];
			int nIndexOfOneRemainNode = stOneRemainNode.m_nRemainNodeIndex;
			bool bFoundWhetherThisNodeIsSeed = CheckWhetherThisNodeIsSeed(nIndexOfOneRemainNode, vnSeedNodesList);
			if (bFoundWhetherThisNodeIsSeed == true){	continue;	} // If this node is a seed node, then we do nothing and continue to check the next node.
			COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIndexOfOneRemainNode];
			bool bIsThisNodeArticulationNode = aOneNodeRefer.GetArticulationNodeFlag();
			stOneRemainNode.m_bCriticalNodeFlag = bIsThisNodeArticulationNode;
			if (bIsThisNodeArticulationNode == false) // Not articulation node.
			{
				bIsThereAnyNonCriticalNodeRemain = true;
				m_vnIdxOfIndependentNonArticLowDegreeNodes.push_back(nIdx);
				int nNodeIdxOfBiconnectedComponent = aOneNodeRefer.GetOneIdxOfBiconnectedComponents(0);
				m_vnIdxOfExistingBlocksIndex.push_back(nNodeIdxOfBiconnectedComponent);
				break;
			} else {
				nNumOfCriticalNodesEncountered++;
			}
		}
	}

	return bIsThereAnyNonCriticalNodeRemain;
};

bool COneSubnetwork::CheckWhetherThisNodeIsSeed(int nIndexOfOneNode, vector<int> & vnSeedNodesList){
	bool bFoundWhetherThisNodeIsSeed = false;
	int nNumOfSeeds = vnSeedNodesList.size();
	for (int nIdx = 0; nIdx < nNumOfSeeds; nIdx++)
	{
		int nIndexOfOneSeedNode = vnSeedNodesList[nIdx];
		if (nIndexOfOneSeedNode == nIndexOfOneNode)
		{
			bFoundWhetherThisNodeIsSeed = true; break;
		}
	}
	return bFoundWhetherThisNodeIsSeed;
};

void COneSubnetwork::ReCalculateArticulationNodes(){
	// 1. Initialize the nodes color.
	int nNumOfNodes = m_vaPPIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		bool bRemainNodeFlag = m_vbPPIAllNodesRemainFlag[nIdx];
		if (bRemainNodeFlag == true)
		{
			COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIdx];
			aOneNodeRefer.SetThisNodeColor(0); // WHITE
			aOneNodeRefer.SetArticulationNodeFlag(false);
			aOneNodeRefer.SetDiscoverTime(0);
			aOneNodeRefer.SetLowestDiscoverTime(nNumOfNodes);
			aOneNodeRefer.SetNumOfChild4RootNodeInDFSTree(0);
			aOneNodeRefer.SetParentNodeIndexInDFSTree(0);
		}
	}
	// 2. Do DFS and find the articulation nodes.
	int nTime = 0;
	stack<COneEdge> vaDFSEdgeStack;
	int nIdxOfBiconnectedComponents = 0;
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		bool bRemainNodeFlag = m_vbPPIAllNodesRemainFlag[nIdx];
		if (bRemainNodeFlag == true)
		{
			COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIdx];
			int nNodeColor = aOneNodeRefer.GetThisNodeColor();
			if (nNodeColor == 0) // WHITE
			{
				aOneNodeRefer.SetParentNodeIndexInDFSTree(-1); // the parent of root node is NIL with node index "-1".
				aOneNodeRefer.ArticulationNode(nTime, m_vaPPIAllNodes, m_vbPPIAllNodesRemainFlag, vaDFSEdgeStack, nIdxOfBiconnectedComponents);
			}
		}
	}
	m_vnNumOfBiconnectedComponents.push_back(nIdxOfBiconnectedComponents);
	// Count the number of articulation nodes.
	int nNumOfArticulationNodes = 0;
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		bool bRemainNodeFlag = m_vbPPIAllNodesRemainFlag[nIdx];
		if (bRemainNodeFlag == true)
		{
			COneNode & aOneNodeRefer = m_vaPPIAllNodes[nIdx];
			bool bArticulationNodeFlag = aOneNodeRefer.GetArticulationNodeFlag();
			if (bArticulationNodeFlag == true)
			{	// if this node is articulation node.
				nNumOfArticulationNodes++;
			}
		}
	}
	m_vnNumOfArticulationNodes.push_back(nNumOfArticulationNodes);
};

void COneSubnetwork::DeleteIndepdtLowDegreeNodesAndUpdateRelatedEdges(double dNodeDegreeThreshold)
{	// This function will delete nodes (in "m_vnIdxOfIndependentNonArticLowDegreeNodes") from "m_vstRemainNodesIndexAndWeighDegree".
	// And, update the related edges, in "m_vbAllEdgesRemainFlag".
	// Therefore, we will not affect the variable "m_vaAllNodes" and "m_vaAllEdges".
	int nNumOfNodesToBeDeleted = m_vnIdxOfIndependentNonArticLowDegreeNodes.size();
	for (int nIdx1 = 0; nIdx1 < nNumOfNodesToBeDeleted; nIdx1++)
	{
		int nIdxOfOneNodeToDelete = m_vnIdxOfIndependentNonArticLowDegreeNodes[nIdx1];
		// +++++++
		// Delete this node with index "nIdxOfOneNodeToDelete" in "m_vstRemainNodesIndexAndWeighDegree".
		stRemainNode& aOneNodeToBeDeleted = m_vstRemainNodesIndexAndWeighDegree[nIdxOfOneNodeToDelete];
		int nIndexOfOneNodeToBeDeleted = aOneNodeToBeDeleted.m_nRemainNodeIndex;
		// Delete this node in PPI network.
		COneNode& aOneNodeRefer = m_vaPPIAllNodes[nIndexOfOneNodeToBeDeleted];
		m_vbPPIAllNodesRemainFlag[nIndexOfOneNodeToBeDeleted] = false;
		int nNumOfEdges = aOneNodeRefer.GetNumberOfAdjacentNodesInducedEdges();
		for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
		{
			int nOneEdgeIndex = aOneNodeRefer.GetOneAdjacentNodeInducedEdgesIndex(nIdx);
			m_vbPPIAllEdgesRemainFlag[nOneEdgeIndex] = false;
		}
		// Delete this node in GGI network.
		COneNode& aOneGGINodeRefer = m_vaGGIAllNodes[nIndexOfOneNodeToBeDeleted];
		m_vbGGIAllNodesRemainFlag[nIndexOfOneNodeToBeDeleted] = false;
		nNumOfEdges = aOneGGINodeRefer.GetNumberOfAdjacentNodesInducedEdges();
		double dAssignedEdgeWeight = 0.0; // 20130923William. The summation of edge weight assigned to this node.
		for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
		{
			int nOneEdgeIndex = aOneGGINodeRefer.GetOneAdjacentNodeInducedEdgesIndex(nIdx);
			m_vbGGIAllEdgesRemainFlag[nOneEdgeIndex] = false;
			if (nNumOfNodesToBeDeleted == 1)
			{
				// Add this edge weight to the "dAssignedEdgeWeight"
				int nOneAdjNodeIndex = aOneGGINodeRefer.GetOneAdjacentNodeIndex(nIdx);
				bool bIsThisNodeRemain = m_vbGGIAllNodesRemainFlag[nOneAdjNodeIndex];
				if (bIsThisNodeRemain == true)
				{
					double dOneEdgeWeight = aOneGGINodeRefer.GetOneAdjacentEdgeWeight(nIdx);
					dAssignedEdgeWeight += dOneEdgeWeight;
				}
			}
		}
		if (nNumOfNodesToBeDeleted == 1) {
			aOneGGINodeRefer.SetAssignedEdgeWeight(dAssignedEdgeWeight); // If there is only one node, we need to set the real assigned edge weight.
		} else {
			aOneGGINodeRefer.SetAssignedEdgeWeight(dNodeDegreeThreshold); // If there are multiple nodes to be deleted, we need to set the threshold as assigned edge weight.
		}
		// Erase this edge from the remaining nodes list.
		m_vstRemainNodesIndexAndWeighDegree.erase(m_vstRemainNodesIndexAndWeighDegree.begin() + nIdxOfOneNodeToDelete);
		m_vnIndexOfDeletedNode.push_back(nIndexOfOneNodeToBeDeleted);
	}
	// Store the index of accumulated deleted nodes.
	int nNumOfDeletedNodesAccumulated = m_vnIndexOfDeletedNode.size() - 1;
	m_vnIndexOfDeletedNodeAccumulated.push_back(nNumOfDeletedNodesAccumulated);
};

double COneSubnetwork::CalculateTheDensityOfTheSubnetwork(){
	// Calculate the density of remaining network, and update the delete process tracking information.
	double dRemainNetworkDensity = 0.0;
	double dSumOfRemainNodeWeight = 0.0;
	double dSumOfRemainEdgeWeight = 0.0;
	int nNumOfAllNodes = m_vbGGIAllNodesRemainFlag.size();
	for (int nIdx = 0; nIdx < nNumOfAllNodes; nIdx++)
	{
		bool bIsThisOneRemainNode = m_vbGGIAllNodesRemainFlag[nIdx];
		if (bIsThisOneRemainNode == true)
		{
			COneNode& aOneNodeRefer = m_vaGGIAllNodes[nIdx];
			double dOneNodeWeight = aOneNodeRefer.GetNodeWeight();
			dSumOfRemainNodeWeight += dOneNodeWeight;
		}
	}
	int nNumOfAllEdges = m_vbGGIAllEdgesRemainFlag.size();
	for (int nIdx = 0; nIdx < nNumOfAllEdges; nIdx++)
	{
		bool bIsThisOneRemainEdge = m_vbGGIAllEdgesRemainFlag[nIdx];
		if (bIsThisOneRemainEdge == true)
		{
			COneEdge& aOneEdgeRefer = m_vaGGIAllEdges[nIdx];
			double dOneEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
			dSumOfRemainEdgeWeight += dOneEdgeWeight;
		}
	}
	int nNumOfRemainNodes = m_vstRemainNodesIndexAndWeighDegree.size();
	if (nNumOfRemainNodes > 0)
	{	// to avoid divide by 0.
		dRemainNetworkDensity = dSumOfRemainEdgeWeight / dSumOfRemainNodeWeight;
	}
	m_vdRemainNetworkDensity.push_back(dRemainNetworkDensity);
	return dRemainNetworkDensity;
}

void COneSubnetwork::ExtractTheDensestSubgraph(COneSimpleSubnetwork& aDensestSubnetwork, bool bDoWeConsiderDCSKProblem, int nNumOfNodesInSubgraph)
{	// "vnGenesIndex" input, it will help to decide the node type.
	// "aDensestSubnetwork" output, the extracted subgraph will be stored in this variable.
	int nMaxDensityIndex = 0;
	if (bDoWeConsiderDCSKProblem == false)
	{	// we do not consider DCS_k. We only consider DCS.
		nMaxDensityIndex = max_element(m_vdRemainNetworkDensity.begin(), m_vdRemainNetworkDensity.end()) - m_vdRemainNetworkDensity.begin();
	} else { // we need to consider DCS_k
		int nNumOfRemainingNodes = GetNumOfRemainNodes();
		if (nNumOfRemainingNodes > nNumOfNodesInSubgraph)
		{ // the number of remaining nodes is greater than K "nNumOfNodesInSubgraph".
			nMaxDensityIndex = m_vdRemainNetworkDensity.end() - m_vdRemainNetworkDensity.begin();
		} else { // the number of remaining nodes is less than K "nNumOfNodesInSubgraph".
			nMaxDensityIndex = m_vdRemainNetworkDensity.end() - m_vdRemainNetworkDensity.begin() - (nNumOfNodesInSubgraph - nNumOfRemainingNodes) - 1;
		}
	}
	// 1. Find the Maximum density and the corresponding number of deleted nodes.
	double dMaxDensity = m_vdRemainNetworkDensity[nMaxDensityIndex];
	m_dDensityOfDensestSubgraph = dMaxDensity;
	if (nMaxDensityIndex == 0)
	{
		cout << "the original whole graph has the largest density: " << dMaxDensity << endl;
		return;
	}
	// Get the accumulated deleted node index.
	if (nMaxDensityIndex <= 2){
		cout << "20130922William: ErrorIndex Please Search me ..." << endl;
	}
	int nMaxDensityIndexAccumulated = m_vnIndexOfDeletedNodeAccumulated[nMaxDensityIndex - 1];
	// 2. Scan each node, if it is no in the previous (0, nMaxDensityIndex) node index, then that node will be in the densest subgraph.
	int nNumOfAllNodes = m_vaPPIAllNodes.size();
	for (int nIdx = 0; nIdx < nNumOfAllNodes; nIdx++)
	{
		bool bIsThisNodeDeletedNode = false;
		for (int nIdx2 = 0; nIdx2 <= nMaxDensityIndexAccumulated; nIdx2++)
		{	// the node in the index "nMaxDensityIndexAccumulated" is already deleted.
			int nIndexOfOneDeletedNode = m_vnIndexOfDeletedNode[nIdx2];
			if (nIndexOfOneDeletedNode == nIdx)
			{
				bIsThisNodeDeletedNode = true;
				break;
			}
		}
		if (bIsThisNodeDeletedNode == false)
		{ // If this node is not deleted. I mean it is in the remaining densest subgraph.
			COneNode& aOneNodeRefer = m_vaPPIAllNodes[nIdx];
			COneNode aOneNodeInDenseSubGrph;
			string sNodeName = aOneNodeRefer.GetNodeName();
			double dNodeWeight = aOneNodeRefer.GetNodeWeight();
			aOneNodeInDenseSubGrph.SetNodeName(sNodeName);
			aOneNodeInDenseSubGrph.SetNodeWeight(dNodeWeight);
			aOneNodeInDenseSubGrph.SetIndexInAllNodes(nIdx);
			aDensestSubnetwork.AddOneNode(aOneNodeInDenseSubGrph);
		}
	}
	// 3. Add PPI Edges.
	int nNumOfEdges = m_vaPPIAllEdges.size();
	int nNumOfNodesInDenestSubGraph = aDensestSubnetwork.GetNumOfNodes();
	for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaPPIAllEdges[nIdx];
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		bool bFoundNode1 = false;
		bool bFoundNode2 = false;
		int  nFoundNode1Index = 0;
		int  nFoundNode2Index = 0;
		for (int nIdx2 = 0; nIdx2 < nNumOfNodesInDenestSubGraph; nIdx2++)
		{
			COneNode& aOneNodeRefer = aDensestSubnetwork.GetOneNodeRefer(nIdx2);
			int nIndexInAllNodes = aOneNodeRefer.GetIndexInAllNodes();
			if (nIndexInAllNodes == nIndexOfNode1)
			{
				bFoundNode1 = true;
				nFoundNode1Index = nIdx2; // the node index is already reassigned.
			}
			if (nIndexInAllNodes == nIndexOfNode2)
			{
				bFoundNode2 = true;
				nFoundNode2Index = nIdx2; // the node index is already reassigned.
			}
			if (bFoundNode1 == true && bFoundNode2 == true)
			{
				break;
			}
		}
		if (bFoundNode1 == true && bFoundNode2 == true)
		{
			COneEdge aOneEdgeInDenseSubGrph;
			aOneEdgeInDenseSubGrph.SetIndexOfNode1(nFoundNode1Index);
			aOneEdgeInDenseSubGrph.SetIndexOfNode2(nFoundNode2Index);
			double dTwoLocusTestValue = aOneEdgeRefer.GetEdgeWeight();
			aOneEdgeInDenseSubGrph.SetEdgeWeight(dTwoLocusTestValue);
			aDensestSubnetwork.AddOnePPIEdge(aOneEdgeInDenseSubGrph);
		}
	}
	// 4. Then, Add GGI Edges.
	int nNumOfGGIEdges = m_vaGGIAllEdges.size();
	for (int nIdx = 0; nIdx < nNumOfGGIEdges; nIdx++)
	{
		COneEdge& aOneEdgeRefer = m_vaGGIAllEdges[nIdx];
		int nIndexOfNode1 = aOneEdgeRefer.GetIndexOfNode1();
		int nIndexOfNode2 = aOneEdgeRefer.GetIndexOfNode2();
		bool bFoundNode1 = false;
		bool bFoundNode2 = false;
		int  nFoundNode1Index = 0;
		int  nFoundNode2Index = 0;
		for (int nIdx2 = 0; nIdx2 < nNumOfNodesInDenestSubGraph; nIdx2++)
		{
			COneNode& aOneNodeRefer = aDensestSubnetwork.GetOneNodeRefer(nIdx2);
			int nIndexInAllNodes = aOneNodeRefer.GetIndexInAllNodes();
			if (nIndexInAllNodes == nIndexOfNode1)
			{
				bFoundNode1 = true;
				nFoundNode1Index = nIdx2; // the node index is already reassigned.
				if (bFoundNode1 == true && bFoundNode2 == true)
				{
					break;
				}
			}
			if (nIndexInAllNodes == nIndexOfNode2)
			{
				bFoundNode2 = true;
				nFoundNode2Index = nIdx2; // the node index is already reassigned.
				if (bFoundNode1 == true && bFoundNode2 == true)
				{
					break;
				}
			}
		}
		if (bFoundNode1 == true && bFoundNode2 == true)
		{
			COneEdge aOneEdgeInDenseSubGrph;
			aOneEdgeInDenseSubGrph.SetIndexOfNode1(nFoundNode1Index);
			aOneEdgeInDenseSubGrph.SetIndexOfNode2(nFoundNode2Index);
			double dTwoLocusTestValue = aOneEdgeRefer.GetEdgeWeight();
			aOneEdgeInDenseSubGrph.SetEdgeWeight(dTwoLocusTestValue);
			aDensestSubnetwork.AddOneGGIEdge(aOneEdgeInDenseSubGrph);
		}
	}
	// 4. ReCalcualte the density of this densest subgraph, and verify it with the previous result.
	nNumOfNodesInDenestSubGraph = aDensestSubnetwork.GetNumOfNodes();
	double dSumOfNodeWeight = 0.0;
	for (int nIdx = 0; nIdx < nNumOfNodesInDenestSubGraph; nIdx++)
	{
		COneNode& aOneNodeRefer = aDensestSubnetwork.GetOneNodeRefer(nIdx);
		double dNodeWeightValue = aOneNodeRefer.GetNodeWeight();
		dSumOfNodeWeight += dNodeWeightValue;
	}
	int nNumOfEdgesInDenestSubGraph = aDensestSubnetwork.GetNumOfGGIEdges();
	double dSumOfEdgeWeight = 0.0;
	for (int nIdx = 0; nIdx < nNumOfEdgesInDenestSubGraph; nIdx++)
	{
		COneEdge& aOneEdgeRefer = aDensestSubnetwork.GetOneGGIEdgeRefer(nIdx);
		double dEdgeWeight = aOneEdgeRefer.GetEdgeWeight();
		dSumOfEdgeWeight += dEdgeWeight;
	}
	double dDensityOfDensestSubGraph = dSumOfEdgeWeight / dSumOfNodeWeight;
	cout << dMaxDensity << "  " << dDensityOfDensestSubGraph << endl;
};

void COneSubnetwork::ApproximationRatioAnalysis(){
	// calculate the approximation ratio == the maximum edge weight assigned to each node.
	int nNumOfNodes = m_vaGGIAllNodes.size();
	double dMaximumAssignedEdgeWeight = 0.0;
	for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
	{
		COneNode & aOneNodeRefer = m_vaGGIAllNodes[nIdx];
		double dOneAssignedEdgeWeight = aOneNodeRefer.GetAssignedEdgeWeight();
		if (nIdx == 0)
		{
			dMaximumAssignedEdgeWeight = dOneAssignedEdgeWeight;
		} else {
			if (dMaximumAssignedEdgeWeight < dOneAssignedEdgeWeight)
			{
				dMaximumAssignedEdgeWeight = dOneAssignedEdgeWeight;
			}
		}
	}
	m_dMaximumAssignedEdgeWeight = dMaximumAssignedEdgeWeight;
	m_dApproximationRatio = m_dMaximumAssignedEdgeWeight / m_dDensityOfDensestSubgraph;
};
