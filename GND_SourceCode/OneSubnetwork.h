#pragma once
#include "MyHeader.h"
#include "OneNode.h"
#include "OneEdge.h"

typedef struct
{
	int	   m_nRemainNodeIndex; // the index of nodes, which is still in the remaining subnetwork.
	double m_dRemainNodeQueryBiasedDegree; // the query biased node degree
	bool   m_bCriticalNodeFlag; // "critical node" means the nodes, whose deletion will disconnect the subnetwork.
} stRemainNode;

// This class is created to store the subnetwork, formed by the nodes and induced edges within 3-hops of one pathway
class COneSimpleSubnetwork
{
public:
	COneSimpleSubnetwork(void);
	~COneSimpleSubnetwork(void);
public:
	// the variables for the nodes and edges, of the original network.
	vector<COneNode> m_vaAllNodes; // the nodes will change (the adjacent nodes and edges list will change).
	vector<COneEdge> m_vaPPIAllEdges; // the edges will change according to the deleting of nodes. This variable and the below one always keep the same number of elements.
	vector<COneEdge> m_vaGGIAllEdges;
public:
	COneSimpleSubnetwork(const COneSimpleSubnetwork& aOneSubnetwork)
	{
		int nNumOfNodes = aOneSubnetwork.GetNumOfNodes();
		m_vaAllNodes.resize(nNumOfNodes);
		for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
		{
			m_vaAllNodes[nIdx] = aOneSubnetwork.GetOneNode(nIdx);
		}
		int nNumOfPPIEdges = aOneSubnetwork.GetNumOfPPIEdges();
		m_vaPPIAllEdges.resize(nNumOfPPIEdges);
		for (int nIdx = 0; nIdx < nNumOfPPIEdges; nIdx++)
		{
			m_vaPPIAllEdges[nIdx] = aOneSubnetwork.GetOnePPIEdge(nIdx);
		}
		int nNumOfGGIEdges = aOneSubnetwork.GetNumOfGGIEdges();
		m_vaGGIAllEdges.resize(nNumOfGGIEdges);
		for (int nIdx = 0; nIdx < nNumOfGGIEdges; nIdx++)
		{
			m_vaGGIAllEdges[nIdx] = aOneSubnetwork.GetOneGGIEdge(nIdx);
		}
	};
	COneSimpleSubnetwork& operator = (const COneSimpleSubnetwork& aOneSubnetwork)
	{
		int nNumOfNodes = aOneSubnetwork.GetNumOfNodes();
		m_vaAllNodes.resize(nNumOfNodes);
		for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
		{
			m_vaAllNodes[nIdx] = aOneSubnetwork.GetOneNode(nIdx);
		}
		int nNumOfPPIEdges = aOneSubnetwork.GetNumOfPPIEdges();
		m_vaPPIAllEdges.resize(nNumOfPPIEdges);
		for (int nIdx = 0; nIdx < nNumOfPPIEdges; nIdx++)
		{
			m_vaPPIAllEdges[nIdx] = aOneSubnetwork.GetOnePPIEdge(nIdx);
		}
		int nNumOfGGIEdges = aOneSubnetwork.GetNumOfGGIEdges();
		m_vaGGIAllEdges.resize(nNumOfGGIEdges);
		for (int nIdx = 0; nIdx < nNumOfGGIEdges; nIdx++)
		{
			m_vaGGIAllEdges[nIdx] = aOneSubnetwork.GetOneGGIEdge(nIdx);
		}
		return *this;
	};
public:
	inline int GetNumOfNodes() const {return m_vaAllNodes.size();};
	inline COneNode GetOneNode(int nIdxOfNode) const {return m_vaAllNodes[nIdxOfNode];};
	inline COneNode& GetOneNodeRefer(int nIdxOfNode){return m_vaAllNodes[nIdxOfNode];};
	inline void AddOneNode(COneNode aOneNode){m_vaAllNodes.push_back(aOneNode);};
	inline int GetNumOfPPIEdges() const {return m_vaPPIAllEdges.size();};
	inline COneEdge GetOnePPIEdge(int nIdxOfEdge) const {return m_vaPPIAllEdges[nIdxOfEdge];};
	inline COneEdge& GetOnePPIEdgeRefer(int nIdxOfEdge){return m_vaPPIAllEdges[nIdxOfEdge];};
	inline void AddOnePPIEdge(COneEdge aOneEdge){m_vaPPIAllEdges.push_back(aOneEdge);};
	inline int GetNumOfGGIEdges() const {return m_vaGGIAllEdges.size();};
	inline COneEdge GetOneGGIEdge(int nIdxOfEdge) const {return m_vaGGIAllEdges[nIdxOfEdge];};
	inline COneEdge& GetOneGGIEdgeRefer(int nIdxOfEdge){return m_vaGGIAllEdges[nIdxOfEdge];};
	inline void AddOneGGIEdge(COneEdge aOneEdge){m_vaGGIAllEdges.push_back(aOneEdge);};
public:
	void FindTheNodeNewIndex(int nIndexOfNode1, int & nIndexOfNode1New);
};

