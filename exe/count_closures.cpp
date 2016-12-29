#include "Escape/GraphIO.h"
#include "Escape/EdgeHash.h"
#include "Escape/Digraph.h"
#include "Escape/Triadic.h"
#include "Escape/Graph.h"
#include "Escape/GetAllCounts.h"

using namespace Escape;

int main(int argc, char *argv[])
{
  Graph g;
  if (loadGraph(argv[1], g, 1, IOFormat::escape))
    exit(1);

  printf("Loaded graph\n");
  CGraph cg = makeCSR(g);
  cg.sortById();
  printf("Converted to CSR\n");

  printf("Relabeling graph\n");
  CGraph cg_relabel = cg.renameByDegreeOrder();
  cg_relabel.sortById();

  Count *common = new Count[cg_relabel.nVertices+1];
  Count *closed = new Count[cg_relabel.nVertices+1];

  Count maximum = cClosure(&cg_relabel, common, closed);

  FILE* f = fopen("out.txt","w");
  if (!f)
  {
      printf("could not write to output to out.txt\n");
      return 0;
  }
  
  for(Count i = 1; i <= maximum; i++)
  {
      if (common[i] != 0)
      fprintf(f,"%ld %ld %ld\n",i,common[i],closed[i]);
  }

  fclose(f);
}
