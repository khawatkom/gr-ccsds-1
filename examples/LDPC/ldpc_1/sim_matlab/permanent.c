/*
 * permanent.c   Matlab MEX C-programming
 *   Compute the permanent of a square matrix.  This routine seems to 
 * be efficient for sparse matrices.
 *
 * Copyright 2013 Brian K. Butler
 */
/* Installation instructions:  First run "mex -setup" in MATLAB.
 * This routine compiles with either LCC and Microsoft Visual C++ 2008,
 * Express Edition.  The latter runs a little faster (~5%).  Compile
 * using "mex -v permanent.c".  Ready to run a test...
 *
 * Testing: Run the following in MATLAB
 *
    A=[1,0,0,0,0,0,0,0,1,0,0,1;...
        0,1,0,0,0,0,0,0,1,1,0,0;...
        0,0,1,0,0,0,0,0,0,1,1,0;...
        0,0,0,1,0,0,0,0,0,0,1,1;...
        0,0,0,0,1,0,0,0,1,1,1,0;...
        0,0,0,0,0,1,0,0,0,1,1,1;...
        0,0,0,0,0,0,1,0,1,0,1,1;...
        0,0,0,0,0,0,0,1,1,1,0,1;...
        0,0,0,0,1,1,0,0,1,0,0,0;...
        0,0,0,0,0,1,1,0,0,1,0,0;...
        0,0,0,0,0,0,1,1,0,0,1,0;...
        0,0,0,0,1,0,0,1,0,0,0,1];
    start=tic; p=permanent(A); t=toc(start)
 *
 * The resulting permanent should be 89.  
 */
 
#include "matrix.h"
#include "mex.h"
#define A_IN (prhs[0])

static double local_perm(int currow);
static void   prnsorted( );
static int    m=0;  // matrix order
static double *a;   // pointer to matrix data
static int    *crflgs;  // col-removal flags

/* the gateway function */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
    int n;
	double *pp;
    
    if (nrhs != 1)
        mexErrMsgTxt("Function 'permanent' requires 1 input.\n");
    if (nlhs > 1)
        mexErrMsgTxt("Function 'permanent' only defined for 0 or 1 output.\n");
    if (!mxIsDouble(A_IN) || mxIsComplex(A_IN))
    {
        mexPrintf("Input is of type:\t%s\n",mxGetClassName(A_IN));
        mexErrMsgTxt("Function 'permanent' only uses real double input.\n");
    }
    m = mxGetM(A_IN);
    n = mxGetN(A_IN);

    //mexPrintf("num rows and cols %d %d\n", m, n);
	if (m != n)
    {
        mexErrMsgTxt("Must be square.  Error inside permanent()\n");
    }

	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	pp = mxGetPr(plhs[0]);

    if (m == 1)
	{
		*pp = *mxGetPr(A_IN);
	}
	else 
	{
        crflgs = (int *)mxCalloc(m, sizeof(int));
        a = mxGetPr(A_IN);
		*pp = local_perm(0);
        mxFree(crflgs);
	}
}

double local_perm(int currow) {
// 'currow' indicates the current row, which we are to follow in the cofactor expansion.
    double p=0.0;
    int j;
    
    //mexPrintf("local_perm %d %d\t", m, currow);
    //if (rstart >= 0) {
    //	mexPrintf("with entries\t "); prnsorted( );
    //}
    if (m-currow == 1)  // down to a 1x1
    {
        for (j=0; j<m; j++) {
            if (crflgs[j])
                continue;
            
            p = a[currow*m+j];
            break;
        }
        //mexPrintf("1x1 p=%f \n", p);
    }
    else {
        for (j=0; j<m; j++) {
            if (crflgs[j])
                continue;
            
            if (a[currow*m+j]) {
                crflgs[j]=1;
                p = p + a[currow*m+j] * local_perm(currow+1);
                crflgs[j]=0;
            }
        } // end for
    } //end else
    //mexPrintf("%dx%d perm=%f \n", m-currow,m-currow,p);
    return (p);
}

void prnsorted( ) 
{
    int j;
    
    for (j=0; j<m; j++) {
        if (crflgs[j])
            continue;       
        mexPrintf("%d  ", j);
    }
    mexPrintf("\n");
}
