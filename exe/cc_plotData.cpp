//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  cc_plotData.cpp
//  Christopher Gradwohl
//  Writes Cluster Coefficant and Vertex Degree data to plotData.txt
//
//
//  to get plot use:
//      $ python3 plot_CC.py <PATH TO GRAPH>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Escape/GraphIO.h"
#include "Escape/EdgeHash.h"
#include "Escape/Digraph.h"
#include "Escape/Triadic.h"
#include "Escape/Graph.h"
#include <inttypes.h>
#include <cstdlib>
#include <iostream>
using namespace Escape;


// getClusterCo()
// returns the Clustering Coefficiant of vertex v
double getClusterCo(int v, CGraph cGraph, TriangleInfo triGraph) {
    int degree   = (cGraph.offsets[v+1] - cGraph.offsets[v]);
    double top   = (2*(triGraph.perVertex[v]));
    if( top==0 ) return 0.0;
    else{
        double bott  = (degree*(degree - 1));
        double ratio = (top)/(bott);
        return ratio;
    }
}


// getDegree()
// returns the Degree of vertex v
int getDegree(int v, CGraph cGraph) {
    int degree   = (cGraph.offsets[v+1] - cGraph.offsets[v]);
    return degree;
}


int main(int argc, char *argv[]) {


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Graph Processing
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Graph g;
    if (loadGraph(argv[1], g, 1, IOFormat::escape)) exit(1);
    CGraph cg                = makeCSR(g);
    cg.sortById();
    CGraph cg_ordered        = cg.renameByDegreeOrder();
    cg_ordered.sortById();
    CDAG cg_dag              = degreeOrdered(&cg_ordered);
    TriangleInfo cg_triangle = betterWedgeEnumerator(&(cg_dag.outlist));


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Writes formatted data to CCplotData.txt
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    double n          = cg_ordered.nVertices;
    double thisdegree = getDegree(0, cg_ordered);
    double count      = 0;
    double CCtotal    = 0.0;
    double CCavg      = 0.0;
    FILE *fp;
    fp = fopen("../wrappers/CCplotData.txt","w");
    for( int i=0; i<=n; i++ ){
        if( getDegree(i, cg_ordered)==thisdegree ){
            CCtotal = CCtotal + getClusterCo(i, cg_ordered, cg_triangle);
            count++;
        }else if( getDegree(i, cg_ordered)!=thisdegree ){
            CCavg   = CCtotal/count;
            fprintf(fp,"%f %f\n", thisdegree, CCavg);
            CCtotal = 0.0;
            count   = 0.0;
            CCavg   = 0.0;
            while( getDegree(i, cg_ordered)!=thisdegree && i!=n ) thisdegree++;
            CCtotal = CCtotal + getClusterCo(i, cg_ordered, cg_triangle);
            count++;
        }
    }


}
