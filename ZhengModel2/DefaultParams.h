// Contains default values for all parameters
#include "math.h"

//Neuron parameters
//Next parameters computed with the following values: (for an LP neuron)
#define _RESISTIVITY (400.0) //[ohm-cm] inter-compartment resistivity--NOT REALLY!!!!! CHANGE!!!!
#define _SOMA_R (25.0) //[um] assume soma is a cylinder of radius R, length L
#define _SOMA_L (400.0) //[um] use this in computing default axial conduct.
#define _HILLOCK_R (5.0) //[um] assume hillock is a cylinder
#define _HILLOCK_L (200.0) //[um] use this for distance in computing default axial conductance 

// Compartment parameters
#define _C 1.0  //Capacitance [uF/(cm^2)]
#define _V0 -60.0 //Initial voltage [mV]
#define _Ca0 0.05 //Initial Ca concentration [uM]
// Ca++ flow parameters
#define _SHELL_T 1.14 //[um] thickness of shell in which compute Ca conc.;used in UMCA_TO_NA conversion in compartment
#define _NERNST_CA 12.20 //[mV] coefficient kT/q for T = 10 C = 283 K
#define _CA_VALENCE 2.0
#define _TAU_CA 200.0 //[ms]  //ZHENG uses 20.0
#define _CA_OUT 3000.0 //[uM]  //ZHENG, Jorge uses 13mM
#define _CA_REST 0.05 //[uM]
#define _NA_TO_UMCA_PREFACTOR (1.038e4) //[um^3][e][uM]/([ms][nA])
//#define _UMCA_TO_NA 1.48 //Hardwired to fit ZHENG's model (for 20ms time constant and r = 12.5um, 
						//l=400um cylinder compartment, shell thickness 1.14um.)

// Channel  parameters
#define _TAU_GMAX 5000.0 //[ms]  

#define _SOMA_Leak_EXISTS TRUE
#define _HILLOCK_Leak_EXISTS TRUE
#define _Leak_CaCHANNEL FALSE //Does this channel admit Calcium?
//#define _Leak_E -54.402 //Squid/Larry's classReversal potential [mV]
#define _Leak_E -50.0 //ZHENG
//#define _Leak_GMAX 0.3  //Max conductance [mS/(cm^2)]
#define _Leak_GMAX0 0.01 //ZHENG  -- INITIAL max conductance [mS/(cm^2)]
#define _Leak_NUM_M 0 //Number of activation gates 
#define _Leak_NUM_H 0 //Number of inactivation gates
#define _Leak_DC_COUPLING 0  //weights for sensor error signal for this channel
#define _Leak_Slow_COUPLING 0
#define _Leak_Fast_COUPLING 0 

#define _SOMA_Na_EXISTS FALSE
#define _HILLOCK_Na_EXISTS TRUE
#define _Na_CaCHANNEL FALSE
#define _Na_E 50.0  
#define _Na_GMAX0 120.0 
#define _Na_NUM_M 3  
#define _Na_NUM_H 1 
#define _Na_DC_COUPLING 0  
#define _Na_Slow_COUPLING 0
#define _Na_Fast_COUPLING 1

#define _SOMA_CaT_EXISTS TRUE
#define _HILLOCK_CaT_EXISTS FALSE
#define _CaT_CaCHANNEL TRUE
#define _CaT_E (_NERNST_CA * log(_CA_OUT/_CA_REST)) //nernst equation 
#define _CaT_GMAX0 0.75  
#define _CaT_NUM_M 3  
#define _CaT_NUM_H 1
#define _CaT_DC_COUPLING 0  
#define _CaT_Slow_COUPLING 1
#define _CaT_Fast_COUPLING 0

#define _SOMA_CaP_EXISTS TRUE
#define _HILLOCK_CaP_EXISTS FALSE
#define _CaP_CaCHANNEL TRUE
#define _CaP_E (_NERNST_CA * log(_CA_OUT/_CA_REST))  
#define _CaP_GMAX0 1.7  
#define _CaP_NUM_M 3  
#define _CaP_NUM_H 1
#define _CaP_DC_COUPLING 0  
#define _CaP_Slow_COUPLING 1
#define _CaP_Fast_COUPLING 0

#define _SOMA_A_EXISTS TRUE
#define _HILLOCK_A_EXISTS TRUE
#define _A_CaCHANNEL FALSE
#define _A_E -80.0  
#define _A_GMAX0 4.5  
#define _A_NUM_M 3
#define _A_NUM_H 1
#define _A_DC_COUPLING -1  
#define _A_Slow_COUPLING -1
#define _A_Fast_COUPLING 0

