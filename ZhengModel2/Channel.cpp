//Channel.cpp : implementation file
// 

#include "stdafx.h"
#include "ZhengModelHeaders.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//Global vars
	char ChanName[12][_MAX_CHANNELS] = {"Leak", "Na", "CaT", "CaP", "A", "KCa", "Kd", "h"};
/////////////////////////////////////////////////////////////////////////////
// CChannel

IMPLEMENT_SERIAL(CChannel, CObject, _VERSION_NUMBER)


CChannel::CChannel(int type, BOOL exists) //constructor
{
	m_type = type;  //type of channel, e.g. I_Na
	m_name = CString(ChanName[m_type]);
	m_m = 0.0;
	m_h = 0.0;
	m_bExists = exists;
	m_tau_gMax = _TAU_GMAX;
	switch (type) {
		case _I_Leak:
			m_bCaChannel = _Leak_CaCHANNEL;
			m_E = _Leak_E;
			m_gMax0 = _Leak_GMAX0;
			m_gMax = _Leak_GMAX0;
			m_p = _Leak_NUM_M;  
			m_q = _Leak_NUM_H; 
			m_SensorCoupling[_DC] = _Leak_DC_COUPLING;
			m_SensorCoupling[_Slow] = _Leak_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _Leak_Fast_COUPLING; break;
		case _I_Na:
			m_bCaChannel = _Na_CaCHANNEL;
			m_E = _Na_E;
			m_gMax0 = _Na_GMAX0;
			m_gMax = _Na_GMAX0;
			m_p = _Na_NUM_M;  
			m_q = _Na_NUM_H; 
			m_SensorCoupling[_DC] = _Na_DC_COUPLING;
			m_SensorCoupling[_Slow] = _Na_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _Na_Fast_COUPLING; break;
		case _I_CaT:
			m_bCaChannel = _CaT_CaCHANNEL;
			m_E = _CaT_E;
			m_gMax0 = _CaT_GMAX0;
			m_gMax = _CaT_GMAX0;
			m_p = _CaT_NUM_M;  
			m_q = _CaT_NUM_H; 
			m_SensorCoupling[_DC] = _CaT_DC_COUPLING;
			m_SensorCoupling[_Slow] = _CaT_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _CaT_Fast_COUPLING; break;
		case _I_CaP:
			m_bCaChannel = _CaP_CaCHANNEL;
			m_E = _CaP_E;
			m_gMax0 = _CaP_GMAX0;
			m_gMax = _CaP_GMAX0;
			m_p = _CaP_NUM_M;  
			m_q = _CaP_NUM_H; 
			m_SensorCoupling[_DC] = _CaP_DC_COUPLING;
			m_SensorCoupling[_Slow] = _CaP_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _CaP_Fast_COUPLING; break;
		case _I_A:
			m_bCaChannel = _A_CaCHANNEL;
			m_E = _A_E;
			m_gMax0 = _A_GMAX0;
			m_gMax = _A_GMAX0;
			m_p = _A_NUM_M;  
			m_q = _A_NUM_H; 
			m_SensorCoupling[_DC] = _A_DC_COUPLING;
			m_SensorCoupling[_Slow] = _A_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _A_Fast_COUPLING; break;
		case _I_KCa:
			m_bCaChannel = _KCa_CaCHANNEL;
			m_E = _KCa_E;
			m_gMax0 = _KCa_GMAX0;
			m_gMax = _KCa_GMAX0;
			m_p = _KCa_NUM_M;  
			m_q = _KCa_NUM_H; 
			m_SensorCoupling[_DC] = _KCa_DC_COUPLING;
			m_SensorCoupling[_Slow] = _KCa_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _KCa_Fast_COUPLING; break;
		case _I_Kd:
			m_bCaChannel = _Kd_CaCHANNEL;
			m_E = _Kd_E;
			m_gMax0 = _Kd_GMAX0;
			m_gMax = _Kd_GMAX0;
			m_p = _Kd_NUM_M;  
			m_q = _Kd_NUM_H; 
			m_SensorCoupling[_DC] = _Kd_DC_COUPLING;
			m_SensorCoupling[_Slow] = _Kd_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _Kd_Fast_COUPLING; break;
		case _I_h:
			m_bCaChannel = _h_CaCHANNEL;
			m_E = _h_E;
			m_gMax0 = _h_GMAX0;
			m_gMax = _h_GMAX0;
			m_p = _h_NUM_M;  
			m_q = _h_NUM_H; 
			m_SensorCoupling[_DC] = _h_DC_COUPLING;
			m_SensorCoupling[_Slow] = _h_Slow_COUPLING;
			m_SensorCoupling[_Fast] = _h_Fast_COUPLING; break;
	}
}

