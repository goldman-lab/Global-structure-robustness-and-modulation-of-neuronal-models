// NeuronPage.cpp : implementation file
//

#include "stdafx.h"
#include "ZhengModelHeaders.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CNeuronPage property page

IMPLEMENT_SERIAL(CNeuronPage, CPropertyPage, _VERSION_NUMBER)

CNeuronPage::CNeuronPage() : CPropertyPage(CNeuronPage::IDD)
{
	m_bChangingChannels = FALSE;
	m_bChangingSensors = FALSE;
	m_bChangingSynapses = FALSE;
	m_bChangingComparts = FALSE;
	m_bChangeMade = TRUE;
	double conductance = S_TO_mS*2.0*_PI*SQR(_SOMA_R*_HILLOCK_R)/(CM_TO_UM*_RESISTIVITY*
						(_SOMA_L*SQR(_HILLOCK_R) + _HILLOCK_L*SQR(_SOMA_R)));
	for (int i = 0; i < _MAX_COMPARTS; i++) {
		m_C_arr[i] = _C;
		m_V0_arr[i] = _V0;
		m_Ca0_arr[i] = _Ca0;
		if (i == _Soma) {
			m_R_arr[i] = _SOMA_R;
			m_L_arr[i] = _SOMA_L;
			m_Area_arr[i] = 2.0*_PI*_SOMA_L*_SOMA_R / SQR(CM_TO_UM);
			m_gAxial_arr[i] = conductance/m_Area_arr[i];
		}
		else if (i == _Hillock) {
			m_R_arr[i] = _HILLOCK_R;
			m_L_arr[i] = _HILLOCK_L;
			m_Area_arr[i] = 2.0*_PI*_HILLOCK_L*_HILLOCK_R / SQR(CM_TO_UM);
			m_gAxial_arr[i] = conductance/m_Area_arr[i];
		}
		else AfxMessageBox("No such compartment!");
		m_bChangedV0[i] = FALSE;
		m_bChangedCa0[i] = FALSE;
		m_bChangedR[i] = FALSE;
		m_bChangedL[i] = FALSE;
		m_bChangedC[i] = FALSE;
		for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
			switch (chan) {
				case _I_Leak:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_Leak_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_Leak_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _Leak_CaCHANNEL;
					m_E_arr[i][chan] = _Leak_E;
					m_gmax0_arr[i][chan] = _Leak_GMAX0;
					m_chan_num_m_arr[i][chan] = _Leak_NUM_M;  
					m_chan_num_h_arr[i][chan] = _Leak_NUM_H; break;
				case _I_Na:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_Na_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_Na_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan]= _Na_CaCHANNEL;
					m_E_arr[i][chan] = _Na_E;
					m_gmax0_arr[i][chan] = _Na_GMAX0;
					m_chan_num_m_arr[i][chan] = _Na_NUM_M;  
					m_chan_num_h_arr[i][chan] = _Na_NUM_H; break;
				case _I_CaT:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_CaT_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_CaT_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _CaT_CaCHANNEL;
					m_E_arr[i][chan] = _CaT_E;
					m_gmax0_arr[i][chan] = _CaT_GMAX0;
					m_chan_num_m_arr[i][chan] = _CaT_NUM_M;  
					m_chan_num_h_arr[i][chan] = _CaT_NUM_H; break;
				case _I_CaP:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_CaP_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_CaP_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _CaP_CaCHANNEL;
					m_E_arr[i][chan] = _CaP_E;
					m_gmax0_arr[i][chan] = _CaP_GMAX0;
					m_chan_num_m_arr[i][chan] = _CaP_NUM_M;  
					m_chan_num_h_arr[i][chan] = _CaP_NUM_H; break;
				case _I_A:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_A_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_A_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _A_CaCHANNEL;
					m_E_arr[i][chan] = _A_E;
					m_gmax0_arr[i][chan] = _A_GMAX0;
					m_chan_num_m_arr[i][chan] = _A_NUM_M;  
					m_chan_num_h_arr[i][chan] = _A_NUM_H; break;
				case _I_KCa:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_KCa_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_KCa_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _KCa_CaCHANNEL;
					m_E_arr[i][chan] = _KCa_E;
					m_gmax0_arr[i][chan] = _KCa_GMAX0;
					m_chan_num_m_arr[i][chan] = _KCa_NUM_M;  
					m_chan_num_h_arr[i][chan] = _KCa_NUM_H; break;
				case _I_Kd:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_Kd_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_Kd_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _Kd_CaCHANNEL;
					m_E_arr[i][chan] = _Kd_E;
					m_gmax0_arr[i][chan] = _Kd_GMAX0;
					m_chan_num_m_arr[i][chan] = _Kd_NUM_M;  
					m_chan_num_h_arr[i][chan] = _Kd_NUM_H; break;
				case _I_h:
					if (i == _Soma)
						m_bChanExists[i][chan] = _SOMA_h_EXISTS;
					else if (i == _Hillock)
						m_bChanExists[i][chan] = _HILLOCK_h_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_bCaChannel[i][chan] = _h_CaCHANNEL;
					m_E_arr[i][chan] = _h_E;
					m_gmax0_arr[i][chan] = _h_GMAX0;
					m_chan_num_m_arr[i][chan] = _h_NUM_M;  
					m_chan_num_h_arr[i][chan] = _h_NUM_H; break;
			}
			m_bChangedE[i][chan] = FALSE;
			m_bChangedgMax0[i][chan] = FALSE;
		}  //for chan
		for (int sens = 0; sens < _MAX_SENSORS; sens++) {
			switch (sens) {
				case _DC:
					if (i == _Soma)
						m_bSensExists[i][sens] = _SOMA_DC_EXISTS;
					else if (i == _Hillock)
						m_bSensExists[i][sens] = _HILLOCK_DC_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_Target_arr[i][sens] = _DC_TARGET;
					m_sens_ValueMax_arr[i][sens] = _DC_VALUEMAX;
					m_sens_tau_m_arr[i][sens] = _DC_TAU_M;  
					m_sens_tau_h_arr[i][sens] = _DC_TAU_H;
					m_sens_Z_m_arr[i][sens] = _DC_M_INF_Z;  
					m_sens_Z_h_arr[i][sens] = _DC_H_INF_Z;
					m_sens_num_m_arr[i][sens] = _DC_NUM_M;  
					m_sens_num_h_arr[i][sens] = _DC_NUM_H; break;
				case _Slow:
					if (i == _Soma)
						m_bSensExists[i][sens] = _SOMA_Slow_EXISTS;
					else if (i == _Hillock)
						m_bSensExists[i][sens] = _HILLOCK_Slow_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_Target_arr[i][sens] = _Slow_TARGET;
					m_sens_ValueMax_arr[i][sens] = _Slow_VALUEMAX;
					m_sens_tau_m_arr[i][sens] = _Slow_TAU_M;  
					m_sens_tau_h_arr[i][sens] = _Slow_TAU_H;
					m_sens_Z_m_arr[i][sens] = _Slow_M_INF_Z;  
					m_sens_Z_h_arr[i][sens] = _Slow_H_INF_Z;
					m_sens_num_m_arr[i][sens] = _Slow_NUM_M;  
					m_sens_num_h_arr[i][sens] = _Slow_NUM_H; break;
				case _Fast:
					if (i == _Soma)
						m_bSensExists[i][sens] = _SOMA_Fast_EXISTS;
					else if (i == _Hillock)
						m_bSensExists[i][sens] = _HILLOCK_Fast_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_Target_arr[i][sens] = _Fast_TARGET;
					m_sens_ValueMax_arr[i][sens] = _Fast_VALUEMAX;
					m_sens_tau_m_arr[i][sens] = _Fast_TAU_M;  
					m_sens_tau_h_arr[i][sens] = _Fast_TAU_H;
					m_sens_Z_m_arr[i][sens] = _Fast_M_INF_Z;  
					m_sens_Z_h_arr[i][sens] = _Fast_H_INF_Z;
					m_sens_num_m_arr[i][sens] = _Fast_NUM_M;  
					m_sens_num_h_arr[i][sens] = _Fast_NUM_H; break;
			}
			m_bChangedSensValueMax[i][sens] = FALSE;
			m_bChangedSensTauM[i][sens] = FALSE;
			m_bChangedSensTauH[i][sens] = FALSE;
			m_bChangedSensZM[i][sens] = FALSE;
			m_bChangedSensZH[i][sens] = FALSE;
			m_bChangedTarget[i][sens] = FALSE;
		} //for sens
		for (int syn = 0; syn < _MAX_SYNAPSES; syn++) {
			switch (syn) {
				case _Inhib:
					if (i == _Soma)
						m_bSynExists[i][syn] = _SOMA_Inhib_EXISTS;
					else if (i == _Hillock)
						m_bSynExists[i][syn] = _HILLOCK_Inhib_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_syn_E_arr[i][syn] = _Inhib_E;
					m_syn_TauDecay_arr[i][syn] = _Inhib_TAU_DECAY;
					m_syn_GStep_arr[i][syn] = _Inhib_GSTEP; break;
				case _Excit:
					if (i == _Soma)
						m_bSynExists[i][syn] = _SOMA_Excit_EXISTS;
					else if (i == _Hillock)
						m_bSynExists[i][syn] = _HILLOCK_Excit_EXISTS;
					else AfxMessageBox("No such compartment!");
					m_syn_E_arr[i][syn] = _Excit_E;
					m_syn_TauDecay_arr[i][syn] = _Excit_TAU_DECAY;
					m_syn_GStep_arr[i][syn] = _Excit_GSTEP; break;
			}
			m_bChangedSynE[i][syn] = FALSE;
			m_bChangedSynTauDecay[i][syn] = FALSE;
			m_bChangedSynGStep[i][syn] = FALSE;
		} //for syn
	} //for i (compartment)

	//{{AFX_DATA_INIT(CNeuronPage)
	m_resistivity = _RESISTIVITY;
	m_gAxial = (S_TO_mS*CM_TO_UM*_SOMA_R * SQR(_HILLOCK_R) /(_SOMA_L*_RESISTIVITY*((_SOMA_L * SQR(_HILLOCK_R)) + (_HILLOCK_L * SQR(_SOMA_R)))));;
	m_Area = 2.0*_PI*_SOMA_L*_SOMA_R/SQR(CM_TO_UM);
	m_C = _C;
	m_V0 = _V0;
	m_Ca0 = _Ca0;
	m_length = _SOMA_L;
	m_radius = _SOMA_R;
	m_CompartType = _Soma;
	m_ChanType = _I_Leak;
	m_SensorType = _DC;
	m_SynType = _Inhib;
	m_E = _Leak_E;
	m_gmax0_A = _A_GMAX0;
	m_gmax0_CaP = _CaP_GMAX0;
	m_gmax0_CaT = _CaT_GMAX0;
	m_gmax0_h = _h_GMAX0;
	m_gmax0_KCa = _KCa_GMAX0;
	m_gmax0_Kd = _Kd_GMAX0;
	m_gmax0_Leak = _Leak_GMAX0;
	m_gmax0_Na = _Na_GMAX0;
	m_chan_num_h = _Leak_NUM_H;
	m_chan_num_m = _Leak_NUM_M;
	m_sens_ValueMax = _DC_VALUEMAX;
	m_sens_num_h = _DC_NUM_H;
	m_sens_num_m = _DC_NUM_M;
	m_sens_tau_h = _DC_TAU_H;
	m_sens_tau_m = _DC_TAU_M;
	m_sens_Z_h = _DC_H_INF_Z;
	m_sens_Z_m = _DC_M_INF_Z;
	m_DC_Target = _DC_TARGET;
	m_Slow_Target = _Slow_TARGET;
	m_Fast_Target = _Fast_TARGET;
	m_syn_E = _Inhib_E;
	m_syn_GStep = _Inhib_GSTEP;
	m_syn_TauDecay = _Inhib_TAU_DECAY;
	//}}AFX_DATA_INIT

}