#define _SOMA_KCa_EXISTS TRUE
#define _HILLOCK_KCa_EXISTS FALSE
#define _KCa_CaCHANNEL FALSE
#define _KCa_E -80.0  
#define _KCa_GMAX0 19  
#define _KCa_NUM_M 4  
#define _KCa_NUM_H 0
#define _KCa_DC_COUPLING -1  
#define _KCa_Slow_COUPLING -1
#define _KCa_Fast_COUPLING 0

#define _SOMA_Kd_EXISTS TRUE
#define _HILLOCK_Kd_EXISTS TRUE
#define _Kd_CaCHANNEL FALSE
//#define _Kd_E -77.0  //squid/Larry's class
#define _Kd_E -80.0 //ZHENG   
#define _Kd_GMAX0 37.0  
#define _Kd_NUM_M 4  
#define _Kd_NUM_H 0
#define _Kd_DC_COUPLING 0  
#define _Kd_Slow_COUPLING -1
#define _Kd_Fast_COUPLING 1

#define _SOMA_h_EXISTS TRUE
#define _HILLOCK_h_EXISTS FALSE
#define _h_CaCHANNEL FALSE
#define _h_E -20.0  
#define _h_GMAX0 1.53  
#define _h_NUM_M 0  
#define _h_NUM_H 1
#define _h_DC_COUPLING 1  
#define _h_Slow_COUPLING 1
#define _h_Fast_COUPLING 0

//synapse parameters
#define _SOMA_Inhib_EXISTS TRUE  //whether an inhibitory synapse exists on this compartment
#define _HILLOCK_Inhib_EXISTS FALSE
#define _Inhib_E -75.0 //mV
#define _Inhib_TAU_DECAY 100.0 //ms decay time of synaptic conductance
#define _Inhib_GSTEP 0.1 //mS/(cm^2)  //increase in conductance due to synaptic input

#define _SOMA_Excit_EXISTS FALSE
#define _HILLOCK_Excit_EXISTS FALSE
#define _Excit_E 0.0 //mV
#define _Excit_TAU_DECAY 50.0 //ms
#define _Excit_GSTEP 0.01 //mS/(cm^2)

//sensor parameters
#define _TAU_UPDATE 10000.0  //used for updating sensor output display

#define _SOMA_DC_EXISTS FALSE
#define _HILLOCK_DC_EXISTS FALSE
#define _DC_TARGET 0.1
#define _DC_VALUEMAX 1.0
#define _DC_TAU_M 500 //[ms]
#define _DC_TAU_H 0 //dummy variable--not used
#define _DC_M_INF_Z 3 //[nA/nF]
#define _DC_H_INF_Z 0 //dummy variable--not used
#define _DC_NUM_M 2
#define _DC_NUM_H 0

#define _SOMA_Slow_EXISTS FALSE
#define _HILLOCK_Slow_EXISTS FALSE
#define _Slow_TARGET 0.1
#define _Slow_VALUEMAX 3.0
#define _Slow_TAU_M 50
#define _Slow_TAU_H 60 
#define _Slow_M_INF_Z 7.2
#define _Slow_H_INF_Z 2.8
#define _Slow_NUM_M 2
#define _Slow_NUM_H 1

#define _SOMA_Fast_EXISTS FALSE
#define _HILLOCK_Fast_EXISTS FALSE
#define _Fast_TARGET 0.1
#define _Fast_VALUEMAX 10.0
#define _Fast_TAU_M 0.5
#define _Fast_TAU_H 1.5
#define _Fast_M_INF_Z 14.2
#define _Fast_H_INF_Z 9.8
#define _Fast_NUM_M 2
#define _Fast_NUM_H 1

//Plotting parameters
#define _TIME_PER_SCREEN 1500.0 //[ms] 
#define _POINTS_PER_SCREEN 3000
#define _MINY (-60.0)
#define _MAXY (40.0)

//for histograms
#define _NUM_BINS 1000

//Run parameters
#define _SEED -3445
#define _DT 0.1   //[ms]
#define _TMAX 999999.0 //[ms]
#define _TIME_PER_GMAX_UPDATE (_TAU_GMAX/10.0)
#define _MAX_INJECT 0.0 //[nA]
#define _INJECT_ON_TIME 100.0 //[ms]
#define _INJECT_ON_TIME2 200.0 //[ms]
#define _INJECT_OFF_TIME 100000.0
#define _SAVE_TO_FILE 0 //FALSE
#define _SPIKE_THRESHOLD 0.0 //[mV]
#define _BURST_THRESHOLD -40.0 //[mV] //for looking for 1-spike bursts
#define _TRANSMIT_THRESHOLD -40.0 //[mV] threshold for graded transmission
#define _TRANSMIT_MAX -15.0 //[mV] saturation of graded transmission
#define _DISPLAY_FLAG 1 //TRUE
