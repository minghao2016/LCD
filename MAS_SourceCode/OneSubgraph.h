#pragma once
#include "MyHeader.h"
#include "OneNode.h"
#include "OneEdge.h"
#include "OneNodeInSubgraph.h"

class COneSubgraph
{
public:
	COneSubgraph(void);
	~COneSubgraph(void);
public:
	// the variables for the nodes and edges, of the original network.
	vector<COneNodeInSubgraph> m_vaAllNodesInSubgraph; // the nodes will change (the adjacent nodes and edges list will change).
	vector<COneEdge> m_vaAllEdgesInSubgraph; // the edges will change according to the deleting of nodes. This variable and the below one always keep the same number of elements.
	double m_dSubgraphDensity;
public:
	COneSubgraph(const COneSubgraph& aOneSubgraph)
	{
		int nNumOfNodes = aOneSubgraph.GetNumOfNodes();
		m_vaAllNodesInSubgraph.resize(nNumOfNodes);
		for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
		{
			m_vaAllNodesInSubgraph[nIdx] = aOneSubgraph.GetOneNode(nIdx);
		}
		int nNumOfEdges = aOneSubgraph.GetNumOfEdges();
		m_vaAllEdgesInSubgraph.resize(nNumOfEdges);
		for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
		{
			m_vaAllEdgesInSubgraph[nIdx] = aOneSubgraph.GetOneEdge(nIdx);
		}
		m_dSubgraphDensity = aOneSubgraph.GetSubgraphDensity();
	};
	COneSubgraph& operator = (const COneSubgraph& aOneSubgraph)
	{
		int nNumOfNodes = aOneSubgraph.GetNumOfNodes();
		m_vaAllNodesInSubgraph.resize(nNumOfNodes);
		for (int nIdx = 0; nIdx < nNumOfNodes; nIdx++)
		{
			m_vaAllNodesInSubgraph[nIdx] = aOneSubgraph.GetOneNode(nIdx);
		}
		int nNumOfEdges = aOneSubgraph.GetNumOfEdges();
		m_vaAllEdgesInSubgraph.resize(nNumOfEdges);
		for (int nIdx = 0; nIdx < nNumOfEdges; nIdx++)
		{
			m_vaAllEdgesInSubgraph[nIdx] = aOneSubgraph.GetOneEdge(nIdx);
		}
		m_dSubgraphDensity = aOneSubgraph.GetSubgraphDensity();
		return *this;
	};
public:
	inline int  GetNumOfNodes() const {return m_vaAllNodesInSubgraph.size();};
	inline COneNodeInSubgraph  GetOneNode(int nIdxOfNode) const {return m_vaAllNodesInSubgraph[nIdxOfNode];};
	inline COneNodeInSubgraph& GetOneNodeRefer(int nIdxOfNode){return m_vaAllNodesInSubgraph[nIdxOfNode];};
	inline void AddOneNode(COneNodeInSubgraph aOneNode){m_vaAllNodesInSubgraph.push_back(aOneNode);};
	inline int  GetNumOfEdges() const {return m_vaAllEdgesInSubgraph.size();};
	inline COneEdge  GetOneEdge(int nIdxOfEdge) const {return m_vaAllEdgesInSubgraph[nIdxOfEdge];};
	inline COneEdge& GetOneEdgeRefer(int nIdxOfEdge){return m_vaAllEdgesInSubgraph[nIdxOfEdge];};
	inline void AddOneEdge(COneEdge aOneEdge){m_vaAllEdgesInSubgraph.push_back(aOneEdge);};
	void AddOneNewEdgeInSubgraph(int nIdxOfNode1InSubgraph, int nIdxOfNode2InSubgraph, COneEdge& aOneEdgeReferInOriWholeGraph);
	inline double GetSubgraphDensity() const {return m_dSubgraphDensity;};
public:
	void FindTheNodeNewIndex(int nIndexOfNode1, int & nIndexOfNode1New);
	void TryToAddOneNodeToTheSubgraph(int nOneNewNodeIndex, vector<COneNode>& vaAllNodesInOriWholeGraph);
	bool FindWhetherThisNodeExistInSubgraph(int nOneAdjNodeIndex, int & nIndexOfAdjNodeInSubgraph);
public:
	void ComputeSubgraphDensity();
	void OutputTheSubgraph(string sOutputFileNameNodes, string sOutputFileNameEdges);
	void ClearAllMemberVariables();
};