CNeuronPage::~CNeuronPage()
{
}

void CNeuronPage::DoDataExchange(CDataExchange* pDX)
{
	//UpdateData(TRUE) takes stuff in edit boxes and puts it in member vars
	//UpdateData(FALSE) takes stuff in member vars and puts them in edit boxes
	//AfxMessageBox("Starting DoDataExchange...");
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CNeuronPage)
	DDX_Text(pDX, IDC_RESISTIVITY_EDIT, m_resistivity);
	DDV_MinMaxDouble(pDX, m_resistivity, 1.e-007, 10000000.);
	DDX_Text(pDX, IDC_AXIAL_CONDUCT_EDIT, m_gAxial);
	DDV_MinMaxDouble(pDX, m_gAxial, 0., 10000000.);
	DDX_Text(pDX, IDC_AREA_EDIT, m_Area);
	DDV_MinMaxDouble(pDX, m_Area, 0., 10000000.);
	DDX_Text(pDX, IDC_C_EDIT, m_C);
	DDV_MinMaxDouble(pDX, m_C, 1.e-003, 1000.);
	DDX_Text(pDX, IDC_INIT_V_EDIT, m_V0);
	DDV_MinMaxDouble(pDX, m_V0, -200., 200.);
	DDX_Text(pDX, IDC_INIT_Ca_EDIT, m_Ca0);
	DDV_MinMaxDouble(pDX, m_Ca0, 0., 100.);
	DDX_Text(pDX, IDC_LENGTH_EDIT, m_length);
	DDV_MinMaxDouble(pDX, m_length, 1.e-007, 10000000.);
	DDX_Text(pDX, IDC_RADIUS_EDIT, m_radius);
	DDV_MinMaxDouble(pDX, m_radius, 1.e-007, 10000.);
	DDX_CBIndex(pDX, IDC_COMPART_COMBO, m_CompartType);
	DDX_CBIndex(pDX, IDC_CHAN_COMBO, m_ChanType);
	DDX_CBIndex(pDX, IDC_SENS_COMBO, m_SensorType);
	DDX_CBIndex(pDX, IDC_SYN_COMBO, m_SynType);
	DDX_Text(pDX, IDC_E_EDIT, m_E);
	DDV_MinMaxDouble(pDX, m_E, -200., 300.);
	DDX_Text(pDX, IDC_A_GMAX0_EDIT, m_gmax0_A);
	DDX_Text(pDX, IDC_CaP_GMAX0_EDIT, m_gmax0_CaP);
	DDX_Text(pDX, IDC_CaT_GMAX0_EDIT, m_gmax0_CaT);
	DDX_Text(pDX, IDC_h_GMAX0_EDIT, m_gmax0_h);
	DDV_MinMaxDouble(pDX, m_gmax0_h, 0., 1000.);
	DDX_Text(pDX, IDC_KCa_GMAX0_EDIT, m_gmax0_KCa);
	DDX_Text(pDX, IDC_Kd_GMAX0_EDIT, m_gmax0_Kd);
	DDX_Text(pDX, IDC_Leak_GMAX0_EDIT, m_gmax0_Leak);
	DDX_Text(pDX, IDC_Na_GMAX0_EDIT, m_gmax0_Na);
	DDX_Text(pDX, IDC_CHAN_NUM_H_EDIT, m_chan_num_h);
	DDX_Text(pDX, IDC_CHAN_NUM_M_EDIT, m_chan_num_m);
	DDX_Text(pDX, IDC_SENS_VALUEMAX_EDIT, m_sens_ValueMax);
	DDX_Text(pDX, IDC_SENS_NUM_H_EDIT, m_sens_num_h);
	DDX_Text(pDX, IDC_SENS_NUM_M_EDIT, m_sens_num_m);
	DDX_Text(pDX, IDC_SENS_TAU_H_EDIT, m_sens_tau_h);
	DDX_Text(pDX, IDC_SENS_TAU_M_EDIT, m_sens_tau_m);
	DDX_Text(pDX, IDC_SENS_Z_H_EDIT, m_sens_Z_h);
	DDX_Text(pDX, IDC_SENS_Z_M_EDIT, m_sens_Z_m);
	DDX_Text(pDX, IDC_DC_TARGET_EDIT, m_DC_Target);
	DDX_Text(pDX, IDC_SLOW_TARGET_EDIT, m_Slow_Target);
	DDX_Text(pDX, IDC_FAST_TARGET_EDIT, m_Fast_Target);
	DDX_Text(pDX, IDC_SYN_E_EDIT, m_syn_E);
	DDV_MinMaxDouble(pDX, m_syn_E, -200., 300.);
	DDX_Text(pDX, IDC_SYN_GSTEP_EDIT, m_syn_GStep);
	DDX_Text(pDX, IDC_SYN_TAUDECAY_EDIT, m_syn_TauDecay);
	//}}AFX_DATA_MAP

	//set values for channel/compartment you were previously viewing
	//ADD FLAG HERE FOR WHEN OPENING FROM SAVED FILE (SERIALIZING)????
	if (!m_bChangingComparts) {
		m_Area_arr[m_CompartType] = m_Area;
		m_gAxial_arr[m_CompartType] = m_gAxial;
		m_C_arr[m_CompartType] = m_C;
		m_R_arr[m_CompartType] = m_radius;
		m_L_arr[m_CompartType] = m_length;
		m_V0_arr[m_CompartType] = m_V0;
		m_Ca0_arr[m_CompartType] = m_Ca0;
		for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
			switch (chan) {
				case _I_Leak: m_gmax0_arr[m_CompartType][chan] = m_gmax0_Leak; break;
				case _I_Na: m_gmax0_arr[m_CompartType][chan] = m_gmax0_Na; break;
				case _I_CaT: m_gmax0_arr[m_CompartType][chan] = m_gmax0_CaT; break;
				case _I_CaP: m_gmax0_arr[m_CompartType][chan] = m_gmax0_CaP; break;
				case _I_A: m_gmax0_arr[m_CompartType][chan] = m_gmax0_A; break;
				case _I_KCa: m_gmax0_arr[m_CompartType][chan] = m_gmax0_KCa; break;
				case _I_Kd: m_gmax0_arr[m_CompartType][chan] = m_gmax0_Kd; break;
				case _I_h: m_gmax0_arr[m_CompartType][chan] = m_gmax0_h; break;
			}
		}
		for (int sens = 0; sens < _MAX_SENSORS; sens++) {
			switch (sens) {
				case _DC: m_Target_arr[m_CompartType][sens] = m_DC_Target; break;
				case _Slow: m_Target_arr[m_CompartType][sens] = m_Slow_Target; break;
				case _Fast: m_Target_arr[m_CompartType][sens] = m_Fast_Target; break;
			}
		}
		if (!m_bChangingChannels) {
			m_E_arr[m_CompartType][m_ChanType] = m_E;
			//m_gmax0_arr[m_CompartType][m_ChanType] = m_gmax;
			m_chan_num_m_arr[m_CompartType][m_ChanType] = m_chan_num_m;
			m_chan_num_h_arr[m_CompartType][m_ChanType] = m_chan_num_h;
		}
		if (!m_bChangingSensors) {
			m_sens_ValueMax_arr[m_CompartType][m_SensorType] = m_sens_ValueMax;
			m_sens_tau_m_arr[m_CompartType][m_SensorType] = m_sens_tau_m;
			m_sens_tau_h_arr[m_CompartType][m_SensorType] = m_sens_tau_h;
			m_sens_Z_m_arr[m_CompartType][m_SensorType] = m_sens_Z_m;
			m_sens_Z_h_arr[m_CompartType][m_SensorType] = m_sens_Z_h;
			m_sens_num_m_arr[m_CompartType][m_SensorType] = m_sens_num_m;
			m_sens_num_h_arr[m_CompartType][m_SensorType] = m_sens_num_h;
		}
		if (!m_bChangingSynapses) {
			m_syn_E_arr[m_CompartType][m_SynType] = m_syn_E;
			m_syn_TauDecay_arr[m_CompartType][m_SynType] = m_syn_TauDecay;
			m_syn_GStep_arr[m_CompartType][m_SynType] = m_syn_GStep;
		}
	}
}


