#include "OneNode.h"

COneNode::COneNode(void)
{
	m_dNodeWeight = 1.0;
	m_dNodeDegree = 0.0;
	m_nColor = 0;
}

COneNode::~COneNode(void)
{
}

void COneNode::ArticulationNode(int & nTime, vector<COneNode> & vaAllNodes, vector<bool> & vbPPIAllNodesRemainFlag, stack<COneEdge> & vaDFSEdgeStack, int & nIdxOfBiconnectedComponents){ // u: current node
	nTime++;
	m_nDiscoverTime = nTime;
	m_nLowestDiscoverTime = nTime;
	m_nColor = 1; // GRAY
	int nNumOfAdjNodes = m_vnAdjacentNodesIndex.size();
	for (int nIdx = 0; nIdx < nNumOfAdjNodes; nIdx++)
	{
		int nOneAdjNodeIndex = m_vnAdjacentNodesIndex[nIdx];
		bool bRemainNodeFlag = vbPPIAllNodesRemainFlag[nOneAdjNodeIndex];
		if (bRemainNodeFlag == true)
		{
			COneNode & aOneAdjNodeRefer = vaAllNodes[nOneAdjNodeIndex]; // v: one adjacent node.
			int nNodeColor = aOneAdjNodeRefer.GetThisNodeColor();
			if (nNodeColor == 0) // v.color == WHITE
			{
				// Push the tree edge into the edge stack
				COneEdge aOneBackEdge;
				aOneBackEdge.SetIndexOfNode1(m_nIndexInAllNodes);
				aOneBackEdge.SetIndexOfNode2(nOneAdjNodeIndex);
				vaDFSEdgeStack.push(aOneBackEdge);
				// Begin recursively call adjacent nodes function.
				aOneAdjNodeRefer.SetParentNodeIndexInDFSTree(m_nIndexInAllNodes); // v.pi = u
				aOneAdjNodeRefer.ArticulationNode(nTime, vaAllNodes, vbPPIAllNodesRemainFlag, vaDFSEdgeStack, nIdxOfBiconnectedComponents);
				if (m_nLowestDiscoverTime > aOneAdjNodeRefer.GetLowestDiscoverTime())
				{	// u.low = min(u.low, v.low)
					m_nLowestDiscoverTime = aOneAdjNodeRefer.GetLowestDiscoverTime();
				}
				if (m_nParentNodeIndexInDFSTree == -1) // Root node in DFS tree.
				{	// if u.pi = NIL ; u is the root node of the DFS tree.
					if (m_nNumOfChild4RootNodeInDFSTree == 1)
					{	// If v is u's second child, then
						m_bArticulationNodeFlag = true;
					}
					if (m_nNumOfChild4RootNodeInDFSTree == 0)
					{	
						m_nNumOfChild4RootNodeInDFSTree++;
					}
				}
				if (m_nParentNodeIndexInDFSTree >= 0 && aOneAdjNodeRefer.GetLowestDiscoverTime() >= m_nDiscoverTime)
				{	// else if v.low >= u.d then
					m_bArticulationNodeFlag = true;
				}
				if (aOneAdjNodeRefer.GetLowestDiscoverTime() >= m_nDiscoverTime)
				{	// This node can be root node.
					// Start a new biconnected component;
					while (!vaDFSEdgeStack.empty()){
						COneEdge & aOneEdgeRefer = vaDFSEdgeStack.top();
						int nIdxOfFstNode = aOneEdgeRefer.GetIndexOfNode1();
						int nIdxOfSndNode = aOneEdgeRefer.GetIndexOfNode2();
						COneNode & aOneNodeRefer21 = vaAllNodes[nIdxOfFstNode];
						int nDiscoverTime21 = aOneNodeRefer21.GetDiscoverTime();
						if (nDiscoverTime21 >= aOneAdjNodeRefer.GetDiscoverTime())
						{
							COneNode & aOneNodeRefer22 = vaAllNodes[nIdxOfSndNode];
							aOneNodeRefer21.AddOneIdxOfBiconnectedComponents(nIdxOfBiconnectedComponents);
							aOneNodeRefer22.AddOneIdxOfBiconnectedComponents(nIdxOfBiconnectedComponents);
							vaDFSEdgeStack.pop();
						} else {
							break; // break this "while (!vaDFSEdgeStack.empty()){" loop.
						}
					}
					COneEdge & aOneEdgeRefer = vaDFSEdgeStack.top();
					int nIdxOfFstNode = aOneEdgeRefer.GetIndexOfNode1();
					int nIdxOfSndNode = aOneEdgeRefer.GetIndexOfNode2();
					if (nIdxOfFstNode == m_nIndexInAllNodes && nIdxOfSndNode == nOneAdjNodeIndex)
					{
						COneNode & aOneNodeRefer21 = vaAllNodes[nIdxOfFstNode];
						COneNode & aOneNodeRefer22 = vaAllNodes[nIdxOfSndNode];
						aOneNodeRefer21.AddOneIdxOfBiconnectedComponents(nIdxOfBiconnectedComponents);
						aOneNodeRefer22.AddOneIdxOfBiconnectedComponents(nIdxOfBiconnectedComponents);
						vaDFSEdgeStack.pop();
					} else {
						cout << "(v,w) problem " << nIdxOfFstNode << " " << nIdxOfSndNode << endl;
					}
					nIdxOfBiconnectedComponents++;
				}
			}
			if (nNodeColor != 0 && m_nParentNodeIndexInDFSTree != aOneAdjNodeRefer.GetIndexInAllNodes()) // (u, v) is an back edge.
			{	// Not WHITE // else if v != u.pi then
				if (m_nLowestDiscoverTime > aOneAdjNodeRefer.GetDiscoverTime())
				{
					m_nLowestDiscoverTime = aOneAdjNodeRefer.GetDiscoverTime();
				}
			}
			if (nNodeColor != 0 && m_nParentNodeIndexInDFSTree != aOneAdjNodeRefer.GetIndexInAllNodes() && m_nDiscoverTime > aOneAdjNodeRefer.GetDiscoverTime()) // (u, v) is an back edge.
			{	// Not WHITE // else if v != u.pi then
				COneEdge aOneBackEdge;
				aOneBackEdge.SetIndexOfNode1(m_nIndexInAllNodes);
				aOneBackEdge.SetIndexOfNode2(nOneAdjNodeIndex);
				vaDFSEdgeStack.push(aOneBackEdge);
			}
		}
	}
};

void COneNode::AddOneIdxOfBiconnectedComponents(int nIdxOfBiconnectedComponents) {
	int nNumOfIdxOfBiconnectedComponents = m_vnIdxOfBiconnectedComponents.size();
	bool bDuplicate = false;
	for (int nIdx = nNumOfIdxOfBiconnectedComponents - 1; nIdx >= 0; nIdx--)
	{
		int nOneIdxOfBiconnectedComponent = m_vnIdxOfBiconnectedComponents[nIdx];
		if (nOneIdxOfBiconnectedComponent == nIdxOfBiconnectedComponents)
		{
			bDuplicate = true; break;
		}
	}
	if (bDuplicate == false)
	{
		m_vnIdxOfBiconnectedComponents.push_back(nIdxOfBiconnectedComponents);
	}
};
