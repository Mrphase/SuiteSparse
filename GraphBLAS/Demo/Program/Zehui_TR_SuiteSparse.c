#include "simple_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "graphblas_demos.h"
// #include <iostream>
// #include "graph.h"
// using namespace std;
// macro used by OK(...) to free workspace if an error occurs
#define FREE_ALL                    \
{                                   \
    GrB_Matrix_free (&A) ;          \
    if (Pd != NULL) free (Pd) ;     \
    if (Pi != NULL) free (Pi) ;     \
    if (P2 != NULL) free (P2) ;     \
}
//------------------------------------------------------------------------------
// print a complex matrix
//------------------------------------------------------------------------------

// when printed, 1 is added to all row indices so the results can be
// checked in MATLAB

void print_complex_matrix (GrB_Matrix A, char *name)
{
    GrB_Index nrows, ncols, nentries ;

    GrB_Matrix_nvals (&nentries, A) ;
    GrB_Matrix_nrows (&nrows, A) ;
    GrB_Matrix_ncols (&ncols, A) ;

    printf (
        "\n%% GraphBLAS matrix %s: nrows: %.16g ncols %.16g entries: %.16g\n",
        name, (double) nrows, (double) ncols, (double) nentries) ;

    GrB_Index *I = (GrB_Index *) malloc (MAX (nentries,1) * sizeof (GrB_Index));
    GrB_Index *J = (GrB_Index *) malloc (MAX (nentries,1) * sizeof (GrB_Index));
    int *X = (int *)
        malloc (MAX (nentries,1) * sizeof (int)) ;

    // if (Complex == GxB_FC64)
    // {
    //     GxB_Matrix_extractTuples_FC64 (I, J, X, &nentries, A) ;
    // }
    // else
    // {
    //     GrB_Matrix_extractTuples_UDT (I, J, X, &nentries, A) ;
    // }

GrB_Matrix_extractTuples_INT32(I, J, X, &nentries, A) ;
    printf ("%s = sparse (%.16g,%.16g) ;\n", name,
        (double) nrows, (double) ncols) ;
    for (int64_t k = 0 ; k < nentries ; k++)
    {
        printf ("    %s (%.16g,%.16g) =  (%20.16g) + (%20.16g)*1i ;\n",
            name, (double) (1 + I [k]), (double) (1 + J [k]),
            creal (X [k]), cimag (X [k])) ;
    }
    printf ("%s\n", name) ;

    free (I) ;
    free (J) ;
    free (X) ;
}

//------------------------------------------------------------------------------
// C = A*B for complex matrices
//------------------------------------------------------------------------------

