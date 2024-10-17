//random definitions I find useful, random number generators
#include <StdAfx.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "mark_math.h"


//RANDOM NUMBER GENERATORS
//uniform dist. rand between 0.0 and 1.0(excluding endpts)
double ran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	double temp;

	if (*idum <= 0 || !iy) {		//Initialize
		if (-(*idum) < 1) *idum=1;  //be sure to prevent idum=0
		else *idum = -(*idum);
		for (j=NTAB+7; j>=0; j--) { //load the shuffle table (after 8 warm-ups)
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;							//start here when not initializing
	*idum=IA*(*idum-k*IQ)-IR*k;				//Compute idum=(IA*idum) % IM without overflows by Schrage's method
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;								//will be in the range 0..NTAB-1
	iy=iv[j];								//output previously stored value and refill the shuffle table.
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;	//Because users don't expect endpoint values
	else return temp;
}


//exp. dist. rand w/unit mean, using ran1(idum) as source of uniform deviates
double RandExp(long *idum) 
{
	double ran1(long *idum);
	double dum;

	do 
		dum=ran1(idum);
	while (dum == 0.0);
	return -log(dum);
}


//normal dist. rand w/mean zero, variance 1, using ran1(idum) 
double RandGauss(long *idum)
{
	
	//float ran1(long *idum);
	static int iset = 0;
	static double gset;
	double fac, rsq, v1, v2;

	if (iset == 0) {
		do {
			v1 = 2.0 * ran1(idum) - 1.0;
			v2 = 2.0 * ran1(idum) - 1.0;
			rsq = v1*v1 + v2*v2;
		}
		while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0*log(rsq)/rsq);
		gset = v1*fac;
		iset = 1;
		return v2*fac;
	}
	else {
		iset = 0;
		return gset;
	}
}


//DATA TYPES -- matrices of doubles for number-crunching (from num. recipes)
void nrerror(char error_text[]) { //Numerical recipes standard error handler
	fprintf(stderr, "Numerical Recipes run-time error...\n");
	fprintf(stderr, "%s\n",error_text);
	fprintf(stderr, "...now exiting to system...\n");
	exit(1);
}

