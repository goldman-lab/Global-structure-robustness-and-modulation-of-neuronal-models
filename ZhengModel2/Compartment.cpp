//Compartment.cpp : implementation file
// 

#include "stdafx.h"
#include "ZhengModelHeaders.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompartment

IMPLEMENT_SERIAL(CCompartment, CObject, _VERSION_NUMBER)


CCompartment::CCompartment(int type) //constructor
{
	m_type = type;  //type of compartment, e.g. Soma
	//init member vars
	m_V0 = _V0;  //initial value -- stored into here from interface
	m_V = 0.0;  //[uF/cm^2]
	m_Ca0 = _Ca0;  //initial value -- stored into here from interface
	m_Ca = 0.0;  //[uM]
	m_C = _C;  //[uF/cm^2]
	m_tau_eff = _TAU_UPDATE; 
	double conductance = S_TO_mS*2.0*_PI*SQR(_SOMA_R*_HILLOCK_R)/(CM_TO_UM*_RESISTIVITY *
						(_SOMA_L*SQR(_HILLOCK_R) + _HILLOCK_L*SQR(_SOMA_R)));
	BOOL chan_exists[_MAX_CHANNELS];
	BOOL syn_exists[_MAX_SYNAPSES];
	BOOL sensor_exists[_MAX_SENSORS];
	double shell_vol;
	switch (type) {
		case _Soma:
			m_Area = 2.0*_PI*_SOMA_R*_SOMA_L/SQR(CM_TO_UM);
			shell_vol = _PI*_SOMA_L*_SHELL_T*(2*_SOMA_R - _SHELL_T); //[um^3]
			m_gAxial[_Soma] = 0.0;
			m_gAxial[_Hillock] = conductance/(m_Area);
			chan_exists[_I_Leak] = _SOMA_Leak_EXISTS;
			chan_exists[_I_Na] = _SOMA_Na_EXISTS;
			chan_exists[_I_CaT] = _SOMA_CaT_EXISTS;
			chan_exists[_I_CaP] = _SOMA_CaP_EXISTS;
			chan_exists[_I_A] = _SOMA_A_EXISTS;
			chan_exists[_I_KCa] = _SOMA_KCa_EXISTS;
			chan_exists[_I_Kd] = _SOMA_Kd_EXISTS;
			chan_exists[_I_h] = _SOMA_h_EXISTS;
			syn_exists[_Inhib] = _SOMA_Inhib_EXISTS;
			syn_exists[_Excit] = _SOMA_Excit_EXISTS;
			sensor_exists[_DC] = _SOMA_DC_EXISTS;
			sensor_exists[_Slow] = _SOMA_Slow_EXISTS;
			sensor_exists[_Fast] = _SOMA_Fast_EXISTS;
			break;
		case _Hillock:
			m_Area = 2.0*_PI*_HILLOCK_R*_HILLOCK_L/SQR(CM_TO_UM);
			shell_vol = _PI*_HILLOCK_L*_SHELL_T*(2*_HILLOCK_R - _SHELL_T); //[um^3]
			m_gAxial[_Soma] = conductance/m_Area;
			m_gAxial[_Hillock] = 0.0;
			chan_exists[_I_Leak] = _HILLOCK_Leak_EXISTS;
			chan_exists[_I_Na] = _HILLOCK_Na_EXISTS;
			chan_exists[_I_CaT] = _HILLOCK_CaT_EXISTS;
			chan_exists[_I_CaP] = _HILLOCK_CaP_EXISTS;
			chan_exists[_I_A] = _HILLOCK_A_EXISTS;
			chan_exists[_I_KCa] = _HILLOCK_KCa_EXISTS;
			chan_exists[_I_Kd] = _HILLOCK_Kd_EXISTS;
			chan_exists[_I_h] = _HILLOCK_h_EXISTS;
			syn_exists[_Inhib] = _HILLOCK_Inhib_EXISTS;
			syn_exists[_Excit] = _HILLOCK_Excit_EXISTS;
			sensor_exists[_DC] = _HILLOCK_DC_EXISTS;
			sensor_exists[_Slow] = _HILLOCK_Slow_EXISTS;
			sensor_exists[_Fast] = _HILLOCK_Fast_EXISTS;
			break;
	}
	m_nA_to_uMCa = _NA_TO_UMCA_PREFACTOR*_TAU_CA/(_CA_VALENCE*shell_vol); //[uM]/[nA]
	m_ChanArray.SetSize(0, _MAX_CHANNELS);
	int i;
	for (i = 0; i < _MAX_CHANNELS; i++) 
		m_ChanArray.Add(new CChannel(i, chan_exists[i]));
	m_SynArray.SetSize(0, _MAX_SYNAPSES);
	for (i = 0; i < _MAX_SYNAPSES; i++) 
		m_SynArray.Add(new CSynapse(i, syn_exists[i]));
	m_SensorArray.SetSize(0, _MAX_SENSORS);
	for (i = 0; i < _MAX_SENSORS; i++) 
		m_SensorArray.Add(new CSensor(i, sensor_exists[i]));
	
}

CCompartment::~CCompartment()
{
	int i;
	for (i=0; i<_MAX_CHANNELS; i++)
		delete m_ChanArray[i];
	m_ChanArray.RemoveAll();
	for (i=0; i<_MAX_SYNAPSES; i++)
		delete m_SynArray[i];
	m_SynArray.RemoveAll();
	for (i=0; i<_MAX_SENSORS; i++)
		delete m_SensorArray[i];
	m_SensorArray.RemoveAll();
}

