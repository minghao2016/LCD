#pragma once
#include "MyHeader.h"

class COneNode
{
public:
	COneNode(void);
	~COneNode(void);
public:
	string m_sNodeName;
	double m_dNodeWeight;
	int    m_nNodeIndex; // The index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	vector<int> m_vnAdjNodeIndexList;
	vector<int> m_vnAdjEdgeIndexList;
	int    m_nNodeRegion;
	bool   m_bIsThisNodeInAdjNodesList; // true: this node is in already put into the adj nodes list; false: no, not yet.
public:
	COneNode(const COneNode& aOneNode)
	{
		m_sNodeName = aOneNode.GetNodeName();
		m_dNodeWeight = aOneNode.GetNodeWeight();
		m_nNodeIndex = aOneNode.GetNodeIndex();
		int nNumOfAdjNodes = aOneNode.GetNumOfAdjNodes();
		m_vnAdjNodeIndexList.resize(nNumOfAdjNodes);
		for (int nIdx = 0; nIdx < nNumOfAdjNodes; nIdx++)
		{
			m_vnAdjNodeIndexList[nIdx] = aOneNode.GetOneOfAdjNodeIndex(nIdx);
		}
		int nNumOfAdjEdges = aOneNode.GetNumOfAdjEdges();
		m_vnAdjEdgeIndexList.resize(nNumOfAdjEdges);
		for (int nIdx = 0; nIdx < nNumOfAdjEdges; nIdx++)
		{
			m_vnAdjEdgeIndexList[nIdx] = aOneNode.GetOneOfAdjEdgeIndex(nIdx);
		}
		m_nNodeRegion = aOneNode.GetNodeRegion();
		m_bIsThisNodeInAdjNodesList = aOneNode.GetIsThisNodeInAdjNodesList();
	};
	COneNode& operator = (const COneNode& aOneNode)
	{
		m_sNodeName = aOneNode.GetNodeName();
		m_dNodeWeight = aOneNode.GetNodeWeight();
		m_nNodeIndex = aOneNode.GetNodeIndex();
		int nNumOfAdjNodes = aOneNode.GetNumOfAdjNodes();
		m_vnAdjNodeIndexList.resize(nNumOfAdjNodes);
		for (int nIdx = 0; nIdx < nNumOfAdjNodes; nIdx++)
		{
			m_vnAdjNodeIndexList[nIdx] = aOneNode.GetOneOfAdjNodeIndex(nIdx);
		}
		int nNumOfAdjEdges = aOneNode.GetNumOfAdjEdges();
		m_vnAdjEdgeIndexList.resize(nNumOfAdjEdges);
		for (int nIdx = 0; nIdx < nNumOfAdjEdges; nIdx++)
		{
			m_vnAdjEdgeIndexList[nIdx] = aOneNode.GetOneOfAdjEdgeIndex(nIdx);
		}
		m_nNodeRegion = aOneNode.GetNodeRegion();
		m_bIsThisNodeInAdjNodesList = aOneNode.GetIsThisNodeInAdjNodesList();
		return *this;
	};
public:
	// operation on "m_sNodeName"
	inline string GetNodeName() const {return m_sNodeName;};
	inline void SetNodeName(string sNodeName){m_sNodeName = sNodeName;};
	// operation on "m_dNodeWeight"
	inline double GetNodeWeight() const {return m_dNodeWeight;};
	inline void SetNodeWeight(double dNodeWeight){m_dNodeWeight = dNodeWeight;};
	// operation on "m_nNodeIndex"
	inline int  GetNodeIndex() const {return m_nNodeIndex;};
	inline void SetNodeIndex(int nNodeIndex){m_nNodeIndex = nNodeIndex;};
	// operation on "m_vnAdjNodeIndexList"
	inline int  GetNumOfAdjNodes() const {return m_vnAdjNodeIndexList.size();};
	inline int  GetOneOfAdjNodeIndex(int nIdx) const {return m_vnAdjNodeIndexList[nIdx];};
	inline void AddOneOfAdjNodeIndex(int nIdxOfNode) {m_vnAdjNodeIndexList.push_back(nIdxOfNode);};
	// operation on "m_vnAdjEdgeIndexList"
	inline int  GetNumOfAdjEdges() const {return m_vnAdjEdgeIndexList.size();};
	inline int  GetOneOfAdjEdgeIndex(int nIdx) const {return m_vnAdjEdgeIndexList[nIdx];};
	inline void AddOneOfAdjEdgeIndex(int nIdxOfEdge) {m_vnAdjEdgeIndexList.push_back(nIdxOfEdge);};
public:
	// operation on "m_bIsThisNodeInAdjNodesList"
	inline bool   GetIsThisNodeInAdjNodesList()const{return m_bIsThisNodeInAdjNodesList;};
	inline void   SetIsThisNodeInAdjNodesList(bool bIsThisNodeInAdjNodesList){m_bIsThisNodeInAdjNodesList = bIsThisNodeInAdjNodesList;};
	// operation on "m_nNodeRegion"
	inline int  GetNodeRegion() const {return m_nNodeRegion;};
	inline void SetNodeRegion(int nNodeRegion){m_nNodeRegion = nNodeRegion;};
};
