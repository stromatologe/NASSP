/**************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2004-2005



  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************** Revision History ****************************
  *	$Log$
  *	Revision 1.9  2009/09/02 18:26:46  vrouleau
  *	MultiThread support for vAGC
  *	
  *	Revision 1.8  2009/09/01 06:18:32  dseagrav
  *	LM Checkpoint Commit. Added switches. Added history to LM SCS files. Added bitmap to LM. Added AIDs.
  *	
  *	Revision 1.7  2009/08/24 02:20:20  dseagrav
  *	LM Checkpoint Commit: Adds more systems, heater power drains, fix use of stage before init
  *	
  *	Revision 1.6  2009/08/16 03:12:38  dseagrav
  *	More LM EPS work. CSM to LM power transfer implemented. Optics bugs cleared up.
  *	
  *	Revision 1.5  2009/08/10 02:23:06  dseagrav
  *	LEM EPS (Part 2)
  *	Split ECAs into channels, Made bus cross tie system, Added ascent systems and deadface/staging logic.
  *	
  *	Revision 1.4  2009/08/01 23:06:33  jasonims
  *	LM Optics Code Cleaned Up... Panel Code added for LM Optics... Knobs activated... Counter and Computer Controls still to come.
  *	
  *	Revision 1.3  2009/08/01 19:48:33  jasonims
  *	LM Optics Code Added, along with rudimentary Graphics for AOT.
  *	Reticle uses GDI objects to allow realtime rotation.
  *	
  *	Revision 1.2  2009/07/11 13:40:19  jasonims
  *	DockingProbe Work
  *	
  *	Revision 1.1  2009/02/18 23:21:14  tschachim
  *	Moved files as proposed by Artlav.
  *	
  *	Revision 1.17  2008/01/16 04:14:23  movieman523
  *	Rewrote docking probe separation code and moved the CSM_LEM code into a single function in the Saturn class.
  *	
  *	Revision 1.16  2008/01/14 01:17:03  movieman523
  *	Numerous changes to move payload creation from the CSM to SIVB.
  *	
  *	Revision 1.15  2008/01/12 04:14:10  movieman523
  *	Pass payload information to SIVB and have LEM use the fuel masses passed to it.
  *	
  *	Revision 1.14  2008/01/11 05:24:11  movieman523
  *	Added LEM fuel masses; currently they're passed to the LEM but it ignores them.
  *	
  *	Revision 1.13  2008/01/09 15:00:20  lassombra
  *	Added support for checklistController to save/load state.
  *	
  *	Added support for new scenario options LEMCHECK <lem checklist file and LEMCHECKAUTO <whether the lem should automatically execute checklists.
  *	
  *	Will document new options on the wiki
  *	
  *	Revision 1.12  2008/01/09 01:46:45  movieman523
  *	Added initial support for talking to checklist controller from MFD.
  *	
  *	Revision 1.11  2007/12/21 18:10:27  movieman523
  *	Revised docking connector code; checking in a working version prior to a rewrite to automate the docking process.
  *	
  *	Revision 1.10  2007/12/21 01:00:09  movieman523
  *	Really basic Checklist MFD based on Project Apollo MFD, along with the various support functions required to make it work.
  *	
  *	Revision 1.9  2007/11/30 17:46:32  movieman523
  *	Implemented remaining meters as 0-5V voltmeters for now.
  *	
  *	Revision 1.8  2007/11/30 16:40:40  movieman523
  *	Revised LEM to use generic voltmeter and ammeter code. Note that the ED battery select switch needs to be implemented to fully support the voltmeter/ammeter now.
  *	
  *	Revision 1.7  2007/11/25 06:55:42  movieman523
  *	Tidied up surface ID code, moving the enum from a shared include file to specific versions for the Saturn and LEM classes.
  *	
  *	Revision 1.6  2007/06/06 15:02:09  tschachim
  *	OrbiterSound 3.5 support, various fixes and improvements.
  *	
  *	Revision 1.5  2006/08/21 03:04:38  dseagrav
  *	This patch adds DC volt/amp meters and associated switches, which was an unholy pain in the
  *	
  *	Revision 1.4  2006/08/20 08:28:06  dseagrav
  *	LM Stage Switch actually causes staging (VERY INCOMPLETE), Incorrect "Ascent RCS" removed, ECA outputs forced to 24V during initialization to prevent IMU/LGC failure on scenario load, Valves closed by default, EDS saves RCS valve states, would you like fries with that?
  *	
  *	Revision 1.3  2006/08/18 05:45:01  dseagrav
  *	LM EDS now exists. Talkbacks wired to a power source will revert to BP when they lose power.
  *	
  *	Revision 1.2  2006/08/13 23:12:41  dseagrav
  *	Joystick improvements
  *	
  *	Revision 1.1  2006/08/13 16:01:53  movieman523
  *	Renamed LEM. Think it all builds properly, I'm checking it in before the lightning knocks out the power here :).
  *	
  **************************************************************************/

#if !defined(_PA_LEM_H)
#define _PA_LEM_H

#include "FDAI.h"

// DS20060413 Include DirectInput
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
// DS20060730 Include LM SCS
#include "lmscs.h"

// Cosmic background temperature in degrees F
#define CMBG_TEMP -459.584392

//
// Valves.
//
#define N_LEM_VALVES	32

#define LEM_RCS_MAIN_SOV_A				1
#define LEM_RCS_MAIN_SOV_B				2

//
// Lem state settings from scenario file, passed from CSM.
//

#include "lemswitches.h"
#include "missiontimer.h"

#include "connector.h"
#include "checklistController.h"
#include "payload.h"

// Systems things
// ELECTRICAL
// LEM to CSM Power Connector
class LEMPowerConnector : public Connector
{
public:
	LEMPowerConnector();
	int csm_power_latch;
	bool ReceiveMessage(Connector *from, ConnectorMessage &m);
};

// XLunar Bus Controller Voltage Source
class LEM_XLBSource : public e_object {
public:
	LEM_XLBSource();							// Cons
	void SetVoltage(double v);
	void DrawPower(double watts);
};


// XLunar Bus Controller
class LEM_XLBControl : public e_object {
public:
	LEM_XLBControl();	// Cons
	void Init(LEM *s);
	void UpdateFlow(double dt);
	void DrawPower(double watts);

	LEM *lem;					// Pointer at LEM
	LEM_XLBSource dc_output;	// DC output
};

// Electrical Control Assembly Subchannel
class LEM_ECAch : public e_object {
public:
	LEM_ECAch();								 // Cons
	void Init(LEM *s,e_object *src, int inp); // Init
	void UpdateFlow(double dt);
	void DrawPower(double watts);
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);

	IndicatorSwitch *dc_source_tb;    // Pointer at TB
	LEM *lem;					// Pointer at LEM
	e_object *dc_source;		// Associated battery
	int input;                  // Channel input selector
};

