/*!
 * \file chillusage_test.cc
 *
 * \brief Tests the part of IEGenLib interface that will be needed in CHILL.
 *
 * \date Started: 8/16/13
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013, Colorado State University <br>
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 *
 *
 * HOW TO USE INTERFACAE TO SIMPLIFICATION ALGORITHM for
 * simplifying constraints set containing Uninterpreted Function Calls:
 * 
 * (1) You need to define uninterpreted function calls (UFCs) that appear
 *        in constraints for iegenlib enviroment. The example in the main
 *        function shows how to do this. Note that, you need to this only once
 *        for sets that have same UFCs during one run
 *
 * (2) You need to put constraints in iegenlib Set (or Relation).
 * 
 * (3) Create a std::set that includes which tuple variables (loop iterators)
 *     we are not going to project out. 
 *
 * (4) Apply a heuristic to remove expensive constraints that
 *     is keeping us from projecting out an iterator:
         for instance: 
                        col(j) < n  would keep us from projecting 'j'
       We only remove constraints up to a maximum number determined by user.
 *
 * (5) You can add user defined constraints to the Sets as demonstrated
 *     in examples. 
 * 
 * (6) Use simplifyForPartialParallel function that is main interface for the
 *     algorithm. If Sets are not satisfiable the function would return NULL.
 *     Otherwise it would return the simplified result as iegenlib Set.
 * 
 * (7) Print out result (if not NULL) using toISLString() function. 
 
 *  We have demonstrated these steps with two example test cases ILU and Gauss-Seidel 
 */

#include <gtest/gtest.h>

#include <iegenlib.h>
using namespace iegenlib;
using namespace std;

/*!
 * \class ChillUsageTest
 *
 * \brief Class to test CHILL usage of IEGenLib.
 *
 * This class holds gtest test cases that test the functionality in
 * iegenlib that will be needed by CHILL.
 */
