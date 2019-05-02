
This archive contains 9 data files, each one corresponding to a parity check matrix from the AR4JA family of codes.  

The filename should indicate which code each is for.

In all cases the variable nodes to be punctured are the (M) variable nodes with highest indices.

All files were created on a computer running Microsoft Windows OS.

Alist format: is a text (ASCII) format to store the sparse matrix popularized by David MacKay, Matthew Davey, 
and John Lafferty.  My 'C' language routine for writing out the alist file begins...

	fprintf( outfp, "%d %d\n", N, M);
	fprintf( outfp, "%d %d\n", maxvardegree, maxcheckdegree);

	for (n=0 ; n<N ; n++)
	{
		degree = 0;
		for (m=0 ; m< M ; m++)
			if ( H[m*N + n] )
				degree++;
		fprintf( outfp, "%d ", degree);  // degree of each col
	}
	fprintf( outfp, "\n");

	for (m=0 ; m< M ; m++)
	{
		degree = 0;
		for (n=0 ; n<N ; n++)
			if ( H[m*N + n] )
				degree++;
		fprintf( outfp, "%d ", degree);  // degree of each row
	}
	fprintf( outfp, "\n");

Matlab format: is a binary format written by MathWorks' MATLAB 7.7 (R2008b) program.  
The file was created by using the native format for sparse matrices in MATLAB.  
Assume H is in the MATLAB workspace as a double precision floating point, sparse 
matrix; then issue the command "save AR4JA_r45_k1024 H"

Brian Butler, Feb 2010 (original README)
              Mar 2011 (updated to README v2 to fix reversed degree fields)
