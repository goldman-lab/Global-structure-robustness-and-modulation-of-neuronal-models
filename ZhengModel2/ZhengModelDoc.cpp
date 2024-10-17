// ZhengModelDoc.cpp : implementation of the CZhengModelDoc class
//

#include "stdafx.h"
#include "fstream"
using namespace std;
#include "iostream"
#include "stdio.h"
#include "ZhengModelHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZhengModelDoc

IMPLEMENT_SERIAL(CZhengModelDoc, CDocument, _VERSION_NUMBER)

BEGIN_MESSAGE_MAP(CZhengModelDoc, CDocument)
	//{{AFX_MSG_MAP(CZhengModelDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZhengModelDoc construction/destruction

CZhengModelDoc::CZhengModelDoc()
{
	m_dtmin = _DT;
	m_dtmax = _DT;
	m_dt = _DT;
	for (int i = 0; i < _MAX_COMPARTS; i++) {
		m_IInject[i] = 0.0;
		for (int step = 0; step < _MAX_INJ_TIMES; step++) {
			m_MaxInj[i][step] = 0.0;
		}
	}
	m_pTheNeuron = new CNeuron;
	m_bEndRun = FALSE;
}

CZhengModelDoc::~CZhengModelDoc()
{
	
	delete m_pTheNeuron;
}

BOOL CZhengModelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CZhengModelDoc diagnostics

#ifdef _DEBUG
void CZhengModelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZhengModelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZhengModelDoc commands

BOOL CZhengModelDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{	
	if (!CDocument::OnOpenDocument(lpszPathName)) {
		return FALSE;
	}
	
	
	return TRUE;
}

void CZhengModelDoc::DeleteContents() 
{
	/*if (m_bOpenCase) {
	/*for (int i=0; i<_MAX_COMPARTS; i++) {
		for (int j=0; j<_MAX_CHANNELS; j++) {
			delete GetCompart(i)->m_ChanArray[j];
		}
		GetCompart(i)->m_ChanArray.RemoveAll();
	}

		delete m_pTheNeuron;
	}*/
	
	CDocument::DeleteContents();
}


BOOL CZhengModelDoc::PeekAndPump()
{
	MSG msg;

	while (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}
	}
	return TRUE;
}

