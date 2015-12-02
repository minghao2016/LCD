#include "OneEdge.h"

COneEdge::COneEdge(void)
{
	m_nIndexOfNode1 = 0; // the First  Node's index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	m_nIndexOfNode2 = 0; // the Second Node's index in "vector<COneNode> m_vaAllNodes;" in COneGraph class.
	m_dEdgeWeight = 0.0; // the maximum two-locus test value among the SNPs Pairs between two genes.
}

COneEdge::~COneEdge(void)
{
}