// Bus feed controller object
class LEM_BusFeed : public e_object {
public:
	LEM_BusFeed();							// Cons
	void Init(LEM *s,e_object *sra,e_object *srb); // Init
	void UpdateFlow(double dt);
	void DrawPower(double watts);

	LEM *lem;					// Pointer at LEM
	e_object *dc_source_a;		// This has two inputs.
	e_object *dc_source_b;
};

// Voltage source item for cross-tie balancer
class LEM_BCTSource : public e_object {
public:
	LEM_BCTSource();							// Cons
	void SetVoltage(double v);
};


// Bus cross-tie balancer object
class LEM_BusCrossTie : public e_object {
public:
	LEM_BusCrossTie();	// Cons
	void LEM_BusCrossTie::Init(LEM *s,DCbus *sra,DCbus *srb,CircuitBrakerSwitch *cb1,CircuitBrakerSwitch *cb2,CircuitBrakerSwitch *cb3,CircuitBrakerSwitch *cb4);
	void UpdateFlow(double dt);
	void DrawPower(double watts);

	LEM *lem;					// Pointer at LEM
	DCbus *dc_bus_cdr;
	DCbus *dc_bus_lmp;
	LEM_BCTSource dc_output_cdr;
	LEM_BCTSource dc_output_lmp;
	CircuitBrakerSwitch *lmp_bus_cb,*lmp_bal_cb;
	CircuitBrakerSwitch *cdr_bus_cb,*cdr_bal_cb;
	double last_cdr_ld;
	double last_lmp_ld;
};

// Inverter
class LEM_INV : public e_object {
public:
	LEM_INV();							// Cons
	void Init(LEM *s);
	void UpdateFlow(double dt);
	void DrawPower(double watts);
	int active;
	LEM *lem;					// Pointer at LM
	e_object *dc_input;
};

// ABORT SENSOR ASSEMBLY (ASA)
class LEM_ASA{
public:
	LEM_ASA();							// Cons
	void Init(LEM *s); // Init
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);
	void TimeStep(double simdt);
	LEM *lem;					// Pointer at LEM
	h_Radiator hsink;			// Case (Connected to primary coolant loop)
	Boiler heater;				// Heater
};

// EXPLOSIVE DEVICES SYSTEM
class LEM_EDS{
public:
	LEM_EDS();							// Cons
	void Init(LEM *s); // Init
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);
	void TimeStep();
	LEM *lem;					// Pointer at LEM
	bool LG_Deployed;           // Landing Gear Deployed Flag	
};

// Landing Radar
class LEM_LR{
public:
	LEM_LR();
	void Init(LEM *s);
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);
	void TimeStep(double simdt);
	double GetAntennaTempF();
	double lastTemp;

	LEM *lem;					// Pointer at LEM
	h_Radiator antenna;			// Antenna (loses heat into space)
	Boiler antheater;			// Antenna Heater (puts heat back into antenna)
};

// Rendezvous Radar
class LEM_RR{
public:
	LEM_RR();
	void Init(LEM *s);
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);
	void TimeStep(double simdt);
	double GetAntennaTempF();

	LEM *lem;					// Pointer at LEM
	h_Radiator antenna;			// Antenna (loses heat into space)
	Boiler antheater;			// Antenna Heater (puts heat back into antenna)
};

// S-Band Steerable Antenna
class LEM_SteerableAnt{
public:
	LEM_SteerableAnt();
	void Init(LEM *s);
	void SaveState(FILEHANDLE scn, char *start_str, char *end_str);
	void LoadState(FILEHANDLE scn, char *end_str);
	void TimeStep(double simdt);
	double GetAntennaTempF();

	LEM *lem;					// Pointer at LEM
	h_Radiator antenna;			// Antenna (loses heat into space)
	Boiler antheater;			// Antenna Heater (puts heat back into antenna)
};

///
/// \ingroup LEM
///
class LEM : public Payload, public PanelSwitchListener {

public:

	///
	/// This enum gives IDs for the surface bitmaps. We don't use #defines because we want
	/// to automatically calculate the maximum number of bitmaps.
	///
	/// Note that this is copied from the Saturn code, so many of these values aren't actually
	/// needed for the LEM. Tidy it up later as appropriate.
	///
	/// \ingroup LEM
	///
	enum SurfaceID
	{
		//
		// First value in the enum must be set to one. Entry zero is not
		// used.
		//
		SRF_INDICATOR							=	 1,
		SRF_NEEDLE,
		SRF_DIGITAL,
		SRF_SWITCHUP,
		SRF_SWITCHLEVER,
		SRF_SWITCHGUARDS,
		SRF_ABORT,
		SRF_ALTIMETER,
		SRF_THRUSTMETER,
		SRF_SEQUENCERSWITCHES,
		SRF_LMTWOPOSLEVER,
		SRF_MASTERALARM_BRIGHT,
		SRF_DSKY,
		SRF_THREEPOSSWITCH,
		SRF_MFDFRAME,
		SRF_MFDPOWER,
		SRF_ROTATIONALSWITCH,
		SRF_SUITCABINDELTAPMETER,
		SRF_THREEPOSSWITCH305,
		SRF_LMABORTBUTTON,
		SRF_LMMFDFRAME,
		SRF_LMTHREEPOSLEVER,
		SRF_LMTHREEPOSSWITCH,
		SRF_DSKYDISP,
		SRF_FDAI,
		SRF_FDAIROLL,
		SRF_CWSLIGHTS,
		SRF_EVENT_TIMER_DIGITS,
		SRF_DSKYKEY,
		SRF_ECSINDICATOR,
		SRF_SWITCHUPSMALL,
		SRF_CMMFDFRAME,
		SRF_COAS,
		SRF_THUMBWHEEL_SMALLFONTS,
		SRF_CIRCUITBRAKER,
		SRF_THREEPOSSWITCH20,
		SRF_THUMBWHEEL_SMALLFONTS_DIAGONAL,
		SRF_THREEPOSSWITCH30,
		SRF_SWITCH20,
		SRF_SWITCH30,
		SRF_SWITCH20LEFT,
		SRF_THREEPOSSWITCH20LEFT,
		SRF_GUARDEDSWITCH20,
		SRF_SWITCHGUARDPANEL15,
		SRF_THUMBWHEEL_SMALLFONTS_DIAGONAL_LEFT48,
		SRF_THREEPOSSWITCH30LEFT,
		SRF_SWITCH30LEFT,
		SRF_THREEPOSSWITCH305LEFT,
		SRF_SWITCH305LEFT,
		SRF_FDAIPOWERROTARY,
		SRF_DIRECTO2ROTARY,
		SRF_ECSGLYCOLPUMPROTARY,
		SRF_GTACOVER,
		SRF_DCVOLTS,
		SRF_ACVOLTS,
		SRF_DCAMPS,
		SRF_LMYAWDEGS,
		SRF_LMPITCHDEGS,
		SRF_LMSIGNALSTRENGTH,
		SRF_POSTLDGVENTVLVLEVER,
		SRF_SPSMAXINDICATOR,
		SRF_ECSROTARY,
		SRF_GLYCOLLEVER,
		SRF_LEMROTARY,
		SRF_FDAIOFFFLAG,
		SRF_FDAINEEDLES,
		SRF_THUMBWHEEL_LARGEFONTS,
		SRF_SPS_FONT_WHITE,
		SRF_SPS_FONT_BLACK,
		SRF_BORDER_34x29,
		SRF_BORDER_34x61,
		SRF_BORDER_55x111,
		SRF_BORDER_46x75,
		SRF_BORDER_39x38,
		SRF_BORDER_92x40,
		SRF_BORDER_34x33,
		SRF_BORDER_29x29,
		SRF_BORDER_34x31,
		SRF_BORDER_50x158,
		SRF_BORDER_38x52,
		SRF_BORDER_34x34,
		SRF_BORDER_90x90,
		SRF_BORDER_84x84,
		SRF_BORDER_70x70,
		SRF_BORDER_23x20,
		SRF_BORDER_78x78,
		SRF_BORDER_32x160,
		SRF_BORDER_72x72,
		SRF_BORDER_75x64,
		SRF_BORDER_34x39,
		SRF_THUMBWHEEL_SMALL,
		SRF_THUMBWHEEL_LARGEFONTSINV,
		SRF_SWLEVERTHREEPOS,
		SRF_LEM_COAS1,
		SRF_LEM_COAS2,
		SRF_THC,
		SRF_SUITRETURN_LEVER,
		SRF_OPTICS_HANDCONTROLLER,
		SRF_MARK_BUTTONS,
		SRF_THREEPOSSWITCHSMALL,
		SRF_AOTRETICLEKNOB,
		SRF_AOTSHAFTKNOB,

