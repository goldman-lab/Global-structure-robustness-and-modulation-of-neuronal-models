// NeuronPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNeuronPage dialog

class CNeuronPage : public CPropertyPage
{
	DECLARE_SERIAL(CNeuronPage)

// Construction
public:
	CNeuronPage();
	~CNeuronPage();

//member variables
	//make arrays to hold channel info
	double  m_Area_arr[_MAX_COMPARTS];
	double  m_gAxial_arr[_MAX_COMPARTS];
	double  m_C_arr[_MAX_COMPARTS];
	double  m_V0_arr[_MAX_COMPARTS];
	double  m_Ca0_arr[_MAX_COMPARTS];
	double  m_R_arr[_MAX_COMPARTS];  //array of radii of cylindrical compartments
	double  m_L_arr[_MAX_COMPARTS];  //array of lengths
	double  m_gmax0_arr[_MAX_COMPARTS][_MAX_CHANNELS];
	double	m_E_arr[_MAX_COMPARTS][_MAX_CHANNELS];
	int		m_chan_num_h_arr[_MAX_COMPARTS][_MAX_CHANNELS];
	int		m_chan_num_m_arr[_MAX_COMPARTS][_MAX_CHANNELS];
	double	m_Target_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_sens_ValueMax_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_sens_tau_h_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_sens_tau_m_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_sens_Z_h_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_sens_Z_m_arr[_MAX_COMPARTS][_MAX_SENSORS];
	int		m_sens_num_h_arr[_MAX_COMPARTS][_MAX_SENSORS];
	int		m_sens_num_m_arr[_MAX_COMPARTS][_MAX_SENSORS];
	double	m_syn_E_arr[_MAX_COMPARTS][_MAX_SYNAPSES];
	double	m_syn_TauDecay_arr[_MAX_COMPARTS][_MAX_SYNAPSES];
	double	m_syn_GStep_arr[_MAX_COMPARTS][_MAX_SYNAPSES];
	BOOL m_bChangeMade;  //TRUE when apply button enabled
	BOOL m_bChanExists[_MAX_COMPARTS][_MAX_CHANNELS];
	BOOL m_bSensExists[_MAX_COMPARTS][_MAX_SENSORS];
	BOOL m_bSynExists[_MAX_COMPARTS][_MAX_SYNAPSES];
	BOOL m_bCaChannel[_MAX_COMPARTS][_MAX_CHANNELS]; //TRUE if a Ca-admitting Chan
	BOOL m_bChangedV0[_MAX_COMPARTS];  //used to make change/kill focus cooperate 
	BOOL m_bChangedCa0[_MAX_COMPARTS];  //used to make change/kill focus cooperate 
	BOOL m_bChangedC[_MAX_COMPARTS];  //used to make change/kill focus cooperate 
	BOOL m_bChangedR[_MAX_COMPARTS];  //used to make change/kill focus cooperate 
	BOOL m_bChangedL[_MAX_COMPARTS];  //used to make change/kill focus cooperate 
	BOOL m_bChangedE[_MAX_COMPARTS][_MAX_CHANNELS]; //used to make change/kill focus cooperate
	BOOL m_bChangedgMax0[_MAX_COMPARTS][_MAX_CHANNELS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSensValueMax[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSensTauM[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSensTauH[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSensZM[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSensZH[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedTarget[_MAX_COMPARTS][_MAX_SENSORS]; //used to make change/kill focus cooperate
	BOOL m_bChangedSynE[_MAX_COMPARTS][_MAX_SYNAPSES]; //used to make change/kill focus cooperate
	BOOL m_bChangedSynTauDecay[_MAX_COMPARTS][_MAX_SYNAPSES]; //used to make change/kill focus cooperate
	BOOL m_bChangedSynGStep[_MAX_COMPARTS][_MAX_SYNAPSES]; //used to make change/kill focus cooperate

	//changed sensor BOOL's
	BOOL m_bChangingChannels;
	BOOL m_bChangingSensors;
	BOOL m_bChangingSynapses;
	BOOL m_bChangingComparts;

//cell controls
	CComboBox& ctrlChanCombo() { return *(CComboBox *) GetDlgItem (IDC_CHAN_COMBO); }
	CComboBox& ctrlCompartCombo() { return *(CComboBox *) GetDlgItem (IDC_COMPART_COMBO); }
	CComboBox& ctrlSensCombo() { return *(CComboBox *) GetDlgItem (IDC_SENS_COMBO); }
	CComboBox& ctrlSynCombo() { return *(CComboBox *) GetDlgItem (IDC_SYN_COMBO); }
	CEdit& ctrlgAxialEdit() { return *(CEdit *) GetDlgItem (IDC_AXIAL_CONDUCT_EDIT); }
	CEdit& ctrlAreaEdit() { return *(CEdit *) GetDlgItem (IDC_AREA_EDIT); }
	CEdit& ctrlCEdit() { return *(CEdit *) GetDlgItem (IDC_C_EDIT); }
	CEdit& ctrlLEdit() { return *(CEdit *) GetDlgItem (IDC_LENGTH_EDIT); }
	CEdit& ctrlREdit() { return *(CEdit *) GetDlgItem (IDC_RADIUS_EDIT); }
	CEdit& ctrlInitVEdit() { return *(CEdit *) GetDlgItem (IDC_INIT_V_EDIT); }
	CEdit& ctrlInitCaEdit() { return *(CEdit *) GetDlgItem (IDC_INIT_Ca_EDIT); }
	CEdit& ctrlgMax0LeakEdit() { return *(CEdit *) GetDlgItem (IDC_Leak_GMAX0_EDIT); }
	CEdit& ctrlgMax0NaEdit() { return *(CEdit *) GetDlgItem (IDC_Na_GMAX0_EDIT); }
	CEdit& ctrlgMax0CaTEdit() { return *(CEdit *) GetDlgItem (IDC_CaT_GMAX0_EDIT); }
	CEdit& ctrlgMax0CaPEdit() { return *(CEdit *) GetDlgItem (IDC_CaP_GMAX0_EDIT); }
	CEdit& ctrlgMax0AEdit() { return *(CEdit *) GetDlgItem (IDC_A_GMAX0_EDIT); }
	CEdit& ctrlgMax0KCaEdit() { return *(CEdit *) GetDlgItem (IDC_KCa_GMAX0_EDIT); }
	CEdit& ctrlgMax0KdEdit() { return *(CEdit *) GetDlgItem (IDC_Kd_GMAX0_EDIT); }
	CEdit& ctrlgMax0hEdit() { return *(CEdit *) GetDlgItem (IDC_h_GMAX0_EDIT); }
	CEdit& ctrlgMaxLeakStatic() { return *(CEdit *) GetDlgItem (IDC_Leak_GMAX_STATIC); }
	CEdit& ctrlgMaxNaStatic() { return *(CEdit *) GetDlgItem (IDC_Na_GMAX_STATIC); }
	CEdit& ctrlgMaxCaTStatic() { return *(CEdit *) GetDlgItem (IDC_CaT_GMAX_STATIC); }
	CEdit& ctrlgMaxCaPStatic() { return *(CEdit *) GetDlgItem (IDC_CaP_GMAX_STATIC); }
	CEdit& ctrlgMaxAStatic() { return *(CEdit *) GetDlgItem (IDC_A_GMAX_STATIC); }
	CEdit& ctrlgMaxKCaStatic() { return *(CEdit *) GetDlgItem (IDC_KCa_GMAX_STATIC); }
	CEdit& ctrlgMaxKdStatic() { return *(CEdit *) GetDlgItem (IDC_Kd_GMAX_STATIC); }
	CEdit& ctrlgMaxhStatic() { return *(CEdit *) GetDlgItem (IDC_h_GMAX_STATIC); }
	CEdit& ctrlEEdit() { return *(CEdit *) GetDlgItem (IDC_E_EDIT); }
	CEdit& ctrlChan_Num_mEdit() { return *(CEdit *) GetDlgItem (IDC_CHAN_NUM_M_EDIT); }
	CEdit& ctrlChan_Num_hEdit() { return *(CEdit *) GetDlgItem (IDC_CHAN_NUM_H_EDIT); }
	CEdit& ctrlDC_TargetEdit() { return *(CEdit *) GetDlgItem (IDC_DC_TARGET_EDIT); }
	CEdit& ctrlSlow_TargetEdit() { return *(CEdit *) GetDlgItem (IDC_SLOW_TARGET_EDIT); }
	CEdit& ctrlFast_TargetEdit() { return *(CEdit *) GetDlgItem (IDC_FAST_TARGET_EDIT); }
	CEdit& ctrlDC_ValueStatic() { return *(CEdit *) GetDlgItem (IDC_DC_VALUE_STATIC); }
	CEdit& ctrlSlow_ValueStatic() { return *(CEdit *) GetDlgItem (IDC_SLOW_VALUE_STATIC); }
	CEdit& ctrlFast_ValueStatic() { return *(CEdit *) GetDlgItem (IDC_FAST_VALUE_STATIC); }
	CEdit& ctrlSens_ValueMaxEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_VALUEMAX_EDIT); }
	CEdit& ctrlSens_Tau_mEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_TAU_M_EDIT); }
	CEdit& ctrlSens_Tau_hEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_TAU_H_EDIT); }
	CEdit& ctrlSens_Z_mEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_Z_M_EDIT); }
	CEdit& ctrlSens_Z_hEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_Z_H_EDIT); }
	CEdit& ctrlSens_Num_mEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_NUM_M_EDIT); }
	CEdit& ctrlSens_Num_hEdit() { return *(CEdit *) GetDlgItem (IDC_SENS_NUM_H_EDIT); }
	CEdit& ctrlSyn_EEdit() { return *(CEdit *) GetDlgItem (IDC_SYN_E_EDIT); }
	CEdit& ctrlSyn_TauDecayEdit() { return *(CEdit *) GetDlgItem (IDC_SYN_TAUDECAY_EDIT); }
	CEdit& ctrlSyn_GStepEdit() { return *(CEdit *) GetDlgItem (IDC_SYN_GSTEP_EDIT); }
	CButton& ctrlApplyButt() { return *(CButton *) GetDlgItem (IDC_APPLY_BUTT); }
	CButton& ctrlRemoveChanButt() { return *(CButton *) GetDlgItem (IDC_REMOVE_CHAN_BUTT); }
	CButton& ctrlRemoveSensButt() { return *(CButton *) GetDlgItem (IDC_REMOVE_SENS_BUTT); }
	CButton& ctrlRemoveSynButt() { return *(CButton *) GetDlgItem (IDC_REMOVE_SYN_BUTT); }