CChannel::~CChannel()
{
}

void CChannel::m_Update_m(double V, double Ca, double dt)
{
	m_m = ((m_m_inf(V, Ca)*(1. - exp(-dt/m_tau_m(V)))) + (m_m*exp(-dt/m_tau_m(V))));
}

void CChannel::m_Update_h(double V, double dt)
{
	m_h = ((m_h_inf(V)*(1. - exp(-dt/m_tau_h(V)))) + (m_h*exp(-dt/m_tau_h(V))));
}

void CChannel::m_Update_gMax(int compartment, double dt)
{
	m_gMax = (m_gMax*exp(dt*m_TotalSensorError(compartment)/m_tau_gMax));
}

void CChannel::m_Update_E(double Ca)
//for Ca channels: assumes given T (_NERNST_CA) and extracell Ca (CA_OUT)
{
	m_E = _NERNST_CA*log(_CA_OUT/Ca);
}

double CChannel::m_g() {
	return (m_gMax * pow(m_m, m_p) * pow(m_h, m_q));
}

double CChannel::m_I(double V) { //in [uA/cm^2]
	return (m_gMax * pow(m_m, m_p) * pow(m_h, m_q) * (V - m_E));
}

double CChannel::m_TotalSensorError(int compartment) {
	double Total = 0.0;
	for (int s = 0; s < _MAX_SENSORS; s++) {
		if (GetSensor(compartment, s)->m_bExists) {
			Total += m_SensorCoupling[s] * GetSensor(compartment,s)->m_Error();
		}
	}
	return(Total);
}

//next 4 functions return m_infinity, h_infinity, tau_m, and tau_h
double CChannel::m_m_inf(double V, double Ca) {
	//below were for simple hodg-hux squid axon
	/*double alpha, beta;
	switch (m_type) {
		case _I_Na:
			alpha = (0.1*(V + 40.)/(1. - exp(-0.1*(V + 40.))));
			beta = (4.*exp(-0.0556*(V + 65.)));
			break;
		case _I_Kd:
			alpha = (0.01*(V + 55.)/(1. - exp(-0.1*(V + 55.))));
			beta = (0.125*exp(-0.0125*(V + 65.)));
			break;
	}
	return (alpha/(alpha + beta));*/
	
	//BELOW ARE ZHENG'S MODEL PARAMETERS
	double m1, m2, denom;  //parameters defining the function
	switch (m_type) {
		case _I_Na:   
			m1 = 25.5; m2 = -5.29; break;
		case _I_CaT:
			m1 = 27.1; m2 = -7.2; break;
		case _I_CaP:
			m1 = 33.0; m2 = -8.1; break;
		case _I_A:
			m1 = 27.2; m2 = -8.7; break;
		case _I_KCa:
			m1 = 28.3; m2 = -12.6; break;
		case _I_Kd: 
			m1 = 12.3; m2 = -11.8; break;
		case _I_h: //actually proctolin current--temporary
			m1 = 50.0; m2 = -5; break;
		default:
			AfxMessageBox("No such channel!!!");
	}
	denom = 1.0 + exp((V + m1)/m2);
	if (m_type == _I_KCa)
		return (Ca/(Ca + 3.0))/denom;
	else return 1./denom;
}

double CChannel::m_h_inf(double V) {
	//below were for simple hodg-hux squid axon
	/*double alpha, beta;
	alpha = (0.07*exp(-0.05*(V + 65.)));
	beta = (1./(1. + exp(-0.1*(V + 35.))));
	return (alpha/(alpha + beta));*/
	
	//BELOW ARE ZHENG'S MODEL PARAMETERS
	double h1, h2;
	switch (m_type) {
		case _I_Na:
			h1 = 48.9; h2 = 5.18; break;
		case _I_CaT:
			h1 = 32.1; h2 = 5.5; break;
		case _I_CaP:
			h1 = 60.0; h2 = 6.2; break;
		case _I_A:
			h1 = 56.9; h2 = 4.9; break;
		case _I_h: //shouldn't run for proctolin
			h1 = 70.0; h2 = 6.0; break;
		default:
			AfxMessageBox("No such channel!!!");
	}
	return 1./(1. + exp((V + h1)/h2));
}