		//
		// NSURF MUST BE THE LAST ENTRY HERE. PUT ANY NEW SURFACE IDS ABOVE THIS LINE
		//
		nsurf	///< nsurf gives the count of surfaces for the array size calculation.
	};

	LEM(OBJHANDLE hObj, int fmodel);
	virtual ~LEM();

	void Init();
	void SetStateEx(const void *status);
	void SetLmVesselDockStage();
	void SetLmVesselHoverStage();
	void SetLmAscentHoverStage();
	void SetLmLandedMesh();
	void SetGimbal(bool setting);
	void GetMissionTime(double &Met);
	void AbortStage();
	void StartAscent();
	void CheckRCS();

	bool clbkLoadPanel (int id);
	bool clbkLoadVC(int id);
	bool clbkPanelMouseEvent (int id, int event, int mx, int my);
	bool clbkPanelRedrawEvent (int id, int event, SURFHANDLE surf);
	int  clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
	void clbkPreStep (double simt, double simdt, double mjd);
	void clbkPostStep(double simt, double simdt, double mjd);
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkSaveState (FILEHANDLE scn);
	bool clbkLoadGenericCockpit ();
	void clbkMFDMode (int mfd, int mode);

	void PanelSwitchToggled(ToggleSwitch *s);
	void PanelIndicatorSwitchStateRequested(IndicatorSwitch *s); 
	void PanelRotationalSwitchChanged(RotationalSwitch *s);
	void PanelThumbwheelSwitchChanged(ThumbwheelSwitch *s);

	// Panel SDK
	bool GetValveState(int valve);
	void SetValveState(int valve, bool open);

	// DS20060416 RCS management
	void SetRCSJet(int jet,bool fire);
	void SetRCSJetLevel(int jet, double level);
	//
	// These functions must be virtual so they can be called from the Saturn V or the LEVA
	//

	virtual bool SetupPayload(PayloadSettings &ls);
	virtual void PadLoad(unsigned int address, unsigned int value);
	virtual void StopEVA();

	PROPELLANT_HANDLE ph_RCSA,ph_RCSB;   // RCS Fuel A and B, replaces ph_rcslm0
	PROPELLANT_HANDLE ph_Dsc, ph_Asc; // handles for propellant resources
	THRUSTER_HANDLE th_hover[2];               // handles for orbiter main engines,added 2 for "virtual engine"
	// There are 16 RCS. 4 clusters, 4 per cluster.
	THRUSTER_HANDLE th_rcs[16];
	// These RCSes are for Orbiter's use and should be deleted once the internal guidance is working.
	THRUSTER_HANDLE th_rcs_orbiter_rot[24];
	THRUSTER_HANDLE th_rcs_orbiter_lin[16];
	THGROUP_HANDLE thg_hover;		          // handles for thruster groups
	SURFHANDLE exhaustTex;

	// DS20060413 DirectInput stuff
	// Handle to DLL instance
	HINSTANCE dllhandle;
	// pointer to DirectInput class itself
	LPDIRECTINPUT8 dx8ppv;
	// Joysticks-Enabled flag / counter - Zero if we aren't using DirectInput, nonzero is the number of joysticks we have.
	int js_enabled;
	// Pointers to DirectInput joystick devices
	LPDIRECTINPUTDEVICE8 dx8_joystick[2]; // One for THC, one for RHC, ignore extras
	DIDEVCAPS			 dx8_jscaps[2];   // Joystick capabilities
	DIJOYSTATE2			 dx8_jstate[2];   // Joystick state
	HRESULT				 dx8_failure;     // DX failure reason
	int rhc_id;							  // Joystick # for the RHC
	int rhc_rot_id;						  // ID of ROTATOR axis to use for RHC Z-axis
	int rhc_sld_id;                       // ID of SLIDER axis to use for RHC Z-axis
	int rhc_rzx_id;                       // Flag to use native Z-axis as RHC Z-axis
	int thc_id;                           // Joystick # for the THC
	int thc_rot_id;						  // ID of ROTATOR axis to use for THC Z-axis
	int thc_sld_id;                       // ID of SLIDER axis to use for THC Z-axis
	int thc_rzx_id;                       // Flag to use native Z-axis as THC Z-axis	
	int thc_tjt_id;                       // Flag to use axis as TTCA THROTTLE/JETS select lever
	int rhc_debug;						  // Flags to print debugging messages.
	int thc_debug;
	bool rhc_auto;						  ///< RHC Z-axis auto detection
	bool thc_auto;						  ///< THC Z-axis auto detection
	bool rhc_thctoggle;					  ///< Enable RHC/THC toggle
	int rhc_thctoggle_id;				  ///< RHC button id for RHC/THC toggle
	int rhc_pos[3];                       // RHC x/y/z positions
	int ttca_mode;                        // TTCA Throttle/Jets Mode
#define TTCA_MODE_THROTTLE 0
#define TTCA_MODE_JETS 1
	int ttca_throttle_pos;                // TTCA THROTTLE-mode position

protected:

	//
	// PanelSDK functions as a interface between the
	// actual System & Panel SDK and VESSEL class
	//
	// Note that this must be defined early in the file, so it will be initialised
	// before any other classes which rely on it at creation. Don't move it further
	// down without good reason, or you're likely to crash!
	//

    PanelSDK Panelsdk;