// This class is created to store the subnetwork, formed by the nodes and induced edges within 3-hops of one pathway
class COneSubnetwork
{
public:
	COneSubnetwork(void);
	~COneSubnetwork(void);
public:
	// the variables for the nodes and edges, of the original network.
	vector<COneNode> m_vaPPIAllNodes; // the nodes will change (the adjacent nodes and edges list will change).
	vector<bool>	 m_vbPPIAllNodesRemainFlag;
	vector<COneEdge> m_vaPPIAllEdges; // the edges will change according to the deleting of nodes. This variable and the below one always keep the same number of elements.
	vector<bool>	 m_vbPPIAllEdgesRemainFlag; // "true" this edge remain the subnetwork; "false", this edge is deleted when we delete one node with minimum weighted degree.
	vector<COneNode> m_vaGGIAllNodes;
	vector<bool>	 m_vbGGIAllNodesRemainFlag;
	vector<COneEdge> m_vaGGIAllEdges;
	vector<bool>	 m_vbGGIAllEdgesRemainFlag;
	// the variable to store the remaining subnetwork's nodes and its weighted degree.
	vector<stRemainNode> m_vstRemainNodesIndexAndWeighDegree; // the information of the remain nodes.
	// the variables to track the nodes deleting process.
	vector<int>		 m_vnIndexOfDeletedNode; // the number of deleted nodes.
	vector<double>	 m_vdRemainNetworkDensity; // the density of the remaining subnetwork.
	vector<int>		 m_vnNumOfCriticalNodesEncounteredWhenDelete; // when we try to delete nodes with minimum weighted degree, if we encounter the candidate node, we need to skip it, and test for the next min node.

	stack<int>		 m_knDFSStack; // the stack used for non-recursive DFS.
	vector<int>		 m_vnNumOfBiconnectedComponents; // the number of biconnected components at each iteration.
	vector<int>		 m_vnNumOfArticulationNodes; // the number of articulation nodes at each iteration.
	vector<int>		 m_vnNumOfIndptNonArticLowDegreeNodesAtEachIter; // the number of nodes deleted at each iteration.

	vector<int>		 m_vnIdxOfIndependentNonArticLowDegreeNodes; // the index of independent non-articulation low degree nodes, which are going to be deleted at one iteration.
	vector<int>		 m_vnIdxOfExistingBlocksIndex; // the unique index of blocks, which are in the above variable "m_vnIdxOfIndependentNonArticLowDegreeNodes".
	vector<int>		 m_vnIndexOfDeletedNodeAccumulated; // 

	double			 m_dMaximumAssignedEdgeWeight; // the maximum assigned edge weight during the node deletion process.
	double			 m_dDensityOfDensestSubgraph; // the density of the densest subgraph.
	double			 m_dApproximationRatio; // the approximation ratio.
public:
	inline int GetNumOfPPINodes() const {return m_vaPPIAllNodes.size();};
	inline COneNode GetOnePPINode(int nIdxOfNode) const {return m_vaPPIAllNodes[nIdxOfNode];};
	inline void AddOnePPINode(COneNode aOneNode){m_vaPPIAllNodes.push_back(aOneNode);};

	inline int GetNumOfPPIEdges() const {return m_vaPPIAllEdges.size();};
	inline COneEdge GetOnePPIEdge(int nIdxOfEdge) const {return m_vaPPIAllEdges[nIdxOfEdge];};
	inline void AddOnePPIEdge(COneEdge aOneEdge){m_vaPPIAllEdges.push_back(aOneEdge);};
	inline int GetNumOfPPIRemainEdgeFlag() const {return m_vbPPIAllEdgesRemainFlag.size();};
	inline bool GetOnePPIRemainEdgeFlag(int nIdx) const {return m_vbPPIAllEdgesRemainFlag[nIdx];};

	inline int GetNumOfGGINodes() const {return m_vaGGIAllNodes.size();};
	inline COneNode GetOneGGINode(int nIdxOfNode) const {return m_vaGGIAllNodes[nIdxOfNode];};
	inline void AddOneGGINode(COneNode aOneNode){m_vaGGIAllNodes.push_back(aOneNode);};

	inline int GetNumOfGGIEdges() const {return m_vaGGIAllEdges.size();};
	inline COneEdge GetOneGGIEdge(int nIdxOfEdge) const {return m_vaGGIAllEdges[nIdxOfEdge];};
	inline void AddOneGGIEdge(COneEdge aOneEdge){m_vaGGIAllEdges.push_back(aOneEdge);};

	inline int GetNumOfGGIRemainEdgeFlag() const {return m_vbGGIAllEdgesRemainFlag.size();};
	inline bool GetOneGGIRemainEdgeFlag(int nIdx) const {return m_vbGGIAllEdgesRemainFlag[nIdx];};

