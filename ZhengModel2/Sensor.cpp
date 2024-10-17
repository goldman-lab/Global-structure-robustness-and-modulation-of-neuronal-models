//Sensor.cpp : implementation file
//Sensors work like Hodgkin-Huxley channels, possessing 1st order kinetics for
//activating & inactivating and a maximum value; they achieve a value which is
//compared to its target in aiming for a particular neural activity pattern

#include "stdafx.h"
#include "GlobalDefs.h"
#include "DefaultParams.h"
#include "Sensor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSensor

IMPLEMENT_SERIAL(CSensor, CObject, _VERSION_NUMBER)


CSensor::CSensor(int type, BOOL exists) //constructor
{
	m_type = type;  //type of sensor, e.g. _DC, _Fast, _Slow
	m_m = 0.0;
	m_h = 0.0;
	m_AveValue = 0.0;
	m_bExists = exists;
	switch (type) {
		case _DC:
			m_Target = _DC_TARGET;
			m_ValueMax = _DC_VALUEMAX;
			m_tau_m = _DC_TAU_M;
			m_tau_h = _DC_TAU_H;  //dummy--not currently used
			m_m_inf_Z = _DC_M_INF_Z; //where m_inf curve = 1/2
			m_h_inf_Z = _DC_H_INF_Z; //dummy--not currently used
			m_p = _DC_NUM_M;
			m_q = _DC_NUM_H; break;
		case _Slow:
			m_Target = _Slow_TARGET;
			m_ValueMax = _Slow_VALUEMAX;
			m_tau_m = _Slow_TAU_M;
			m_tau_h = _Slow_TAU_H;
			m_m_inf_Z = _Slow_M_INF_Z; //where m_inf curve = 1/2
			m_h_inf_Z = _Slow_H_INF_Z; //where h_inf curve = 1/2
			m_p = _Slow_NUM_M;
			m_q = _Slow_NUM_H; break;
		case _Fast:
			m_Target = _Fast_TARGET;
			m_ValueMax = _Fast_VALUEMAX;
			m_tau_m = _Fast_TAU_M;
			m_tau_h = _Fast_TAU_H;
			m_m_inf_Z = _Fast_M_INF_Z; //where m_inf curve = 1/2
			m_h_inf_Z = _Fast_H_INF_Z; //where h_inf curve = 1/2
			m_p = _Fast_NUM_M;
			m_q = _Fast_NUM_H; break;
	}
}

CSensor::~CSensor()
{
}

void CSensor::m_Update_m(double I_Ca_per_nF, double dt)
{
	m_m = ((m_m_inf(I_Ca_per_nF)*(1. - exp(-dt/m_tau_m))) + (m_m*exp(-dt/m_tau_m)));
}

void CSensor::m_Update_h(double I_Ca_per_nF, double dt)
{
	m_h = ((m_h_inf(I_Ca_per_nF)*(1. - exp(-dt/m_tau_h))) + (m_h*exp(-dt/m_tau_h)));
}

void CSensor::m_Update_AveValue(double tau_eff, double dt)
{
	m_AveValue = ((m_Value()*(1. - exp(-dt/tau_eff))) + (m_AveValue*exp(-dt/tau_eff)));
}

double CSensor::m_Value() 
{
	return (m_ValueMax * pow(m_m, m_p) * pow(m_h, m_q));
}

double CSensor::m_Error()
{ 
	return (m_Target - m_Value());
}

//next 2 functions return m_infinity, h_infinity
double CSensor::m_m_inf(double I_Ca_per_nF) 
{
	return (1.0/(1.0 + exp(m_m_inf_Z + I_Ca_per_nF)));
}

double CSensor::m_h_inf(double I_Ca_per_nF) 
{
	return (1.0/(1.0 + exp(-I_Ca_per_nF - m_h_inf_Z)));
}

/////////////////////////////////////////////////////////////////////////////
// CSensor serialization

void CSensor::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_type;
		ar << m_m;
		ar << m_h;
		ar << m_AveValue;
		ar << m_bExists;
		ar << m_Target;
		ar << m_ValueMax;
		ar << m_tau_m;
		ar << m_tau_h;
		ar << m_m_inf_Z;
		ar << m_h_inf_Z;
		ar << m_p;
		ar << m_q;
	}
	else
	{
		ar >> m_type;
		ar >> m_m;
		ar >> m_h;
		ar >> m_AveValue;
		ar >> m_bExists;
		ar >> m_Target;
		ar >> m_ValueMax;
		ar >> m_tau_m;
		ar >> m_tau_h;
		ar >> m_m_inf_Z;
		ar >> m_h_inf_Z;
		ar >> m_p;
		ar >> m_q;
	}

}