	void RedrawPanel_Thrust (SURFHANDLE surf);
	void RedrawPanel_XPointer (SURFHANDLE surf);
	void RedrawPanel_MFDButton(SURFHANDLE surf, int mfd, int side, int xoffset, int yoffset);
	void MousePanel_MFDButton(int mfd, int event, int mx, int my);
	void ReleaseSurfaces ();
	void ResetThrusters();
	void AttitudeLaunch1();
	void SeparateStage (UINT stage);
	void InitPanel (int panel);
	void SetSwitches(int panel);
	void AddRCS_LMH(double TRANZ);
	void AddRCS_LMH2(double TRANZ);
	void ToggleEVA();
	void SetupEVA();
	void SetView();
	void RedrawPanel_Horizon (SURFHANDLE surf);
	void RedrawPanel_AOTReticle (SURFHANDLE surf);
	void SwitchClick();
	void CabinFanSound();
	void VoxSound();
	void ButtonClick();
	void GuardClick();
	void AbortFire();
	void InitPanel();
	void DoFirstTimestep();
	void LoadDefaultSounds();
	void GetDockStatus();

	bool CabinFansActive();
	bool AscentEngineArmed();

	void SystemsTimestep(double simt, double simdt);
	void SystemsInit();
	bool ProcessConfigFileLine (FILEHANDLE scn, char *line);
	//
	// Save/Load support functions.
	//

	int GetCSwitchState();
	void SetCSwitchState(int s);
	int GetSSwitchState();
	void SetSSwitchState(int s);
	int GetLPSwitchState();
	void SetLPSwitchState(int s);
	int GetRPSwitchState();
	void SetRPSwitchState(int s);

	SURFHANDLE srf[nsurf];  // handles for panel bitmaps

	double actualVEL;
	double actualALT;
	double actualFUEL;
	double aVAcc;
	double aVSpeed;
	double aHAcc;
	double ALTN1;
	double SPEEDN1;
	double VSPEEDN1;
	double aTime;
	double AtempP ;
	double AtempY ;
	double AtempR ;
	double MissionTime;

	// Panel components
	PanelSwitches MainPanel;
	PanelSwitchScenarioHandler PSH;

	SwitchRow AbortSwitchesRow;

	PushSwitch AbortSwitch;
	PushSwitch AbortStageSwitch;
	bool AbortStageSwitchLight;

	/////////////////
	// LEM panel 1 //
	/////////////////

	FDAI fdaiLeft;
	int fdaiDisabled;
	int fdaiSmooth;

	HBITMAP hBmpFDAIRollIndicator;

	SwitchRow LeftXPointerSwitchRow;
	ToggleSwitch LeftXPointerSwitch;

	SwitchRow GuidContSwitchRow;
	ToggleSwitch GuidContSwitch;
	ThreePosSwitch ModeSelSwitch;
	ToggleSwitch AltRngMonSwitch;

	SwitchRow LeftMonitorSwitchRow;
	ToggleSwitch RateErrorMonSwitch;
	ToggleSwitch AttitudeMonSwitch;

	SwitchRow MPSRegControlSwitchRow;
	IndicatorSwitch ASCHeReg1TB;
	IndicatorSwitch ASCHeReg2TB;
	IndicatorSwitch DESHeReg1TB;
	IndicatorSwitch DESHeReg2TB;
	ThreePosSwitch ASCHeReg1Switch;
	ThreePosSwitch ASCHeReg2Switch;
	ThreePosSwitch DESHeReg1Switch;
	ThreePosSwitch DESHeReg2Switch;

	SwitchRow FDAILowerSwitchRow;
	ToggleSwitch ShiftTruSwitch;
	ToggleSwitch RateScaleSwitch;
	ToggleSwitch ACAPropSwitch;
	
	SwitchRow EngineThrustContSwitchRow;
	ToggleSwitch THRContSwitch;
	ToggleSwitch MANThrotSwitch;
	ToggleSwitch ATTTranslSwitch;
	ToggleSwitch BALCPLSwitch;

	SwitchRow PropellantSwitchRow;
	ThreePosSwitch QTYMonSwitch;
	ThreePosSwitch TempPressMonSwitch;

	SwitchRow HeliumMonRotaryRow;
	RotationalSwitch HeliumMonRotary;

	/////////////////
	// LEM panel 2 //
	/////////////////

	FDAI fdaiRight;

	SwitchRow RightMonitorSwitchRow;
	ToggleSwitch RightRateErrorMonSwitch;
	ToggleSwitch RightAttitudeMonSwitch;

	SwitchRow TempPressMonRotaryRow;
	RotationalSwitch TempPressMonRotary;

	SwitchRow RCSAscFeedTBSwitchRow;
	IndicatorSwitch RCSAscFeed1ATB;
	IndicatorSwitch RCSAscFeed2ATB;
	IndicatorSwitch RCSAscFeed1BTB;
	IndicatorSwitch RCSAscFeed2BTB;

	SwitchRow RCSAscFeedSwitchRow;
	ThreePosSwitch RCSAscFeed1ASwitch;
	ThreePosSwitch RCSAscFeed2ASwitch;
	ThreePosSwitch RCSAscFeed1BSwitch;
	ThreePosSwitch RCSAscFeed2BSwitch;

	SwitchRow RCSQuad14TBSwitchRow;
	IndicatorSwitch RCSQuad1ACmdEnableTB;
	IndicatorSwitch RCSQuad4ACmdEnableTB;
	IndicatorSwitch RCSQuad1BCmdEnableTB;
	IndicatorSwitch RCSQuad4BCmdEnableTB;

	SwitchRow RCSQuad14SwitchRow;
	ThreePosSwitch RCSQuad1ACmdEnableSwitch;
	ThreePosSwitch RCSQuad4ACmdEnableSwitch;
	ThreePosSwitch RCSQuad1BCmdEnableSwitch;
	ThreePosSwitch RCSQuad4BCmdEnableSwitch;

	SwitchRow RCSQuad23TBSwitchRow;
	IndicatorSwitch RCSQuad2ACmdEnableTB;
	IndicatorSwitch RCSQuad3ACmdEnableTB;
	IndicatorSwitch RCSQuad2BCmdEnableTB;
	IndicatorSwitch RCSQuad3BCmdEnableTB;

	SwitchRow RCSQuad23SwitchRow;
	ThreePosSwitch RCSQuad2ACmdEnableSwitch;
	ThreePosSwitch RCSQuad3ACmdEnableSwitch;
	ThreePosSwitch RCSQuad2BCmdEnableSwitch;
	ThreePosSwitch RCSQuad3BCmdEnableSwitch;

	SwitchRow RCSXfeedTBSwitchRow;
	IndicatorSwitch RCSXFeedTB;

	SwitchRow RCSXfeedSwitchRow;
	ThreePosSwitch RCSXFeedSwitch;

	// DS20060406 RCS MAIN SHUTOFF VALVES
	SwitchRow RCSMainSOVTBRow;
	LEMValveTalkback RCSMainSovATB;
	LEMValveTalkback RCSMainSovBTB;