void CZhengModelDoc::RunEngine(double tmax, BOOL SaveFlag, BOOL DisplayFlag, double InjOnTime, double InjOnTime2, double InjOffTime)
{
	//int num_steps = (int)(tmax/dt);
	//int num_steps_per_screen = (int)(_TIME_PER_SCREEN/dt); //number of calculated steps/screenful
	//int graph_frequency = num_steps_per_screen/_POINTS_PER_SCREEN; //integer division(drops remainder)
	//int gmax_update_frequency = (int)(_TIME_PER_GMAX_UPDATE/dt); //number of steps between updates
	
	double ScaleFactorNa = 1.0;
	double ScaleFactorCaT = 1.0;
	double ScaleFactorCaP = 1.0;
	double ScaleFactorA = 1.0;
	double ScaleFactorKCa = 1.0;
	double ScaleFactorKd = 1.0;
	double* SpikeTimes;
	SpikeTimes = vector(0, _MAX_SPIKES - 1);
	double* ISIList;
	ISIList = vector(0, _MAX_SPIKES - 1);
	double* ISIHistogram; //histogram of ISI's
	ISIHistogram = vector(0, _NUM_BINS - 1);
	double MeanISI, SigmaISI, CVISI;
	int MeanISIbin;
	double MaxISI = 0.0;
	//double ave, adev, sdev, var, skew, kurt;
	double AveRate = 0.0;
	double BurstRate = -1.0;
	double ZeroSpikeBurstRate = 0.0;
	double SpikesPerBurst = -1.0;
	long NumISISpike = 0;
	long NumISIBurst = 0;
	double MeanISIBurst = 0.0;
	double MeanISISpike = 0.0;
	double num_steps;
	double DCSensorAveVal = 0.0;
	double SlowSensorAveVal = 0.0;
	double FastSensorAveVal = 0.0;
	long NumSpikes = 0;
	int NumSilent = 0; //Number of runs classified as silent cells
	int NumBurst = 0;
	int NumTonic = 0;
	time_t timet;
	long seed = -(long)time(&timet);
	//long seed = _SEED;
	BOOL SpikeFlag = TRUE;
	BOOL JustSpikedFlag = FALSE;
	BOOL BurstSearchFlag = FALSE;
	BOOL BurstFlag = FALSE;
	double DataTakingTime;
	double TotalBurstAmplitude;
	double AmplitudePerBurst;
	double Vmin, VAve;
	BOOL NewBurstFlag = TRUE; //for 0-spike burster identification

	char str[5000];
	ofstream SummaryFile("SummaryData.dat");
	if (!SummaryFile) cerr << "couldn't open file StateGrid.dat" << endl;
	//ofstream SensorFile("SensStateDiagram.dat");
	//if (!SensorFile) cerr << "couldn't open sensor state diagram data file" << endl;
	ofstream VandIFile("VandIFile.dat");
	if (!VandIFile) cerr << "couldn't open file VandIFile.dat" << endl;
	ofstream PRCFile("PRC1.05HzEPSP2.dat"); //Phase response curve file with phase out vs. phase in of IPSP
	if (!PRCFile) cerr << "couldn't open file PRC.dat" << endl;

	//FOR PHASE RESPONSE CURVE COMPUTATION
	BOOL PRCDone;
	BOOL ComputingPhaseResponse;
	BOOL TestPulseFired;
	double PhaseStartTime,PhaseIn,PhaseOut,Period;
	int NumPhasePoints = 1;

	for (double IPSPSize = 0.0; IPSPSize < 0.0055; IPSPSize += 0.1) {
		GetSyn(_Soma, _Inhib)->m_gStep = IPSPSize;
	for (int ThisPhasePoint = 0; ThisPhasePoint < NumPhasePoints; ThisPhasePoint++) {
	PRCDone = FALSE;
	ComputingPhaseResponse = FALSE;
	TestPulseFired = FALSE;
	ScaleFactorNa = 1.0;
	ScaleFactorCaT = 1.0;
	ScaleFactorCaP = 1.0;
	ScaleFactorA = 1.0;
	ScaleFactorKCa = 1.0;
	ScaleFactorKd = 1.0;
	double gMinNa = 0.0;
	double gMinCaT = 0.0;
	double gMinCaP = 0.0;
	double gMinA = 0.0;
	double gMinKCa = 0.0;
	double gMinKd = 0.0;
	//for 1SBurster analysis
	double MeanNa = 283.15;
	double MeanCaT = 3.45;
	double MeanCaP = 2.76;
	double MeanA = 26.24;
	double MeanKCa = 145.60;
	double MeanKd = 37.95;
	double SigmaNa = 240.67;
	double SigmaCaT = 1.18;
	double SigmaCaP = 0.94;
	double SigmaA = 20.11;
	double SigmaKCa = 88.57;
	double SigmaKd = 50.62;
	double gNa;
	double gCaT; 
	double gA;
	double gKCa;
	double gKd; 
	double EllipseSum;
	//For covariance analysis (again of 1SB)
	double Means[5];
	Means[0] = MeanNa; Means[1] = MeanCaT; Means[2] = MeanA; Means[3] = MeanKCa; Means[4] = MeanKd;
	double StdDevs[5];
	StdDevs[0] = 242.16; StdDevs[1] = 88.53; StdDevs[2] = 43.15; StdDevs[3] = 19.83; StdDevs[4] = 1.10;
	double gMaxVect[5];
	double DotProduct;
	double **EigenMatrix;
	EigenMatrix = matrix(0, 4, 0, 4); //holds eigenvectors in columns
	EigenMatrix[0][0] = 0.9936; EigenMatrix[0][1] = -0.0147; EigenMatrix[0][2] = 0.1114; EigenMatrix[0][3] = -0.0057; EigenMatrix[0][4] = 0.0009;
	EigenMatrix[1][0] = -0.0007; EigenMatrix[1][1] = 0.0009; EigenMatrix[1][2] = -0.0024; EigenMatrix[1][3] = -0.0188; EigenMatrix[1][4] = 0.9998;
	EigenMatrix[2][0] = 0.0031; EigenMatrix[2][1] = 0.0179; EigenMatrix[2][2] = -0.0757; EigenMatrix[2][3] = -0.9968; EigenMatrix[2][4] = -0.0189;
	EigenMatrix[3][0] = -0.0136; EigenMatrix[3][1] = -0.9997; EigenMatrix[3][2] = -0.0119; EigenMatrix[3][3] = -0.0171; EigenMatrix[3][4] = 0.0005;
	EigenMatrix[4][0] = -0.1117; EigenMatrix[4][1] = -0.0090; EigenMatrix[4][2] = 0.9908; EigenMatrix[4][3] = -0.0758; EigenMatrix[4][4] = 0.0009;


	//FOR MANY RUN RANDOM
	for (int ThisRun = 0; ThisRun < 1; ThisRun++) {
	
	//FOR GRID
	
	//for (double NaGmax = 100.0; NaGmax < 750.0; NaGmax += 100.0) {
	/*
	GetChan(_Soma, _I_Na)->m_gMax = 200.0;//NaGmax;
	for (double CaTGmax = .625; CaTGmax < 4.5; CaTGmax += .625) {
		GetChan(_Soma, _I_CaT)->m_gMax = CaTGmax;
		GetChan(_Soma, _I_CaP)->m_gMax = 0.8*CaTGmax;
		for (double AGmax = 9.375; AGmax < 70.0; AGmax += 9.375) {
			GetChan(_Soma, _I_A)->m_gMax = AGmax;
			for (double KCaGmax = 37.5; KCaGmax < 275.0; KCaGmax += 37.5) {
				GetChan(_Soma, _I_KCa)->m_gMax = KCaGmax;
				for (double KdGmax = 25.0; KdGmax < 180.0; KdGmax += 25.0) {
					GetChan(_Soma, _I_Kd)->m_gMax = KdGmax;
	*/
		//initialize local counter for average sensor values
		num_steps = 0.0;
		DCSensorAveVal = 0.0;
		SlowSensorAveVal = 0.0;
		FastSensorAveVal = 0.0;
		//sprintf(str, "Run number %d", ThisRun);
		//AfxMessageBox(str);
		for (int i = 0; i < _MAX_SPIKES; i++) {
			SpikeTimes[i] = 0.0;
			ISIList[i] = 0.0;
		}
		AveRate = 0.0;
		NumSpikes = 0;
		//COMPUTE, DISPLAY, AND SAVE INITIAL VALUES, AND SET UP OUTPUT STREAMS
		//initialize V
		double V0[_MAX_COMPARTS];
		for (i = 0; i < _MAX_COMPARTS; i++) {
			V0[i] = GetCompart(i)->m_V0;
			GetCompart(i)->m_V = V0[i];
		}
		//initialize Ca (and corresponding reversal E's)
		double Ca0[_MAX_COMPARTS];
		for (i = 0; i < _MAX_COMPARTS; i++) {
			Ca0[i] = GetCompart(i)->m_Ca0;
			GetCompart(i)->m_Ca = Ca0[i];
			//update Ca channel reversal potentials
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				if (GetChan(i, chan)->m_bCaChannel) {
					if (GetChan(i, chan)->m_bExists) {
						GetChan(i, chan)->m_Update_E(Ca0[i]);
					}
				}
			}
		}
		//initialize gmax's
		//FOR RANDOM POINT RUN USING MANYRUNINITIAL3 AS INITIAL DATA
		//or, for 1 spike bursters, using ManyRunMean1SB as initial data
		
		for (i = 0; i < _MAX_COMPARTS; i++) {
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				switch (chan) {
					//commented lines are for DON'T VARY Na, Kd
					case _I_Na:  GetChan(i, chan)->m_gMax = gMinNa + ScaleFactorNa * GetChan(i, chan)->m_gMax0; break;
					//case _I_Na:  GetChan(i, chan)->m_gMax = gMinNa + 1.0 * GetChan(i, chan)->m_gMax0; break;
					case _I_A:  GetChan(i, chan)->m_gMax = gMinA + ScaleFactorA * GetChan(i, chan)->m_gMax0; break;
					case _I_KCa:  GetChan(i, chan)->m_gMax = gMinKCa + ScaleFactorKCa * GetChan(i, chan)->m_gMax0; break;
					case _I_Kd:  GetChan(i, chan)->m_gMax = gMinKd + ScaleFactorKd * GetChan(i, chan)->m_gMax0; break;
					//case _I_Kd:  GetChan(i, chan)->m_gMax = gMinKd + 1.0 * GetChan(i, chan)->m_gMax0; break;
					case _I_CaT:  GetChan(i, chan)->m_gMax = gMinCaT + ScaleFactorCaT * GetChan(i, chan)->m_gMax0; break;
					case _I_CaP:  GetChan(i, chan)->m_gMax = 0.8 * GetChan(i, _I_CaT)->m_gMax; break;
					default:  GetChan(i, chan)->m_gMax = GetChan(i, chan)->m_gMax0;
				}
			}
		}
		
		//
		
		//Ready the output streams
		/*sprintf(str, "Spikes_%.3g_%.3g_%.3g_%.2g_%.2g.dat", GetChan(_Soma, _I_A)->m_gMax, GetChan(_Soma, _I_KCa)->m_gMax, GetChan(_Soma, _I_Kd)->m_gMax, GetRunPage()->m_MaxInjSoma, GetRunPage()->m_MaxInjSoma2);
		ofstream SpikeFile(str);
		//AfxMessageBox(str);
		if (!SpikeFile) cerr << "couldn't open Spikes data file" << endl;*/
		//Voltage file saving next 3 lines: 
		/*sprintf(str, "V_%.3g_%.3g_%.3g_%.2g_%.2g.dat", GetChan(_Soma, _I_A)->m_gMax, GetChan(_Soma, _I_KCa)->m_gMax, GetChan(_Soma, _I_Kd)->m_gMax, GetRunPage()->m_MaxInjSoma, GetRunPage()->m_MaxInjSoma2);
		ofstream Vfile(str);
		 (!Vfile) cerr << "couldn't open Voltage data file" << endl;*/

		//make a "% error sphere" of radius 1 and reject values that aren't within it -- for next time through the loop
		//do {
		//RANDOM POINT RUN USING MANYRUNINITIAL3 AS INITIAL DATA
		
			ScaleFactorNa = 2.0 * ran1(&seed);
			ScaleFactorCaT = 10.0 * ran1(&seed);
			ScaleFactorCaP = ScaleFactorCaT;
			ScaleFactorA = 5.0 * ran1(&seed);
			ScaleFactorKCa = 6.0 * ran1(&seed);
			ScaleFactorKd = 10.0 * ran1(&seed);
		
		//RANDOM POINT RUN to find covariance of ellipses: cuz rejection quick, just start w/ManyRunInitial3
		//and then enforce fitting in 5-D covariance ellipse
		/*
		do {	
			ScaleFactorNa = 2.0 * ran1(&seed);
			ScaleFactorCaT = 10.0 * ran1(&seed);
			ScaleFactorCaP = ScaleFactorCaT;
			ScaleFactorA = 5.0 * ran1(&seed);
			ScaleFactorKCa = 6.0 * ran1(&seed);
			ScaleFactorKd = 10.0 * ran1(&seed);
			
			gMaxVect[0] = ScaleFactorNa * 400.0;
			gMaxVect[1] = ScaleFactorCaT * 0.5;
			gMaxVect[2] = ScaleFactorA * 15.0;
			gMaxVect[3] = ScaleFactorKCa * 50.0;
			gMaxVect[4] = ScaleFactorKd * 20.0;

			//gMaxVect[0] = GetChan(_Soma, _I_Na)->m_gMax;
			//gMaxVect[1] = GetChan(_Soma, _I_CaT)->m_gMax;
			//gMaxVect[2] = GetChan(_Soma, _I_A)->m_gMax;
			//gMaxVect[3] = GetChan(_Soma, _I_KCa)->m_gMax;
			//gMaxVect[4] = GetChan(_Soma, _I_Kd)->m_gMax;

			EllipseSum = 0.0;
			for (int eigen = 0; eigen <= 4; eigen++) {
				DotProduct = 0;
				for (int chan = 0; chan <= 4; chan++) {
					DotProduct += EigenMatrix[chan][eigen]*(gMaxVect[chan] - Means[chan]);
				}
				EllipseSum += DotProduct*DotProduct/(StdDevs[eigen]*StdDevs[eigen]);
			}
		sprintf(str, "Na = %g, EllipseSum = %g", gMaxVect[0], EllipseSum);
		AfxMessageBox(str);
		} while (EllipseSum > 1.0);
		//sprintf(str, "EllipseSum = %g", EllipseSum);
		//AfxMessageBox(str);
		*/

		//RANDOM POINT RUN USING ManyRunMean1SB as initial data
		/*
		do {
			gMinNa = Max(0, MeanNa - SigmaNa);
			gMinCaT = Max(0, MeanCaT - SigmaCaT);
			gMinA = Max(0, MeanA - SigmaA);
			gMinKCa = Max(0, MeanKCa - SigmaKCa);
			gMinKd = Max(0, MeanKd - SigmaKd);

			ScaleFactorNa = (MeanNa + SigmaNa - gMinNa) * ran1(&seed) / MeanNa;
			ScaleFactorCaT = (MeanCaT + SigmaCaT - gMinCaT) * ran1(&seed) / MeanCaT;
			ScaleFactorA = (MeanA + SigmaA - gMinA) * ran1(&seed) / MeanA;
			ScaleFactorKCa = (MeanKCa + SigmaKCa - gMinKCa) * ran1(&seed) / MeanKCa;
			ScaleFactorKd = (MeanKd + SigmaKd - gMinKd) * ran1(&seed) / MeanKd;
			
			gNa = gMinNa + ScaleFactorNa * MeanNa;
			gCaT = gMinCaT + ScaleFactorCaT * MeanCaT;
			gA = gMinA + ScaleFactorA * MeanA;
			gKCa = gMinKCa + ScaleFactorKCa * MeanKCa;
			gKd = gMinKd + ScaleFactorKd * MeanKd;

			EllipseSum = 0.0;
			EllipseSum += (gNa-MeanNa)*(gNa-MeanNa)/(SigmaNa*SigmaNa);
			EllipseSum += (gCaT-MeanCaT)*(gCaT-MeanCaT)/(SigmaCaT*SigmaCaT);
			EllipseSum += (gA-MeanA)*(gA-MeanA)/(SigmaA*SigmaA);
			EllipseSum += (gKCa-MeanKCa)*(gKCa-MeanKCa)/(SigmaKCa*SigmaKCa);
			EllipseSum += (gKd-MeanKd)*(gKd-MeanKd)/(SigmaKd*SigmaKd);
			//sprintf(str, "EllipseSum = %g", EllipseSum);
			//AfxMessageBox(str);
		} while (EllipseSum > 1.0);
		*/
		//} while ((SQR(ScaleFactorA - 1.0) + SQR(ScaleFactorKCa - 1.0) + SQR(ScaleFactorKd - 1.0)) > 1.0);
			//} while (ScaleFactorNa < 0.063); //ignore Na values < 25
	
		//PUT THIS HERE IF WANT FIRST POINT RANDOM ALSO
		/*
		for (i = 0; i < _MAX_COMPARTS; i++) {
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				switch (chan) {
					//commented lines are for DON'T VARY Na, Kd
					case _I_Na:  GetChan(i, chan)->m_gMax = gMinNa + ScaleFactorNa * GetChan(i, chan)->m_gMax0; break;
					//case _I_Na:  GetChan(i, chan)->m_gMax = gMinNa + 1.0 * GetChan(i, chan)->m_gMax0; break;
					case _I_A:  GetChan(i, chan)->m_gMax = gMinA + ScaleFactorA * GetChan(i, chan)->m_gMax0; break;
					case _I_KCa:  GetChan(i, chan)->m_gMax = gMinKCa + ScaleFactorKCa * GetChan(i, chan)->m_gMax0; break;
					case _I_Kd:  GetChan(i, chan)->m_gMax = gMinKd + ScaleFactorKd * GetChan(i, chan)->m_gMax0; break;
					//case _I_Kd:  GetChan(i, chan)->m_gMax = gMinKd + 1.0 * GetChan(i, chan)->m_gMax0; break;
					case _I_CaT:  GetChan(i, chan)->m_gMax = gMinCaT + ScaleFactorCaT * GetChan(i, chan)->m_gMax0; break;
					case _I_CaP:  GetChan(i, chan)->m_gMax = 0.8 * GetChan(i, _I_CaT)->m_gMax; break;
					default:  GetChan(i, chan)->m_gMax = GetChan(i, chan)->m_gMax0;
				}
			}
		}
		*/

		//start channel activation/inactivation at steady-state
		for (i = 0; i < _MAX_COMPARTS; i++) {
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				if ((GetChan(i, chan)->m_p) != 0)
					GetChan(i, chan)->m_m = GetChan(i, chan)->m_m_inf(V0[i], Ca0[i]);
				if ((GetChan(i, chan)->m_q) != 0)
					GetChan(i, chan)->m_h = GetChan(i, chan)->m_h_inf(V0[i]);
			}
		}
		//start sensor activation/inactivation at steady-state
		for (i = 0; i < _MAX_COMPARTS; i++) {
			for (int s = 0; s < _MAX_SENSORS; s++) {
				if ((GetSensor(i, s)->m_p) != 0)
					GetSensor(i, s)->m_m = GetSensor(i, s)->m_m_inf(GetCompart(i)->m_ICa());
				if ((GetSensor(i, s)->m_q) != 0)
					GetSensor(i, s)->m_h = GetSensor(i, s)->m_h_inf(GetCompart(i)->m_ICa());
			}
		}


		double t = 0.0;
		for (i = 0; i < _MAX_COMPARTS; i++) {
			m_IInject[i] = 0.0;
			if ((t >= InjOnTime) AND (t < InjOnTime2)) {
				m_IInject[i] = m_MaxInj[i][0]; //0=first step
				//NEXT LINES ADDED FOR CASE OF ADDING NOISE OF VARIANCE -IInject * m_C
				time_t timet;
				long seed = -(long)time(&timet);
				m_IInject[i] = (m_IInject[i]/sqrt(m_dt)) * RandGauss(&seed);
			}
			else if ((t >= InjOnTime2) AND (t < InjOffTime)) {
				m_IInject[i] = m_MaxInj[i][1]; //1=2nd step
				//NEXT LINES ADDED FOR CASE OF ADDING NOISE OF VARIANCE -IInject * m_C
				time_t timet;
				long seed = -(long)time(&timet);
				m_IInject[i] = (m_IInject[i]/sqrt(m_dt)) * RandGauss(&seed);
			}
		}
		if (DisplayFlag) {
			//GetPlotsDialog()->DoPlots((int)t, num_steps_per_screen, graph_frequency); //int(t) = t_step
			GetPlotsDialog()->DoPlots(t);
		}
		/*if (SaveFlag) {
			Vfile << t << " " << GetCompart(_Soma)->m_V << endl;//writing to file stuff for init vals
		}*/

		//RUN NEURON

		//ADAPTIVE STEP ALGORITHM--VARIABLE dt
		double VOld[_MAX_COMPARTS];
		double VNew[_MAX_COMPARTS];
		double num_gmax_updates = 0;
		BOOL step_up_flag;
		//if (dtmin != dtmax) {
			for (i = 0; i < _MAX_COMPARTS; i++) {
				VOld[i] = GetCompart(i)->m_V;
			}
			m_dt = m_dtmin;  //start at min for safety
			DataTakingTime = 20000.0;
			TotalBurstAmplitude = 0.0;
			ZeroSpikeBurstRate = 0.0;
			Vmin = 100.0;
			VAve = 0.0;
			while (t < tmax) {
				t += m_dt;
				//compute
				for (i = 0; i < _MAX_COMPARTS; i++) {
					m_IInject[i] = 0.0;
					if ((t >= InjOnTime) AND (t < InjOnTime2)) {
						m_IInject[i] = m_MaxInj[i][0]; //0=first step
						//NEXT LINES ADDED FOR CASE OF ADDING NOISE OF VARIANCE -IInject * m_C
						time_t timet;
						long seed = -(long)time(&timet);
						m_IInject[i] = (m_IInject[i]/sqrt(m_dt)) * RandGauss(&seed);
					}
					else if ((t >= InjOnTime2) AND (t < InjOffTime)) {
						m_IInject[i] = m_MaxInj[i][1]; //1=2nd step
						//NEXT LINES ADDED FOR CASE OF ADDING NOISE OF VARIANCE -IInject * m_C
						time_t timet;
						long seed = -(long)time(&timet);
						m_IInject[i] = (m_IInject[i]/sqrt(m_dt)) * RandGauss(&seed);
					}
				}		
				m_pTheNeuron->ComputeMe(m_IInject, m_dt);
				for (i=0; i < _MAX_COMPARTS; i++) {
					VNew[i] = GetCompart(i)->m_V;
				}
				
				//update step size
				for (i=0; i < _MAX_COMPARTS; i++) {
					if (fabs(VNew[i] - VOld[i]) > 1.0) { //decrease dt if any V's change by more than 1 mV
						m_dt = 0.2 * m_dt;
						if (m_dt < m_dtmin) {
							m_dt = m_dtmin;
						}
						break;
					}
				}
				step_up_flag = TRUE;
				for (i=0; i < _MAX_COMPARTS; i++) {
					if (fabs(VNew[i] - VOld[i]) > 0.5) { //increase dt only if ALL V's change by <= 0.2 mV
						step_up_flag = FALSE;	
					}
				}
				if (step_up_flag) {
					m_dt = 3.0 * m_dt;
					if (m_dt > m_dtmax) {
						m_dt = m_dtmax;
					}
				}

				//output
				if (DisplayFlag) {//for plotting fixed time & # of points/screen
					GetPlotsDialog()->DoPlots(t);
				}
				if ((int)(t/_TIME_PER_GMAX_UPDATE) != num_gmax_updates) { //currently every 500 ms
					num_gmax_updates = (int)(t/_TIME_PER_GMAX_UPDATE);
					GetNeuronPage()->UpdateGMaxDisplay();
					GetNeuronPage()->UpdateSensorValueDisplay();
				}
				//if (SaveFlag) {  //Hardwired to taking 20 sec's of data
					if ((t > tmax - DataTakingTime - 6000.0) AND (t < tmax - DataTakingTime - 1000.0)) {
						//search for minimum voltage, for case of zero-spike bursters
						if (VNew[_Soma] < Vmin) {
							Vmin = VNew[_Soma];
						}
					}
					if ((t > tmax - DataTakingTime - 1000.0) AND (t < tmax - 1000.0)) {
						num_steps += 1.0;
						//identify 0 spike bursters (throw this out if AveRate > 0.09)
						if ((NewBurstFlag) AND (VNew[_Soma] > _TRANSMIT_THRESHOLD) ) {
							NewBurstFlag = FALSE;
							//JustBurstFlag = TRUE;
							//NumBursts++;
							//will get overwritten if a spiking cell
							ZeroSpikeBurstRate += 1000.0/DataTakingTime; 
						}
						/*
						if ((NewBurstFlag) AND (VOld[_Soma] > VNew[_Soma])) {
							JustBurstFlag = FALSE;
							//BurstSearchFlag = TRUE;
						}*/
						if (((VNew[_Soma] - Vmin) < 2.0) AND (VNew[_Soma] < VOld[_Soma])) {
							NewBurstFlag = TRUE;
						}
						if ((SpikeFlag) AND (VNew[_Soma] > _SPIKE_THRESHOLD) AND (VOld[_Soma] < VNew[_Soma])) {
							SpikeFlag = FALSE;
							/*if (SaveFlag) {
								SpikeFile << t << endl;
							}*/
							SpikeTimes[NumSpikes] = t;

							//FOR COMPUTING PHASE RESPONSE CURVE--MAY REQUIRE CONSISTENT FIXED TIME STEP
							//HARDWIRED FOR 3 SPIKES/BURST AND FOR SPIKES W/IN BURST <100ms apart, BETWEEN > 100ms apart
							if (!PRCDone) {
								//possibly change below to several cycles of counting???
								if ((ComputingPhaseResponse) AND ((SpikeTimes[NumSpikes] - SpikeTimes[NumSpikes-1]) > 100.0)) {
									PRCDone = TRUE;
									PhaseOut = (SpikeTimes[NumSpikes] - PhaseStartTime - Period)/Period;
								}
								if ((SpikeTimes[NumSpikes] > 34000) AND (!ComputingPhaseResponse)
									AND ((SpikeTimes[NumSpikes] - SpikeTimes[NumSpikes - 1]) > 100.0)) {//start PRC calculation
									ComputingPhaseResponse = TRUE;
									Period = SpikeTimes[NumSpikes] - SpikeTimes[NumSpikes - 3];
									PhaseStartTime = SpikeTimes[NumSpikes];
								}
							}
							//

							JustSpikedFlag = TRUE;
							NumSpikes++;
							AveRate += 1000.0/DataTakingTime; //# of spikes/20 sec of data-taking
						}
						if ((JustSpikedFlag) AND (VOld[_Soma] > VNew[_Soma])) {
							JustSpikedFlag = FALSE;
							//BurstSearchFlag = TRUE;
						}
						if (VNew[_Soma] < _SPIKE_THRESHOLD) {
							SpikeFlag = TRUE;
						}
						//assume graded transmission for _trans_thresh < V, with saturation at trans_max
						if (VNew[_Soma] > _TRANSMIT_THRESHOLD) {
							TotalBurstAmplitude += (Max(VNew[_Soma],_TRANSMIT_MAX) - _TRANSMIT_THRESHOLD)*m_dt;
						}
						VAve += VNew[_Soma]*m_dt/DataTakingTime;
						/*
						if (VNew[_Soma] < _BURST_THRESHOLD) {//maybe redefine burst_thresh relative to min V?
							BurstSearchFlag = FALSE;
						}
						*/
						/*
						if ((BurstSearchFlag) AND (VOld[_Soma] < VNew[_Soma])) {
							BurstFlag = TRUE;
						}*/
					}
					//FOR PHASE RESPONSE CURVE COMPUTATION
					if (ComputingPhaseResponse) {
						if (!TestPulseFired) {
							//NOTE:NumSpikes-1 holds most recent spike time
							if ((t - SpikeTimes[NumSpikes-1]) > ((double)ThisPhasePoint*Period/(double)NumPhasePoints)) {
								GetSyn(_Soma, _Inhib)->m_SpikeReceived();
								PhaseIn = (t - PhaseStartTime)/Period;
								TestPulseFired = TRUE;
								//AfxMessageBox("Test pulse fired");
							}
						}
					}
					//
					if ((t > tmax - 11000.0) AND (t < tmax - 1000.0)) {
						//NEXT LINES ARE FOR 1-COMPARTMENT MAP OF SENSOR SPACE
						DCSensorAveVal += GetSensor(_Soma, _DC)->m_AveValue;
						SlowSensorAveVal += GetSensor(_Soma, _Slow)->m_AveValue;
						FastSensorAveVal += GetSensor(_Soma, _Fast)->m_AveValue;
					}
					//if ((t > InjOnTime - 21000.0) AND (t < InjOffTime - 16000.0)) {
					if ((t > InjOffTime - 19000.0) AND (t < InjOffTime - 16000.0)) {
						if (SaveFlag) {//voltages
							//PUT BACK IN NEXT LINE TO SAVE A VOLTAGE TRACE
							VandIFile << t << " " << VNew[_Soma] << endl; 
							/*
							VandIFile << " " << GetCompart(_Soma)->m_ITotal(0.0);
							for (int chan = _I_Leak; chan <= _I_h; chan++) {
								VandIFile << " " << GetChan(_Soma, chan)->m_I(VNew[_Soma]);
							}
							VandIFile << " " << GetCompart(_Soma)->m_Ca << endl;
							*/
						}
					}
				//}
				//re-set VOld
				for (i=0; i < _MAX_COMPARTS; i++) {
					VOld[i] = VNew[i];
				}
				if (!PeekAndPump())	// check for windows messages from user
					break;
				if (m_bEndRun) { //end run due to user clicking Finish button
					//m_bEndRun = FALSE;
					break;
				}
			}
			if (m_bEndRun) { //end run due to user clicking Finish button
				m_bEndRun = FALSE;
				break;
			}
			DCSensorAveVal /= num_steps;
			SlowSensorAveVal /= num_steps;
			FastSensorAveVal /= num_steps;
			MaxISI = ISIComputer(ISIList, SpikeTimes, NumSpikes);
			if (NumSpikes > 1) {
				Histogram(MaxISI, _NUM_BINS, NumSpikes - 1, ISIList, ISIHistogram);
				MeanISI = Mean(ISIList, NumSpikes - 1);
				SigmaISI = StdDev(ISIList, NumSpikes - 1);
				//next line requires NumSpikes >2
				//moment(ISIList, NumSpikes - 1, &ave, &adev, &sdev, &var, &skew, &kurt);
				CVISI = SigmaISI/MeanISI;
				sprintf(str, "Histogram: ");
				MeanISIbin = (int)((MeanISI/MaxISI)*(double)_NUM_BINS);
				for (i = 0; i < _NUM_BINS; i++) {
					sprintf(str, "%s %g", str, ISIHistogram[i]);
					if (i == MeanISIbin)
						sprintf(str, "%sR", str);
				}
				if (DisplayFlag) {
					AfxMessageBox(str);
				}
			} else {
				MeanISI = -1;
				SigmaISI = -1;
				CVISI = -1;
				//ave = -1;
				//sdev = -1;
				//kurt = -1;
			}
			//sprintf(str, "ISIs_%.3g_%.3g_%.3g_%.2g_%.2g.dat", GetChan(_Soma, _I_A)->m_gMax, GetChan(_Soma, _I_KCa)->m_gMax, GetChan(_Soma, _I_Kd)->m_gMax, GetRunPage()->m_MaxInjSoma, GetRunPage()->m_MaxInjSoma2);
			//ofstream ISIFile(str);
			//if (!ISIFile) cerr << "couldn't open ISI data file" << endl;
			if (SaveFlag) {
				//for (int i = 0; i < NumSpikes - 1; i++) {
					//ISIFile << ISIList[i] << endl;
				//}
				for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
					SummaryFile << GetChan(_Soma, chan)->m_gMax << " ";
				}
				//for (sens = 0; sens < _MAX_SENSORS; sens++) {
					SummaryFile << DCSensorAveVal << " " << SlowSensorAveVal << " " << FastSensorAveVal << " "; 
				//}
			}
			if (AveRate < 0.09) {
				if (SaveFlag) {
					SummaryFile << "D "; //dead cell
				}
				if (ZeroSpikeBurstRate > 0.01) {
					AmplitudePerBurst = 1000.0*TotalBurstAmplitude/(ZeroSpikeBurstRate*DataTakingTime);
					BurstRate = ZeroSpikeBurstRate;
				} else {
					AmplitudePerBurst = -1.0;
					BurstRate = 0.0;
				}
				SpikesPerBurst = 0.0;
				NumSilent += 1;
			} else if (CVISI > 0.3) {//maybe better to split data in half about mean and use skewness
				if (SaveFlag) {
					SummaryFile << "B "; //bursting cell
				}
				NumBurst += 1;
				//calculate burst rate and #spikes/burst: crude, use average values of interburst and interspike ISI's
				NumISISpike = 0;
				NumISIBurst = 0;
				MeanISISpike = 0.0;
				MeanISIBurst = 0.0;
				for (i = 0; i < NumSpikes - 1; i++) {
					if (ISIList[i] <= MeanISI) {
						NumISISpike += 1;
						MeanISISpike += ISIList[i];
					} else if (ISIList[i] > MeanISI) {
						NumISIBurst += 1;
						MeanISIBurst += ISIList[i];
					} else AfxMessageBox("Error in calculating MeanISI");
				}
				MeanISISpike /= (double)(NumISISpike)*1000.0; //convert to [sec]
				MeanISIBurst /= (double)(NumISIBurst)*1000.0;
				BurstRate = (1.0 - AveRate*MeanISISpike)/(MeanISIBurst - MeanISISpike);
				AmplitudePerBurst = 1000.0*TotalBurstAmplitude/(BurstRate*DataTakingTime);
				SpikesPerBurst = AveRate/BurstRate;
				if (DisplayFlag) {
					sprintf(str, "AveRate %g, intraburst interspike %g, interburst %g, TransmitPerBurst %g \n%g Hz burster with %g spikes per burst", 
						    AveRate, MeanISISpike, MeanISIBurst, AmplitudePerBurst, BurstRate, SpikesPerBurst);
					AfxMessageBox(str);
					sprintf(str, "DC sensor %g, SlowSensor %g, FastSensor %g", DCSensorAveVal, SlowSensorAveVal, FastSensorAveVal);
					AfxMessageBox(str);
				}
			} else if (CVISI <= 0.3) {
				/*if (BurstFlag) {//single-spike burster
					if (DisplayFlag) {
						AfxMessageBox("1 spike burster");
					}
					if (SaveFlag) {
						SummaryFile << "B "; //bursting
					}
					BurstRate = AveRate;
					SpikesPerBurst = 1.0;
					NumBurst += 1;
				} else {//tonic	*/
					if (SaveFlag) {
						SummaryFile << "T "; //tonically firing
					}
					BurstRate = AveRate;
					AmplitudePerBurst = 1000.0*TotalBurstAmplitude/(AveRate*DataTakingTime);
					SpikesPerBurst = 1.0;
					NumTonic += 1;
					if (DisplayFlag) {
						sprintf(str, "AveRate %g, TransmitPerSpike %g", AveRate, AmplitudePerBurst);
						AfxMessageBox(str);
					}
				//}
			} else AfxMessageBox("Error in classifying dead, tonic, bursting");
			//Re-set flags
			JustSpikedFlag = FALSE;
			//JustBurstFlag = FALSE;
			BurstSearchFlag = FALSE;
			BurstFlag = FALSE;
			NewBurstFlag = FALSE;
			if (SaveFlag) {
				SummaryFile << AveRate << " " << BurstRate << " " << SpikesPerBurst << " ";
				//SummaryFile << ave << " " << sdev << " " << CVISI << " " << kurt << " " << seed << endl;
				SummaryFile << AmplitudePerBurst << " " << VAve << " " << CVISI << " " << seed << endl;
				//PRCFile << GetSyn(_Soma, _Inhib)->m_E << " " << GetSyn(_Soma, _Inhib)->m_tau_decay << " ";
				//PRCFile << GetSyn(_Soma, _Inhib)->m_gStep << " " << PhaseIn << " " << PhaseOut << endl;
			}
	//NEXT LINE FOR MANY RUN INITIAL
		} //for ThisRun
		} //for ThisPhasePoint
		} //for IPSPSize
	
   //FOR GRID
   /*
					} //for KdGmax
			} //for KCaGmax
		} //for AGmax
	} //for CaTGmax
	//} for NaGmax
   */
	if (DisplayFlag) {
		sprintf(str, "NumSilent = %d, NumBurst = %d, NumTonic = %d", NumSilent, NumBurst, NumTonic);
		AfxMessageBox(str);
	}
	//}  //end if (dtmin != dtmax)


	//free_vector(SpikeTimes, 0, _MAX_SPIKES - 1);
	//free_vector(ISIList, 0, _MAX_SPIKES - 1);
	//free_vector(ISIHistogram, 0, _NUM_BINS - 1);
	//free_matrix(EigenMatrix, 0, 4, 0, 4);
	
	//FOR FIXED TIME STEP dt
	/*int T;
	for (int t_step = 1; t_step <= num_steps; t_step++) {
		t = t_step * dt;
		T = t_step%num_steps_per_screen;
		for (i = 0; i < _MAX_COMPARTS; i++) {
			m_IInject[i] = 0.0;
			if ((t >= InjOnTime) AND (t < InjOffTime))
					m_IInject[i] = m_MaxInj[i];
		}
		m_pTheNeuron->ComputeMe(m_IInject, dt);
		if (DisplayFlag AND T%graph_frequency == 0) {//for plotting fixed time & # of points/screen
			GetPlotsDialog()->DoPlots(t_step, num_steps_per_screen, graph_frequency);
		}
		if (t_step%gmax_update_frequency == 0) {
			GetNeuronPage()->UpdateGMaxDisplay();
			GetNeuronPage()->UpdateSensorValueDisplay();
		}
		if (SaveFlag) {
			if (t_step%graph_frequency == 0)
				Vfile << t << " " << GetCompart(_Soma)->m_V << endl;//writing to file stuff for init vals
		}
		if (!PeekAndPump())	// check for windows messages from user
			break;
		if (m_bEndRun) { //end run due to user clicking Finish button
			m_bEndRun = FALSE;
			break;
		}
	}*/
}

/////////////////////////////////////////////////////////////////////////////
// CZhengModelDoc serialization

void CZhengModelDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_dtmin;
		ar << m_dtmax;
		ar << m_dt;
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar << m_IInject[i];
			for (int step = 0; step < _MAX_INJ_TIMES; step++) {
				ar << m_MaxInj[i][step];
			}
		}
		ar << m_bEndRun;
	}
	else
	{
		ar >> m_dtmin;
		ar >> m_dtmax;
		ar >> m_dt;
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar >> m_IInject[i];
			for (int step = 0; step < _MAX_INJ_TIMES; step++) {
				ar >> m_MaxInj[i][step];
			}
		}
		ar >> m_bEndRun;

	}
	m_pTheNeuron->Serialize(ar);
	GetPlotsDialog()->Serialize(ar);
	GetRunPage()->Serialize(ar);
	GetNeuronPage()->Serialize(ar);
	GetDisplayPage()->Serialize(ar);
}