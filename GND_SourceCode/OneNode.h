#pragma once
#include "MyHeader.h"
#include "OneEdge.h"

class COneNode
{
public:
	COneNode(void);
	~COneNode(void);
public:
	int m_nIndexInAllNodes; // the index in "m_vaAllNodes".
	string m_sNodeName; // Gene name
	double m_dNodeWeight;
	double m_dNodeDegree; // Single-locus test value
	vector<int> m_vnAdjacentNodesIndex; // the adjacent nodes indices list. The index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	vector<int> m_vnAdjacentNodesInducedEdgesIndex; // the index in "vector<COneEdge> m_vaAllEdges;" in COneGraph class.
	vector<double> m_vdAdjcentEdgeWeight; // the edge weight between this node and one adjacent node.
	// 20130807 The following variables are for the articulation nodes.
	int m_nColor; // "0", WHITE; "1", GRAY; "2", BLACK.
	int m_nDiscoverTime;
	int m_nLowestDiscoverTime;
	int m_nParentNodeIndexInDFSTree; // the parent nodes index in DFS Tree.
	int m_nNumOfChild4RootNodeInDFSTree; // the number of child nodes for the Root Node in the DFS Tree.
	bool m_bArticulationNodeFlag; // the flag of whether or not this node is articulation node. "false", not articulation node; "true", yes.
	int  m_nIndexInThisSubnetwork; // index of node in "m_vaPPIAllNodes" in class "COneSubnetwork".	
	vector<int> m_vnIdxOfBiconnectedComponents; // the index of biconnected components.
	// 20130923William. The following is for the assigned edge weight, for approximation ratio analysis
	double m_dAssignedEdgeWeight;
public:
	COneNode(const COneNode& aOneNode)
	{
		m_nIndexInAllNodes = aOneNode.GetIndexInAllNodes();
		m_sNodeName = aOneNode.GetNodeName();
		m_dNodeWeight = aOneNode.GetNodeWeight();
		m_dNodeDegree = aOneNode.GetNodeDegree();
		int nNumOfAdjacentNodes = aOneNode.GetNumberOfAdjacentNodes();
		m_vnAdjacentNodesIndex.resize(nNumOfAdjacentNodes);
		for (int nIdx = 0; nIdx < nNumOfAdjacentNodes; nIdx++)
		{
			int nOneAdjacentNodeIndex = aOneNode.GetOneAdjacentNodeIndex(nIdx);
			m_vnAdjacentNodesIndex[nIdx] = nOneAdjacentNodeIndex;
		}
		int nNumOfAdjacentNodesInducedEdges = aOneNode.GetNumberOfAdjacentNodesInducedEdges();
		m_vnAdjacentNodesInducedEdgesIndex.resize(nNumOfAdjacentNodesInducedEdges);
		for (int nIdx = 0; nIdx < nNumOfAdjacentNodesInducedEdges; nIdx++)
		{
			int nOneAdjacentNodeInduceEdgeIndex = aOneNode.GetOneAdjacentNodeInducedEdgesIndex(nIdx);
			m_vnAdjacentNodesInducedEdgesIndex[nIdx] = nOneAdjacentNodeInduceEdgeIndex;
		}
		int nNumOfAdjacentEdgeWeight = aOneNode.GetNumberOfAdjacentEdgeWeight();
		m_vdAdjcentEdgeWeight.resize(nNumOfAdjacentEdgeWeight);
		for (int nIdx = 0; nIdx < nNumOfAdjacentEdgeWeight; nIdx++)
		{
			double dOneEdgeWeight = aOneNode.GetOneAdjacentEdgeWeight(nIdx);
			m_vdAdjcentEdgeWeight[nIdx] = dOneEdgeWeight;
		}
		m_nColor = aOneNode.GetThisNodeColor();
		m_nDiscoverTime = aOneNode.GetDiscoverTime();
		m_nLowestDiscoverTime = aOneNode.GetLowestDiscoverTime();
		m_nParentNodeIndexInDFSTree = aOneNode.GetParentNodeIndexInDFSTree();
		m_nNumOfChild4RootNodeInDFSTree = aOneNode.GetNumOfChild4RootNodeInDFSTree();
		m_bArticulationNodeFlag = aOneNode.GetArticulationNodeFlag();
		m_nIndexInThisSubnetwork = aOneNode.GetIndexInThisSubnetwork();
		int nNumOfIdxOfBiconnectedComponents = aOneNode.GetNumOfIdxOfBiconnectedComponents();
		m_vnIdxOfBiconnectedComponents.resize(nNumOfIdxOfBiconnectedComponents);
		for (int nIdx = 0; nIdx < nNumOfIdxOfBiconnectedComponents; nIdx++)
		{
			int nOneIdxOfBiconnectedComponent = aOneNode.GetOneIdxOfBiconnectedComponents(nIdx);
			m_vnIdxOfBiconnectedComponents[nIdx] = nOneIdxOfBiconnectedComponent;
		}
		m_dAssignedEdgeWeight = aOneNode.GetAssignedEdgeWeight();
	};
	COneNode& operator = (const COneNode& aOneNode)
	{
		m_nIndexInAllNodes = aOneNode.GetIndexInAllNodes();
		m_sNodeName = aOneNode.GetNodeName();
		m_dNodeWeight = aOneNode.GetNodeWeight();
		m_dNodeDegree = aOneNode.GetNodeDegree();
		int nNumOfAdjacentNodes = aOneNode.GetNumberOfAdjacentNodes();
		m_vnAdjacentNodesIndex.resize(nNumOfAdjacentNodes);
		for (int nIdx = 0; nIdx < nNumOfAdjacentNodes; nIdx++)
		{
			int nOneAdjacentNodeIndex = aOneNode.GetOneAdjacentNodeIndex(nIdx);
			m_vnAdjacentNodesIndex[nIdx] = nOneAdjacentNodeIndex;
		}
		int nNumOfAdjacentNodesInducedEdges = aOneNode.GetNumberOfAdjacentNodesInducedEdges();
		m_vnAdjacentNodesInducedEdgesIndex.resize(nNumOfAdjacentNodesInducedEdges);
		for (int nIdx = 0; nIdx < nNumOfAdjacentNodesInducedEdges; nIdx++)
		{
			int nOneAdjacentNodeInduceEdgeIndex = aOneNode.GetOneAdjacentNodeInducedEdgesIndex(nIdx);
			m_vnAdjacentNodesInducedEdgesIndex[nIdx] = nOneAdjacentNodeInduceEdgeIndex;
		}
		int nNumOfAdjacentEdgeWeight = aOneNode.GetNumberOfAdjacentEdgeWeight();
		m_vdAdjcentEdgeWeight.resize(nNumOfAdjacentEdgeWeight);
		for (int nIdx = 0; nIdx < nNumOfAdjacentEdgeWeight; nIdx++)
		{
			double dOneEdgeWeight = aOneNode.GetOneAdjacentEdgeWeight(nIdx);
			m_vdAdjcentEdgeWeight[nIdx] = dOneEdgeWeight;
		}
		m_nColor = aOneNode.GetThisNodeColor();
		m_nDiscoverTime = aOneNode.GetDiscoverTime();
		m_nLowestDiscoverTime = aOneNode.GetLowestDiscoverTime();
		m_nParentNodeIndexInDFSTree = aOneNode.GetParentNodeIndexInDFSTree();
		m_nNumOfChild4RootNodeInDFSTree = aOneNode.GetNumOfChild4RootNodeInDFSTree();
		m_bArticulationNodeFlag = aOneNode.GetArticulationNodeFlag();
		m_nIndexInThisSubnetwork = aOneNode.GetIndexInThisSubnetwork();
		int nNumOfIdxOfBiconnectedComponents = aOneNode.GetNumOfIdxOfBiconnectedComponents();
		m_vnIdxOfBiconnectedComponents.resize(nNumOfIdxOfBiconnectedComponents);
		for (int nIdx = 0; nIdx < nNumOfIdxOfBiconnectedComponents; nIdx++)
		{
			int nOneIdxOfBiconnectedComponent = aOneNode.GetOneIdxOfBiconnectedComponents(nIdx);
			m_vnIdxOfBiconnectedComponents[nIdx] = nOneIdxOfBiconnectedComponent;
		}
		m_dAssignedEdgeWeight = aOneNode.GetAssignedEdgeWeight();
		return *this;
	};
public:
	// operation on "m_nIndexInAllNodes"
	inline int GetIndexInAllNodes() const {return m_nIndexInAllNodes;};
	inline void SetIndexInAllNodes(int nIndexInAllNodes){m_nIndexInAllNodes = nIndexInAllNodes;};
	// operation on "m_sNodeName"
	inline string GetNodeName() const {return m_sNodeName;};
	inline void SetNodeName(string sNodeName){m_sNodeName = sNodeName;};
	// operation on "m_dNodeWeight"
	inline double GetNodeWeight() const {return m_dNodeWeight;};
	inline void SetNodeWeight(double dNodeWeight){m_dNodeWeight = dNodeWeight;};
	// operation on "m_dNodeDegree"
	inline double GetNodeDegree() const {return m_dNodeDegree;};
	inline void SetNodeDegree(double dNodeDegree){m_dNodeDegree = dNodeDegree;};
	// operation on "m_vnAdjacentNodesIndex"
	inline int GetNumberOfAdjacentNodes() const {return m_vnAdjacentNodesIndex.size();};
	inline int GetOneAdjacentNodeIndex(int nIdxOfAdjacentNodeIndex) const {return m_vnAdjacentNodesIndex[nIdxOfAdjacentNodeIndex];};
	inline void AddOneAdjacentNodeIndex(int nOneAdjacentNodeIndex){m_vnAdjacentNodesIndex.push_back(nOneAdjacentNodeIndex);};
	// operation on "m_vnAdjacentNodesInducedEdgesIndex"
	inline int GetNumberOfAdjacentNodesInducedEdges() const {return m_vnAdjacentNodesInducedEdgesIndex.size();};
	inline int GetOneAdjacentNodeInducedEdgesIndex(int nIdxOfAdjacentNodeInduceEdgesIndex) const {return m_vnAdjacentNodesInducedEdgesIndex[nIdxOfAdjacentNodeInduceEdgesIndex];};
	inline void AddOneAdjacentNodeInducedEdgesIndex(int nOneAdjacentNodeInduceEdgeIndex){m_vnAdjacentNodesInducedEdgesIndex.push_back(nOneAdjacentNodeInduceEdgeIndex);};
	// operation on "m_vdAdjcentEdgeWeight"
	inline int GetNumberOfAdjacentEdgeWeight() const {return m_vdAdjcentEdgeWeight.size();};
	inline double GetOneAdjacentEdgeWeight(int nIdxOfAdjacentEdgeWeight) const {return m_vdAdjcentEdgeWeight[nIdxOfAdjacentEdgeWeight];};
	inline void AddOneAdjacentEdgeWeight(double dOneAdjacentEdgeWeight){m_vdAdjcentEdgeWeight.push_back(dOneAdjacentEdgeWeight);};
public:
	// operation on "m_nColor"
	inline int GetThisNodeColor() const {return m_nColor;};
	inline void SetThisNodeColor(int nColor){m_nColor = nColor;};
	// operation on "m_nDiscoverTime"
	inline int GetDiscoverTime() const {return m_nDiscoverTime;};
	inline void SetDiscoverTime(int nDiscoverTime){m_nDiscoverTime = nDiscoverTime;};
	// operation on "m_nLowestDiscoverTime"
	inline int GetLowestDiscoverTime() const {return m_nLowestDiscoverTime;};
	inline void SetLowestDiscoverTime(int nLowestDiscoverTime){m_nLowestDiscoverTime = nLowestDiscoverTime;};
	// operation on "m_nParentNodeIndexInDFSTree"
	inline int GetParentNodeIndexInDFSTree() const {return m_nParentNodeIndexInDFSTree;};
	inline void SetParentNodeIndexInDFSTree(int nParentNodeIndexInDFSTree){m_nParentNodeIndexInDFSTree = nParentNodeIndexInDFSTree;};
	// operation on "m_nNumOfChild4RootNodeInDFSTree"
	inline int GetNumOfChild4RootNodeInDFSTree() const {return m_nNumOfChild4RootNodeInDFSTree;};
	inline void SetNumOfChild4RootNodeInDFSTree(int nNumOfChild4RootNodeInDFSTree){m_nNumOfChild4RootNodeInDFSTree = nNumOfChild4RootNodeInDFSTree;};
	inline void IncreaseNumOfChild4RootNodeInDFSTree() {m_nNumOfChild4RootNodeInDFSTree++;};
	// operation on "m_bArticulationNodeFlag"
	inline bool GetArticulationNodeFlag() const {return m_bArticulationNodeFlag;};
	inline void SetArticulationNodeFlag(bool bArticulationNodeFlag){m_bArticulationNodeFlag = bArticulationNodeFlag;};
	// operation on "m_nIndexInThisSubnetwork"
	inline int  GetIndexInThisSubnetwork() const {return m_nIndexInThisSubnetwork;};
	inline void SetIndexInThisSubnetwork(int nIndexInThisSubnetwork){m_nIndexInThisSubnetwork = nIndexInThisSubnetwork;};
	// operation on "m_vnIdxOfBiconnectedComponents"
	inline int  GetNumOfIdxOfBiconnectedComponents() const {return m_vnIdxOfBiconnectedComponents.size();};
	inline int  GetOneIdxOfBiconnectedComponents(int nIdx) const {return m_vnIdxOfBiconnectedComponents[nIdx];};
	void AddOneIdxOfBiconnectedComponents(int nIdxOfBiconnectedComponents);
	// operation on "m_dAssignedEdgeWeight"
	inline double GetAssignedEdgeWeight() const {return m_dAssignedEdgeWeight;};
	inline void   SetAssignedEdgeWeight(double dAssignedEdgeWeight){m_dAssignedEdgeWeight = dAssignedEdgeWeight;};
public:
	void ArticulationNode(int & nTime, vector<COneNode> & vaAllNodes, vector<bool> & vbPPIAllNodesRemainFlag, stack<COneEdge> & vaDFSEdgeStack, int & nIdxOfBiconnectedComponents);
};