BEGIN_MESSAGE_MAP(CNeuronPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNeuronPage)
	ON_BN_CLICKED(IDC_REMOVE_CHAN_BUTT, OnRemoveChanButt)
	ON_BN_CLICKED(IDC_APPLY_BUTT, OnApplyButt)
	ON_CBN_SELCHANGE(IDC_CHAN_COMBO, OnSelchangeChanCombo)
	ON_EN_CHANGE(IDC_C_EDIT, OnChangeCEdit)
	ON_EN_KILLFOCUS(IDC_E_EDIT, OnKillfocusEEdit)
	ON_EN_KILLFOCUS(IDC_INIT_V_EDIT, OnKillfocusInitVEdit)
	ON_EN_CHANGE(IDC_INIT_Ca_EDIT, OnChangeINITCaEDIT)
	ON_EN_CHANGE(IDC_E_EDIT, OnChangeEEdit)
	ON_EN_CHANGE(IDC_INIT_V_EDIT, OnChangeInitVEdit)
	ON_CBN_SELCHANGE(IDC_COMPART_COMBO, OnSelchangeCompartCombo)
	ON_EN_CHANGE(IDC_LENGTH_EDIT, OnChangeLengthEdit)
	ON_EN_CHANGE(IDC_RADIUS_EDIT, OnChangeRadiusEdit)
	ON_EN_CHANGE(IDC_RESISTIVITY_EDIT, OnChangeResistivityEdit)
	ON_EN_KILLFOCUS(IDC_C_EDIT, OnKillfocusCEdit)
	ON_EN_KILLFOCUS(IDC_LENGTH_EDIT, OnKillfocusLengthEdit)
	ON_EN_KILLFOCUS(IDC_RADIUS_EDIT, OnKillfocusRadiusEdit)
	ON_EN_KILLFOCUS(IDC_INIT_Ca_EDIT, OnKillfocusINITCaEDIT)
	ON_BN_CLICKED(IDC_REMOVE_SENS_BUTT, OnRemoveSensButt)
	ON_CBN_SELCHANGE(IDC_SENS_COMBO, OnSelchangeSensCombo)
	ON_EN_CHANGE(IDC_CHAN_NUM_M_EDIT, OnChangeChanNumMEdit)
	ON_EN_CHANGE(IDC_CHAN_NUM_H_EDIT, OnChangeChanNumHEdit)
	ON_EN_CHANGE(IDC_SENS_NUM_M_EDIT, OnChangeSensNumMEdit)
	ON_EN_CHANGE(IDC_SENS_NUM_H_EDIT, OnChangeSensNumHEdit)
	ON_EN_CHANGE(IDC_Leak_GMAX0_EDIT, OnChangeLeakGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_Leak_GMAX0_EDIT, OnKillfocusLeakGMAX0EDIT)
	ON_EN_CHANGE(IDC_Na_GMAX0_EDIT, OnChangeNaGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_Na_GMAX0_EDIT, OnKillfocusNaGMAX0EDIT)
	ON_EN_CHANGE(IDC_CaT_GMAX0_EDIT, OnChangeCaTGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_CaT_GMAX0_EDIT, OnKillfocusCaTGMAX0EDIT)
	ON_EN_CHANGE(IDC_CaP_GMAX0_EDIT, OnChangeCaPGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_CaP_GMAX0_EDIT, OnKillfocusCaPGMAX0EDIT)
	ON_EN_CHANGE(IDC_A_GMAX0_EDIT, OnChangeAGmax0Edit)
	ON_EN_KILLFOCUS(IDC_A_GMAX0_EDIT, OnKillfocusAGmax0Edit)
	ON_EN_CHANGE(IDC_KCa_GMAX0_EDIT, OnChangeKCaGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_KCa_GMAX0_EDIT, OnKillfocusKCaGMAX0EDIT)
	ON_EN_CHANGE(IDC_Kd_GMAX0_EDIT, OnChangeKdGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_Kd_GMAX0_EDIT, OnKillfocusKdGMAX0EDIT)
	ON_EN_CHANGE(IDC_h_GMAX0_EDIT, OnChangehGMAX0EDIT)
	ON_EN_KILLFOCUS(IDC_h_GMAX0_EDIT, OnKillfocushGMAX0EDIT)
	ON_EN_CHANGE(IDC_SENS_VALUEMAX_EDIT, OnChangeSensValuemaxEdit)
	ON_EN_KILLFOCUS(IDC_SENS_VALUEMAX_EDIT, OnKillfocusSensValuemaxEdit)
	ON_EN_CHANGE(IDC_SENS_TAU_M_EDIT, OnChangeSensTauMEdit)
	ON_EN_KILLFOCUS(IDC_SENS_TAU_M_EDIT, OnKillfocusSensTauMEdit)
	ON_EN_CHANGE(IDC_SENS_TAU_H_EDIT, OnChangeSensTauHEdit)
	ON_EN_KILLFOCUS(IDC_SENS_TAU_H_EDIT, OnKillfocusSensTauHEdit)
	ON_EN_CHANGE(IDC_SENS_Z_M_EDIT, OnChangeSensZMEdit)
	ON_EN_KILLFOCUS(IDC_SENS_Z_M_EDIT, OnKillfocusSensZMEdit)
	ON_EN_CHANGE(IDC_SENS_Z_H_EDIT, OnChangeSensZHEdit)
	ON_EN_KILLFOCUS(IDC_SENS_Z_H_EDIT, OnKillfocusSensZHEdit)
	ON_EN_CHANGE(IDC_DC_TARGET_EDIT, OnChangeDcTargetEdit)
	ON_EN_KILLFOCUS(IDC_DC_TARGET_EDIT, OnKillfocusDcTargetEdit)
	ON_EN_CHANGE(IDC_SLOW_TARGET_EDIT, OnChangeSlowTargetEdit)
	ON_EN_KILLFOCUS(IDC_SLOW_TARGET_EDIT, OnKillfocusSlowTargetEdit)
	ON_EN_CHANGE(IDC_FAST_TARGET_EDIT, OnChangeFastTargetEdit)
	ON_EN_KILLFOCUS(IDC_FAST_TARGET_EDIT, OnKillfocusFastTargetEdit)
	ON_CBN_SELCHANGE(IDC_SYN_COMBO, OnSelchangeSynCombo)
	ON_EN_CHANGE(IDC_SYN_E_EDIT, OnChangeSynEEdit)
	ON_EN_KILLFOCUS(IDC_SYN_E_EDIT, OnKillfocusSynEEdit)
	ON_EN_CHANGE(IDC_SYN_TAUDECAY_EDIT, OnChangeSynTaudecayEdit)
	ON_EN_KILLFOCUS(IDC_SYN_TAUDECAY_EDIT, OnKillfocusSynTaudecayEdit)
	ON_EN_CHANGE(IDC_SYN_GSTEP_EDIT, OnChangeSynGstepEdit)
	ON_EN_KILLFOCUS(IDC_SYN_GSTEP_EDIT, OnKillfocusSynGstepEdit)
	ON_BN_CLICKED(IDC_REMOVE_SYN_BUTT, OnRemoveSynButt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeuronPage message handlers

BOOL CNeuronPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//ctrlCSpin().SetRange(0,1000);
	m_bChangeMade = FALSE;
	ctrlApplyButt().EnableWindow(FALSE);

	//fill in static text boxes with appropriate starting values
	char str[10];
	//gmax's
	sprintf(str, "%.3g", m_gmax0_Leak);
	ctrlgMaxLeakStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_Na);
	ctrlgMaxNaStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_CaT);
	ctrlgMaxCaTStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_CaP);
	ctrlgMaxCaPStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_A);
	ctrlgMaxAStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_KCa);
	ctrlgMaxKCaStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_Kd);
	ctrlgMaxKdStatic().SetWindowText(str);
	sprintf(str, "%.3g", m_gmax0_h);
	ctrlgMaxhStatic().SetWindowText(str);
	//sensor values--leave as 0

	//enable/disable window depending on whether channel exists
	ctrlEEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	//if a Ca Channel, disable user set for E -- determined by init Ca (Nernst Equ.)
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlEEdit().EnableWindow(!m_bCaChannel[m_CompartType][m_ChanType]);
	for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
		switch (chan) {
				case _I_Leak: 
					ctrlgMax0LeakEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxLeakStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_Na:   
					ctrlgMax0NaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxNaStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_CaT:  
					ctrlgMax0CaTEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxCaTStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_CaP:  
					ctrlgMax0CaPEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxCaPStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_A:    
					ctrlgMax0AEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxAStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_KCa:  
					ctrlgMax0KCaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxKCaStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_Kd:  
					ctrlgMax0KdEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxKdStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
				case _I_h:  
					ctrlgMax0hEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
					ctrlgMaxhStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
		}
	}
	//enable/disable window depending on whether sensor exists
	ctrlSens_ValueMaxEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	for (int sens = 0; sens < _MAX_SENSORS; sens++) {		
		switch (sens) {
			case _DC: 
				ctrlDC_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				ctrlDC_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				break;
			case _Slow:  
				ctrlSlow_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				ctrlSlow_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				break;
			case _Fast:  
				ctrlFast_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				ctrlFast_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				break;
		}
	}
	//enable/disable window depending on whether sensor exists
	ctrlSyn_EEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlRemoveChanButt().SetWindowText("&Remove");
	else ctrlRemoveChanButt().SetWindowText("A&dd");
	if (m_bSensExists[m_CompartType][m_SensorType])
		ctrlRemoveSensButt().SetWindowText("Remove");
	else ctrlRemoveSensButt().SetWindowText("Add");
	if (m_bSynExists[m_CompartType][m_SynType])
		ctrlRemoveSynButt().SetWindowText("Remove");
	else ctrlRemoveSynButt().SetWindowText("Add");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNeuronPage::UpdateGMaxDisplay() {
	char str[10];
	if (IsWindow(m_hWnd)) {
		for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
			if (m_bChanExists[m_CompartType][chan]) {
				sprintf(str, "%.4g", GetChan(m_CompartType, chan)->m_gMax);
				switch (chan) {
					case _I_Leak: ctrlgMaxLeakStatic().SetWindowText(str); break;
					case _I_Na: ctrlgMaxNaStatic().SetWindowText(str); break;
					case _I_CaT: ctrlgMaxCaTStatic().SetWindowText(str); break;
					case _I_CaP: ctrlgMaxCaPStatic().SetWindowText(str); break;
					case _I_A: ctrlgMaxAStatic().SetWindowText(str); break;
					case _I_KCa: ctrlgMaxKCaStatic().SetWindowText(str); break;
					case _I_Kd: ctrlgMaxKdStatic().SetWindowText(str); break;
					case _I_h: ctrlgMaxhStatic().SetWindowText(str); break;
				}
			}
		}
	}
}

