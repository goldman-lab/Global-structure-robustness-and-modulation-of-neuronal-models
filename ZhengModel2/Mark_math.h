//Assorted useful math functions, many from Numerical Recipes

//MISCELLANEOUS
#define AND &&
#define OR ||
#define FALSE 0
#define TRUE 1
#define _PI (3.141592653589793238462)
static double sqrarg;  //next 2 lines is square fn. as stolen from num. recipes in C
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)
#define UA_TO_NA (1.0e3) //=uA/nA
#define CM_TO_UM (1.0e4) //= cm/um
#define S_TO_mS (1.0e3) //= S/mS

//FOR RANDOM NUMBER GENERATORS
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS (1.2e-7)
#define RNMX (1.0-EPS)

//for data types
#define NR_END 1
#define FREE_ARG char*

//Random number generators--idum is a pointer to the seed
double ran1(long *idum);  //uniform 0.0 to 1.0
double RandExp(long *idum);  //Exponential, mean 1
double RandGauss(long *idum); //Gaussian, mean 0, variance 1

//Data types
void nrerror(char error_text[]); //Numerical recipes standard error handler
double *vector(long nl, long nh);
void free_vector(double *v, long nl, long nh);
double **matrix(long nrl, long nrh, long ncl, long nch);
void free_matrix(double **m, long nrl, long nrh, long ncl, long nch);

//Useful functions
double Max(double a, double b); //maximum of a and b
int Factorial(int n);
double Poisson(int N, double AveN); //Poisson distribution as a function of N
double Binomial(int n, int N, double p); //Binomial distribution as a fn. of n < N
double Mean(double *Vect, long NumElements);
double StdDev(double *Vect, long NumElements);
void moment(double *data, long n, double *ave, double *adev, double *sdev, double *var, double *skew, double *curt);
double ISIComputer(double *ISIList, double *SpikeList, long NumSpikes); //returns largest ISI
void Histogram(double MaxEntry, int NumBins, long ListLength, double *List, double *hist);

//Matrix manipulation routines
void MatrixMult(double* Result, double** Matrix, double* Vect, int NumRows, int NumCols);
void Eigenvals(double* Result, double** Matrix);
void Eigenvects(double** Result, double** Matrix);
void Eigensystem(double** S, double* Lambda, double** M);
double Determinant(double** Matrix);
void Inverse(double** Result, double** Matrix);