#include "MyHeader.h"
#include "OneGraph.h"

int main(int argc, char * argv[]){
	// 1. The variables to store the input and output file names or folders.
	string sInputFileNameNodes; // Input file name for nodes of the network.
	string sInputFileNameNodeWeights; // Input file name for node weights.
	string sInputFileNameEdges; // Input file name for edges of the network.
	string sInputFileNameSeeds; // Input file name for the seed nodes.
	string sOutputFolder; // The folder to output the results.
	int    nNumOfIterK = 100; // 0 - 2 . the degree threshold in the greedy node deletion algorithm.

	// 2. Parsing the input parameters.
	int nIdx = 1; // index for parsing the input parameters.
	while (nIdx < argc)
	{
		string sToken1 = argv[nIdx];
		string sToken2 = argv[nIdx + 1];
		nIdx = nIdx + 2;
		if (sToken1.compare("-n") == 0){ // Input file name for nodes of the graph.
			sInputFileNameNodes = sToken2;
		} else if (sToken1.compare("-w") == 0){ // Input file name for node weights.
			sInputFileNameNodeWeights = sToken2;
		} else if (sToken1.compare("-e") == 0){ // Input file name for edges of the graph.
			sInputFileNameEdges = sToken2;
		} else if (sToken1.compare("-s") == 0){ // Input file name for seed nodes.
			sInputFileNameSeeds = sToken2;
		} else if (sToken1.compare("-o") == 0){ // Output file path
			sOutputFolder = sToken2;
		} else if (sToken1.compare("-k") == 0){ // parameter K
			nNumOfIterK = atoi(sToken2.c_str());
		}
	}

	// toy dataset in Fig 4
/*	sInputFileNameNodes = "../ToyGraph/01Nodes.txt"; // GeneName SingleLocusTestValue
	sInputFileNameNodeWeights = "../ToyGraph/03NodeWeights.txt"; // nIndexOfGene1 nIndexOfGene2 dTransProb dEdgeWeight
	sInputFileNameEdges = "../ToyGraph/02Edges.txt"; // nIndexOfGene1 nIndexOfGene2 dTwoLocusTestValue
	sInputFileNameSeeds = "../ToyGraph/04Seeds.txt";
	sOutputFolder  = "../ToyGraph/Output/";	 */

	// 3. The main variable: an instance of the COneGraph class.
	COneGraph aOneGraph;
	
	// 4. Set the input parameters and read the input files.
	aOneGraph.ReadNodesFile(sInputFileNameNodes);
	aOneGraph.ReadNodeWeightsFile(sInputFileNameNodeWeights);
	aOneGraph.ReadEdgesFile(sInputFileNameEdges);
	aOneGraph.ReadSeedNodeFile(sInputFileNameSeeds);

	// 5. Pre-processing the graph.
	aOneGraph.SetUpAdjacentNodesAndInducedEdgeForEachNode();
	aOneGraph.SetOutputFileFolder(sOutputFolder);
	aOneGraph.SetNumOfIterK(nNumOfIterK);

	// 6. The maximum adjacency search algorithm begins.
	aOneGraph.MaximumAdjacencySearch();
	aOneGraph.ExtractAndStoreTheSubgraph();
	aOneGraph.OutputTheLocalCommunity();

	return 0;
}