void CNeuronPage::UpdateSensorValueDisplay() {
	char str[10];
	if (IsWindow(m_hWnd)) {
		for (int sens = 0; sens < _MAX_SENSORS; sens++) {
			//if (m_bSensExists[m_CompartType][sens]) {
				sprintf(str, "%.4g", GetSensor(m_CompartType, sens)->m_AveValue);
				switch (sens) {
					case _DC: ctrlDC_ValueStatic().SetWindowText(str); break;
					case _Slow: ctrlSlow_ValueStatic().SetWindowText(str); break;
					case _Fast: ctrlFast_ValueStatic().SetWindowText(str); break;
				}
			//}
		}
	}
}
	
void CNeuronPage::OnRemoveChanButt() 
{
	m_bChanExists[m_CompartType][m_ChanType] = !m_bChanExists[m_CompartType][m_ChanType];
	ctrlEEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	
	switch (m_ChanType) {
		case _I_Leak: 
			ctrlgMax0LeakEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxLeakStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_Na:   
			ctrlgMax0NaEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxNaStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_CaT:  
			ctrlgMax0CaTEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxCaTStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_CaP:  
			ctrlgMax0CaPEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxCaPStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_A:    
			ctrlgMax0AEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxAStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_KCa:  
			ctrlgMax0KCaEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxKCaStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_Kd:  
			ctrlgMax0KdEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxKdStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
		case _I_h:  
			ctrlgMax0hEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); 
			ctrlgMaxhStatic().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]); break;
	}
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlRemoveChanButt().SetWindowText("&Remove");
	else ctrlRemoveChanButt().SetWindowText("A&dd");
	m_bChangeMade = TRUE;
	ctrlApplyButt().EnableWindow(TRUE);	
}

void CNeuronPage::OnRemoveSensButt() 
{
	m_bSensExists[m_CompartType][m_SensorType] = !m_bSensExists[m_CompartType][m_SensorType];
	ctrlSens_ValueMaxEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	
	switch (m_SensorType) {
		case _DC: 
			ctrlDC_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); 
			ctrlDC_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
		case _Slow: 
			ctrlSlow_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); 
			ctrlSlow_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
		case _Fast: 
			ctrlFast_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); 
			ctrlFast_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
	}
	if (m_bSensExists[m_CompartType][m_SensorType])
		ctrlRemoveSensButt().SetWindowText("&Remove");
	else ctrlRemoveSensButt().SetWindowText("A&dd");
	m_bChangeMade = TRUE;
	ctrlApplyButt().EnableWindow(TRUE);
	
}

void CNeuronPage::OnRemoveSynButt() 
{
	m_bSynExists[m_CompartType][m_SynType] = !m_bSynExists[m_CompartType][m_SynType];
	ctrlSyn_EEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	if (m_bSynExists[m_CompartType][m_SynType])
		ctrlRemoveSynButt().SetWindowText("Remove");
	else ctrlRemoveSynButt().SetWindowText("Add");
	m_bChangeMade = TRUE;
	ctrlApplyButt().EnableWindow(TRUE);
	
}

