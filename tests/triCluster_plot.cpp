//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// triCluster_plot.cpp
// Christopher Gradwohl
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Escape/GraphIO.h"
#include "Escape/EdgeHash.h"
#include "Escape/Digraph.h"
#include "Escape/Triadic.h"
#include "Escape/Graph.h"
#include <inttypes.h>
#include <cstdlib>
#include<iostream>
using namespace Escape;


double getClusterCo(int v, CGraph cGraph, TriangleInfo triGraph) {

  int degree   = (cGraph.offsets[v+1] - cGraph.offsets[v]);
  double top   = (2*(triGraph.perVertex[v]));
  double bott  = (degree*(degree - 1));
  double ratio = (top)/(bott);
  return ratio;

}

int main(int argc, char *argv[]) {


    //________________________________________
    // Graph Processing
    //----------------------------------------
    Graph g;
    if (loadGraph(argv[1], g, 1, IOFormat::escape)) exit(1);
    CGraph cg                = makeCSR(g);
    cg.sortById();
    CGraph cg_ordered        = cg.renameByDegreeOrder();
    cg_ordered.sortById();
    CDAG cg_dag              = degreeOrdered(&cg_ordered);
    TriangleInfo cg_triangle = betterWedgeEnumerator(&(cg_dag.outlist));


    //________________________________________
    // getClusterCo() for all verticies
    //----------------------------------------
    int n = cg_ordered.nVertices;
    for( int i=1; i<=n; i++ ){
      printf("getClusterCo(%i) = %f\n", i, getClusterCo(i, cg_ordered, cg_triangle));
    }


}
