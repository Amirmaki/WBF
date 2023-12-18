#ifndef __ZONE_H
#define __ZONE_H

// Contents ----------------------------------------------------------------
//
// zone class
//
// Description
//
// This class defines all data and functions for one zone in a steel
// industry furnace.
//
// Creation : C.Martinet 11/02/92
// Modified :            05/04/92 add data for FIRST
//			 28/04/92 add height
//			 25/06/92 add Target
//			 06/07/92 add copy contructor, setpoint and
//				  response time
//			 16/07/92 add resp time to constructor arguments
//			 03/02/93 add new data
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __OBJECT_H
#include <Object.h>
#endif

// End Interface Dependencies ------------------------------------------------

// Class //

class Zone : public Object
{
public:
//member functions
       //constructor
       Zone(float, float, float, float, float tcd = 0, float loss = 0,
	    int respt = 15, float qgm = 0, float qam = 0,
	    float spmin = 800, float spmax = 1350, float spvar = 10,
	    float kz = 1, float rap = 1);

       //copy constructor
       Zone(const Zone&);

       //destructor
       virtual ~Zone();

//member functions
       //inline functions
       float absBeg() const { return AbsBeg; }
       float absEnd() const { return AbsEnd; }
       float absTcpl() const { return AbsTcpl; }
       float absTemp() const { return Temp; }
       float celTemp() { return Temp-273; }
       void setTemp(float temp) { Temp = temp+273; }
       float absTarget() const { return Target; }
       float celTarget() const { return Target-273; }
       void setTarget(float target) { Target = target+273; }
       float absSetPoint() const { return SetPoint; }
       float celSetPoint() { return SetPoint-273; }
       void setSetPoint(float setpoint) { SetPoint = setpoint+273; }
       int responseTime() const { return ResponseTime; }
       void setResponseTime(int r) { ResponseTime = r; }

//added 05/Apr/92
       float losses() const { return Losses; }
       void setLosses(float loss) { Losses = loss; }
//end added
       float production () const {return Production;}
       void setProduction (float prod) {Production = prod;}
       float therm_t () const {return Therm_t;}
       void setTherm_t (float therm) {Therm_t = therm;}
       float rapport () const {return Rapport;}
       void setRapport(float rap) {Rapport = rap;} 

       float height() const { return Height; }
       float gasFlowRate() const { return GasFlowRate; }
       void setGasFlowRate(float gasf) { GasFlowRate = gasf; }
//added 29/04/92
       float enthProd() const { return EnthProd; }
       void setEnthProd(float penth) { EnthProd = penth; }
       float enthGas() const { return EnthGas; }
       void setEnthGas(float genth) { EnthGas = genth; }
       float enthFume() const { return EnthFume; }
       void setEnthFume(float fenth) { EnthFume = fenth; }
//end added

//added 03/02/93
       float tcplDepth() const { return TcplDepth; }
       float maxGasFlowRate() const { return MaxGasFlowRate; }
       float maxAirFlowRate() const { return MaxAirFlowRate; }
       float absMinSetPoint() const { return MinSetPoint; }
       float celMinSetPoint() const { return MinSetPoint - 273; }
       void setMinSetPoint(float s) { MinSetPoint = s + 273; }
       float absMaxSetPoint() const { return MaxSetPoint; }
       float celMaxSetPoint() const { return MaxSetPoint - 273; }
       void setMaxSetPoint(float s) { MaxSetPoint = s + 273; }
       float maxSpVar() const { return MaxSpVar; }
       void setMaxSpVar(float v) { MaxSpVar = v; }
       float  epsiFKZ() const { return EpsiFKZ; };
       void setEpsiFKZ(float k) {EpsiFKZ = k; }
       float airFlowRate() const { return AirFlowRate; }
       void setAirFlowRate(float airf) { AirFlowRate = airf; }
//end added

       //other function
       int isInZone(float); //return 1 if passed argument value
			    //is greater then AbsBeg and lower or
			    //equal to AbsEnd, else return 0

       int isUnderTcpl(float,float); //return 1 if Tcpl abscissa is
				     //greater than first float
				     //argument and lower or equal to
				     //second argument else return 0

//added 03/02/93
       float absTVatTcpl(float); //return roof temperature (øK) according to
				 //Temp,Height,TcplDepth and argument
				 //product surface temp (øK)

       float absTcplForTv(float,float); //return tcpl temperature (øK)
				 //according to Height,TcplDepth and
				 //arguments roof temp (øK) and product
				 //surface temp
//end added

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
	float AbsBeg; // zone beginning abscissa (m)
	float AbsEnd; // zone ending abscissa (m)
	float AbsTcpl; // zone thermocouple abscissa (m)
	float Temp; //zone thermocouple measure (øK)
	float Height; //distance between product and zone roof (m)
	float Losses; //zone losses (KJ/h)
	float GasFlowRate; //zone gas flow rate (Nm3/h)
	float EnthProd; //enthalpy taken by the product (kJ/h)
	float EnthGas;  //enthalpy of gas (kJ/h)
	float EnthFume; //enthalpy of fumes (kJ/h)
	float Target; //target temp for product at end of zone (øK)
	float SetPoint; //calculated setpoint temp used to
			//control the zone (øK)
	int ResponseTime; //zone response time (mn);
//added 03/02/93
	float TcplDepth; //distance between roof and tcpl (m)
	float MaxGasFlowRate; //maximum available gas flow rate (Nm3/h)
	float MaxAirFlowRate; //maximum available air flow rate (Nm3/h)
	float MinSetPoint;    //minimum authorize setpoint (øK)
	float MaxSetPoint;    //maximum authorize setpoint (øK)
	float MaxSpVar;       //maximum Setpoint variation (ø/mn)
	float EpsiFKZ;        //zone coeff for epsiF calculation
	float AirFlowRate;    //zone air flow rate (Nm3/h)
        float Production;  // production t/h servant au calcul de rap de deb
        float Therm_t;
        float Rapport; 
//end added
};

// Description -------------------------------------------------------------
// Constructor :
//      Zone(float absbeg, float absend, float abstcpl,
//	     float zheight, float tcd, float loss, int respt,
//           float qgm, float qam,
//	     float spmin, float spmax, float spvar, float kz);
//      absbeg -> zone beginning abscissa (m)
//      absend -> zone ending abscissa (m)
//      abstcpl -> zone thermocouple abscissa
//	zheight -> distance between product and roof (m)
//	tcd -> distance between roof and tcpl (m)
//	loss -> zone losses (KJ/h)
//      respt -> zone response time (mn), default is 15mn
//	qgm -> maximum available gas flow rate (Nm3/h)
//	qam -> maximum available air flow rate (Nm3/h)
//      spmin -> minimum authorize setpoint (øC)
//      spmax -> maximum authorize setpoint (øC)
//      spvar -> maximum authorize setpoint variation (ø/mn)
//	kz -> coefficient for gas emissivity correction
//
// End ---------------------------------------------------------------------


#endif // ifndef __ZONE_H //

