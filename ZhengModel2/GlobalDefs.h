//***Global header file -- definitions****


//miscellaneous
//NOTE:SYNAPSE STUFF NOT YET SAVED IN THIS VERSION
//    & number of plots in display page also not saved
#define _VERSION_NUMBER 3 //Version #, for serialization compatibility

//#define OR ||
//#define AND &&

//array sizes
#define _MAX_SPIKES 10000 //for use in pulling out spike times from run
#define _MAX_INJ_TIMES 2 //for use in external injection array
#define _MAX_COMPARTS 2 //for use in CObArray
#define _MAX_CHANNELS 8  //for use in CObArray
#define _MAX_SYNAPSES 2  //for use in CObArray
#define _MAX_SENSORS 3 //for use in CObArray
#define _MAX_PLOTS 6 //maximum number of plots allowed (for declaring arrays)

//compartment types
#define _Soma 0
#define _Hillock 1

//channel types
#define _I_Leak 0
#define _I_Na 1
#define _I_CaT 2
#define _I_CaP 3
#define _I_A 4
#define _I_KCa 5
#define _I_Kd 6
#define _I_h 7

//synapse types
#define _Inhib 0
#define _Excit 1

//sensor types
#define _DC 0
#define _Slow 1
#define _Fast 2