void CCompartment::ComputeMe(double dt) //computes channels' activations/inactivations
{
	//to be more efficient, the following should be diagonalized with Ca channels, but
	//should be ok cuz on much slower time scale
	//compute Ca level in this compartment--flows in CaT, CaP
	m_Ca = (((_CA_REST - m_nA_to_uMCa*m_ICa())*(1. - exp(-dt/_TAU_CA))) + (m_Ca*exp(-dt/_TAU_CA)));
	
	//update Ca channel reversal potentials
	for (int i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bCaChannel) {
			if (m_Chan(i)->m_bExists) {
				m_Chan(i)->m_Update_E(m_Ca);
			}
		}
	}

	//compute channels' activation/inactivation
	for (i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bExists) {
			if ((m_Chan(i)->m_p) != 0)
				m_Chan(i)->m_Update_m(m_V, m_Ca, dt);
			if ((m_Chan(i)->m_q) != 0)
				m_Chan(i)->m_Update_h(m_V, dt);
		}
	}

	//compute synaptic g's: ADDED 11/99
	for (i = 0; i < _MAX_SYNAPSES; i++) {
		if (m_Syn(i)->m_bExists) {
			m_Syn(i)->m_Update_g(dt);
		}
	}

	//compute sensors' activation/inactivation
	double ICa_per_nF = m_ICa()/(UA_TO_NA * m_C * m_Area); //[uA/uF or equivalently nA/nF] 
	for (int s = 0; s < _MAX_SENSORS; s++) {
		//if (m_Sens(s)->m_bExists) {
			if ((m_Sens(s)->m_p) != 0)
				m_Sens(s)->m_Update_m(ICa_per_nF, dt);
			if ((m_Sens(s)->m_q) != 0)
				m_Sens(s)->m_Update_h(ICa_per_nF, dt);
			m_Sens(s)->m_Update_AveValue(m_tau_eff, dt);
		//}
	}
	
	//compute gmaxs
	for (i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bExists) {
			m_Chan(i)->m_Update_gMax(m_type, dt);
		}
	}

}

double CCompartment::m_ITotal(double IInject)
{
	//IInject sent here is in units of nA per capacitance(uF) per area
	//all currents then converted to nA
	double Total = -IInject * m_C; //injected current
	for (int i = 0; i < _MAX_COMPARTS; i++) {//current from other compartment
		Total += UA_TO_NA * m_gAxial[i] * (m_V - GetCompart(i)->m_V);
	}
	for (i = 0; i < _MAX_CHANNELS; i++) {//total channel current
		if (m_Chan(i)->m_bExists) {
			Total += UA_TO_NA * m_Chan(i)->m_I(m_V);
		}
	}
	for (i = 0; i < _MAX_SYNAPSES; i++) {//synaptic current
		if (m_Syn(i)->m_bExists) {
			Total += UA_TO_NA * m_Syn(i)->m_I(m_V);
		}
	}
	Total *= m_Area;
	return (Total);
}

double CCompartment::m_ICa()
{
	//all currents converted to nA
	double Total = 0.0;
	for (int i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bCaChannel) {
			if (m_Chan(i)->m_bExists) {
				Total += UA_TO_NA * m_Chan(i)->m_I(m_V);
			}
		}
	}
	Total *= m_Area;  //convert to [nA] from [nA/cm^2]
	return (Total);
}


double CCompartment::m_D()
{
	double Total = 0.0;
	for (int i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bExists) {
			Total += (m_Chan(i)->m_g()) * (m_Chan(i)->m_E)/m_C;
		}
	}
	for (i = 0; i < _MAX_SYNAPSES; i++) {
		if (m_Syn(i)->m_bExists) {
			Total += (m_Syn(i)->m_g) * (m_Syn(i)->m_E)/m_C;
		}
	}
	return (Total);
}

double CCompartment::m_gAxialTot()
{
	double Total = 0.0;
	for (int i = 0; i < _MAX_COMPARTS; i++) {
		Total += m_gAxial[i];
	}
	return (Total);
}

double CCompartment::m_gMemTot() //Total membrane and synaptic conductance
{
	double Total = 0.0;
	for (int i = 0; i < _MAX_CHANNELS; i++) {
		if (m_Chan(i)->m_bExists) {
			Total += m_Chan(i)->m_g();
		}
	}
	for (i = 0; i < _MAX_SYNAPSES; i++) {
		if (m_Syn(i)->m_bExists) {
			Total += m_Syn(i)->m_g;
		}
	}
	return (Total);
}

/////////////////////////////////////////////////////////////////////////////
// CCompartment serialization

void CCompartment::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_type;
		ar << m_V0;  
		ar << m_V;  
		ar << m_Ca0; 
		ar << m_Ca;  
		ar << m_C;  
		ar << m_tau_eff;
		ar << m_Area;
		ar << m_gAxial[_Soma];
		ar << m_gAxial[_Hillock];
		ar << m_nA_to_uMCa;
	}	
	else
	{
		ar >> m_type;
		ar >> m_V0;  
		ar >> m_V;  
		ar >> m_Ca0; 
		ar >> m_Ca;  
		ar >> m_C; 
		ar >> m_tau_eff;
		ar >> m_Area;
		ar >> m_gAxial[_Soma];
		ar >> m_gAxial[_Hillock];
		ar >> m_nA_to_uMCa;
	}
	m_ChanArray.Serialize(ar);
	m_SynArray.Serialize(ar);
	m_SensorArray.Serialize(ar);
}