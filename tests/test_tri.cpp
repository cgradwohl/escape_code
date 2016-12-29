#include "Escape/GraphIO.h"
#include "Escape/EdgeHash.h"
#include "Escape/Digraph.h"
#include "Escape/Triadic.h"
#include "Escape/Graph.h"

using namespace Escape;

int main(int argc, char *argv[])
{


  // note: Graph g becomes a COO representaion i.e. struct Graph{}
  Graph g;


  // loadGraph()
  // loadGraph(const char *path, Graph& graph, int undirected, IOFormat fmt);
  // Loads a graph from one of the file formats we support. The returned
  // graph should be freed with delGraph when you are done with it.
  // note: this if statement is a nice way to test if loadgraph() worked properly.
  if (loadGraph(argv[1], g, 1, IOFormat::escape))
    exit(1);

  printf("Loaded graph\n");


  // makeCSG()
  // Make a CSR graph( i.e. struct CGraph{}) from a COO graph (i.e. struct Graph).
  // If inPlace is true, the input graph is destroyed, i.e. you should not call delGraph.
  CGraph cg = makeCSR(g);


  // This sorts each individual adjacency list by vertex ID. This is useful for
  // doing a binary search, or for merging neighbor lists to find common neighbors.
  cg.sortById();
  printf("Converted to CSR\n");


  // Construct DAG based on degree ordering
  // This is the CDAG for the DAG where each edge points from lower degree endpoint to higher degree endpoint.
  // The outlist in CDAG is guaranteed to be sorted by degrees. This means that the neighbors
  // of every vertex in the outlist are sorted by their degrees in g. This is quite useful in
  // further processing.
  CDAG dag = degreeOrdered(&cg);
  printf("Created DAG\n");


  // Structure that stores triangle information of graph
  /*struct TriangleInfo
  {
      Count total; // total number of triangles
      Count *perVertex;  // array storing number of triangles for each vertex
      Count *perEdge;   // arry storing number of triangles for each edge

  };*/
  TriangleInfo info;


  // I dont think I will be using this, This is an old version of the triangle enumerator
  /*if (argc > 2)
  {
      printf("Old version\n");
      info = wedgeEnumerator(&(dag.outlist));
      printf("Enumerated wedges\n");
      printf("Triangles = %ld\n",info.total);
  }*/


  printf("New version\n");
  printf("Relabeling graph\n");


  // renameByDegreeOrder()
  // This outputs a new, isomorphic CGraph where vertex labels are in increasing order
  // corresponding to degree.
  // Thus, (after the relabeling), for all i < j, the degree of i is less than that of j.
  CGraph cg_new = cg.renameByDegreeOrder();


  // sortById()
  // This sorts each individual adjacency list by vertex ID. This is useful for
  // doing a binary search, or for merging neighbor lists to find common neighbors.
  cg_new.sortById();


  // degreeOrdered()
  // Construct DAG based on degree ordering
  // This is the CDAG for the DAG where each edge points from lower degree endpoint to higher degree endpoint.
  // The outlist in CDAG is guaranteed to be sorted by degrees. This means that the neighbors
  // of every vertex in the outlist are sorted by their degrees in g. This is quite useful in
  // further processing.
  dag = degreeOrdered(&cg_new);


  // betterWedgeEnumerator()
  // This wedge enumeration algorithm produces all triangles, and is more
  // efficient than the previous version. We use binary search to find edges, and also pass
  // the original graph to cut down on queries.
  // Input: a pointer gout to a CGraph labeled according to degree
  // Output: a TriangleInfo for g. The ordering of edges in perEdge (of TriangleInfo) is that same as g.
  // enumerates the outdag field of the CGRAPH dag, where dag is degreeOrdered()
  info = betterWedgeEnumerator(&(dag.outlist));


  printf("Enumerated wedges\n");
  printf("Triangles = %ld\n",info.total);


  printf("dag has %ld edges\n", dag.inlist.nEdges);


}
