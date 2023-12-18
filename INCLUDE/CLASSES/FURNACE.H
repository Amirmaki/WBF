#ifndef __FURNACE_H
#define __FURNACE_H

// Contents ----------------------------------------------------------------
//
// Furnace class
//
// Description
//
// This class defines all data and functions for one steel
// industry furnace.
//
// Creation : C.Martinet 11/02/92
// Mofified :            05/04/92 add data for FIRST
//                       28/04/92 zone inf.
//			 06/07/92 add copy contructor
//			 16/07/92 add zone response time for zone creation
//			 03/02/93 new arguments to addZone
// F Destrebecq          Adapted for pacing data and double step data
//
// Description -------------------------------------------------------------
// Constructor :
//
//   Furnace(float L, float w, float hs, float hi, float pci,
//	     float step,float step2, int nbstep,int dblstep,
//           float chstepabs , float begzu , float endzu ,
//           float pcomb = 0,float volfume = 0, float frt = 0, float art = 0);
//
//   L -> furnace length (m)
//   w -> furnace width (m)
//   hs -> roof <-> product distance (m) (upper roof)
//   hi -> roof <-> product distance (m) (bottom roof)
//   pci -> gas heat capacity (kJ/Nm3)
//   step -> length of one beam step (m)
//   step2 -> length of the second beam step (m)
//   nbstep -> number of step between 2 charging operations
//   dblstep -> 0 : simple step furnace      1 : double step furnace
//   chstepabs -> abscissa limit between the two walking beam
//   begzu -> abscissa of the beginning of zone utile (m)
//   endzu -> abscissa of the end of zone utile (m)
//added on 05/APR/92
//   pcomb -> pouvoir comburivore
//   volfume -> volume fumee
//   frt -> recup law : fume temp. (øC)
//   art -> recup law : air temp.  (øC)
//end added
// End ---------------------------------------------------------------------

// Interface Dependencies ---------------------------------------------------
#ifndef __ARRAY_
#include <Array.h>
#endif
#ifndef __DEQUE_H
#include <Deque.h>
#endif
#ifndef __TIME_H
#include <time.h>
#endif
// End Interface Dependencies ------------------------------------------------

#define FumeSlope 	100.0		    // Pour les zones neutres (°C/m)

// Class
class Product;
class Batch;
class Furnace : public Object
{ public: // member functions
    // constructor
    Furnace(float,float,float,float,float,float,float,int,int,float,float,
	    float,float pcomb = 0,float volfume = 0, float frt = 0,
	    float art = 0 );
    // copy constructor
    Furnace(const Furnace&);
    // destructor
    virtual ~Furnace();

    // inline functions
    float chargingAbs();
    //FD 16/08/93
    float begZuAbs() const { return BegZuAbs; }
    float endZuAbs() const { return EndZuAbs; }
    void  setBegZuAbs(float begAbs) { BegZuAbs = begAbs ; }
    void  setEndZuAbs(float endAbs) { EndZuAbs = endAbs ; }
    //end added FD
    float length() const { return Length; }
    float width() const { return Width; }
    float height() const { return Height; }
    float heightI() const { return HeightI; }
    float absRecupTemp() const { return RecupTemp; }
    float absRecupTempI() const { return RecupTempI; }
    float celRecupTemp() { return RecupTemp - 273; }
    float celRecupTempI() { return RecupTempI - 273; }
    void  setRecupTemp(float celrect) { RecupTemp = celrect+273; }
    void  setRecupTempI(float celrect) { RecupTempI = celrect+273; }
    //fnct pour tenir de la date correspondant à la dernière avance
    long GetSecBeforeStep() {return (time(NULL)-SecBeforeStep);}
    long GetDateBeforeStep(){return (SecBeforeStep);}
    void SetSecBeforeStep(long val){SecBeforeStep = val;}
    void FixSecBeforeStep(){SecBeforeStep = time(NULL);}
    
    
    //added on 05/APR/92
    float pouvComb() const { return PouvComb; }
    float volFume() const { return VolFume; }
    float celFumeRecTemp() const { return FumeRecTemp - 273; }
    float celAirRecTemp() const { return AirRecTemp - 273; }
    //end added
    float celFlameTemp() const { return FlameTemp - 273; }
    float celCombAirTemp() const { return CombAirTemp - 273; }
    float gasFlowRate() const { return GasFlowRate; }
    float gasFlowRateI() const { return GasFlowRateI; }
    float specifConsum() const { return SpecifConsum; }
    float hourProd() const { return HourProd; }
    Batch *theBatch() const { return ChargingBatch; }
    void setChargingBatch(Batch *Bptr) { ChargingBatch = Bptr; }
    float wbeamStep() const { return WbeamStep; }
    void setWbeamStep(float newwbs) { WbeamStep = newwbs; }
    //FD 13/09/93 for double step
    float wbeamStep2() const { return WbeamStep2; }
    void setWbeamStep2(float newwbs) { WbeamStep2 = newwbs; }
    float changeStepAbs() const { return ChangeStepAbs; }
    void setChangeStepAbs(float newabs) { ChangeStepAbs = newabs; }
    int doubleStep() const { return DoubleStep ; }
    int nbStep() const { return NbStep; }
    void setNbStep(int newnbs) { NbStep = newnbs; }
    int currentNbStep() const { return CurrentNbStep; }
    float pci() const { return Pci; }
    void setPci(float newpci) { Pci = newpci; }
    // Returns a reference to the array of upper zone
    Array& theZone() const { return (Array&)(*TheZone); }
     // Returns a reference to the array of bottom zone
    Array& theZoneI() const { return (Array&)(*TheZoneI); }
    // Returns a reference to the product list
    Deque& theProd() const { return (Deque&)(*TheProd); }
    // Returns a pointer to the upper RoofTemp array

