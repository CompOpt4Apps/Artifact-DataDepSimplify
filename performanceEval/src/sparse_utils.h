//
// Created by kazem on 10/27/18.
//

#ifndef ICHOL_HLEVEL_SPARSE_UTILS_H
#define ICHOL_HLEVEL_SPARSE_UTILS_H
/* ========================================================================== */
/* === cholmod_nnz ========================================================== */
/* ========================================================================== */
#include <vector>
#include "def.h"

/* Return the number of entries in a sparse matrix.
 *
 * workspace: none
 * integer overflow cannot occur, since the matrix is already allocated.
 */

long int getNNZ
  (
    /* ---innerPartsSize[l]- input ---- */
    int ncol,
    int *Ap,
    int *Anz,
    int packed,
    /* --------------- */
    int &status
  )
{
 size_t nz ;
 int j ;

 /* ---------------------------------------------------------------------- */
 /* get inputs */
 /* ---------------------------------------------------------------------- */

 /*RETURN_IF_NULL_COMMON (EMPTY) ;
 RETURN_IF_NULL (A, EMPTY) ;
 RETURN_IF_XTYPE_INVALID (A, CHOLMOD_PATTERN, CHOLMOD_ZOMPLEX, EMPTY) ;
 Common->status = CHOLMOD_OK ;*/
 status = TRUE;
 /* ---------------------------------------------------------------------- */
 /* return nnz (A) */
 /* ---------------------------------------------------------------------- */

 if (packed)
 {
  //Ap = A->p ;
  //RETURN_IF_NULL (Ap, EMPTY) ;
  nz = Ap [ncol] ;
 }
 else
 {
  //Anz = A->nz ;
  //RETURN_IF_NULL (Anz, EMPTY) ;
  nz = 0 ;
  for (j = 0 ; j < ncol ; j++)
  {
   nz += MAX (0, Anz [j]) ;
  }
 }
 return (nz) ;
}


/* ========================================================================== */
/* === cholmod_add_size_t =================================================== */
/* ========================================================================== */

/* Safely compute a+b, and check for integer overflow.  If overflow occurs,
 * return 0 and set OK to FALSE.  Also return 0 if OK is FALSE on input. */

size_t add_size_t (size_t a, size_t b, int *ok)
{
 size_t s = a + b ;
 (*ok) = (*ok) && (s >= a) ;
 return ((*ok) ? s : 0) ;
}


/* ========================================================================== */
/* === cholmod_mult_size_t ================================================== */
/* ========================================================================== */

/* Safely compute a*k, where k should be small, and check for integer overflow.
 * If overflow occurs, return 0 and set OK to FALSE.  Also return 0 if OK is
 * FALSE on input. */

size_t mult_size_t(size_t a, size_t k, int *ok)
{
 size_t p = 0, s ;
 while (*ok)
 {
  if (k % 2)
  {
   p = p + a ;
   (*ok) = (*ok) && (p >= a) ;
  }
  k = k / 2 ;
  if (!k) return (p) ;
  s = a + a ;
  (*ok) = (*ok) && (s >= a) ;
  a = s ;
 }
 return (0) ;
}

int makeUnique(int *node2Par,
               std::vector<int> &list,
               int n, bool *ws){
 int min=INT32_MAX;
 for (int ii = 0; ii < list.size(); ) {
  int tmp = node2Par[list[ii]];
  if(!ws[tmp]){//if first time
   ws[tmp]= true;
   min=min>tmp?tmp:min;
   ii++;
  }else{//otherwise remove it
   list.erase(list.begin()+ii);
  }
 }
//Reset it for future use
 for (int ii = 0; ii < list.size(); ++ii) {
  int tmp = node2Par[list[ii]];
  ws[tmp] = false;
 }
 return min;//returns cluster with min number.
}


/* depth-first-search of the graph of a matrix, starting at node j
 * depth-first-search of the graph of a matrix, starting at node j
 * modified to report the intersections
 * marked[i]=0; not visited
 * marked[i]=1; visited now
 * marked[i]=-1; visited in previous CC
 * clashedNodes has the list of nodes clashed with visited nodes
 * in previous CCs*/