	SwitchRow RCSMainSOVSwitchRow;
	LEMValveSwitch RCSMainSovASwitch;
	LEMValveSwitch RCSMainSovBSwitch;

	SwitchRow RightACAPropSwitchRow;
	ToggleSwitch RightACAPropSwitch;

	SwitchRow ClycolSuitFanRotaryRow;
	RotationalSwitch ClycolRotary;
	RotationalSwitch SuitFanRotary;

	SwitchRow QtyMonRotaryRow;
	RotationalSwitch QtyMonRotary;

	/////////////////
	// LEM panel 3 //
	/////////////////
	
	SwitchRow EngineArmSwitchesRow;
	ThreePosSwitch EngineArmSwitch;

	SwitchRow EngineDescentCommandOverrideSwitchesRow;
	ToggleSwitch EngineDescentCommandOverrideSwitch;

	SwitchRow ModeControlSwitchesRow;
	PGNSSwitch ModeControlPNGSSwitch;
	ThreePosSwitch ModeControlAGSSwitch;
    UnguardedIMUCageSwitch IMUCageSwitch;

	SwitchRow RadarAntTestSwitchesRow;
	ThreePosSwitch LandingAntSwitch;
	ThreePosSwitch RadarTestSwitch;

	SwitchRow TestMonitorRotaryRow;
	RotationalSwitch TestMonitorRotary;

	SwitchRow SlewRateSwitchRow;
	ToggleSwitch SlewRateSwitch;

	SwitchRow RendezvousRadarRotaryRow;
	RotationalSwitch RendezvousRadarRotary;

	SwitchRow StabContSwitchesRow;
	ToggleSwitch DeadBandSwitch;
	ThreePosSwitch GyroTestLeftSwitch;
	ThreePosSwitch GyroTestRightSwitch;

	SwitchRow AttitudeControlSwitchesRow;
	ThreePosSwitch RollSwitch;
	ThreePosSwitch PitchSwitch;
	ThreePosSwitch YawSwitch;

	SwitchRow TempMonitorRotaryRow;
	RotationalSwitch TempMonitorRotary;

	SwitchRow RCSSysQuadSwitchesRow;
	ThreePosSwitch RCSSysQuad1Switch;
	ThreePosSwitch RCSSysQuad2Switch;
	ThreePosSwitch RCSSysQuad3Switch;
	ThreePosSwitch RCSSysQuad4Switch;

	SwitchRow LightingSwitchesRow;
	ToggleSwitch SidePanelsSwitch;
	ThreePosSwitch FloodSwitch;

	SwitchRow FloodRotaryRow;
	RotationalSwitch FloodRotary;

	SwitchRow LampToneTestRotaryRow;
	RotationalSwitch LampToneTestRotary;

	SwitchRow RightXPointerSwitchRow;
	ToggleSwitch RightXPointerSwitch;

	SwitchRow ExteriorLTGSwitchRow;
	ThreePosSwitch ExteriorLTGSwitch;

	//
	// Currently these are just 0-5V meters; at some point we may want
	// to change them to a different class.
	//

	SwitchRow RaderSignalStrengthMeterRow;
	DCVoltMeter RadarSignalStrengthMeter;

	/////////////////
	// LEM panel 4 //
	/////////////////

	SwitchRow Panel4LeftSwitchRow;
	ToggleSwitch LeftACA4JetSwitch;
	ToggleSwitch LeftTTCATranslSwitch;

	SwitchRow Panel4RightSwitchRow;
	ToggleSwitch RightACA4JetSwitch;
	ToggleSwitch RightTTCATranslSwitch;

	//////////////////
	// LEM panel 11 //
	//////////////////

	SwitchRow Panel11CB1SwitchRow;
	CircuitBrakerSwitch SE_WND_HTR_AC_CB;
	CircuitBrakerSwitch HE_PQGS_PROP_DISP_AC_CB;
	CircuitBrakerSwitch SBD_ANT_AC_CB;
	CircuitBrakerSwitch ORDEAL_AC_CB;
	CircuitBrakerSwitch AQS_AC_CB;
	CircuitBrakerSwitch AOT_LAMP_ACB_CB;
	CircuitBrakerSwitch LMP_FDAI_AC_CB;
	CircuitBrakerSwitch NUM_LTG_AC_CB;
	CircuitBrakerSwitch AC_B_INV_1_FEED_CB;
	CircuitBrakerSwitch AC_B_INV_2_FEED_CB;
	CircuitBrakerSwitch AC_A_INV_1_FEED_CB;
	CircuitBrakerSwitch AC_A_INV_2_FEED_CB;
	LEMVoltCB           AC_A_BUS_VOLT_CB;
	CircuitBrakerSwitch CDR_WND_HTR_AC_CB;
	CircuitBrakerSwitch TAPE_RCDR_AC_CB;
	CircuitBrakerSwitch AOT_LAMP_ACA_CB;
	CircuitBrakerSwitch RDZ_RDR_AC_CB;
	CircuitBrakerSwitch DECA_GMBL_AC_CB;
	CircuitBrakerSwitch INTGL_LTG_AC_CB;

	SwitchRow Panel11CB2SwitchRow;
	CircuitBrakerSwitch RCS_A_MAIN_SOV_CB;
	CircuitBrakerSwitch RCS_A_QUAD4_TCA_CB;
	CircuitBrakerSwitch RCS_A_QUAD3_TCA_CB;
	CircuitBrakerSwitch RCS_A_QUAD2_TCA_CB;
	CircuitBrakerSwitch RCS_A_QUAD1_TCA_CB;
	CircuitBrakerSwitch RCS_A_ISOL_VLV_CB;
	CircuitBrakerSwitch RCS_A_ASC_FEED_2_CB;
	CircuitBrakerSwitch RCS_A_ASC_FEED_1_CB;
	CircuitBrakerSwitch THRUST_DISP_CB;
	CircuitBrakerSwitch MISSION_TIMER_CB;
	CircuitBrakerSwitch CDR_XPTR_CB;
	CircuitBrakerSwitch RNG_RT_ALT_RT_DC_CB;
	CircuitBrakerSwitch GASTA_DC_CB;
	CircuitBrakerSwitch CDR_FDAI_DC_CB;
	CircuitBrakerSwitch COAS_DC_CB;
	CircuitBrakerSwitch ORDEAL_DC_CB;
	CircuitBrakerSwitch RNG_RT_ALT_RT_AC_CB;
	CircuitBrakerSwitch GASTA_AC_CB;
	CircuitBrakerSwitch CDR_FDAI_AC_CB;