    float *theRoofTemp()  const { return RoofTemp; }
    // Returns a pointer to the bottom RoofTemp array
    float *theRoofTempI() const  { return RoofTempI; }
    // Returns a pointer to the upper FumeTemp array
    float *theFumeTemp() const  { return FumeTemp; }
    // Returns a pointer to the bottom FumeTemp array
    float *theFumeTempI() const { return FumeTempI; } 
    // Other functions
    void addZone(int,float,float,float,float,float tcd = 0,
		 float losses = 0, int respt = 15, float qgm = 0,
		 float qam = 0, float spmin = 800, float spmax = 1350,
		 float spvar = 10, float kz = 1,float rap=1);
		 // Add a zone in zone array
		 // arguments are upper zone (0) or bottom zone (1)
		 // zone contructor arguments
    int nbProd() const;     // Returns the number of products in the furnace
    int nbZone() const;  // Returns the number of upper zones in the furnace
    int nbZoneI() const;// Returns the number of bottom zones in the furnace
    void empty(); 			// Delete all product in the furnace
    int calcul();   // Makes all calculations, return 1 if ok, else return 0
    int calcRoofTemp();     // Calculates the roof temperature along furnace
			       		// Return 1 if all ok, else return 0
    int calcRoofTempI();    // Calculates the roof temperature along furnace
			   // bottom roof, return 1 if all ok, else return 0
    float roofTemp(float); 	     // Returns the roof temperature (øK) at
						   // specified abscissa (m)
    float roofTempI(float);   // Returns the bottom roof temperature (øK) at
    					           // specified abscissa (m)
    int calcFumeTemp();     // Calculates the fume temperature along furnace
			   		// return 1 if all ok, else return 0
    int calcFumeTempI();    // Calculates the fume temperature along furnace
		            // bottom roof,return 1 if all ok, else return 0
    float fumeTemp(float); 	     // Returns the fume temperature (øK) at
			      			   // specified abscissa (m)
    float fumeTempI(float);   // Returns the bottom fume temperature (øK) at
    						   // specified abscissa (m)
    int calcProdTemp();     // Makes each product calculate its temperatures
    // Modif jl C
    void CalculThermThick(float refabsc,Product& REFprod);
    // end modif
    void makeStep(float step= 999999.0);  // Perform a walking beam movement
			      // if step is not specified, WbeamStep is used
    void makeStep2(int);          // Perform a wbeamstep for the beam 2 only
			      // Application falck 0 : backward, 1 : forward
    int testCharging(float abs = -1,int steptest = 1);
    //if steptest = 1 (default) return 0 if number of step since last
    //charging is lower than NbStep or there is not enough space for
    //one product(less than product width (default) or specified
    //abscissa), test only space if steptest = 0, return 1 if ok for charging
    int charge(float abs = -1, int testcha = 2, float nseccha = 0,float cad=0,
    	       int CharTemp=20);
    // performs a charging operation
    //if abscissa is not specified, it is set to product width
    //if testcha = 2 ,call testCharging(abs) (test step+space)
    //if testcha = 1 ,call testCharging(abs,0) (test space)
    //if testcha = 0, no charging test is performed
    //nseccha is passed residence time
    //function returns 1 if charging hes been made, else it returns 0
    Product& discharge(int destroyToo = 1);
    //performs a discharging operation on product
    //beyong the furnace length, returns a reference
    //to the discharged product if destroyToo = 0 (user becomes
    //the owner of the object and must destroy it himself,
    //else destroy the product object (default) and returns
    //(Product&)NOOBJECT
    void updateResTime(float); //add the specified number of seconds
			       //to the residence time of each
			       //product in the furnace
    void UpdateDataEndZone(float nbsec); //mise à jour des données zone prech
    					//et égalisation
    void calcCombAirTemp(); //calculates the combustion air temp.
    void calcFlameTemp(); //calculates the flame temperature
    void calcProdPerHour(int); //calculates the production (t/h)
    float calcZoneGasFlowRate();//calculates each zone gas flow rate
				//returns the new fume temp. at recup
				//entry (øC) (upper zone)
    float calcZoneGasFlowRateI();//calculates each zone gas flow rate
				 //returns the new fume temp. at recup
				 //entry (øC) (bottom zone)
    void calcSpecifConsum(); //calculates the specific consumption.
    float calcGasFlowRate(); //make all gas flow rate and
			     //specific consumption calculation
			     //returns the new fume temp. at recup entry (øC)
    int dataload (int,char *); // charge les données de la DB
    int pro_dataload ( Batch * [] , int , char * );
    // crée la liste des produits à partir de la database
    int ctrl_dataload ( int  , char * );
    int pro_datasave ( int , char * );
    void vide();
    //sauvegarde de la dernière billette défournée
    int DisBil_datasave(Product *disP,char *DB_path);
    int profile_datasave ( int ,  char *);
    void SaveRatio(); //sauvegarde des rapprots de debits
    //******************
    //jlc 14/09/94******
    //******************
    //cette fonction recherche les objectifs dans la liste des objectifs
    // en fonction de la cadence et du type de produit
    //si les objectifs ont été trouvés, la fonction renvoie leur N° d'enr.
    //sinon la fonction renvoie -1
    int RechercheTarget(float Prod,int Num);
    //cette fonction renvoie:
    // - le Type de produit correspondant à l'identité Id s'il existe
    // - -1 sinon
    int LireProdType(int Id);
    // cette fonction affecte les objectifs au dernier produit enfourné
    int affectObj();
    //fixation l'objectif courant
    //cette fonction recherche les objectifs des produits qui
    //se trouve en fin de zone afin de les recopier dans l'objectif
    //courant (cf CTRLFOUR.DTA)
    int FixeObjActif();
    //sauvegarde l'heure depuis la dernière avance
    int datasave( char *);
    //member inherited from Object class :
    virtual classType       isA() const;
    virtual char           *nameOf() const;
    virtual hashValueType   hashValue() const;
    virtual int             isEqual( const Object& ) const;
    virtual void            printOn( ostream& ) const;
    //member data
  public:
    long CalcDuration;        //number of seconds to make a calculation
    long ProdCalcDuration;    //mean duration to make a product calcul.(ms)
    static float epsif1;      //  6.7916e-5 coeff for epsif calc
    static float epsif2;      //  3073
    static float minepsif;    //  .15 min value for epsif
    static float maxepsif;    //  .4 max value for epsif
  protected:
    float Length;        //furnace length (m)
    float Width;         //furnace width (m)
    float Height;        //height beetween products and upper roof (m)
    float HeightI;       //height beetween products and bottom roof (m)
    float Pci;           //gas Pci (kJ/Nm3)
    float WbeamStep;     //walking beam step (m)
    float WbeamStep2;    //second w beam FD 13/09/93 (m)
    float ChangeStepAbs; //limit between the two walking beams (m)
    int DoubleStep;      //0 -> simple step furnace
			 //1 -> double step furnace
    int NbStep; 	 //number of step between 2 charging operations
    //added on 05/APR/92
    float PouvComb;      //pouvoir comburivore
    float VolFume;       //volume fumee
    float FumeRecTemp;   //recup law : fume temp. (øK)
    float AirRecTemp;    //recup law : air temp. (øK)
    float FlameTemp;     //flame temp. (øK)
    float CombAirTemp;   //combustion air temp. (øK)
    float GasFlowRate;   //gas flow rate (Nm3/h) (upper)
    float GasFlowRateI;  //gas flow rate (Nm3/h) (bottom)
    float SpecifConsum;  //specific consumption (MJ/t)
    float HourProd;	     //production per hour (t/h)
    long SecBeforeStep; //temps écoulé depuis la dernière avance
    //end added
    int CurrentNbStep;    //number of step since last charging operation
    Batch *ChargingBatch; //pointer on batch object defining data of
			  //next charged products
    Array *TheZone;       //pointer to an array of upper zones
    Array *TheZoneI;      //pointer to an array of bottom zones
    Deque *TheProd;       //list of products : left side will be
			  //charging side, right side discharging side
    float RecupTemp;      //recuperator temperature (øK) (upper)
    float RecupTempI;     //recuperator temperature (øK) (bottom)
    float *RoofTemp;  //roof temperature on 100 points
    			  //along the upper furnace
    float *RoofTempI; //roof temperature on 100 points
			  //along the bottom furnace
    float *FumeTemp;  //Fume temperature on 100 points
			  //along the upper furnace
    float *FumeTempI; //Fume temperature on 100 points
			  //along the bottom furnace
    //add FD 16/08/93 for pacing application
    float BegZuAbs ; // abscissa of the beginning of the zone utile
    float EndZuAbs ; // abscissa of the end of the zone utile
};
#endif // ifndef __FURNACE_H //