int dfs_CSC_CC (size_t  n, int j, int *Gp, int *Gi,
                int *marked, int top, int *xi,
                int *pstack,
                std::vector<int> &clashedNodes,
                const int *pinv) {
 int i, p, p2, done, jnew, head = 0 ;
 //if (!CS_CSC (G) || !xi || !pstack) return (-1) ;    /* check inputs */
 //Gp = G->p ; Gi = G->i ;
 xi [0] = j ;                /* initialize the recursion stack */

 while (head >= 0) {
  j = xi [head] ;         /* get j from the top of the recursion stack */
  jnew = pinv ? (pinv [j]) : j ;
  if (!marked[jnew])  {
   marked[jnew] = 1;       /* mark node j as visited */
   pstack [head] =  Gp [jnew] ;
  }
  if(marked[jnew]==-1){//visited in previous CCs
   clashedNodes.push_back(jnew);
  }
  done = 1 ;                  /* node j done if no unvisited neighbors */
  p2 = Gp [jnew+1] ;
  for (p = pstack [head] ; p < p2 ; p++)  /* examine all neighbors of j */
  {
   i = Gi [p] ;            /* consider neighbor node i */
   if (marked[i]==-1)
    clashedNodes.push_back(i);//Another node from prev CCs.
   if (marked[i]) continue ;   /* skip visited node i */
   pstack [head] = p ;     /* pause depth-first search of node j */
   xi [++head] = i ;       /* start dfs at node i */
   done = 0 ;              /* node j is not done */
   break ;                 /* break, to start dfs (i) */
  }
  if (done)               /* depth-first search at node j is done */
  {
   head-- ;            /* remove j from the recursion stack */
   xi [--top] = j ;    /* and place in the output stack */
  }
 }
 return (top) ;
}


/* non-recursive version for actual use */

int dfsC		/* return the new value of k */
  (
    int p,		/* start the DFS at a root node p */
    int k,		/* start the node numbering at k */
    int *Post,	/* Post ordering, modified on output */
    int *Head ,	/* Head [p] = youngest child of p; EMPTY on output */
    int *Next ,	/* Next [j] = sibling of j; unmodified */
    int *Pstack 	/* workspace of size n, undefined on input or output */
  )
{
 int j, phead ;

 /* put the root node on the stack */
 Pstack [0] = p ;
 phead = 0 ;

 /* while the stack is not empty, do: */
 while (phead >= 0)
 {
  /* grab the node p from top of the stack and get its youngest child j */
  p = Pstack [phead] ;
  j = Head [p] ;
  if (j == EMPTY)
  {
   /* all children of p ordered.  remove p from stack and order it */
   phead-- ;
   Post [k++] = p ;	/* order node p as the kth node */
  }
  else
  {
   /* leave p on the stack.  Start a DFS at child node j by putting
    * j on the stack and removing j from the list of children of p. */
   Head [p] = Next [j] ;
   Pstack [++phead] = j ;
  }
 }
 return (k) ;	/* the next node will be numbered k */
}

//From CSPARSE
/* depth-first-search of the graph of a matrix, starting at node j */
int dfs (int j, int *Gp, int *Gi, int top, int *xi, int *pstack, const int *pinv)
{
 int i, p, p2, done, jnew, head = 0 ;
 //if (!CS_CSC (G) || !xi || !pstack) return (-1) ;    /* check inputs */
 //Gp = G->p ; Gi = G->i ;
 xi [0] = j ;                /* initialize the recursion stack */

 while (head >= 0)
 {
  j = xi [head] ;         /* get j from the top of the recursion stack */
  jnew = pinv ? (pinv [j]) : j ;
  if (!CS_MARKED (Gp, j))
  {
   CS_MARK (Gp, j) ;       /* mark node j as visited */
   pstack [head] = (jnew < 0) ? 0 : CS_UNFLIP (Gp [jnew]) ;
  }
  done = 1 ;                  /* node j done if no unvisited neighbors */
  p2 = (jnew < 0) ? 0 : CS_UNFLIP (Gp [jnew+1]) ;
  for (p = pstack [head] ; p < p2 ; p++)  /* examine all neighbors of j */
  {
   i = Gi [p] ;            /* consider neighbor node i */
   if (CS_MARKED (Gp, i)) continue ;   /* skip visited node i */
   pstack [head] = p ;     /* pause depth-first search of node j */
   xi [++head] = i ;       /* start dfs at node i */
   done = 0 ;              /* node j is not done */
   break ;                 /* break, to start dfs (i) */
  }
  if (done)               /* depth-first search at node j is done */
  {
   head-- ;            /* remove j from the recursion stack */
   xi [--top] = j ;    /* and place in the output stack */
  }
 }
 return (top) ;
}