	SwitchRow Panel11CB3SwitchRow;
	CircuitBrakerSwitch PROP_DES_HE_REG_VENT_CB;
	CircuitBrakerSwitch HTR_RR_STBY_CB;
	CircuitBrakerSwitch HTR_RR_OPR_CB;
	CircuitBrakerSwitch HTR_LR_CB;
	CircuitBrakerSwitch HTR_DOCK_WINDOW_CB;
	CircuitBrakerSwitch HTR_AOT_CB;
	CircuitBrakerSwitch INST_SIG_CONDR_1_CB;
	CircuitBrakerSwitch CDR_SCS_AEA_CB;
	CircuitBrakerSwitch CDR_SCS_ABORT_STAGE_CB;
	CircuitBrakerSwitch CDR_SCS_ATCA_CB;
	CircuitBrakerSwitch CDR_SCS_AELD_CB;
	CircuitBrakerSwitch SCS_ENG_CONT_CB;
	CircuitBrakerSwitch SCS_ATT_DIR_CONT_CB;
	CircuitBrakerSwitch SCS_ENG_START_OVRD_CB;
	CircuitBrakerSwitch SCS_DECA_PWR_CB;
	CircuitBrakerSwitch EDS_CB_LG_FLAG;
	CircuitBrakerSwitch EDS_CB_LOGIC_A;
	CircuitBrakerSwitch CDR_LTG_UTIL_CB;
	CircuitBrakerSwitch CDR_LTG_ANUN_DOCK_COMPNT_CB;

	SwitchRow Panel11CB4SwitchRow;
	CircuitBrakerSwitch IMU_OPR_CB;
	CircuitBrakerSwitch IMU_SBY_CB;
	CircuitBrakerSwitch LGC_DSKY_CB;

	SwitchRow Panel11CB5SwitchRow;
	// Bus feed tie breakers
	CircuitBrakerSwitch CDRBatteryFeedTieCB1;
	CircuitBrakerSwitch CDRBatteryFeedTieCB2;
	CircuitBrakerSwitch CDRCrossTieBusCB;
	CircuitBrakerSwitch CDRCrossTieBalCB;
	CircuitBrakerSwitch CDRXLunarBusTieCB;
	// ECA control & Voltmeter
	CircuitBrakerSwitch CDRDesECAContCB;
	CircuitBrakerSwitch CDRDesECAMainCB;
	CircuitBrakerSwitch CDRAscECAContCB;
	CircuitBrakerSwitch CDRAscECAMainCB;
	LEMVoltCB CDRDCBusVoltCB;
	// AC Inverter 1 feed
	CircuitBrakerSwitch CDRInverter1CB;

	/////////////////
	// LEM Panel 5 //
	/////////////////

	SwitchRow Panel5SwitchRow;
	ThreePosSwitch TimerContSwitch;
	ThreePosSwitch TimerSlewHours;
	ThreePosSwitch TimerSlewMinutes;
	ThreePosSwitch TimerSlewSeconds;
	ToggleSwitch LtgORideAnunSwitch;
	ToggleSwitch LtgORideNumSwitch;
	ToggleSwitch LtgORideIntegralSwitch;
	ToggleSwitch LtgSidePanelsSwitch;
	RotationalSwitch LtgFloodOhdFwdKnob;
	RotationalSwitch LtgAnunNumKnob;
	RotationalSwitch LtgIntegralKnob;
	// There's a +X TRANSLATION button here too

	/////////////////
	// LEM Panel 8 //
	/////////////////

	SwitchRow Panel8SwitchRow;
	ToggleSwitch EDMasterArm;
	ToggleSwitch EDDesVent;
	ThreePosSwitch EDASCHeSel;
	ToggleSwitch EDDesPrpIsol;
	ToggleSwitch EDLGDeploy;
	ToggleSwitch EDHePressRCS;
	ToggleSwitch EDHePressDesStart;
	ToggleSwitch EDHePressASC;
	ToggleSwitch EDStage;
	ToggleSwitch EDStageRelay;
	ThreePosSwitch EDDesFuelVent;
	ThreePosSwitch EDDesOxidVent;
	IndicatorSwitch EDLGTB;
	IndicatorSwitch EDDesFuelVentTB;
	IndicatorSwitch EDDesOxidVentTB;
	// Audio section
	ThreePosSwitch CDRAudSBandSwitch;
	ThreePosSwitch CDRAudICSSwitch;
	ToggleSwitch CDRAudRelaySwitch;
	ThreePosSwitch CDRAudVOXSwitch;
	ToggleSwitch CDRAudioControlSwitch;
	ThreePosSwitch CDRAudVHFASwitch;
	ThreePosSwitch CDRAudVHFBSwitch;
	ThumbwheelSwitch CDRAudSBandVol;
	ThumbwheelSwitch CDRAudVHFAVol;
	ThumbwheelSwitch CDRAudVHFBVol;
	ThumbwheelSwitch CDRAudICSVol;
	ThumbwheelSwitch CDRAudMasterVol;
	ThumbwheelSwitch CDRAudVOXSens;
	ThreePosSwitch CDRCOASSwitch;


	bool RCS_Full;
	bool Eds;

	bool toggleRCS;

	bool Cswitch1;
	bool Cswitch2;
	bool Cswitch3;
	bool Cswitch4;
	bool Cswitch5;
	bool Cswitch6;
	bool Cswitch7;
	bool Cswitch8;
	bool Cswitch9;

	bool Sswitch1;
	bool Sswitch2;
	bool Sswitch3;
	bool Sswitch4;
	bool Sswitch5;
	bool Sswitch6;
	bool Sswitch7;
	bool Sswitch8;
	bool Sswitch9;

	bool RPswitch1;
	bool RPswitch2;
	bool RPswitch3;
	bool RPswitch4;
	bool RPswitch5;
	bool RPswitch6;
	bool RPswitch7;
	bool RPswitch8;
	bool RPswitch9;
	bool RPswitch10;
	bool RPswitch11;
	bool RPswitch12;
	bool RPswitch13;
	bool RPswitch14;
	bool RPswitch15;
	bool RPswitch16;

	bool LPswitch1;
	bool LPswitch2;
	bool LPswitch3;
	bool LPswitch4;
	bool LPswitch5;
	bool LPswitch6;
	bool LPswitch7;
	bool SPSswitch;
	bool EDSswitch;

	bool DESHE1switch;
	bool DESHE2switch;

//	int ENGARMswitch;
	

	bool QUAD1switch;
	bool QUAD2switch;
	bool QUAD3switch;
	bool QUAD4switch;
	bool QUAD5switch;
	bool QUAD6switch;
	bool QUAD7switch;
	bool QUAD8switch;

	bool AFEED1switch;
	bool AFEED2switch;
	bool AFEED3switch;
	bool AFEED4switch;

	bool LDGswitch;

	bool ED1switch;
	bool ED2switch;
	bool ED4switch;
	bool ED5switch;
	bool ED6switch;

	bool ED7switch;
	bool ED8switch;
	bool ED9switch;

	bool GMBLswitch;

	bool ASCHE1switch;
	bool ASCHE2switch;

	bool RCSQ1switch;
	bool RCSQ2switch;
	bool RCSQ3switch;
	bool RCSQ4switch;