void CNeuronPage::OnApplyButt() 
{
	//AfxMessageBox("Starting OnApplyButt...");	

	//fill in Neuron parameters
	double conductance = (S_TO_mS/CM_TO_UM)*2.0*_PI*SQR(m_R_arr[_Soma]*m_R_arr[_Hillock])/(m_resistivity*
						(m_L_arr[_Soma]*SQR(m_R_arr[_Hillock]) + m_L_arr[_Hillock]*SQR(m_R_arr[_Soma])));
	double area;
	double shell_vol; //vol of shell in which compute Ca concentration
	double min_sensor_target;
	for (int i = 0; i < _MAX_COMPARTS; i++) {
		area = 2.0*_PI*m_R_arr[i]*m_L_arr[i]/SQR(CM_TO_UM);
		shell_vol = _PI*m_L_arr[i]*_SHELL_T*(2*m_R_arr[i] - _SHELL_T); //[um^3]
		GetCompart(i)->m_Area = area;
		GetCompart(i)->m_nA_to_uMCa = (_NA_TO_UMCA_PREFACTOR*_TAU_CA/(_CA_VALENCE*shell_vol)); //[uM]/[nA]
		if (i == _Soma) {
			GetCompart(i)->m_gAxial[_Soma] = 0.0;
			GetCompart(i)->m_gAxial[_Hillock] = conductance/area;
		}
		else if (i == _Hillock) {
			GetCompart(i)->m_gAxial[_Soma] = conductance/area;
			GetCompart(i)->m_gAxial[_Hillock] = 0.0;
		}
		else AfxMessageBox("No such compartment!");
		GetCompart(i)->m_C = m_C_arr[i];
		GetCompart(i)->m_V0 = m_V0_arr[i];
		GetCompart(i)->m_Ca0 = m_Ca0_arr[i];
		for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
			GetChan(i, chan)->m_bExists = m_bChanExists[i][chan];
			GetChan(i, chan)->m_E = m_E_arr[i][chan];
			//only update initial/new variable values if they have changed
			if (m_bChangedgMax0[i][chan]) {
				GetChan(i, chan)->m_gMax0 = m_gmax0_arr[i][chan];
				//UPDATE GMAX TO EQUAL GMAX0
				GetChan(i, chan)->m_gMax = m_gmax0_arr[i][chan];
				m_bChangedgMax0[i][chan] = FALSE;
			}
			GetChan(i, chan)->m_p = m_chan_num_m_arr[i][chan];
			GetChan(i, chan)->m_q = m_chan_num_h_arr[i][chan];		
		}
		min_sensor_target = 99999999; //larger than ever should be
		for (int sens = 0; sens < _MAX_SENSORS; sens++) {
			GetSensor(i, sens)->m_bExists = m_bSensExists[i][sens];
			GetSensor(i, sens)->m_ValueMax = m_sens_ValueMax_arr[i][sens];
			GetSensor(i, sens)->m_tau_m = m_sens_tau_m_arr[i][sens];
			GetSensor(i, sens)->m_tau_h = m_sens_tau_h_arr[i][sens];
			GetSensor(i, sens)->m_m_inf_Z = m_sens_Z_m_arr[i][sens];
			GetSensor(i, sens)->m_h_inf_Z = m_sens_Z_h_arr[i][sens];
			GetSensor(i, sens)->m_p = m_sens_num_m_arr[i][sens];
			GetSensor(i, sens)->m_q = m_sens_num_h_arr[i][sens];
			GetSensor(i, sens)->m_Target = m_Target_arr[i][sens];
			if ((m_bSensExists[i][sens]) AND (m_Target_arr[i][sens] < min_sensor_target)) {
				min_sensor_target = m_Target_arr[i][sens];	
			}
		}
		for (int syn = 0; syn < _MAX_SYNAPSES; syn++) {
			GetSyn(i, syn)->m_bExists = m_bSynExists[i][syn];
			GetSyn(i, syn)->m_E = m_syn_E_arr[i][syn];
			GetSyn(i, syn)->m_tau_decay = m_syn_TauDecay_arr[i][syn];
			GetSyn(i, syn)->m_gStep = m_syn_GStep_arr[i][syn];
		}
		//GetCompart(i)->m_tau_eff = _TAU_GMAX/min_sensor_target;
		//ADD BACK IN NEXT LINE IF DOING ADJUST ON THE FLY CODE--
		//REMOVED FOR PURPOSES OF MAPPING CONDUCTANCE SPACE AND SENSOR SPACE
		//GetCompart(i)->m_tau_eff = _TAU_GMAX/(10.0*min_sensor_target);
	}
	
	//Do the data exchange -- this is normally handled by the OnOK handler, but we
	//have eliminated that function.
	//UpdateData(TRUE);
	m_bChangeMade = FALSE;
	ctrlApplyButt().EnableWindow(FALSE);
	
}