class ChillUsageTest : public::testing::Test {
    public:
    protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

/*! Tests modifying the access relations for loop coalescing.
*/
TEST_F(ChillUsageTest, LoopCoalescing)
{
    // from CHILL, Anand defines the access relations.
    // example: y[i] += a[j]*x[col[j]];
    // v0 stands for version 0
    Relation* r0_v0 = new Relation("{[i,j]->[i]}");
    Relation* r1_v0 = new Relation("{[i,j]->[j]}");
    Relation* r2_v0 = new Relation("{[i,j]->[r2] : r2=col(j)}");

    // from CHILL, Anand declares the uninterpreted function
    // that performs coalescing and defines the coalescing
    // transformation.
    iegenlib::setCurrEnv(); // Clears out the environment
    iegenlib::appendCurrEnv("c", // UF name
        // UF domain
        new Set("{[i,j]:0<=i && i <nrows && ja(i)<=j && j<ja(i+1) }"),
        // UF range
        new Set("{[k]:0<=k && k<nnz}"),
        // c function is bijective
        true,
        // no monotonicity
        Monotonic_NONE );               
    Relation* T_coalesce = new Relation("{[i,j]->[k]:k = c(i,j)}");  
    // c for coalesce?  we talked about R but that was for inverse of above

    // then applies the transformation to each of the access relation
    Relation* T_coalesce_inv = T_coalesce->Inverse();
    EXPECT_EQ("{ [k] -> [i, j] : k - c(i, j) = 0 }",
        T_coalesce_inv->prettyPrintString() );

    Relation* r0_v1 = r0_v0->Compose(T_coalesce_inv);
    EXPECT_EQ("{ [k] -> [i] : i - c_inv(k)[0] = 0 }", 
        r0_v1->prettyPrintString() );

    Relation* r1_v1 = r1_v0->Compose(T_coalesce_inv);
    EXPECT_EQ("{ [k] -> [j] : j - c_inv(k)[1] = 0 }",
        r1_v1->prettyPrintString() );

    Relation* r2_v1 = r2_v0->Compose(T_coalesce_inv);
    EXPECT_EQ("{ [k] -> [r2] : r2 - col(c_inv(k)[1]) = 0 }",
        r2_v1->prettyPrintString() );


        
    ////// To get the updated access relation do the following:
    // Find tuple variable 1 as a function of tuple variables 0 through 0
    Exp* r0_v1_exp = r0_v1->findFunction(1,0,0);
    EXPECT_EQ("c_inv(k)[0]", 
        r0_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    Exp* r1_v1_exp = r1_v1->findFunction(1,0,0);
    EXPECT_EQ("c_inv(k)[1]", 
        r1_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    Exp* r2_v1_exp = r2_v1->findFunction(1,0,0);
    EXPECT_EQ("col(c_inv(k)[1])", 
        r2_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    delete r0_v0;
    delete r0_v1;
    delete r0_v1_exp;
    delete r1_v0;
    delete r1_v1;
    delete r1_v1_exp;
    delete r2_v0;
    delete r2_v1;
    delete r2_v1_exp;
    delete T_coalesce;
    delete T_coalesce_inv;
}

/*! Tests modifying the access relations for loop coalescing
    and indicating that k=j.
*/
TEST_F(ChillUsageTest, LoopCoalescingKequalJ)
{
    // from CHILL, Anand defines the access relations.
    // example: y[i] += a[j]*x[col[j]];
    // v0 stands for version 0
    Relation* r0_v0 = new Relation("{[i,j]->[i]}");
    Relation* r1_v0 = new Relation("{[i,j]->[j]}");
    Relation* r2_v0 = new Relation("{[i,j]->[r2] : r2=col(j)}");

    // from CHILL, Anand declares the uninterpreted function
    // that performs coalescing and defines the coalescing
    // transformation.
    iegenlib::setCurrEnv(); // Clears out the environment
    iegenlib::appendCurrEnv("c", // UF name
        // UF domain
        new Set("{[i,j]:0<=i && i <nrows && ja(i)<=j && j<ja(i+1) }"),
        // UF range
        new Set("{[k]:0<=k && k<nnz}"),
        // c function is bijective
        true,
        // no monotonicity
        Monotonic_NONE );               
               
    Relation* T_coalesce = new Relation("{[i,j]->[k]:k = c(i,j) && k=j}");  

    // then applies the transformation to each of the access relation
    Relation* T_coalesce_inv = T_coalesce->Inverse();
    EXPECT_EQ("{ [k] -> [i, j] : k - j = 0 && k - c(i, j) = 0 }",
        T_coalesce_inv->prettyPrintString() );

    Relation* r0_v1 = r0_v0->Compose(T_coalesce_inv);
    EXPECT_EQ(
        "{ [k] -> [i] : k - c(c_inv(k)[0], k) = 0 && i - c_inv(k)[0] = 0 }", 
        r0_v1->prettyPrintString() );

    Relation* r1_v1 = r1_v0->Compose(T_coalesce_inv);
    EXPECT_EQ("{ [k] -> [j] : k - j = 0 && k - c(c_inv(k)[0], k) = 0 }",
        r1_v1->prettyPrintString() );

    Relation* r2_v1 = r2_v0->Compose(T_coalesce_inv);
    EXPECT_EQ("{ [k] -> [r2] : k - c(c_inv(k)[0], k) = 0 && r2 - col(k) = 0 }",
        r2_v1->prettyPrintString() );


        
    ////// To get the updated access relation do the following:
    // Find tuple variable 1 as a function of tuple variables 0 through 0
    Exp* r0_v1_exp = r0_v1->findFunction(1,0,0);
    EXPECT_EQ("c_inv(k)[0]", 
        r0_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    Exp* r1_v1_exp = r1_v1->findFunction(1,0,0);
    EXPECT_EQ("k", 
        r1_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    Exp* r2_v1_exp = r2_v1->findFunction(1,0,0);
    EXPECT_EQ("col(k)", 
        r2_v1_exp->prettyPrintString(r0_v1->getTupleDecl()) );
    
    delete r0_v0;
    delete r0_v1;
    delete r0_v1_exp;
    delete r1_v0;
    delete r1_v1;
    delete r1_v1_exp;
    delete r2_v0;
    delete r2_v1;
    delete r2_v1_exp;
    delete T_coalesce;
    delete T_coalesce_inv;
}
#if 0
/*! Tests adding constraints between UFCalls such as
    forall e, index(e) <= diagptr(e).
*/
TEST_F(ChillUsageTest, AddUFConstraints)
{
    Relation* r1 = new Relation("{[i,j]->[k] : k=col(j) && 0<=i && i<N"
                                "&& index(i)<=j && j<index(i+1)}");
    Relation* r1add = r1->addUFConstraints("index", "<=", "diagptr");
    Relation* expect1 = new Relation("{[i,j]->[k] : k=col(j) && 0<=i && i<N"
                                     "&& index(i)<=j && j<index(i+1)"
                                     "&& index(i)<=diagptr(i)"
                                     "&& index(i+1)<=diagptr(i+1)}");

    //EXPECT_EQ(expect1->prettyPrintString(), r1add->prettyPrintString());
    
    delete r1;
    delete r1add;
    delete expect1;
}

/*! Test cases for CSR Gauss-Seidel code's data access dependencies.
*/
TEST_F(ChillUsageTest, GS_CSR_DepSimplification)
{

/* Following is the Gauss-Seidel code. Dependence analysis of this code
   would identify 1 pair of read/write (inbetween S1)
   data accesses that may be a data dependence. This pair produces two 
   distinct conjunctions considering the ordering of accesses. Overall there 
   are 2 distinct conjunctions for the complete dependence relation.
   
   We will apply simplififcation algorithm to these 2 conjunctions.

for (i=0; i < N; i++) {
    for (j=rowptr[i];j<rowptr[i + 1];j++) {
S1:     y[i] -= values[j]*y[colidx[j]];
    }
}
*/

    // (1)
    // Introduce the UFCalls to enviroment, and indicate their domain, range
    // whether they are bijective, or monotonic.
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_NONE);


    // (2)
    // Putting constraints in an iegenlib::Relation
    // Flow dependence checking for: read in S1 (y[colidx[j]]); write in S1 (y[i]);

    Relation* flow = new Relation("{ [i,j] -> [ip,jp] : i<ip && i=colidx(jp) "
                     "&& 0 <= i && i < m && 0 <= ip && ip < m "
                        "&& rowptr(i) <= j && j < rowptr(i+1) "
                   "&& rowptr(ip) <= jp && jp < rowptr(ip+1) }");

    // Expected outputs 
    Relation* ex_flow = new Relation("{ [i] -> [ip,jp] : i=colidx(jp) "
                              "&& colidx(jp) < ip && ip < m-1 "
                                  "&& rowptr(i) < rowptr(i+1) "
                    "&& rowptr(ip) <= jp && jp < rowptr(ip+1) "
     "&& colidx(jp) >= 0 && rowptr(i) >= 0 && rowptr(ip) >= 0 "
                  "&& rowptr(i+1) < nnz && rowptr(ip+1) < nnz}");


    // (3)
    // Specify loops that are going to be parallelized, so we are not going to
    // project them out.
    std::set<int> parallelTvs;
    parallelTvs.insert(0);
    parallelTvs.insert(2);

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* extendedFlow = flow->boundDomainRange();
    std::set<Exp> ignore = extendedFlow->constraintsDifference(flow);
    extendedFlow->removeExpensiveConstraints(parallelTvs, 0, ignore);

    // (5)
    // Add user defined constraints

    // (6)
    // Simplifyng the constraints relation
    Relation* flow_sim = extendedFlow->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results: if not satisfiable returns NULL

    // Verify the results 
    if ( ex_flow != NULL && flow_sim != NULL) {
        EXPECT_EQ(ex_flow->toISLString(), flow_sim->toISLString());
    } else {
        EXPECT_EQ(ex_flow, flow_sim);
    }


    // (2)
    // Putting constraints in an iegenlib::Relation
    // Anti dependence checking for: read in S1 (y[colidx[j]]); write in S1 (y[i]);

    Relation* anti = new Relation("{ [i,j] -> [ip,jp] : ip<i && i=colidx(jp) "
                     "&& 0 <= i && i < m && 0 <= ip && ip < m "
                        "&& rowptr(i) <= j && j < rowptr(i+1) "
                   "&& rowptr(ip) <= jp && jp < rowptr(ip+1) }");


    Relation* ex_anti = new Relation("{ [i] -> [ip,jp] : i=colidx(jp) "
                               "&& 0 <= ip && ip < colidx(jp) "
                                  "&& rowptr(i) < rowptr(i+1) "
                    "&& rowptr(ip) <= jp && jp < rowptr(ip+1) "
     "&& colidx(jp) < m-1 && rowptr(i) >= 0 && rowptr(ip) >= 0 "
                  "&& rowptr(i+1) < nnz && rowptr(ip+1) < nnz}");


    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* extendedAnti = anti->boundDomainRange();
    ignore = extendedFlow->constraintsDifference(anti);
    extendedAnti->removeExpensiveConstraints(parallelTvs, 0, ignore);
    // (5)
    // Add user defined constraints

    // (6)
    // Simplifyng the constraints relation
    Relation* anti_sim = extendedAnti->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results: if not satisfiable returns NULL

    // Verify the results 
    if ( ex_anti != NULL && anti_sim != NULL) {
        EXPECT_EQ(ex_anti->toISLString(), anti_sim->toISLString());
    } else {
        EXPECT_EQ(ex_anti, anti_sim);
    }


    delete ex_flow;
    delete ex_anti;
    delete flow;
    delete anti;
    delete flow_sim;
    delete anti_sim;
}

//#if 0
/*! Test cases for CSR ILU code's data access dependencies
*/
TEST_F(ChillUsageTest, ILU_CSR_DepSimplification)
{

/* Following is the ILU0 code. Dependence analysis of this code
   would identify 8 pairs of read/write or write/write(in or between S1 & S2)
   data accesses that may be data dependences. Each pair produces two 
   distinct conjunctions considering the ordering of accesses. Overall there 
   are 16 distinct conjunctions for the complete dependence relation.
   
   We will apply simplififcation algorithm to all of these 16 conjunctions.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    // (1)
    // Introduce the UFCalls to enviroment, and indicate their domain, range
    // whether they are bijective, or monotonic.
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);


/* The first pair of possible data access dependences is comming from:
   a read in S1 (v[k]);  a write in S1 (v[k]);
   A flow (F1) or anti (A1) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    // (2)
    // Putting constraints in an iegenlib::Relation
    // Anti dependence checking for: read in S1 (v[k]); write in S1 (v[k]);

    Relation *A1 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m1"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = kp }");

    // expected output  (for testing purposes)
    Relation *ex_A1 = NULL;

    // (3) 
    // Specify loops that are going to be parallelized, so we are not going to
    // project them out. Here "i" and "ip"
    std::set<int> parallelTvs;
    parallelTvs.insert(0);
    parallelTvs.insert(4);

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA1 = A1->boundDomainRange();
    std::set<Exp> ignore = DRA1->constraintsDifference(A1);
    DRA1->removeExpensiveConstraints(parallelTvs, 2, ignore);
    // (5)
    // How to add user defined constraint
    Relation *A1_extend = DRA1->addUFConstraints("rowptr","<=", "diagptr");


    // (6)
    // Simplifyng the constraints relation
    Relation *A1_sim = A1_extend->simplifyForPartialParallel(parallelTvs);


    // (7)
    // Print out results: if not satisfiable returns NULL

    // Verify the results 
    if ( ex_A1 != NULL && A1_sim != NULL) {
        EXPECT_EQ(ex_A1->toISLString(), A1_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A1, A1_sim);
    }


    // (2)
    // Putting constraints in an iegenlib::Relation
    // Flow dependence checking for: read in S1 (v[k]); write in S1 (v[k]);

    Relation *F1 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = kp}");

    Relation *ex_F1 = NULL;


    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF1 = F1->boundDomainRange();
    ignore = DRF1->constraintsDifference(A1);
    DRF1->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // (5)
    // Adding user defined constraint
    Relation *F1_extend = DRF1->addUFConstraints("rowptr","<=", "diagptr");

    // (6)
    // Simplifyng the constraints relation
    Relation *F1_sim = F1_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_F1 != NULL && F1_sim != NULL) {
        EXPECT_EQ(ex_F1->toISLString(), F1_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F1, F1_sim);
    }

    delete A1;
    delete F1;
    delete DRA1;
    delete DRF1;
    delete A1_extend;
    delete A1_sim;
    delete F1_extend;
    delete F1_sim;

    // ----------------------------


/* The second pair of possible data access dependences is comming from:
   a read in S1 (v[diag[col[k]]]);  a write in S1 (v[k]);
   A flow (F2) or anti (A2) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A2 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = diagptr(colidx(kp)) }");

    Relation *ex_A2 = NULL;

    Relation *F2 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = diagptr(colidx(kp))}");

    Relation *ex_F2 = NULL;


    //--- Simplifying Anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA2 = A2->boundDomainRange();
    ignore = DRA2->constraintsDifference(A2);
    DRA2->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *A2_extend = DRA2->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *A2_sim = A2_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results
 

    // Verify the results 
    if ( ex_A2 != NULL && A2_sim != NULL) {
        EXPECT_EQ(ex_A2->toISLString(), A2_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A2, A2_sim);
    }


    //--- Simplifying flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF2 = F2->boundDomainRange();
    ignore = DRF2->constraintsDifference(F2);
    DRF2->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *F2_extend = DRF2->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *F2_sim = F2_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_F2 != NULL && F2_sim != NULL) {
        EXPECT_EQ(ex_F2->toISLString(), F2_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F2, F2_sim);
    }

    delete A2;
    delete F2;
    delete DRA2;
    delete DRF2;
    delete A2_extend;
    delete A2_sim;
    delete F2_extend;
    delete F2_sim;

    // ----------------------------


/* The third pair of possible data access dependences is comming from:
   a write in S1 (v[k]);  a write in S2 (v[j1]);
   One of two possible output dependences can exist: 
       F3 (write_S1 before write_S2) or A3 (write_S2 before write_S1)

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A3 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = j1p}");

    Relation *ex_A3 = NULL;

    Relation *F3 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = j1p}");

    Relation *ex_F3 = NULL;

    //--- Simplifying Anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA3 = A3->boundDomainRange();
    ignore = DRA3->constraintsDifference(A3);
    DRA3->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *A3_extend = DRA3->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *A3_sim = A3_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_A3 != NULL && A3_sim != NULL) {
        EXPECT_EQ(ex_A3->toISLString(), A3_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A3, A3_sim);
    }


    //--- Simplifying flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF3 = F3->boundDomainRange();
    ignore = DRF3->constraintsDifference(F3);
    DRF3->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *F3_extend = DRF3->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *F3_sim = F3_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_F3 != NULL && F3_sim != NULL) {
        EXPECT_EQ(ex_F3->toISLString(), F3_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F3, F3_sim);
    }


    delete A3;
    delete F3;
    delete DRA3;
    delete DRF3;
    delete A3_extend;
    delete A3_sim;
    delete F3_extend;
    delete F3_sim;

    // ----------------------------

/* The fourth pair of possible data access dependences is comming from:
   a write in S1 (v[k]);  a read in S2 (v[j2]);
   A flow (F4) or anti (A4) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A4 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = j2p}");

    Relation *ex_A4 = NULL;

    Relation *F4 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = j2p}");

    Relation *ex_F4 = NULL;


    //--- Simplifying Anit dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA4 = A4->boundDomainRange();
    ignore = DRA4->constraintsDifference(A4);
    DRA4->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *A4_extend = DRA4->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *A4_sim = A4_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_A4 != NULL && A4_sim != NULL) {
        EXPECT_EQ(ex_A4->toISLString(), A4_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A4, A4_sim);
    }


    //--- Simplifying Flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF4 = F4->boundDomainRange();
    ignore = DRF4->constraintsDifference(F4);
    DRF4->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *F4_extend = DRF4->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *F4_sim = F4_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_F4 != NULL && F4_sim != NULL) {
        EXPECT_EQ(ex_F4->toISLString(), F4_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F4, F4_sim);
    }

    delete A4;
    delete F4;
    delete DRA4;
    delete DRF4;
    delete A4_extend;
    delete A4_sim;
    delete F4_extend;
    delete F4_sim;

    // ----------------------------

/* The fifth pair of possible data access dependences is comming from:
   a read in S1 (v[diag[col[k]]]);  a write in S2 (v[j1]);
   A flow (F5) or anti (A5) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A5 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = diagptr(colidx(kp)) }");

    Relation *ex_A5 = new Relation("[ m, nnz ] -> { [i, k] -> [ip, kp] : i = colidx(kp) &&"
" ip >= 0 && k >= 0 && kp >= 0 && colidx(k) >= 0 && colidx(kp) >= 0 &&"
" diagptr(i) >= 0 && diagptr(i + 1) >= 0 && diagptr(ip) >= 0 &&"
" diagptr(ip + 1) >= 0 && diagptr(colidx(k)) >= 0 &&"
" diagptr(colidx(kp)) >= 0 && rowptr(i) >= 0 && rowptr(i + 1) >= 0 &&"
" rowptr(ip) >= 0 && rowptr(ip + 1) >= 0 && rowptr(colidx(k)) >= 0 &&"
" rowptr(colidx(kp)) >= 0 && k - rowptr(i) >= 0 && kp - rowptr(ip) >= 0 &&"
" nnz - diagptr(colidx(k) + 1) >= 0 && nnz - diagptr(colidx(kp) + 1) >= 0 &&"
" nnz - rowptr(colidx(k) + 1) >= 0 && nnz - rowptr(colidx(kp) + 1) >= 0 &&"
" diagptr(i + 1) - rowptr(i + 1) >= 0 && diagptr(ip + 1) - rowptr(ip + 1) >= 0 &&"
" diagptr(colidx(k)) - rowptr(colidx(k)) >= 0 && diagptr(colidx(k) + 1) + 1 >= 0 &&"
" diagptr(colidx(kp)) - rowptr(colidx(kp)) >= 0 && diagptr(colidx(kp) + 1) + 1 >= 0 &&"
" rowptr(colidx(k) + 1) + 1 >= 0 && rowptr(colidx(kp) + 1) + 1 >= 0 &&"
" -ip + m - 2 >= 0 && -ip + colidx(kp) - 1 >= 0 && -k + nnz - 1 >= 0 &&"
" -k + diagptr(i) - 1 >= 0 && -k + diagptr(colidx(kp)) - 1 >= 0 &&"
" -kp + nnz - 1 >= 0 && -kp + diagptr(ip) - 1 >= 0 &&"
" -kp + rowptr(ip + 1) - 2 >= 0 && m - colidx(k) - 2 >= 0 &&"
" m - colidx(kp) - 2 >= 0 && nnz - diagptr(i) - 1 >= 0 &&"
" nnz - diagptr(i + 1) - 1 >= 0 && nnz - diagptr(ip) - 1 >= 0 &&"
" nnz - diagptr(ip + 1) - 1 >= 0 && nnz - diagptr(colidx(k)) - 1 >= 0 &&"
" nnz - diagptr(colidx(kp)) - 1 >= 0 && nnz - rowptr(i) - 1 >= 0 &&"
" nnz - rowptr(i + 1) - 1 >= 0 && nnz - rowptr(ip) - 1 >= 0 &&"
" nnz - rowptr(ip + 1) - 1 >= 0 && nnz - rowptr(colidx(k)) - 1 >= 0 &&"
" nnz - rowptr(colidx(kp)) - 1 >= 0 &&"
" -diagptr(colidx(k)) + rowptr(colidx(k) + 1) + 3 >= 0 &&"
" diagptr(colidx(k) + 1) - rowptr(colidx(k) + 1) + 10 >= 0 &&"
" -diagptr(colidx(kp)) + rowptr(i + 1) - 1 >= 0 &&"
" -diagptr(colidx(kp)) + rowptr(colidx(kp) + 1) + 3 >= 0 &&"
" diagptr(colidx(kp) + 1) - rowptr(colidx(kp) + 1) + 10 >= 0 }");


    Relation *F5 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = diagptr(colidx(kp))}");

    Relation *ex_F5 = new Relation("[ m, nnz ] -> { [i, k] -> [ip, kp] : i = colidx(kp) &&"
" ip >= 0 && k >= 0 && kp >= 0 && colidx(k) >= 0 && colidx(kp) >= 0 &&"
" diagptr(i) >= 0 && diagptr(i + 1) >= 0 && diagptr(ip) >= 0 &&"
" diagptr(ip + 1) >= 0 && diagptr(colidx(k)) >= 0 && diagptr(colidx(kp)) >= 0 &&"
" rowptr(i) >= 0 && rowptr(i + 1) >= 0 && rowptr(ip) >= 0 &&"
" rowptr(ip + 1) >= 0 && rowptr(colidx(k)) >= 0 && rowptr(colidx(kp)) >= 0 &&"
" k - rowptr(i) >= 0 && kp - rowptr(ip) >= 0 &&"
" nnz - diagptr(colidx(k) + 1) >= 0 && nnz - diagptr(colidx(kp) + 1) >= 0 &&"
" nnz - rowptr(colidx(k) + 1) >= 0 && nnz - rowptr(colidx(kp) + 1) >= 0 &&"
" diagptr(i + 1) >= rowptr(i + 1) && diagptr(ip + 1) >= rowptr(ip + 1) &&"
" diagptr(colidx(k)) - rowptr(colidx(k)) >= 0 &&"
" diagptr(colidx(k) + 1) + 1 >= 0 &&"
" diagptr(colidx(kp)) - rowptr(colidx(kp)) >= 0 &&"
" diagptr(colidx(kp) + 1) + 1 >= 0 && rowptr(colidx(k) + 1) + 1 >= 0 &&"
" rowptr(colidx(kp) + 1) + 1 >= 0 && -ip + m - 2 >= 0 &&"
" ip > colidx(k) && nnz > k && -k + diagptr(i) - 1 >= 0 &&"
" -k + diagptr(colidx(k)) - 1 >= 0 && -kp + nnz - 1 >= 0 &&"
" -kp + diagptr(ip) - 1 >= 0 && -kp + rowptr(ip + 1) - 2 >= 0 &&"
" m - colidx(k) - 2 >= 0 && m - colidx(kp) - 2 >= 0 &&"
" nnz - diagptr(i) - 1 >= 0 && nnz - diagptr(i + 1) - 1 >= 0 &&"
" nnz - diagptr(ip) - 1 >= 0 && nnz - diagptr(ip + 1) - 1 >= 0 &&"
" nnz - diagptr(colidx(k)) - 1 >= 0 && nnz - diagptr(colidx(kp)) - 1 >= 0 &&"
" nnz - rowptr(i) - 1 >= 0 && nnz - rowptr(i + 1) - 1 >= 0 &&"
" nnz - rowptr(ip) - 1 >= 0 && nnz - rowptr(ip + 1) - 1 >= 0 &&"
" nnz - rowptr(colidx(k)) - 1 >= 0 && nnz - rowptr(colidx(kp)) - 1 >= 0 &&"
" -diagptr(colidx(k)) + rowptr(i + 1) - 1 >= 0 &&"
" -diagptr(colidx(k)) + rowptr(colidx(k) + 1) + 3 >= 0 &&"
" diagptr(colidx(k) + 1) - rowptr(colidx(k) + 1) + 10 >= 0 &&"
" -diagptr(colidx(kp)) + rowptr(colidx(kp) + 1) + 3 >= 0 &&"
" diagptr(colidx(kp) + 1) - rowptr(colidx(kp) + 1) + 10 >= 0 }");


    //--- Simplifying Anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA5 = A5->boundDomainRange();
    ignore = DRA5->constraintsDifference(A5);
    DRA5->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *A5_extend = DRA5->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *A5_sim = A5_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_A5 != NULL && A5_sim != NULL) {
//        EXPECT_EQ(ex_A5->toISLString(), A5_sim->toISLString());
    } else {
//        EXPECT_EQ(ex_A5, A5_sim);
    }


    //--- Simplifying FLow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF5 =F5->boundDomainRange();
    ignore = DRF5->constraintsDifference(F5);
    DRF5->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *F5_extend = DRF5->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *F5_sim = F5_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_F5 != NULL && F5_sim != NULL) {
//        EXPECT_EQ(ex_F5->toISLString(), F5_sim->toISLString());
    } else {
//        EXPECT_EQ(ex_F5, F5_sim);
    }

    delete A5;
    delete F5;
    delete DRA5;
    delete DRF5;
    delete A5_extend;
    delete A5_sim;
    delete F5_extend;
    delete F5_sim;

    // ----------------------------

/* The sixth pair of possible data access dependences is comming from:
   a write in S2 (v[j1]);  a write in S2 (v[j1]);
   Depending on ordering one of two possible output dependences can exists.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A6 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = j1p}");


    Relation *ex_A6 = NULL;

    Relation *F6 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = j1p}");

    Relation *ex_F6 = NULL;


    //--- Simplifying Anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA6 = A6->boundDomainRange();
    ignore = DRA6->constraintsDifference(A6);
    DRA6->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *A6_extend = DRA6->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *A6_sim = A6_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_A6 != NULL && A6_sim != NULL) {
        EXPECT_EQ(ex_A6->toISLString(), A6_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A6, A6_sim);
    }


    //--- Simplifying Flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF6 = F6->boundDomainRange();
    ignore = DRF6->constraintsDifference(F6);
    DRF6->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // Adding user defined constraint
    Relation *F6_extend = DRF6->addUFConstraints("rowptr","<=", "diagptr");

    // Simplifyng the constraints relation
    Relation *F6_sim = F6_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_F6 != NULL && F6_sim != NULL) {
        EXPECT_EQ(ex_F6->toISLString(), F6_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F6, F6_sim);
    }


    delete A6;
    delete F6;
    delete DRA6;
    delete DRF6;
    delete A6_extend;
    delete A6_sim;
    delete F6_extend;
    delete F6_sim;

    // ----------------------------

/* The seventh pair of possible data access dependences is comming from:
   a read in S1 (v[k]);  a write in S2 (v[j1]);
   A flow (F7) or anti (A7) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A7 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = kp}");

    Relation *ex_A7 = NULL;

    Relation *F7 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = kp}");

    Relation *ex_F7 = NULL;

    //--- Simplifying Anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA7 = A7->boundDomainRange();
    ignore = DRA7->constraintsDifference(A7);
    DRA7->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // (5)
    // Adding user defined constraint
    Relation *A7_extend = DRA7->addUFConstraints("rowptr","<=", "diagptr");

    // (6)
    // Simplifyng the constraints relation
    Relation *A7_sim = A7_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_A7 != NULL && A7_sim != NULL) {
        EXPECT_EQ(ex_A7->toISLString(), A7_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A7, A7_sim);
    }


    //--- Simplifying flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF7 = F7->boundDomainRange();
    ignore = DRF7->constraintsDifference(F7);
    DRF7->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // (5)
    // Adding user defined constraint
    Relation *F7_extend = DRF7->addUFConstraints("rowptr","<=", "diagptr");

    // (6)
    // Simplifyng the constraints relation
    Relation *F7_sim = F7_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_F7 != NULL && F7_sim != NULL) {
        EXPECT_EQ(ex_F7->toISLString(), F7_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F7, F7_sim);
    }

    delete A7;
    delete F7;
    delete DRA7;
    delete DRF7;
    delete A7_extend;
    delete A7_sim;
    delete F7_extend;
    delete F7_sim;

    // ----------------------------

/* The eighth pair of possible data access dependences is comming from:
   a read in S2 (v[j2]);  a write in S2 (v[j1]);
   A flow (F8) or anti (A8) dependence can exist for these access pairs.

for(int i=0; i < n; i++)
{
  for(int k= row[i]; k < diag[i]; k++)
  {
S1: v[k] = v[k] / v[diag[col[k]]]; 
    tmp = v[k];

    int j1 = k + 1, j2 = diag[col[k]] + 1;

    while (j1 < row[i + 1] && j2 < row[col[k] + 1])
    {
      if (col[j1] < col[j2]) ++j1;
      else if (col[j2] < col[j1]) ++j2;
      else {
S2:     v[j1] -= tmp*v[j2]; 
        ++j1; ++j2;
      }
    }
  }
}
*/

    Relation *A8 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = j2p}");

    Relation *ex_A8 = new Relation("[ m, nnz ] -> { [i, k] -> [ip, kp] : "
"i = colidx(kp) && ip >= 0 && colidx(k) >= 0 && rowptr(i) >= 0 "
"&& rowptr(ip) >= 0 && rowptr(colidx(k)) >= 0 && rowptr(colidx(kp)) >= 0 "
"&& k >= rowptr(i) && kp >= rowptr(ip) && diagptr(i + 1) >= rowptr(i + 1) "
"&& diagptr(ip + 1) >= rowptr(ip + 1) && diagptr(colidx(k)) >= rowptr(colidx(k))"
" && diagptr(colidx(k) + 1) - rowptr(colidx(k) + 1) >= 0 && "
"diagptr(colidx(kp)) - rowptr(colidx(kp)) >= 0 && "
"diagptr(colidx(kp) + 1) >= rowptr(colidx(kp) + 1) && diagptr(i) > k "
"&& -k + rowptr(i + 1) - 2 >= 0 && -k + rowptr(colidx(kp) + 1) - 2 >= 0 "
"&& -ip + colidx(kp) - 1 >= 0 && -kp + diagptr(ip) - 1 >= 0 "
"&& -kp + rowptr(ip + 1) - 2 >= 0 && m - colidx(k) - 2 >= 0 "
"&& m - colidx(kp) - 2 >= 0 && nnz - diagptr(i) - 1 >= 0 "
"&& nnz - diagptr(i + 1) - 1 >= 0 && nnz - diagptr(ip) - 1 >= 0 "
"&& nnz > diagptr(ip + 1) && nnz > diagptr(colidx(k) + 1) "
"&& nnz - diagptr(colidx(kp) + 1) - 1 >= 0 && "
"-diagptr(colidx(k)) + rowptr(colidx(k) + 1) - 2 >= 0 && "
"-diagptr(colidx(kp)) + rowptr(i + 1) - 2 >= 0 && "
"-diagptr(colidx(kp)) + rowptr(colidx(kp) + 1) - 2 >= 0 }");

    string ex_A8_str = ex_A8->toISLString();

    Relation *F8 = new Relation("[m] -> {[i,k,j1,j2] -> [ip,kp,j1p,j2p]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = j2p}");

    Relation *ex_F8 = new Relation("[ m, nnz ] -> { [i, k] -> [ip, kp] : "
"i = colidx(kp) && colidx(k) >= 0 && colidx(kp) >= 0 && rowptr(i) >= 0 "
"&& rowptr(colidx(k)) >= 0 && rowptr(colidx(kp)) >= 0 "
"&& k >= rowptr(i) && kp >= rowptr(ip) && diagptr(i + 1) >= rowptr(i + 1) "
"&& diagptr(ip + 1) >= rowptr(ip + 1) && diagptr(colidx(k)) >= rowptr(colidx(k))"
" && diagptr(colidx(k) + 1) >= rowptr(colidx(k) + 1) && rowptr(ip) >= 0 "
"&& diagptr(colidx(kp)) >= rowptr(colidx(kp)) "
"&& diagptr(colidx(kp) + 1) - rowptr(colidx(kp) + 1) >= 0 && "
"-k + diagptr(i) - 1 >= 0 && -k + rowptr(i + 1) - 2 >= 0 && "
"-k + rowptr(colidx(kp) + 1) - 2 >= 0 && -ip + m - 2 >= 0 && "
"ip > colidx(kp) && diagptr(ip) > kp && -kp + rowptr(ip + 1) - 2 >= 0 "
"&& m > colidx(k) + 1 && nnz > diagptr(i) && nnz > diagptr(i + 1) "
"&& nnz > diagptr(ip) && nnz - diagptr(ip + 1) - 1 >= 0 && "
"nnz > diagptr(colidx(k) + 1) && nnz > diagptr(colidx(kp) + 1) "
"&& -diagptr(colidx(k)) + rowptr(colidx(k) + 1) - 2 >= 0 && "
"-diagptr(colidx(kp)) + rowptr(i + 1) - 2 >= 0 && "
"-diagptr(colidx(kp)) + rowptr(colidx(kp) + 1) - 2 >= 0 }"
);

    //--- Simplifying anti dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRA8 = A8->boundDomainRange();
    ignore = DRA8->constraintsDifference(A8);
    DRA8->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // (5)
    // Adding user defined constraint
    Relation *A8_extend = DRA8->addUFConstraints("rowptr","<=", "diagptr");

    // (6)
    // Simplifyng the constraints relation
    Relation *A8_sim = A8_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results


    // Verify the results 
    if ( ex_A8 != NULL && A8_sim != NULL) {
        EXPECT_EQ(ex_A8->toISLString(), A8_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A8, A8_sim);
    }

    //--- Simplifying flow dependence

    // (4)
    // Applying heuristic for removing expensive iterators
    Relation* DRF8 = F8->boundDomainRange();
    ignore = DRF8->constraintsDifference(F8);
    DRF8->removeExpensiveConstraints(parallelTvs, 2, ignore);

    // (5)
    // Adding user defined constraint
    Relation *F8_extend = DRF8->addUFConstraints("rowptr","<=", "diagptr");

    // (6)
    // Simplifyng the constraints relation
    Relation *F8_sim = F8_extend->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results

    // Verify the results 
    if ( ex_F8 != NULL && F8_sim != NULL) {
        EXPECT_EQ(ex_F8->toISLString(), F8_sim->toISLString());
    } else {
        EXPECT_EQ(ex_F8, F8_sim);
    }

    delete A8;
    delete F8;
    delete DRA8;
    delete DRF8;
    delete A8_extend;
    delete A8_sim;
    delete F8_extend;
    delete F8_sim;

}




/*! How to insert and retrieve universially quantified rules
    to and from environment.
*/
TEST_F(ChillUsageTest, InsertRetrieveUniQuantRule)
{
    string type, tupleDecl, leftSide, rightSide;
    int NoRules;
    iegenlib::setCurrEnv(); // Clears out the environment

    // #1 How to universially quantified rules to environment:

    // ##1.1 The environment function appendCurrEnv will add
    // functional consistency rule for all 
    // uninterpreted function symbol (UFSs) and some other 
    // universally quantified rules depending on  the type of 
    // monotonicity (if any) for each UFS using addUniQuantRule function. 
    // For instance for the following since we declare rowPtr
    // as monotonically increasing following rules will get added 
    // to environment automatically:
    //  
    // Functional consistency: 
    //   forall e1, e2 : if e1 = e2 then rowPtr(e1) = rowPtr(e2)   // #0
    // Strictlly increasing monotonocity:
    //   forall e1, e2 : e1 < e2 => rowPtr(e1) < rowPtr(e2)        // #1
    //   forall e1, e2 : rowPtr(e1) = rowPtr(e2) => e1 = e2        // #2
    //   forall e1, e2 : rowPtr(e1) < rowPtr(e2) => e1 < e2        // #3
    //   forall e1, e2 : rowPtr(e1) <= rowPtr(e2) => e1 <= e2      // #4

    iegenlib::appendCurrEnv("rowPtr", // UF name
        // rowPtr domain
        new Set("{[i]: 0 <= i && i < n}"),
        // rowPtr range
        new Set("{[j]: 0 <= j && j < nnz}"),
        // rowPtr function is bijective
        true,
        // monotonicially strictlly increasing
        Monotonic_Increasing );

    // Following will also add a functional consistency rule for colIdx:
    //   forall e1, e2 : if e1 = e2 then colIdx(e1) = colIdx(e2)   // #5
    iegenlib::appendCurrEnv("colIdx", new Set("{[i]: 0 <= i && i < nnz}"),
        new Set("{[j]: 0 <= j && j < n}"), true, Monotonic_NONE );


    // ##1.2 We could also add general universially quantfied rules between
    //  any UFS that is defined in the environment of the form:
    //   forall [e1, e2, ...]  conjunction([ei, ...]) => conjunction([ej, ...])
    //  * conjunction can be any IEGenLib acceptable conjunction of equalities
    //    and inequalities with subset of unversially quantified rules being 
    //    used in the constraints in either side 
    // Following is an example rule:
    //   forall e1, e2,  rowPtr(e1) <= e2 < rowPtr(e1+1) => colIdx(e2) <= e1 // #6
    // And, following demonstrates how we could encode it:

    // A pointer to an instance of the class that 
    // defines universially quantified rules    // Verify the results 
    if ( ex_A8 != NULL && A8_sim != NULL) {
        EXPECT_EQ(ex_A8->toISLString(), A8_sim->toISLString());
    } else {
        EXPECT_EQ(ex_A8, A8_sim);
    }
    UniQuantRule *uqRule;
    // An optional type predefined types are: 
    //  Monotonicity, CoMonotonicity, Triangular, FuncConsistency, TheOthers
    type = ("Triangular");
    // List of universially quantifed variables: forall e1, e2, ...                       
    tupleDecl = ("[e1,e2]");
    // Left side of the rule (p part): p => q
    leftSide = ( "rowPtr(e1) <= e2 and e2 < rowPtr(e1+1)" );
    // Right side of the rule (q part): p => q
    rightSide = ( "colIdx(e2) <= e1" );
    // Definign the rule:
    uqRule = new UniQuantRule(type, tupleDecl, leftSide, rightSide);
    // Adding the rule to environment:
    currentEnv.addUniQuantRule( uqRule );
    // *** Do not delete uqRule since environment owns it.


    // #2 Now, following demonstrates how we query rules and related info:

    // ##2.1 Query the number of available universially quantified Rules:
    NoRules = queryNoUniQuantRules();
    // Currently there are 7 rules are available in the environment:
    // 2 functional consistency rules one for rowPtr and one for colIdx
    // 4 monotonicity related rules for rowPtr (shown above in the example)
    // 1 general rule added in the above example
    EXPECT_EQ( NoRules , 7 );

    // ##2.2 Query the universially quantified Rule No. 3 stored in 
    // the enviroment. The environment still owns returned object:
    //   The rule is a monotonoicty related rule for rowPtr:
    //   forall e1, e2 : rowPtr(e1) < rowPtr(e2) => e1 < e2 
    UniQuantRule* uqRule3 = queryUniQuantRuleEnv(3);  // Indexes start from 0

    // ##2.3 We could query different parts of a rule:

    UniQuantRuleType rt = uqRule3->getType();   // Type
    EXPECT_EQ( rt , iegenlib::Monotonicity );

    // Get left side of the implication in the rule (p part): p => q
    Set* leftSideOfTheRule = uqRule3->getLeftSide();
    // However the left/right hand side are stored as normal IEGenLib Sets:
    Set ex_left_side("{[e1,e2] : rowPtr(e1) < rowPtr(e2) }");
    EXPECT_EQ( *leftSideOfTheRule , ex_left_side );

    // If you want to access just the constraints in the left/right hand sides
    // you could use functionalities in isl_str_manipulation.h/cc for breaking 
    // different parts of a Set/Relation (for details refer to those files):
    srParts RulesLParts;
    RulesLParts = getPartsFromStr(leftSideOfTheRule->prettyPrintString());
    EXPECT_EQ( RulesLParts.constraints , 
               std::string(" -rowPtr(e1) + rowPtr(e2) - 1 >= 0 ") );
    // ** Please note inqualities are stored in the follwoing way in IEGEnLib:
    //    rowPtr(e1) < rowPtr(e2)   ==  -rowPtr(e1) + rowPtr(e2) - 1 >= 0

    //  ##2.4 Query the universially quantified Rule No. 6
    UniQuantRule* uqRule6 = queryUniQuantRuleEnv(6);

    // toString function could be used to get a string representation of a rule:
    string expected_rule_str = "Forall e1, e2   e2 - rowPtr(e1) >= 0 "
                               "&& -e2 + rowPtr(e1 + 1) - 1 >= 0  => "
                               " e1 - colIdx(e2) >= 0 ";
    EXPECT_EQ( uqRule6->toString() , expected_rule_str);

    // Do not delete the defined rules' objects (added or queried) 
    // since environment still owns them.
}
#endif