double CChannel::m_tau_m(double V) {
	//below were for simple hodg-hux squid axon
	/*double alpha, beta;
	switch (m_type) {
		case _I_Na:
			alpha = (0.1*(V + 40.)/(1. - exp(-0.1*(V + 40.))));
			beta = (4.*exp(-0.0556*(V + 65.)));
			break;
		case _I_Kd:
			alpha = (0.01*(V + 55.)/(1. - exp(-0.1*(V + 55.))));
			beta = (0.125*exp(-0.0125*(V + 65.)));
			break;
	}
	return (1./(alpha + beta));*/
	
	//BELOW ARE ZHENG'S MODEL PARAMETERS
	double tm1, tm2, tm3, tm4, denom;
	switch (m_type) {
		case _I_Na:
			tm1 = 1.32; tm2 = -1.26; tm3 = 120.0; tm4 = -25.0; break;
		case _I_CaT:
			tm1 = 21.7; tm2 = -21.3; tm3 = 68.1; tm4 = -20.5; break;
		case _I_CaP:
			tm1 = 1.4; tm2 = 7.0; tm3 = 70.0; tm4 = -13.0; break;
		case _I_A:
			tm1 = 11.6; tm2 = -10.4; tm3 = 32.9; tm4 = -15.2; break;
		case _I_KCa:
			//Usual numbers--ones used in all papers!!!
			tm1 = 90.3; tm2 = -75.1; tm3 = 46.0; tm4 = -22.7; break;
			//playing with different KCa's--lets KCa control # of S/B
			//tm1 = 30.1; tm2 = -25.0; tm3 = 46.0; tm4 = -22.7; break;
		case _I_Kd:  //ZHENG
			tm1 = 7.2; tm2 = -6.4; tm3 = 28.3; tm4 = -19.2; break;
		//case _I_Kd:  //SPEED UP TO TURN PARABOLIC BURSTER INTO PLATEAUING(?)
			//tm1 = 3.6; tm2 = -3.2; tm3 = 28.3; tm4 = -19.2; break;
	}
	if (m_type == _I_h) { //actually for proctolin -- temporary
		return 1000.0/166.0;
	} else {
		if (m_type == _I_CaP)
			denom = exp((V + 27.0)/10.0) + exp((V + tm3)/tm4);
		else denom = 1. + exp((V + tm3)/tm4);
		return tm1 + (tm2/denom);
	}
}

double CChannel::m_tau_h(double V) {
	//below were for simple hodg-hux squid axon
	/*double alpha, beta;
	alpha = (0.07*exp(-0.05*(V + 65.)));
	beta = (1./(1. + exp(-0.1*(V + 35.))));
	return (1./(alpha + beta));*/
	
	//BELOW ARE ZHENG'S MODEL PARAMETERS
	double th1, th2, th3, th4, denom;
	switch (m_type) {
		case _I_Na:
			th1 = 1.5; th2 = 1.0; th3 = 34.9; th4 = 3.6; break;
		case _I_CaT:
			th1 = 105.0; th2 = -89.8; th3 = 55.0; th4 = -16.9; break;
		case _I_CaP:
			th1 = 60.0; th2 = 150.0; th3 = 65.0; th4 = -16.0; break;
		case _I_A:
			th1 = 38.6; th2 = -29.2; th3 = 38.9; th4 = -26.5; break;
		case _I_h: //shouldn't run for proctolin
			th1 = 272.0; th2 = 1499.0; th3 = 42.2; th4 = -8.73; break;
		default:
			AfxMessageBox("No such channel!!!");
	}
	if (m_type == _I_CaP)
		denom = exp((V + 55.0)/9.0) + exp((V + th3)/th4);
	else denom = 1. + exp((V + th3)/th4);
	if (m_type == _I_Na)
		return (0.67/(1. + exp(-(V + 62.9)/10.0)))*(th1 + (th2/denom));
	else return th1 + (th2/denom);
}

/////////////////////////////////////////////////////////////////////////////
// CChannel serialization

void CChannel::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_type;
		ar << m_name;
		ar << m_m;
		ar << m_h;
		ar << m_bExists;
		ar << m_tau_gMax;
		ar << m_bCaChannel;
		ar << m_E;
		ar << m_gMax0;
		ar << m_gMax;
		ar << m_p;  
		ar << m_q;
		ar << m_SensorCoupling[_DC];
		ar << m_SensorCoupling[_Slow];
		ar << m_SensorCoupling[_Fast];
	}	
	else
	{
		ar >> m_type;
		ar >> m_name;
		ar >> m_m;
		ar >> m_h;
		ar >> m_bExists;
		ar >> m_tau_gMax;
		ar >> m_bCaChannel;
		ar >> m_E;
		ar >> m_gMax0;
		ar >> m_gMax;
		ar >> m_p;  
		ar >> m_q;	
		ar >> m_SensorCoupling[_DC];
		ar >> m_SensorCoupling[_Slow];
		ar >> m_SensorCoupling[_Fast];
	}
}