//***CSensor.h : header file ***

class CSensor : public CObject
{
// Construction
	DECLARE_SERIAL (CSensor)

public:
	CSensor() {}  //empty constructor for serialization
	CSensor(int type, BOOL exists);
	
	
//Attributes
public:
	int m_type; //the type of sensor, e.g. _DC =0 (see GlobalDefs)
	double m_m; //sensor activation 
	double m_h; //sensor inactivation
	BOOL m_bExists;  //TRUE if exists, FALSE if been removed
	double m_Target; //Target value we want this sensor to settle down to
	double m_ValueMax;	//max possible sensor value
	double m_tau_m;  //time constant for activation
	double m_tau_h;  //time constant for inactivation
	double m_m_inf_Z; //where m_inf curve = 1/2
	double m_h_inf_Z; //where h_inf curve = 1/2 
	int m_p;	//power activation is raised to in computing sensor Value 
	int m_q;	//power inactivation is raised to in computing sensor Value


//Operations
public:
	void m_Update_m(double I_Ca_per_nF, double dt);
	void m_Update_h(double I_Ca_per_nF, double dt);
	void m_Update_AveValue(double tau_eff, double dt); 
	double m_Value();  //Sensor value which will be compared to target value
	double m_AveValue; //Sensor value "averaged" over gmax effective time constant with exponential decay filter
	double m_Error();  //Target value - sensor value
	double m_m_inf(double I_Ca_per_nF);  //steady-state activation
	double m_h_inf(double I_Ca_per_nF);  //steady-state inactivation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSensor)
	//}}AFX_VIRTUAL
	void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSensor();

private:
};