	inline int GetNumOfRemainNodes() const {return m_vstRemainNodesIndexAndWeighDegree.size();};
	inline stRemainNode GetOneRemainNode(int nIdx) const {return m_vstRemainNodesIndexAndWeighDegree[nIdx];};

	inline int GetNumOfIndexOfDeletedNode() const {return m_vnIndexOfDeletedNode.size();};
	inline int GetOneIndexOfDeletedNode(int nIdx) const {return m_vnIndexOfDeletedNode[nIdx];};
	inline void AddOneIndexOfDeletedNode(int nIdxOfDeletedNode) {m_vnIndexOfDeletedNode.push_back(nIdxOfDeletedNode);};

	inline int GetNumOfRemainNetworkDensity() const {return m_vdRemainNetworkDensity.size();};
	inline double GetOneRemainNetworkDensity(int nIdx) const {return m_vdRemainNetworkDensity[nIdx];};

	inline int GetAllNumOfCriticalNodesEncounteredWhenDelete() const {return m_vnNumOfCriticalNodesEncounteredWhenDelete.size();};
	inline int GetOneNumOfCriticalNodesEncounteredWhenDelete(int nIdx) const {return m_vnNumOfCriticalNodesEncounteredWhenDelete[nIdx];};
	inline void AddOneNumOfCriticalNodesEncounteredWhenDelete(int nIdxOfCriticalNodes){m_vnNumOfCriticalNodesEncounteredWhenDelete.push_back(nIdxOfCriticalNodes);};

	inline int GetNumOfNumOfBiconnectedComponents() const {return m_vnNumOfBiconnectedComponents.size();};
	inline int GetOneNumOfBiconnectedComponents(int nIdx) const {return m_vnNumOfBiconnectedComponents[nIdx];};
	inline void AddOneNumOfBiconnectedComponents(int nNumOfBiconnectedComponents){m_vnNumOfBiconnectedComponents.push_back(nNumOfBiconnectedComponents);};

	inline int GetNumOfNumOfArticulationNodes() const {return m_vnNumOfArticulationNodes.size();};
	inline int GetOneNumOfArticulationNodes(int nIdx) const {return m_vnNumOfArticulationNodes[nIdx];};
	inline void AddOneNumOfArticulationNodes(int nNumOfArticulationNodes){m_vnNumOfArticulationNodes.push_back(nNumOfArticulationNodes);};

	inline int GetNumOfIdxOfIndependentNonArticLowDegreeNodes() const {return m_vnIdxOfIndependentNonArticLowDegreeNodes.size();};
	inline int GetOneIdxOfIndependentNonArticLowDegreeNodes(int nIdx) const {return m_vnIdxOfIndependentNonArticLowDegreeNodes[nIdx];};
	inline void AddOneIdxOfIndependentNonArticLowDegreeNodes(int nIdxOfIndependentNonArticLowDegreeNodes){m_vnIdxOfIndependentNonArticLowDegreeNodes.push_back(nIdxOfIndependentNonArticLowDegreeNodes);};

	inline double GetMaximumAssignedEdgeWeight() const {return m_dMaximumAssignedEdgeWeight;};
	inline double GetDensityOfDensestSubgraph() const {return m_dDensityOfDensestSubgraph;};
	inline double GetApproximationRatio() const {return m_dApproximationRatio;};
public:
	void ClearAllNodesAndEdges(); // clear all the nodes and edges in this subnetwork.
	void InitializeRemainNodesIndexAndRemainEdgeFlag(); // Initialize "m_vnRemainNodesIndex" and "m_vbAllEdgesRemainFlag".
	void InitializeTrackInfor4DeleteHistory(); // "m_vnNumOfDeletedNodes", "m_vdRemainNetworkDensity", "m_vnNumOfCandidateNodesToDelete" will be initialized.
	double CalculateTheDensityOfTheSubnetwork(); // calculate the density of the subnetwork.
	void CalculateNodesWeightedDegree(); // calculate the weighted degree of each node.
	void ReCalculateArticulationNodes(); // The main function to calculate the articulation nodes.
	bool SortRemainNodesAndSelectIndependentNonArticLowDegreeNodes(double dNodeDegreeThreshold, vector<int> & vnSeedNodesList); // select low degree non-articulation nodes
	void DeleteIndepdtLowDegreeNodesAndUpdateRelatedEdges(double dNodeDegreeThreshold); // delete some low degree non-articulation nodes
	void ExtractTheDensestSubgraph(COneSimpleSubnetwork& aDensestSubnetwork, bool bDoWeConsiderDCSKProblem, int nNumOfNodesInSubgraph); // extract the DCS.
	void ApproximationRatioAnalysis(); // compute approximation ratio by Lemma 6.
	bool CheckWhetherThisNodeIsSeed(int nIndexOfOneNode, vector<int> & vnSeedNodesList);
};