// Dialog Data
	//{{AFX_DATA(CNeuronPage)
	enum { IDD = IDD_NEURONPAGE };
	double	m_resistivity;
	double	m_gAxial;
	double	m_Area;
	double	m_C;
	double	m_V0;
	double	m_Ca0;
	double	m_length;
	double	m_radius;
	int		m_CompartType;
	int		m_ChanType;
	int		m_SensorType;
	double	m_E;
	double	m_gmax0_A;
	double	m_gmax0_CaP;
	double	m_gmax0_CaT;
	double	m_gmax0_h;
	double	m_gmax0_KCa;
	double	m_gmax0_Kd;
	double	m_gmax0_Leak;
	double	m_gmax0_Na;
	int		m_chan_num_h;
	int		m_chan_num_m;
	double	m_sens_ValueMax;
	int		m_sens_num_h;
	int		m_sens_num_m;
	double	m_sens_tau_h;
	double	m_sens_tau_m;
	double	m_sens_Z_h;
	double	m_sens_Z_m;
	double	m_DC_Target;
	double	m_Slow_Target;
	double	m_Fast_Target;
	int		m_SynType;
	double	m_syn_E;
	double	m_syn_GStep;
	double	m_syn_TauDecay;
	//}}AFX_DATA


//Operations
public:
	void UpdateGMaxDisplay();  //updates the current value of GMax as determined by the sensor
	void UpdateSensorValueDisplay();  //updates the current value of GMax as determined by the sensor
	
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNeuronPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	void Serialize(CArchive& ar);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNeuronPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoveChanButt();
	afx_msg void OnApplyButt();
	afx_msg void OnSelchangeChanCombo();
	afx_msg void OnChangeCEdit();
	afx_msg void OnKillfocusEEdit();
	afx_msg void OnKillfocusInitVEdit();
	afx_msg void OnChangeINITCaEDIT();
	afx_msg void OnChangeEEdit();
	afx_msg void OnChangeInitVEdit();
	afx_msg void OnSelchangeCompartCombo();
	afx_msg void OnChangeLengthEdit();
	afx_msg void OnChangeRadiusEdit();
	afx_msg void OnChangeResistivityEdit();
	afx_msg void OnKillfocusCEdit();
	afx_msg void OnKillfocusLengthEdit();
	afx_msg void OnKillfocusRadiusEdit();
	afx_msg void OnKillfocusINITCaEDIT();
	afx_msg void OnRemoveSensButt();
	afx_msg void OnSelchangeSensCombo();
	afx_msg void OnChangeChanNumMEdit();
	afx_msg void OnChangeChanNumHEdit();
	afx_msg void OnChangeSensNumMEdit();
	afx_msg void OnChangeSensNumHEdit();
	afx_msg void OnChangeLeakGMAX0EDIT();
	afx_msg void OnKillfocusLeakGMAX0EDIT();
	afx_msg void OnChangeNaGMAX0EDIT();
	afx_msg void OnKillfocusNaGMAX0EDIT();
	afx_msg void OnChangeCaTGMAX0EDIT();
	afx_msg void OnKillfocusCaTGMAX0EDIT();
	afx_msg void OnChangeCaPGMAX0EDIT();
	afx_msg void OnKillfocusCaPGMAX0EDIT();
	afx_msg void OnChangeAGmax0Edit();
	afx_msg void OnKillfocusAGmax0Edit();
	afx_msg void OnChangeKCaGMAX0EDIT();
	afx_msg void OnKillfocusKCaGMAX0EDIT();
	afx_msg void OnChangeKdGMAX0EDIT();
	afx_msg void OnKillfocusKdGMAX0EDIT();
	afx_msg void OnChangehGMAX0EDIT();
	afx_msg void OnKillfocushGMAX0EDIT();
	afx_msg void OnChangeSensValuemaxEdit();
	afx_msg void OnKillfocusSensValuemaxEdit();
	afx_msg void OnChangeSensTauMEdit();
	afx_msg void OnKillfocusSensTauMEdit();
	afx_msg void OnChangeSensTauHEdit();
	afx_msg void OnKillfocusSensTauHEdit();
	afx_msg void OnChangeSensZMEdit();
	afx_msg void OnKillfocusSensZMEdit();
	afx_msg void OnChangeSensZHEdit();
	afx_msg void OnKillfocusSensZHEdit();
	afx_msg void OnChangeDcTargetEdit();
	afx_msg void OnKillfocusDcTargetEdit();
	afx_msg void OnChangeSlowTargetEdit();
	afx_msg void OnKillfocusSlowTargetEdit();
	afx_msg void OnChangeFastTargetEdit();
	afx_msg void OnKillfocusFastTargetEdit();
	afx_msg void OnSelchangeSynCombo();
	afx_msg void OnChangeSynEEdit();
	afx_msg void OnKillfocusSynEEdit();
	afx_msg void OnChangeSynTaudecayEdit();
	afx_msg void OnKillfocusSynTaudecayEdit();
	afx_msg void OnChangeSynGstepEdit();
	afx_msg void OnKillfocusSynGstepEdit();
	afx_msg void OnRemoveSynButt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