int main (int argc, char **argv)
{
    printf("\n------------start------------------\n");
    printf("%s\n",argv[0]);//print command
        int i = 1;
        while(i < argc){printf("%s\t",argv[i]);i++;}
    //--------------------------------------------------------------------------
    // initializations
    //--------------------------------------------------------------------------

    GrB_Info info ;
    GrB_Matrix A = NULL, C = NULL ;
    PageRank *Pd = NULL, *P2 = NULL ;
    iPageRank *Pi = NULL ;
    FILE *Afile = NULL;

    double tic [2], t ;
    OK (GrB_init (GrB_NONBLOCKING)) ;
    int nthreads ;
    OK (GxB_Global_Option_get (GxB_GLOBAL_NTHREADS, &nthreads)) ;
    // fprintf (stderr, "\npagerank_demo: nthreads: %d\n", nthreads) ;
    printf  (        " nthreads: %d\n", nthreads) ;

    //--------------------------------------------------------------------------
    // read a matrix from stdin
    //--------------------------------------------------------------------------
    Afile = fopen (argv [1], "r") ;
    double t1 ;
    
    GrB_Semiring semiring = GrB_PLUS_TIMES_SEMIRING_INT32 ;


    bool one_based = true ;
    if (argc > 1) one_based = strtol (argv [1], NULL, 0) ;
    printf("reading file: %s\n",argv[1]);


    OK (read_matrix (&A, stdin, false, true, one_based, true,
            false)) ;

    // OK (read_matrix (&A,
    //     Afile,      // read matrix from stdin
    //     false,      // unsymmetric
    //     false,      // self edges OK if true, then remove self edges from A
    //     one_based,  // 0-based or 1-based, depending on input arg
    //     true,       // read input file as Boolean
    //     false)) ;    // print status to stdout


    printf("reading file: finish!\n");


///////////////////////////////add self edge  test GrB_Matrix_new

    // OK (GrB_Matrix_new (&A, GrB_BOOL, 10, 10)) ;
    // GrB_Index nrows, ncols, nentries ;
    // GrB_Matrix_nvals (&nentries, A) ;
    // GrB_Matrix_nrows (&nrows, A) ;
    // GrB_Matrix_ncols (&ncols, A) ;
    // printf ("nentries: %ld   nrows : %ld ncols : %ld\n",  nentries, nrows,ncols) ;

    // for(int i=0; i<10; i++){
    //     OK (GrB_Matrix_setElement_BOOL (A, 1, i, i)) ;
    // }
    //  for(int i=0; i<9; i++){
    //     OK (GrB_Matrix_setElement_BOOL (A, 1, i, i+1)) ;
    // }
       
///////////////////////////////add self edge

//     fclose (Afile) ;

//     Afile = NULL ;


    GrB_Index anrows, ancols, bnrows, bncols, anvals, bnvals ;
    OK (GrB_Matrix_nrows (&anrows, A)) ;
    OK (GrB_Matrix_ncols (&ancols, A)) ;
    OK (GrB_Matrix_nvals (&anvals, A)) ;
    

    //--------------------------------------------------------------------------
    // ADD self edge
    //--------------------------------------------------------------------------

    for(int i=0; i<ancols; i++){
        OK (GrB_Matrix_setElement_BOOL (A, 0, i, i)) ;
    }
    OK (GrB_Matrix_nrows (&anrows, A)) ;
    OK (GrB_Matrix_ncols (&ancols, A)) ;
    OK (GrB_Matrix_nvals (&anvals, A)) ;
    //--------------------------------------------------------------------------
    // trans to undirect : make sure nrow == ncol, (add 0 0 0,nnn to mtx )
    //--------------------------------------------------------------------------
//     OK (GrB_Matrix_nvals (&anvals, A)) ;
//     auto nentries = anvals;
//     GrB_Index *I = (GrB_Index *) malloc (MAX (nentries,1) * sizeof (GrB_Index));
//     GrB_Index *J = (GrB_Index *) malloc (MAX (nentries,1) * sizeof (GrB_Index));
//     int *X = (int *)
//         malloc (MAX (nentries,1) * sizeof (int)) ;

//     if (Complex == GxB_FC64)
//     {
//         GxB_Matrix_extractTuples_FC64 (I, J, X, &nentries, A) ;
//     }
//     else
//     {
//         GrB_Matrix_extractTuples_BOOL (I, J, X, &nentries, A) ;
//         printf ("_UDT ,%ld \n",nentries) ;
//     }

// // GrB_Matrix_extractTuples_INT32(I, J, X, &nentries, A) ;

//     printf ("sparse (%.16g,%.16g) ;\n",
//         (double) anrows, (double) ancols) ;
//     for (int64_t k = 0 ; k < nentries ; k++)
//     {
//         // printf ("    (%.16g,%.16g) =  (%20.16g) + (%20.16g)*1i ;\n",
//         //     (double) (1 + I [k]), (double) (1 + J [k]),
//         //     creal (X [k]), cimag (X [k])) ;
//         printf("%ld , %ld\n", I [k], J[k]);
//     }
//     printf ("finish \n") ;

//     free (I) ;
//     free (J) ;
//     free (X) ;
    //--------------------------------------------------------------------------
    // C = MxM (A,B)
    //--------------------------------------------------------------------------

// import_test (&A, 0, 0);

    printf ("Before nentries: %ld   nrows : %ld ncols : %ld\n",  anvals, anrows,ancols) ;
   
    // import_test (&A, 0, 0);

    //--------------------------------------------------------------------------
    // C = MxM (A,B)
    //--------------------------------------------------------------------------

    // OK (GrB_Matrix_new (&C, GrB_BOOL, anrows , ancols )) ;
    // GrB_mxm (A, NULL, NULL, semiring, A, A, NULL) ; ////////////////semiring is right

    // // import_test (&A, 0, 0);
    // GrB_mxm (A, NULL, NULL, semiring, A, A, NULL) ; ////////////////semiring is right
    // // import_test (&A, 0, 0);
    // GrB_mxm (A, NULL, NULL, semiring, A, A, NULL) ; ////////////////semiring is right
    // // import_test (&A, 0, 0);
    // GrB_mxm (A, NULL, NULL, semiring, A, A, NULL) ; ////////////////semiring is right
    // // import_test (&A, 0, 0);



GrB_Index cnrows, cncols, cnvals ;
simple_tic (tic) ;
    for(int i=0; i<ancols; i++){
         printf ("%d round, nvals: %ld\n",i, anvals);

        OK (GrB_Matrix_nvals (&anvals, A)) ;
        GrB_mxm (A, NULL, NULL, semiring, A, A, NULL) ;
        OK (GrB_Matrix_nvals (&cnvals, A)) ;

        if (cnvals == anvals)
        {
            break;
        }
        
    }
    t = simple_toc (tic) ;  

    
    // OK (GrB_Matrix_nrows (&cnrows, C)) ;
    // OK (GrB_Matrix_ncols (&cncols, C)) ;
    // OK (GrB_Matrix_nvals (&cnvals, C)) ;
// import_test (&C, 0, 0);
    OK (GrB_Matrix_nvals (&anvals, A)) ;

    printf ("After nentries: %ld   nrows : %ld ncols : %ld\n Time(ms):\n %f\n",
     anvals-anrows, anrows,ancols, t*1000000) ;

    //  import_test (&A, 0, 0);
    return 0;
}