/* depth-first search and postorder of a tree rooted at node j */
int tdfs(int j, int k, int *head, const int *next, int *post, int *stack)
{
 int i, p, top = 0 ;
 if (!head || !next || !post || !stack) return (-1) ;    /* check inputs */
 stack [0] = j ;                 /* place j on the stack */
 while (top >= 0)                /* while (stack is not empty) */
 {
  p = stack [top] ;           /* p = top of stack */
  i = head [p] ;              /* i = youngest child of p */
  if (i == -1)
  {
   top-- ;                 /* p has no unordered children left */
   post [k++] = p ;        /* node p is the kth postordered node */
  }
  else
  {
   head [p] = next [i] ;   /* remove i from children of p */
   stack [++top] = i ;     /* start dfs on child node i */
  }
 }
 return (k) ;
}


int heightPartitioning_DAG_Trng(int levelNo,
                                int *levelPtr,
                                int *node2Level,
                                int originalHeight,
                                int innerParts,
                                int minLevelDist,
                                int divRate,
                                std::vector<int> &innerPartsSize,
                                std::vector<std::vector<int>> &slackGroups,
                                double *subTreeCost,
                                int *partition2Level,
                                bool sw= false
) {
 int levelCut=0, preLevelCut=0, finalSeqNodes=3;
 int lClusterCnt=0, innerPartsTmp = innerParts;
 //auto partition2Level = new int[levelNo+1]();
 int *accuSlackGroups = new int[levelNo];
 if(levelNo <=minLevelDist){
  partition2Level[0] = 0;
  partition2Level[1] = levelNo;
  innerPartsSize.push_back(1);
  lClusterCnt = 1;
  return lClusterCnt;
 }
 //assigne the nodes in normal level set
 for (int i = 0; i < levelNo; ++i) {
  /*accuSlackGroups[i] = levelPtr[i+1]-levelPtr[i] +
    slackGroups[i].size();*/
  accuSlackGroups[i] = levelPtr[i+1]-levelPtr[i];
  assert(accuSlackGroups[i] >=0);
 }


 partition2Level[0]=0;


 /*if(minLevelDist<=0)
  minLevelDist=2;//default parameter*/
 int tmp=0;
 tmp=minLevelDist;
 if(tmp>partition2Level[lClusterCnt] && tmp<levelNo){
  //Due to tuning parameter we need this check
  partition2Level[++lClusterCnt]=tmp;
  innerPartsTmp = accuSlackGroups[tmp-1]/2 > 1 ? accuSlackGroups[tmp-1] : 1;
  innerPartsSize.push_back(innerPartsTmp);
 }
 tmp += divRate;
 while (tmp < originalHeight-1) {
  //Ensures a certain number of level in each partition
  innerPartsTmp = accuSlackGroups[tmp-1];
  innerPartsSize.push_back(innerPartsTmp>1?innerPartsTmp:1);
  partition2Level[++lClusterCnt]=tmp;
  tmp += divRate;
 }
 partition2Level[++lClusterCnt] = originalHeight + 1;
 innerPartsSize.push_back(1);//The last partition has one element
 delete[]accuSlackGroups;
#if 0
 for (int i1 = 0; i1 <= lClusterCnt; ++i1) {
   std::cout << partition2Level[i1] << ",";
  }
  for (int i1 = lClusterCnt + 1; i1 < 7; ++i1) {
   std::cout << partition2Level[i1] << ",";
  }
 //std::cout<<"\n";
#endif
 return lClusterCnt;
}

#endif //ICHOL_HLEVEL_SPARSE_UTILS_H