double *vector(long nl, long nh) {
//allocate a double vector with subscript range v[nl..nh]
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(double *v, long nl, long nh) {
//free a double vector allocated with vector()
	free((FREE_ARG) (v+nl-NR_END));
}

double **matrix(long nrl, long nrh, long ncl, long nch)
// allocate a double matrix with subscript range m[nrl..nrh][ncl..nch]
{
	long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
	double **m;

	//allocate pointers to row
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) printf("allocation failure 1 in matrix()--whatever the hell that means\n");
	m += NR_END;
	m -= nrl;

	//allocate rows and set pointers to them 
	m[nrl] = (double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()--whatever the hell that means");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	//return pointer ro array of pointers to rows
	return m;
}

void free_matrix(double **m, long nrl, long nrh, long ncl, long nch)
// free a double matrix allocated by matrix()
{
	free((FREE_ARG) (m[nrl] + ncl - NR_END));
	free((FREE_ARG) (m + nrl - NR_END));
}

//USEFUL FUNCTIONS
//returns max of a and b
double Max(double a, double b) {
	if (a > b)
		return(a);
	else return(b);
}

int Factorial(int n) {
	int nFactorial;
	if (n < 0) AfxMessageBox ("Factorial of number < 0 requested!");
	if (n < 2) { 
		nFactorial = 1;
	} else {
		nFactorial = 1;
		for (int i = 2; i <= n; i++) {
			nFactorial *= i;
		}
	}
	return(nFactorial);
}

double Poisson(int N, double AveN) { //value of Poisson distribution P(N)
	return(pow(AveN,N)*exp(-AveN)/Factorial(N));
}

double Binomial(int n, int N, double p) { //value of Binomial dist. P(n,N), n<N
	if (n > N) AfxMessageBox("Binomial n is > N");
	return(Factorial(N)*pow(p,n)*pow(1-p,(N-n))/(Factorial(n)*Factorial(N-n)));
}

//returns mean of elements of vector
double Mean(double *Vect, long NumElements) {
	if (NumElements < 1) {
		AfxMessageBox("0 elements: can't compute mean");
		return(-1.0);
	} else {
		double Ave = 0.0;
		for (long i = 0; i < NumElements; i++) {
			Ave += Vect[i];
		}
		Ave /= (double)NumElements;
		return(Ave);
	}
}

double StdDev(double *Vect, long NumElements) {
	if (NumElements < 2) return(-1.0);
	else {
		double Sigma;
		double Variance;
		double Sum = 0.0;
		double SquaresSum = 0.0;
		for (long i = 0; i < NumElements; i++) {
			Sum += Vect[i];
			SquaresSum += Vect[i]*Vect[i];
		}
		Variance = (SquaresSum - (Sum*Sum/(double)NumElements))/((double)NumElements - 1.0);
		Sigma = sqrt(Variance);
		return(Sigma);
	}
}

//Numerical recipes moment/cumulant computer: but I index from 0 to n-1, not 1 to n
void moment(double *data, long n, double *ave, double *adev, double *sdev, double *var, double *skew, double *curt) {
//Given an array of data, this routine return mean "ave", average deviation "adev", standard deviation "sdev"
//variance "var", skewness "skew", and kurtosis "curt"
	void nrerror(char error_text[]);
	long j;
	double ep = 0.0, s, p;

	if (n <= 1) nrerror("n must be at least 2 in moment");
	s = 0.0;								//1st pass to get the mean
	for (j=0; j < n; j++) s += data[j];
	*ave = s/n;
	*adev=(*var)=(*skew)=(*curt)=0.0;		//2nd pass to get the first (absolute), 2nd, 3rd, & 4th moments
	for (j=0; j < n; j++) {					//of the deviation from the mean
		*adev += fabs(s=data[j]-(*ave));
		ep += s;
		*var += (p=s*s);
		*skew += (p *= s);
		*curt += (p *= s);
	}
	*adev /= n;
	*var=(*var-ep*ep/n)/(n-1);				//Corrected 2-pass formula
	*sdev=sqrt(*var);						//Put the pieces together according to the conventional def.'s
	if (*var) {
		*skew /= (n*(*var)*(*sdev));
		*curt=(*curt)/(n*(*var)*(*var))-3.0;
	} else nrerror("No skew/kurtosis when variance = 0 (in moment)");
}

double ISIComputer(double *ISIList, double *SpikeList, long NumSpikes) {
	double MaxISI = 0.0;
	for (int i = 0; i < NumSpikes - 1; i++) {
		ISIList[i] = SpikeList[i+1] - SpikeList[i];
		if (ISIList[i] > MaxISI) 
			MaxISI = ISIList[i];
	}
	return(MaxISI);
}

void Histogram(double MaxEntry, int NumBins, long ListLength, double *List, double *hist) {
//MaxEntry is the largest entry in the bin	
	double BinSize = MaxEntry/NumBins;
	for (int bin = 0; bin < NumBins; bin++) {
		hist[bin] = 0.0;
	}
	for (int i=0; i < ListLength; i++) {
		bin	= (int)(List[i]/BinSize);
		hist[bin] += 1.0;
	}

}

//Next routine multiplies a matrix times a vector and places resulting vector in result
void MatrixMult(double* Result, double** Matrix, double* Vector, int NumRows, int NumCols) {
	for (int i = 0; i < NumRows; i++) {
		Result[i] = 0.0;
		for (int j = 0; j < NumCols; j++) {
			Result[i] += Matrix[i][j]*Vector[j];
		}
	}
}

//next routine is hardwired for a 2x2 matrix right now
void Eigenvals(double* Lambda, double** M) {
	Lambda[0] = (0.5*((M[0][0] + M[1][1]) + sqrt(SQR(M[0][0] - M[1][1]) + 4.*M[0][1]*M[1][0])));
	Lambda[1] = (0.5*((M[0][0] + M[1][1]) - sqrt(SQR(M[0][0] - M[1][1]) + 4.*M[0][1]*M[1][0])));
}

//next routine is hardwired for a 2x2 matrix right now
//actually returns the change of basis (FROM eigenbasis) matrix S, whose columns are the eigenvects
//Note: eigenvects NOT normalized
void Eigenvects(double** S, double** M) {
	double Lambda[2];  //hardwired at 2 for now
	Eigenvals(Lambda, M);
	S[0][0] = -M[0][1];	
	S[0][1] = -M[0][1];
	S[1][0] = M[0][0] - Lambda[0];
	S[1][1] = M[0][0] - Lambda[1];
}

//receives matrix M, returns eigenvals Lambda and eigenvects as columns of change of basis
//matrix (FROM eigenbasis) S
//Note: eigenvects NOT normalized
void Eigensystem(double** S, double* Lambda, double** M) {
	Lambda[0] = (0.5*((M[0][0] + M[1][1]) + sqrt(SQR(M[0][0] - M[1][1]) + 4.*M[0][1]*M[1][0])));
	Lambda[1] = (0.5*((M[0][0] + M[1][1]) - sqrt(SQR(M[0][0] - M[1][1]) + 4.*M[0][1]*M[1][0])));
	S[0][0] = -M[0][1];	
	S[0][1] = -M[0][1];
	S[1][0] = M[0][0] - Lambda[0];
	S[1][1] = M[0][0] - Lambda[1];
}

//next routine is hardwired for a 2x2 matrix right now
double Determinant(double** Matrix) {
	return ((Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0]));
}

//next routine is hardwired for a 2x2 matrix right now; receives M, puts inverse in Inverse
void Inverse(double** Inverse, double** M) {
	double det = Determinant(M);
	Inverse[0][0] = M[1][1]/det;
	Inverse[0][1] = -M[0][1]/det;
	Inverse[1][0] = -M[1][0]/det;
	Inverse[1][1] = M[0][0]/det;
}