	bool ATT1switch;
	bool ATT2switch;
	bool ATT3switch;

	bool CRSFDswitch;

	bool CABFswitch;

	bool PTTswitch;

	bool RCSS1switch;
	bool RCSS2switch;
	bool RCSS3switch;
	bool RCSS4switch;

	bool X1switch;

	bool GUIDswitch;

	bool ALTswitch;

	bool RATE1switch;
	bool AT1switch;

	bool SHFTswitch;

	bool ETC1switch;
	bool ETC2switch;
	bool ETC3switch;
	bool ETC4switch;

	bool PMON1switch;
	bool PMON2switch;

	bool ACAPswitch;

	bool RATE2switch;
	bool AT2switch;

//	bool DESEswitch;
#define DESEswitch EngineDescentCommandOverrideSwitch.IsUp()

#define ModeControlPNGSAuto ModeControlPNGSSwitch.IsUp()
#define ModeControlPNGSAttHold ModeControlPNGSSwitch.IsCenter()
#define ModeControlPNGSOff ModeControlPNGSSwitch.IsDown()

#define ModeControlAGSAuto ModeControlAGSSwitch.IsUp()
#define ModeControlAGSAttHold ModeControlAGSSwitch.IsCenter()
#define ModeControlAGSOff ModeControlAGSSwitch.IsDown()

	bool SLWRswitch;

	bool DBswitch;

	bool IMUCswitch;

	bool SPLswitch;

	bool X2switch;

	bool P41switch;
	bool P42switch;
	bool P43switch;
	bool P44switch;

	bool AUDswitch;
	bool RELswitch;

	bool CPswitch;

	bool HATCHswitch;

	bool EVAswitch;

	bool COASswitch;

	//////////////////
	// LEM panel 14 //
	//////////////////

	SwitchRow EPSP14VoltMeterSwitchRow;
	DCVoltMeter EPSDCVoltMeter;

	VoltageAttenuator ACVoltsAttenuator;

	SwitchRow EPSP14AmMeterSwitchRow;
	DCAmpMeter EPSDCAmMeter;
	
	SwitchRow EPSLeftControlArea;
	PowerStateRotationalSwitch EPSMonitorSelectRotary;
	LEMInverterSwitch EPSInverterSwitch;
	ThreePosSwitch EPSEDVoltSelect;

	SwitchRow DSCHiVoltageSwitchRow;
	LEMBatterySwitch DSCSEBat1HVSwitch;
	LEMBatterySwitch DSCSEBat2HVSwitch;
	LEMBatterySwitch DSCCDRBat3HVSwitch;
	LEMBatterySwitch DSCCDRBat4HVSwitch;	
	LEMDeadFaceSwitch DSCBattFeedSwitch;

	SwitchRow DSCLoVoltageSwitchRow;
	LEMBatterySwitch DSCSEBat1LVSwitch;
	LEMBatterySwitch DSCSEBat2LVSwitch;
	LEMBatterySwitch DSCCDRBat3LVSwitch;
	LEMBatterySwitch DSCCDRBat4LVSwitch;	

	SwitchRow DSCBatteryTBSwitchRow;
	IndicatorSwitch DSCBattery1TB;
	IndicatorSwitch DSCBattery2TB;
	IndicatorSwitch DSCBattery3TB;
	IndicatorSwitch DSCBattery4TB;
	IndicatorSwitch DSCBattFeedTB;

	SwitchRow ASCBatteryTBSwitchRow;
	IndicatorSwitch ASCBattery5ATB;
	IndicatorSwitch ASCBattery5BTB;
	IndicatorSwitch ASCBattery6ATB;
	IndicatorSwitch ASCBattery6BTB;

	SwitchRow ASCBatterySwitchRow;
	LEMBatterySwitch ASCBat5SESwitch;
	LEMBatterySwitch ASCBat5CDRSwitch;
	LEMBatterySwitch ASCBat6CDRSwitch;
	LEMBatterySwitch ASCBat6SESwitch;

	//
	// Currently these are just 0-5V meters; at some point we may want
	// to change them to a different class.
	//
	SwitchRow ComPitchMeterRow;
	DCVoltMeter ComPitchMeter;

	SwitchRow ComYawMeterRow;
	DCVoltMeter ComYawMeter;

	SwitchRow Panel14SignalStrengthMeterRow;
	DCVoltMeter Panel14SignalStrengthMeter;

	//////////////////
	// LEM panel 16 //
	//////////////////

	SwitchRow Panel16CB1SwitchRow;
	CircuitBrakerSwitch LMP_EVT_TMP_FDAI_DC_CB;
	CircuitBrakerSwitch SE_XPTR_DC_CB;
	CircuitBrakerSwitch RCS_B_ASC_FEED_1_CB;
	CircuitBrakerSwitch RCS_B_ASC_FEED_2_CB;
	CircuitBrakerSwitch RCS_B_ISOL_VLV_CB;
	CircuitBrakerSwitch RCS_B_QUAD1_TCA_CB;
	CircuitBrakerSwitch RCS_B_QUAD2_TCA_CB;
	CircuitBrakerSwitch RCS_B_QUAD3_TCA_CB;
	CircuitBrakerSwitch RCS_B_QUAD4_TCA_CB;
	CircuitBrakerSwitch RCS_B_CRSFD_CB;
	CircuitBrakerSwitch RCS_B_TEMP_PRESS_DISP_FLAGS_CB;
	CircuitBrakerSwitch RCS_B_PQGS_DISP_CB;
	CircuitBrakerSwitch RCS_B_MAIN_SOV_CB;
	CircuitBrakerSwitch PROP_DISP_ENG_OVRD_LOGIC_CB;
	CircuitBrakerSwitch PROP_PQGS_CB;
	CircuitBrakerSwitch PROP_ASC_HE_REG_CB;

	SwitchRow Panel16CB2SwitchRow;
	CircuitBrakerSwitch LTG_FLOOD_CB;
	CircuitBrakerSwitch EDS_CB_LOGIC_B;
	CircuitBrakerSwitch SCS_ASA_CB;

	SwitchRow Panel16CB3SwitchRow;
	CircuitBrakerSwitch ECS_CABIN_REPRESS_CB;

	SwitchRow Panel16CB4SwitchRow;
	CircuitBrakerSwitch HTR_SBD_ANT_CB;
	CircuitBrakerSwitch LMPInverter2CB;
	// ECA control & Voltmeter
	CircuitBrakerSwitch LMPDesECAContCB;
	CircuitBrakerSwitch LMPDesECAMainCB;
	CircuitBrakerSwitch LMPAscECAContCB;
	CircuitBrakerSwitch LMPAscECAMainCB;
	LEMVoltCB LMPDCBusVoltCB;
	// Battery feed tie breakers
	CircuitBrakerSwitch LMPBatteryFeedTieCB1;
	CircuitBrakerSwitch LMPBatteryFeedTieCB2;
	CircuitBrakerSwitch LMPCrossTieBusCB;
	CircuitBrakerSwitch LMPCrossTieBalCB;
	CircuitBrakerSwitch LMPXLunarBusTieCB;

