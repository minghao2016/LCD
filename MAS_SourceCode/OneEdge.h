#pragma once
#include "MyHeader.h"

class COneEdge
{
public:
	COneEdge(void);
	~COneEdge(void);
public:
	int m_nIndexOfNode1; // the First  Node's index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	int m_nIndexOfNode2; // the Second Node's index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	double m_dEdgeWeight; // the maximum two-locus test value among the SNPs Pairs between two genes.
public:
	COneEdge(const COneEdge& aOneEdge)
	{
		m_nIndexOfNode1 = aOneEdge.GetIndexOfNode1();
		m_nIndexOfNode2 = aOneEdge.GetIndexOfNode2();
		m_dEdgeWeight = aOneEdge.GetEdgeWeight();
	};
	COneEdge& operator = (const COneEdge& aOneEdge)
	{
		m_nIndexOfNode1 = aOneEdge.GetIndexOfNode1();
		m_nIndexOfNode2 = aOneEdge.GetIndexOfNode2();
		m_dEdgeWeight = aOneEdge.GetEdgeWeight();
		return *this;
	};
public:
	inline int GetIndexOfNode1() const {return m_nIndexOfNode1;};
	inline void SetIndexOfNode1(int nIndexOfNode1){m_nIndexOfNode1 = nIndexOfNode1;};
	inline int GetIndexOfNode2() const {return m_nIndexOfNode2;};
	inline void SetIndexOfNode2(int nIndexOfNode2){m_nIndexOfNode2 = nIndexOfNode2;};
	inline double GetEdgeWeight() const {return m_dEdgeWeight;};
	inline void SetEdgeWeight(double dEdgeWeight){m_dEdgeWeight = dEdgeWeight;};
};