void CNeuronPage::OnSelchangeCompartCombo() 
{
	m_bChangingComparts = TRUE;
	//OnApplyButt();
	UpdateData(TRUE); //will get the new m_CompartType
	
	//fill in edit boxes, which will then be read into member vars by DoDataExchange
	char str[50];
	sprintf(str, "%.3g", m_Area_arr[m_CompartType]);
	ctrlAreaEdit().SetWindowText(str);
	sprintf(str, "%.3g", m_gAxial_arr[m_CompartType]);
	ctrlgAxialEdit().SetWindowText(str);
	sprintf(str, "%g", m_C_arr[m_CompartType]);
	ctrlCEdit().SetWindowText(str);
	sprintf(str, "%g", m_L_arr[m_CompartType]);
	ctrlLEdit().SetWindowText(str);
	sprintf(str, "%g", m_R_arr[m_CompartType]);
	ctrlREdit().SetWindowText(str);
	sprintf(str, "%g", m_V0_arr[m_CompartType]);
	ctrlInitVEdit().SetWindowText(str);
	sprintf(str, "%g", m_Ca0_arr[m_CompartType]);
	ctrlInitCaEdit().SetWindowText(str);
	sprintf(str, "%g", m_E_arr[m_CompartType][m_ChanType]);
	  //chan edit boxes
	ctrlEEdit().SetWindowText(str);
	sprintf(str, "%d", m_chan_num_m_arr[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().SetWindowText(str);
	sprintf(str, "%d", m_chan_num_h_arr[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().SetWindowText(str);
    for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
		sprintf(str, "%g", m_gmax0_arr[m_CompartType][chan]);
		switch (chan) {
			case _I_Leak: ctrlgMax0LeakEdit().SetWindowText(str); break;
			case _I_Na:   ctrlgMax0NaEdit().SetWindowText(str); break;
			case _I_CaT:  ctrlgMax0CaTEdit().SetWindowText(str); break;
			case _I_CaP:  ctrlgMax0CaPEdit().SetWindowText(str); break;
			case _I_A:    ctrlgMax0AEdit().SetWindowText(str); break;
			case _I_KCa:  ctrlgMax0KCaEdit().SetWindowText(str); break;
			case _I_Kd:  ctrlgMax0KdEdit().SetWindowText(str); break;
			case _I_h:  ctrlgMax0hEdit().SetWindowText(str); break;
		}
	}
	  //sensor edit boxes
	sprintf(str, "%g", m_sens_ValueMax_arr[m_CompartType][m_SensorType]);
	ctrlSens_ValueMaxEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_tau_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_tau_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_Z_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_Z_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().SetWindowText(str);
	sprintf(str, "%d", m_sens_num_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().SetWindowText(str);
	sprintf(str, "%d", m_sens_num_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().SetWindowText(str);
    for (int sens = 0; sens < _MAX_SENSORS; sens++) {
		sprintf(str, "%g", m_Target_arr[m_CompartType][sens]);
		switch (chan) {
			case _DC: ctrlDC_TargetEdit().SetWindowText(str); break;
			case _Slow:  ctrlSlow_TargetEdit().SetWindowText(str); break;
			case _Fast:  ctrlFast_TargetEdit().SetWindowText(str); break;
		}
	}
	  //synapse edit boxes
	sprintf(str, "%g", m_syn_E_arr[m_CompartType][m_SynType]);
	ctrlSyn_EEdit().SetWindowText(str);
	sprintf(str, "%g", m_syn_TauDecay_arr[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().SetWindowText(str);
	sprintf(str, "%g", m_syn_GStep_arr[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().SetWindowText(str);

	UpdateData(TRUE);
	//enable/disable window depending on whether channel exists
	ctrlEEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	for (chan = 0; chan < _MAX_CHANNELS; chan++) {
		switch (chan) {
			case _I_Leak: 
				ctrlgMax0LeakEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxLeakStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_Na:   
				ctrlgMax0NaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxNaStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_CaT:  
				ctrlgMax0CaTEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxCaTStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_CaP:  
				ctrlgMax0CaPEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxCaPStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_A:    
				ctrlgMax0AEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxAStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_KCa:  
				ctrlgMax0KCaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxKCaStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_Kd:  
				ctrlgMax0KdEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxKdStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_h:  
				ctrlgMax0hEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); 
				ctrlgMaxhStatic().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
		}
	}
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlRemoveChanButt().SetWindowText("&Remove");
	else ctrlRemoveChanButt().SetWindowText("A&dd");
	//if a Ca Channel, disable user set for E -- determined by init Ca (Nernst Equ.)
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlEEdit().EnableWindow(!m_bCaChannel[m_CompartType][m_ChanType]);
	//enable/disable window depending on whether sensor exists
	ctrlSens_ValueMaxEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	for (sens = 0; sens < _MAX_SENSORS; sens++) {
		switch (sens) {
			case _DC: 
				ctrlDC_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); 
				ctrlDC_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
			case _Slow:  
				ctrlSlow_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]);
				ctrlSlow_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
			case _Fast:  
				ctrlFast_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]);
				ctrlFast_ValueStatic().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]); break;
		}
	}
	if (m_bSensExists[m_CompartType][m_SensorType])
		ctrlRemoveSensButt().SetWindowText("&Remove");
	else ctrlRemoveSensButt().SetWindowText("A&dd");
	//enable/disable window depending on whether synapse exists
	ctrlSyn_EEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	if (m_bSynExists[m_CompartType][m_SynType])
		ctrlRemoveSynButt().SetWindowText("Remove");
	else ctrlRemoveSynButt().SetWindowText("Add");

	m_bChangingComparts = FALSE;

}

void CNeuronPage::OnSelchangeChanCombo() 
{
	m_bChangingChannels = TRUE;
	//OnApplyButt();
	UpdateData(TRUE); //will get the new m_ChanType
	
	//fill in edit boxes, which will then be read into member vars by DoDataExchange
	char str[30];
	sprintf(str, "%g", m_E_arr[m_CompartType][m_ChanType]);
	ctrlEEdit().SetWindowText(str);
	sprintf(str, "%d", m_chan_num_m_arr[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().SetWindowText(str);
	sprintf(str, "%d", m_chan_num_h_arr[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().SetWindowText(str);
	/*for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
		sprintf(str, "%g", m_gmax0_arr[m_CompartType][chan]);
		switch (chan) {
			case _I_Leak: ctrlgMax0LeakEdit().SetWindowText(str); break;
			case _I_Na:   ctrlgMax0NaEdit().SetWindowText(str); break;
			case _I_CaT:  ctrlgMax0CaTEdit().SetWindowText(str); break;
			case _I_CaP:  ctrlgMax0CaPEdit().SetWindowText(str); break;
			case _I_A:    ctrlgMax0AEdit().SetWindowText(str); break;
			case _I_KCa:  ctrlgMax0KCaEdit().SetWindowText(str); break;
			case _I_Kd:  ctrlgMax0KdEdit().SetWindowText(str); break;
			case _I_h:  ctrlgMax0hEdit().SetWindowText(str); break;
		}
	}*/
	UpdateData(TRUE);
	
	//enable/disable window depending on whether channel exists
	ctrlEEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_mEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	ctrlChan_Num_hEdit().EnableWindow(m_bChanExists[m_CompartType][m_ChanType]);
	/*for (chan = 0; chan < _MAX_CHANNELS; chan++) {
		switch (chan) {
			case _I_Leak: ctrlgMax0LeakEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_Na:   ctrlgMax0NaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_CaT:  ctrlgMax0CaTEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_CaP:  ctrlgMax0CaPEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_A:    ctrlgMax0AEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_KCa:  ctrlgMax0KCaEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_Kd:  ctrlgMax0KdEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
			case _I_h:  ctrlgMax0hEdit().EnableWindow(m_bChanExists[m_CompartType][chan]); break;
		}
	}*/
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlRemoveChanButt().SetWindowText("&Remove");
	else ctrlRemoveChanButt().SetWindowText("A&dd");
	//if a Ca Channel, disable user set for E -- determined by init Ca (Nernst Equ.)
	if (m_bChanExists[m_CompartType][m_ChanType])
		ctrlEEdit().EnableWindow(!m_bCaChannel[m_CompartType][m_ChanType]);

	m_bChangingChannels = FALSE;

	//UpdateData(TRUE);

}

void CNeuronPage::OnSelchangeSensCombo() 
{
	m_bChangingSensors = TRUE;
	//OnApplyButt();
	UpdateData(TRUE); //will get the new m_SensType
	
	//fill in edit boxes, which will then be read into member vars by DoDataExchange
	char str[30];
	sprintf(str, "%g", m_sens_ValueMax_arr[m_CompartType][m_SensorType]);
	ctrlSens_ValueMaxEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_tau_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_tau_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_Z_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().SetWindowText(str);
	sprintf(str, "%g", m_sens_Z_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().SetWindowText(str);
	sprintf(str, "%d", m_sens_num_m_arr[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().SetWindowText(str);
	sprintf(str, "%d", m_sens_num_h_arr[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().SetWindowText(str);
    /*for (int sens = 0; sens < _MAX_SENSORS; sens++) {
		sprintf(str, "%g", m_Target_arr[m_CompartType][m_SensorType]);
		switch (sens) {
			case _DC: ctrlDC_TargetEdit().SetWindowText(str); break;
			case _Slow:  ctrlSlow_TargetEdit().SetWindowText(str); break;
			case _Fast:  ctrlFast_TargetEdit().SetWindowText(str); break;
		}
	}*/
	
	UpdateData(TRUE);
	
	//enable/disable window depending on whether sensor exists
	ctrlSens_ValueMaxEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Tau_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Z_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_mEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	ctrlSens_Num_hEdit().EnableWindow(m_bSensExists[m_CompartType][m_SensorType]);
	/*for (sens = 0; sens < _MAX_SENSORS; sens++) {
		switch (sens) {
			case _DC: ctrlDC_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); break;
			case _Slow:   ctrlSlow_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); break;
			case _Fast:  ctrlFast_TargetEdit().EnableWindow(m_bSensExists[m_CompartType][sens]); break;
		}
	}*/
	if (m_bSensExists[m_CompartType][m_SensorType])
		ctrlRemoveSensButt().SetWindowText("&Remove");
	else ctrlRemoveSensButt().SetWindowText("A&dd");

	m_bChangingSensors = FALSE;

	//UpdateData(TRUE);
	
}

void CNeuronPage::OnSelchangeSynCombo() 
{
	m_bChangingSynapses = TRUE;
	//OnApplyButt();
	UpdateData(TRUE); //will get the new m_SynType
	
	//fill in edit boxes, which will then be read into member vars by DoDataExchange
	char str[30];
	sprintf(str, "%g", m_syn_E_arr[m_CompartType][m_SynType]);
	ctrlSyn_EEdit().SetWindowText(str);
	sprintf(str, "%g", m_syn_TauDecay_arr[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().SetWindowText(str);
	sprintf(str, "%g", m_syn_GStep_arr[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().SetWindowText(str);
	
	UpdateData(TRUE);
	
	//enable/disable window depending on whether sensor exists
	ctrlSyn_EEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_TauDecayEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	ctrlSyn_GStepEdit().EnableWindow(m_bSynExists[m_CompartType][m_SynType]);
	if (m_bSynExists[m_CompartType][m_SynType])
		ctrlRemoveSynButt().SetWindowText("Remove");
	else ctrlRemoveSynButt().SetWindowText("Add");

	m_bChangingSynapses = FALSE;

	//UpdateData(TRUE);
	
}

void CNeuronPage::OnChangeResistivityEdit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE); //updates resistivity
		//fill in edit boxes, which will then be read into member vars by DoDataExchange
		char str[30];
		double conductance = S_TO_mS*2.0*_PI*SQR(m_R_arr[_Soma]*m_R_arr[_Hillock])/(CM_TO_UM*m_resistivity*
							 (m_L_arr[_Soma]*SQR(m_R_arr[_Hillock]) + m_L_arr[_Hillock]*SQR(m_R_arr[_Soma])));
		double gAxial = conductance/m_Area_arr[m_CompartType];
		sprintf(str, "%.3g", gAxial);
		ctrlgAxialEdit().SetWindowText(str);
		UpdateData(TRUE);
		//update other Axial conductances
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			if (i != m_CompartType) {
				m_gAxial_arr[i] = conductance/m_Area_arr[i];
			}
		}
		m_bChangeMade = TRUE;
		ctrlApplyButt().EnableWindow(TRUE);
	}	
}

void CNeuronPage::OnChangeRadiusEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedR[m_CompartType] = TRUE;
	}	
}

void CNeuronPage::OnKillfocusRadiusEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses AND 
		!m_bChangingComparts AND m_bChangedR[m_CompartType]) {
		
		UpdateData(TRUE); //update radius
		//fill in edit boxes, which will then be read into member vars by DoDataExchange
		char str[50];
		double conductance = S_TO_mS*2.0*_PI*SQR(m_R_arr[_Soma]*m_R_arr[_Hillock])/(CM_TO_UM*m_resistivity*
							 (m_L_arr[_Soma]*SQR(m_R_arr[_Hillock]) + m_L_arr[_Hillock]*SQR(m_R_arr[_Soma])));
		double Area = 2.0*_PI*m_R_arr[m_CompartType]*m_L_arr[m_CompartType]/SQR(CM_TO_UM);
		double gAxial = conductance/Area;
		sprintf(str, "%.3g", gAxial);
		ctrlgAxialEdit().SetWindowText(str);
		sprintf(str, "%.3g", Area);
		ctrlAreaEdit().SetWindowText(str);
		UpdateData(TRUE);
		//update other Axial conductances
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			if (i != m_CompartType) {
				m_gAxial_arr[i] = conductance/m_Area_arr[i];
			}
		}

		m_bChangedR[m_CompartType] = FALSE;
	}
	
}

void CNeuronPage::OnChangeLengthEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedL[m_CompartType] = TRUE;
	}	
}

void CNeuronPage::OnKillfocusLengthEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses AND 
		!m_bChangingComparts AND m_bChangedL[m_CompartType]) {
		
		UpdateData(TRUE); //fills in array info
		//fill in edit boxes, which will then be read into member vars by DoDataExchange
		char str[50];
		double conductance = S_TO_mS*2.0*_PI*SQR(m_R_arr[_Soma]*m_R_arr[_Hillock])/(CM_TO_UM*m_resistivity*
							 (m_L_arr[_Soma]*SQR(m_R_arr[_Hillock]) + m_L_arr[_Hillock]*SQR(m_R_arr[_Soma])));
		double Area = 2.0*_PI*m_R_arr[m_CompartType]*m_L_arr[m_CompartType]/SQR(CM_TO_UM);
		double gAxial = conductance/Area;
		sprintf(str, "%.3g", gAxial);
		ctrlgAxialEdit().SetWindowText(str);
		sprintf(str, "%.3g", Area);
		ctrlAreaEdit().SetWindowText(str);
		UpdateData(TRUE);
		//update other Axial conductances
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			if (i != m_CompartType) {
				m_gAxial_arr[i] = conductance/m_Area_arr[i];
			}
		}

		m_bChangedL[m_CompartType] = FALSE;
	}	
}