	///////////////////////////
	// LEM Rendezvous Window //
	///////////////////////////

	int LEMCoas1Enabled;
	int LEMCoas2Enabled;

	bool FirstTimestep;

	bool LAUNCHIND[8];
	bool ABORT_IND;
	bool ENGIND[7];

	double countdown;

	bool bToggleHatch;
	bool bModeDocked;
	bool bModeHover;
	bool HatchOpen;
	bool bManualSeparate;
	bool ToggleEva;
	bool CDREVA_IP;

#define LMVIEW_CDR		0
#define LMVIEW_LMP		1

	int	viewpos;
	
	bool startimer;
	bool ContactOK;
	bool SoundsLoaded;

	bool Crewed;
	bool AutoSlow;

	OBJHANDLE hLEVA;
	OBJHANDLE hdsc;

	ATTACHMENTHANDLE hattDROGUE;

	UINT stage;
	int status;

	//
	// Panel flash.
	//

	double NextFlashUpdate;
	bool PanelFlashOn;

	int Realism;
	int ApolloNo;
	int Landed;

	int SwitchFocusToLeva;

	DSKY dsky;
	LEMcomputer agc;
	Boiler imuheater; // IMU Standby Heater
	h_Radiator imucase; // IMU Case
	IMU imu;	
	LMOptics optics;

	MissionTimer MissionTimerDisplay;
	LEMEventTimer EventTimerDisplay;

	double DescentFuelMassKg;	///< Mass of fuel in descent stage of LEM.
	double AscentFuelMassKg;	///< Mass of fuel in ascent stage of LEM.

#define LMPANEL_MAIN			0
#define LMPANEL_RIGHTWINDOW		1
#define LMPANEL_LEFTWINDOW		2
#define LMPANEL_LPDWINDOW		3
#define LMPANEL_RNDZWINDOW		4
#define LMPANEL_LEFTPANEL		5
#define LMPANEL_AOTVIEW			6
#define LMPANEL_RIGHTPANEL		7

	bool InVC;
	bool InPanel;
	bool InFOV;  
	int  PanelId; 
	double SaveFOV;
	bool CheckPanelIdInTimestep;

	// ChecklistController
	ChecklistController checkControl;

	SoundLib soundlib;

	Sound Sclick;
	Sound Bclick;
	Sound Gclick;
	Sound Rclick;
	Sound LunarAscent;
	Sound StageS;
	Sound S5P100;
	Sound Scontact;
	Sound SDMode;
	Sound SHMode;
	Sound SLEVA;
	Sound SAbort;
	Sound CabinFans;
	Sound Vox;
	Sound Afire;
	Sound Slanding;

	//
	// Connectors.
	//

	///
	/// \brief MFD to panel connector.
	///
	PanelConnector MFDToPanelConnector;

	///
	/// \brief Connector from LEM to CSM when docked.
	///
	MultiConnector LEMToCSMConnector;				// This carries data *FROM* CSMToLEMPowerConnector
	LEMPowerConnector CSMToLEMPowerConnector;		// This sends data *FROM* CSMToLEMPowerSource *TO* LEMToCSMConnector
	PowerSourceConnectorObject CSMToLEMPowerSource; // This looks like an e-object

	char AudioLanguage[64];

	// New Panel SDK stuff
	int *pLEMValves[N_LEM_VALVES];
	bool ValveState[N_LEM_VALVES];

	// POWER AND SUCH

	// Descent batteries
	Battery *Battery1;
	Battery *Battery2;
	Battery *Battery3;
	Battery *Battery4;

	// Ascent batteries
	Battery *Battery5;
	Battery *Battery6;

	// Lunar Stay Battery
	Battery *LunarBattery;

	// Bus Tie Blocks (Not real objects)
	LEM_BusFeed BTB_CDR_A;
	LEM_BusFeed BTB_CDR_B;
	LEM_BusFeed BTB_CDR_C;
	LEM_BusFeed BTB_CDR_D;
	LEM_BusFeed BTB_CDR_E;
	LEM_BusFeed BTB_LMP_A;
	LEM_BusFeed BTB_LMP_B;
	LEM_BusFeed BTB_LMP_C;
	LEM_BusFeed BTB_LMP_D;
	LEM_BusFeed BTB_LMP_E;
	
	// Bus Cross Tie Multiplex (Not real object)
	LEM_BusCrossTie BTC_MPX;

	// XLUNAR Bus Controller
	LEM_XLBControl BTC_XLunar;

	// ECA
	LEM_ECAch ECA_1a; // (DESCENT stage, LMP DC bus)
	LEM_ECAch ECA_1b; // (DESCENT stage, LMP DC bus)
	LEM_ECAch ECA_2a; // (DESCENT stage, CDR DC bus)
	LEM_ECAch ECA_2b; // (DESCENT stage, CDR DC bus)
	LEM_ECAch ECA_3a; // (ASCENT  stage, LMP DC bus)
	LEM_ECAch ECA_3b; // (ASCENT  stage, CDR DC bus)
	LEM_ECAch ECA_4a; // (ASCENT  stage, CDR DC bus)
	LEM_ECAch ECA_4b; // (ASCENT  stage, LMP DC bus)

	// Descent stage deadface bus stubs
	DCbus DES_CDRs28VBusA;
	DCbus DES_CDRs28VBusB;
	DCbus DES_LMPs28VBusA;
	DCbus DES_LMPs28VBusB;

	// CDR and LMP 28V DC busses
	DCbus CDRs28VBus;
	DCbus LMPs28VBus;

	// AC Bus A and B
	// This is a cheat. the ACbus class actually simulates an inverter, which is bad for the LM.
	// So we fake it out with a DC bus instead.
	// Also, I have to get to these from the inverter select switch class	
	public:
	DCbus ACBusA;
	DCbus ACBusB;
	protected:

	// AC inverters
	LEM_INV INV_1;
	LEM_INV INV_2;

	// GNC
	ATCA atca;
	LEM_LR LR;
	LEM_RR RR;

	// COMM
	LEM_SteerableAnt SBandSteerable;
	
	// EDS
	LEM_EDS eds;

	// Abort Guidance System stuff
	LEM_ASA asa;

	bool isMultiThread;

	// Friend classes
	friend class ATCA;
	friend class LEM_EDS;
	friend class LEMcomputer;
	friend class LEMDCVoltMeter;
	friend class LEMDCAmMeter;
	friend class LMOptics;
	friend class LEMBatterySwitch;
	friend class LEMDeadFaceSwitch;
	friend class LEMInverterSwitch;
	friend class LEM_BusCrossTie;
	friend class LEM_XLBControl;
	friend class LEM_LR;
	friend class LEM_RR;
	friend class LEM_ASA;
	friend class LEM_SteerableAnt;
};

extern void LEMLoadMeshes();
extern void InitGParam(HINSTANCE hModule);
extern void FreeGParam();

#endif