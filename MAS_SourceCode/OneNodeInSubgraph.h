#pragma once
#include "MyHeader.h"
#include "OneNode.h"

class COneNodeInSubgraph : public COneNode{
public:
	COneNodeInSubgraph(void);
	~COneNodeInSubgraph(void);
public:
	int m_nIndexOfNodeInSubgraph;
public:
	COneNodeInSubgraph(const COneNodeInSubgraph& aOneNodeInSubgraph) : COneNode(aOneNodeInSubgraph)
	{
		m_nIndexOfNodeInSubgraph = aOneNodeInSubgraph.GetIndexOfNodeInSubgraph();
	};
	COneNodeInSubgraph& operator = (const COneNodeInSubgraph& aOneNodeInSubgraph)
	{
		if(this == &aOneNodeInSubgraph){
			return *this; // to avoid self-assignment
		}
		// Call the assignment operation for base class.
		COneNode::operator=(aOneNodeInSubgraph);
		m_nIndexOfNodeInSubgraph = aOneNodeInSubgraph.GetIndexOfNodeInSubgraph();
		return *this;
	};
public:
	inline int GetIndexOfNodeInSubgraph() const {return m_nIndexOfNodeInSubgraph;};
	inline void SetIndexOfNodeInSubgraph(int nIndexOfNodeInSubgraph) {m_nIndexOfNodeInSubgraph = nIndexOfNodeInSubgraph;};
};