void CNeuronPage::OnChangeCEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedC[m_CompartType] = TRUE;
	}	
}

void CNeuronPage::OnKillfocusCEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses AND 
		!m_bChangingComparts AND m_bChangedC[m_CompartType]) {
		UpdateData(TRUE);
		m_bChangedC[m_CompartType] = FALSE;
	}	
	
}

void CNeuronPage::OnChangeInitVEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedV0[m_CompartType] = TRUE;
	}	
}


void CNeuronPage::OnKillfocusInitVEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses AND 
		m_bChangingComparts AND m_bChangedV0[m_CompartType]) {
		UpdateData(TRUE);
		m_bChangedV0[m_CompartType] = FALSE;
	}	
}

void CNeuronPage::OnChangeINITCaEDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedCa0[m_CompartType] = TRUE;
	}
}

void CNeuronPage::OnKillfocusINITCaEDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses AND
		!m_bChangingComparts AND m_bChangedCa0[m_CompartType]) {
		
		
		UpdateData(TRUE); //update Initial Ca array
		//fill in edit boxes, which will then be read into member vars by DoDataExchange
		char str[50];
		double E = (_NERNST_CA * log(_CA_OUT/m_Ca0_arr[m_CompartType]));
		if (m_bCaChannel[m_CompartType][m_ChanType]) { //if currently viewing a Ca channel
			sprintf(str, "%g", E);
			ctrlEEdit().SetWindowText(str);
			UpdateData(TRUE);
		}
		//update other Ca channel E's
		for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
			if (chan != m_ChanType AND m_bCaChannel[m_CompartType][chan]) {
				m_E_arr[m_CompartType][chan] = E;
			}
		}

		m_bChangedCa0[m_CompartType] = FALSE;

	}
	
}


void CNeuronPage::OnChangeEEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedE[m_CompartType][m_ChanType] = TRUE;
	}
}

void CNeuronPage::OnKillfocusEEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedE[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
}

void CNeuronPage::OnChangeLeakGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_Leak] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusLeakGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_Leak]) {
		UpdateData(TRUE);
	}
	
}

void CNeuronPage::OnChangeNaGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_Na] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusNaGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_Na]) {
		UpdateData(TRUE);
	}
	
}

void CNeuronPage::OnChangeCaTGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_CaT] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusCaTGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_CaT]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangeCaPGMAX0EDIT()
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_CaP] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusCaPGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_CaP]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangeAGmax0Edit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_A] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusAGmax0Edit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_A]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangeKCaGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_KCa] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusKCaGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_KCa]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangeKdGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_Kd] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusKdGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_Kd]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangehGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedgMax0[m_CompartType][_I_h] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocushGMAX0EDIT() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedgMax0[m_CompartType][_I_h]) {
		UpdateData(TRUE);
	}
}

void CNeuronPage::OnChangeSensValuemaxEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSensValueMax[m_CompartType][m_SensorType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSensValuemaxEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSensValueMax[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeSensTauMEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSensTauM[m_CompartType][m_SensorType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSensTauMEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSensTauM[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
}

void CNeuronPage::OnChangeSensTauHEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSensTauH[m_CompartType][m_SensorType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSensTauHEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSensTauH[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeSensZMEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSensZM[m_CompartType][m_SensorType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSensZMEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSensZM[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeSensZHEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSensZH[m_CompartType][m_SensorType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSensZHEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSensZH[m_CompartType][m_ChanType]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeDcTargetEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedTarget[m_CompartType][_DC] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusDcTargetEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedTarget[m_CompartType][_DC]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeSlowTargetEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedTarget[m_CompartType][_Slow] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSlowTargetEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedTarget[m_CompartType][_Slow]) {
		UpdateData(TRUE);
	}	
	
}

void CNeuronPage::OnChangeFastTargetEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedTarget[m_CompartType][_Fast] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusFastTargetEdit() 
{
    if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedTarget[m_CompartType][_Fast]) {
		UpdateData(TRUE);
	}	
	
}
	
void CNeuronPage::OnChangeChanNumMEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CNeuronPage::OnChangeChanNumHEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CNeuronPage::OnChangeSensNumMEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CNeuronPage::OnChangeSensNumHEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CNeuronPage::OnChangeSynEEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSynE[m_CompartType][m_SynType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSynEEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSynE[m_CompartType][m_SynType]) {
		UpdateData(TRUE);
	}		
}

void CNeuronPage::OnChangeSynTaudecayEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSynTauDecay[m_CompartType][m_SynType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSynTaudecayEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSynTauDecay[m_CompartType][m_SynType]) {
		UpdateData(TRUE);
	}
	
}

void CNeuronPage::OnChangeSynGstepEdit() 
{
	if (IsWindowVisible() AND !m_bChangingComparts AND !m_bChangingChannels AND !m_bChangingSensors AND !m_bChangingSynapses) {
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
		m_bChangedSynGStep[m_CompartType][m_SynType] = TRUE;
	}
	
}

void CNeuronPage::OnKillfocusSynGstepEdit() 
{
	if (IsWindowVisible() AND !m_bChangingChannels AND 
		!m_bChangingComparts AND !m_bChangingSensors AND !m_bChangingSynapses AND m_bChangedSynGStep[m_CompartType][m_SynType]) {
		UpdateData(TRUE);
	}
	
}


BOOL CNeuronPage::OnKillActive() 
{
	OnKillfocusEEdit();
	OnKillfocusInitVEdit();
	OnKillfocusINITCaEDIT();
	OnKillfocusRadiusEdit();
	OnKillfocusLengthEdit();
    OnKillfocusCEdit();
	OnKillfocusLeakGMAX0EDIT();
	OnKillfocusNaGMAX0EDIT();
	OnKillfocusCaTGMAX0EDIT();
	OnKillfocusCaPGMAX0EDIT();
	OnKillfocusAGmax0Edit();
	OnKillfocusKCaGMAX0EDIT();
	OnKillfocusKdGMAX0EDIT();
	OnKillfocushGMAX0EDIT();
	
	OnKillfocusSensValuemaxEdit();
	OnKillfocusSensTauMEdit();
	OnKillfocusSensTauHEdit();
	OnKillfocusSensZMEdit();
	OnKillfocusSensZHEdit();
	OnKillfocusDcTargetEdit();
	OnKillfocusSlowTargetEdit();
	OnKillfocusFastTargetEdit();

	OnKillfocusSynEEdit();
	OnKillfocusSynTaudecayEdit();
	OnKillfocusSynGstepEdit();

	return CPropertyPage::OnKillActive();
}

/////////////////////////////////////////////////////////////////////////////
// CNeuronPage serialization

void CNeuronPage::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bChangeMade;
		ar << m_bChangingChannels;
		ar << m_bChangingSensors;
		ar << m_bChangingSynapses;
		ar << m_bChangingComparts;
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar << m_C_arr[i];
			ar << m_V0_arr[i];
			ar << m_Ca0_arr[i];
			ar << m_R_arr[i];
			ar << m_L_arr[i];
			ar << m_Area_arr[i];
			ar << m_gAxial_arr[i];
			ar << m_bChangedV0[i];
			ar << m_bChangedCa0[i];
			ar << m_bChangedR[i];
			ar << m_bChangedL[i];
			ar << m_bChangedC[i];
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				ar << m_bChanExists[i][chan];
				ar << m_bCaChannel[i][chan];
				ar << m_E_arr[i][chan];
				ar << m_gmax0_arr[i][chan];
				ar << m_chan_num_m_arr[i][chan];  
				ar << m_chan_num_h_arr[i][chan];
				ar << m_bChangedE[i][chan];
				ar << m_bChangedgMax0[i][chan];
			}
			for (int sens = 0; sens < _MAX_SENSORS; sens++) {
				ar << m_bSensExists[i][sens];
				ar << m_sens_ValueMax_arr[i][sens];
				ar << m_sens_tau_m_arr[i][sens];  
				ar << m_sens_tau_h_arr[i][sens];
				ar << m_sens_Z_m_arr[i][sens];  
				ar << m_sens_Z_h_arr[i][sens];
				ar << m_sens_num_m_arr[i][sens];  
				ar << m_sens_num_h_arr[i][sens];
				ar << m_Target_arr[i][sens];
				ar << m_bChangedSensValueMax[i][sens];
				ar << m_bChangedSensTauM[i][sens];
				ar << m_bChangedSensTauH[i][sens];
				ar << m_bChangedSensZM[i][sens];
				ar << m_bChangedSensZH[i][sens];
				ar << m_bChangedTarget[i][sens];
			}
			for (int syn = 0; syn < _MAX_SYNAPSES; syn++) {
				ar << m_bSynExists[i][syn];
				ar << m_syn_E_arr[i][syn];
				ar << m_syn_TauDecay_arr[i][syn];
				ar << m_syn_GStep_arr[i][syn];
				ar << m_bChangedSynE[i][syn];
				ar << m_bChangedSynTauDecay[i][syn];
				ar << m_bChangedSynGStep[i][syn];
			}
		}
		ar << m_resistivity;
		ar << m_Area;
		ar << m_gAxial;
		ar << m_C;
		ar << m_V0;
		ar << m_Ca0;
		ar << m_length;
		ar << m_radius;
		ar << m_CompartType;
		ar << m_E;
		ar << m_gmax0_Leak;
		ar << m_gmax0_Na;
		ar << m_gmax0_CaT;
		ar << m_gmax0_CaP;
		ar << m_gmax0_A;
		ar << m_gmax0_KCa;
		ar << m_gmax0_Kd;
		ar << m_gmax0_h;
		ar << m_chan_num_h;
		ar << m_chan_num_m;
		ar << m_ChanType;
		ar << m_sens_ValueMax;
		ar << m_sens_tau_m;
		ar << m_sens_tau_h;
		ar << m_sens_Z_m;
		ar << m_sens_Z_h;
		ar << m_sens_num_m;
		ar << m_sens_num_h;
		ar << m_DC_Target;
		ar << m_Slow_Target;
		ar << m_Fast_Target;
		ar << m_SensorType;
		ar << m_syn_E;
		ar << m_syn_TauDecay;
		ar << m_syn_GStep;
		ar << m_SynType;			
	}	
	else
	{
		ar >> m_bChangeMade;
		ar >> m_bChangingChannels;
		ar >> m_bChangingSensors;
		ar >> m_bChangingSynapses;
		ar >> m_bChangingComparts;
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar >> m_C_arr[i];
			ar >> m_V0_arr[i];
			ar >> m_Ca0_arr[i];
			ar >> m_R_arr[i];
			ar >> m_L_arr[i];
			ar >> m_Area_arr[i];
			ar >> m_gAxial_arr[i];
			ar >> m_bChangedV0[i];
			ar >> m_bChangedCa0[i];
			ar >> m_bChangedR[i];
			ar >> m_bChangedL[i];
			ar >> m_bChangedC[i];
			for (int chan = 0; chan < _MAX_CHANNELS; chan++) {
				ar >> m_bChanExists[i][chan];
				ar >> m_bCaChannel[i][chan];
				ar >> m_E_arr[i][chan];
				ar >> m_gmax0_arr[i][chan];
				ar >> m_chan_num_m_arr[i][chan];  
				ar >> m_chan_num_h_arr[i][chan];
				ar >> m_bChangedE[i][chan];
				ar >> m_bChangedgMax0[i][chan];
			}
			for (int sens = 0; sens < _MAX_SENSORS; sens++) {
				ar >> m_bSensExists[i][sens];
				ar >> m_sens_ValueMax_arr[i][sens];
				ar >> m_sens_tau_m_arr[i][sens];  
				ar >> m_sens_tau_h_arr[i][sens];
				ar >> m_sens_Z_m_arr[i][sens];  
				ar >> m_sens_Z_h_arr[i][sens];
				ar >> m_sens_num_m_arr[i][sens];  
				ar >> m_sens_num_h_arr[i][sens];
				ar >> m_Target_arr[i][sens];
				ar >> m_bChangedSensValueMax[i][sens];
				ar >> m_bChangedSensTauM[i][sens];
				ar >> m_bChangedSensTauH[i][sens];
				ar >> m_bChangedSensZM[i][sens];
				ar >> m_bChangedSensZH[i][sens];
				ar >> m_bChangedTarget[i][sens];
			}
			for (int syn = 0; syn < _MAX_SYNAPSES; syn++) {
				ar >> m_bSynExists[i][syn];
				ar >> m_syn_E_arr[i][syn];
				ar >> m_syn_TauDecay_arr[i][syn];
				ar >> m_syn_GStep_arr[i][syn];
				ar >> m_bChangedSynE[i][syn];
				ar >> m_bChangedSynTauDecay[i][syn];
				ar >> m_bChangedSynGStep[i][syn];
			}
		}
		ar >> m_resistivity;
		ar >> m_Area;
		ar >> m_gAxial;
		ar >> m_C;
		ar >> m_V0;
		ar >> m_Ca0;
		ar >> m_length;
		ar >> m_radius;
		ar >> m_CompartType;
		ar >> m_E;
		ar >> m_gmax0_Leak;
		ar >> m_gmax0_Na;
		ar >> m_gmax0_CaT;
		ar >> m_gmax0_CaP;
		ar >> m_gmax0_A;
		ar >> m_gmax0_KCa;
		ar >> m_gmax0_Kd;
		ar >> m_gmax0_h;
		ar >> m_chan_num_h;
		ar >> m_chan_num_m;
		ar >> m_ChanType;
		ar >> m_sens_ValueMax;
		ar >> m_sens_tau_m;
		ar >> m_sens_tau_h;
		ar >> m_sens_Z_m;
		ar >> m_sens_Z_h;
		ar >> m_sens_num_m;
		ar >> m_sens_num_h;
		ar >> m_DC_Target;
		ar >> m_Slow_Target;
		ar >> m_Fast_Target;
		ar >> m_SensorType;
		ar >> m_syn_E;
		ar >> m_syn_TauDecay;
		ar >> m_syn_GStep;
		ar >> m_SynType;
		
		if (IsWindow(m_hWnd)) {
			UpdateData(FALSE);
			//set a flag here???
			ctrlCompartCombo().SetCurSel(m_CompartType);
			ctrlChanCombo().SetCurSel(m_ChanType);
			ctrlSensCombo().SetCurSel(m_SensorType);
			ctrlSynCombo().SetCurSel(m_SynType);
			ctrlApplyButt().EnableWindow(m_bChangeMade);
		}
	}
}


