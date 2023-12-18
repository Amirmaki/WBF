//STD/0001+modif pf06/07/95
#ifndef __PRODUCT_H
#define __PRODUCT_H


// Contents ----------------------------------------------------------------
//
// product class
//
// Description
//
// This class defines all data and functions for product to be reheated
// in steel industry furnace.
//
// Creation : C.Martinet 11/02/92
// Modified :            07/04/92 add setCelTemp for FIRST
//			 21/04/92 add enthzs
//			 04/05/92 add HalfThermThick, zones checking
//                                and flux density data
//			 15/05/92 add PF and DC
//			 25/06/92 add CoreTemp
//			 05/02/93 readZone returns also kz
//
//   F DESTREBECQ        17/06/93 Add PRODUCTDEBBUG definition for
//                                Serial Port DEBBUG
//                       16/08/93 Add functions setBegZuTime ()
//                                              readBegZuTime ()
//                                Add data BegZuTime used for pacing
//
//   JL COUSIN          15/09/93  Add field and data to calculate the thermal
//                                thickness
//                                Add functions to  calculate the abscissa to
//                                begin the Thermal Thickness calculation
//
//   F DESTREBECQ       08/02/94  Add functions setBegZ3Time()
//                                              readBegZ3Time()
//                                    data      BegZ3Time
//                                 For target decrease in zone 3 (Falck)
//   P Favale           06/07/95  Rajout des objectifs dans la classe product
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

// Serial debbug mode selection
//#define PRODUCTDEBBUG

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __BATCH_
#include "batch.h"
#endif

#ifndef __TIME_H
#include <time.h>
#endif

// End Interface Dependencies ------------------------------------------------

// Class //

class Product : public Object
{
public:
//member functions
       //constructor
       Product(Batch *Bptr, float abs = 0, float gap = -1, float nseccha = 0 ,
	       float cad = 0 , int Tc = 293);

       //copy constructor
       Product(const Product&);

       //destructor
       virtual ~Product();

//member functions
       //inline functions
       Batch *theBatch() const { return TheBatch; }
       void setBatch(Batch *B) { TheBatch = B; }

       // en multitache
       void setIdentity ( int id ) { Identity = id ; }
       int  identity () const { return Identity ; }

       //FD 16/08/93

       void setBegZuTime ()   { BegZuTime = time (NULL) ; }
       void setEndZuTime ()   { EndZuTime = time (NULL) ; }

       void FixBegZuTime (long date)   { BegZuTime = date ; }
       void FixEndZuTime (long date)   { EndZuTime = date ; }


       void setProdRate  (float prodrate) { ProdRate = prodrate ; }
       //FD 08/02/94 for FALCK
       void setBegZ3Time ()   { BegZ3Time = time (NULL) ; }
       // JL C 15/09/93
       void setLeftThermThic(float left) {LeftThermThick=left;}
       void setRightThermThic(float right) {RightThermThick=right;}
       void resetCaltodo (int choice)
	       {
		if (choice == 1) Caltodo1=0;
		if (choice == 2) Caltodo2=0;
	       }
       void setMaxAbsc(float Max) {Maximumabscissa=Max;}
       //end modif

       //FD 16/08/93
       time_t readBegZuTime() const{ return BegZuTime; }
       time_t readEndZuTime() const{ return EndZuTime; }
       float  prodRate () const{ return ProdRate ; }
       //FD 08/02/94 for FALCK
       time_t readBegZ3Time() const { return BegZ3Time; }

       float absMeanTemp() const { return MeanTemp; }
       float absTopSurfTemp() const { return TopSurfTemp; }
       float absBotSurfTemp() const { return BotSurfTemp; }
       float absCoreTemp() const { return CoreTemp; }
       float celMeanTemp() { return MeanTemp - 273; }
       float celTopSurfTemp() { return TopSurfTemp - 273; }
       float celBotSurfTemp() { return BotSurfTemp - 273; }
       float celCoreTemp() { return CoreTemp - 273; }
       float abscissa() const { return Abscissa; }
       float abscissaGap() const { return AbscissaGap; }
       void moveOf( float step ) { Abscissa += step; }
       float residTime () const { return ResidTime; }
       void updateResTime( float nsec ) { ResidTime += nsec; }
       float *theEnthzs() const { return Enthzs; }
       float *theEnthzi() const { return Enthzi; }
       float phiSup() const { return PhiSup; }
       float phiInf() const { return PhiInf; }
       void setPhiSup(float phis) { PhiSup = phis; }
       void setPhiInf(float phii) { PhiInf = phii; }
       float perteFeu() const { return PerteFeu; }
       void setPerteFeu(float pf) { PerteFeu = pf; }
       float decarb() const { return Decarb; }
       void setDecarb(float dc) { Decarb = dc; }
       // Renvoie ou Affecte les objectifs (PF06/07/95)
       int *theTargets() const { return(Targets); }; 
       void setTargets(int *Src);
//modif jl Cousin
       float lThermThick () const {return LeftThermThick;}
       float rThermThick () const {return RightThermThick;}
       float MaxAbscissa () const {return Maximumabscissa;}
       float MaxHalfThermThick() const {return MaximumHalfThermThick;}
       int Cal(int choice) const
	 {
	 if (choice==1) return Caltodo1;
	       else     return Caltodo2;
	 }
//end modif

       //other function
//     modif jl
       void CalMaxAbscSlab(float,float); //Calculate the abscissa to begin
					 //CalThermThic() for slab
       void CalMaxAbscBil(float); //Calculate the abscissa to begin
				  //CalThermThic() for Billet
//     end modif
       int readZoneS(float *, float *); //return upper zone containing
					//the product and zone height & kz
       int readZoneI(float *, float *); //return bottom zone containing
					//the product or -1 if no bottom
					//zone  and zone height & kz

       int calcTemp(int SymetricPhi = 0); //calculates temperature for the
		//product, return 1 if ok, 0 if inconsistant new TopSurfTemp
		//with roof temperature. Argument SymetricPhi is 1 for
		//symetrical heating, whatever the furnace bottom profiles

       void setCelTemp(float,float,float,float); // set the temperature
			//to given TM,TS,TI (given in øC)

       float readTV(); //returns used TV for temp. increase calc.
       float readTF(); //returns used TF for temp. increase calc.
       float readTVI(); //returns used bottom TV
       float readTFI(); //returns used bottom TF
       int   chargingTemp() const {return (ChargingTemp-273);} // (øC)

       //fonction de lecture et ecriture données fin de préchauf
       int readMTPrech()const { return MTPrech; } //Temp moyenne °C
       int readTTPrech()const { return TTPrech; } //Temp moyenne °C; //Temp Surf sup °C
       int readBTPrech()const { return BTPrech; } //Temp Surf inf °C
       int readCTPrech()const { return CTPrech; } //Temp Coeur °C
       int readRTPrech()const { return RTPrech; } //Temp Voute °C
       float readResTPrech()const { return ResTPrech; } //Temps sejour mn
       void setMTPrech(int aux) { MTPrech = aux; }
       void setTTPrech(int aux) { TTPrech = aux; }
       void setBTPrech(int aux) { BTPrech = aux; }
       void setCTPrech(int aux) { CTPrech = aux; }
       void setRTPrech(int aux) { RTPrech = aux; }
       void setResTPrech(float aux) { ResTPrech = aux; }

       //fonction de lecture et ecriture données fin d'égalisation
       int readMTNorm()const { return MTNorm; } //Temp moyenne °C
       int readTTNorm()const { return TTNorm; } //Temp moyenne °C; //Temp Surf sup °C
       int readBTNorm()const { return BTNorm; } //Temp Surf inf °C
       int readCTNorm()const { return CTNorm; } //Temp Coeur °C
       int readRTNorm()const { return RTNorm; } //Temp Voute °C
       float readResTNorm()const { return ResTNorm; } //Temps sejour mn
       void setMTNorm(int aux) { MTNorm = aux; }
       void setTTNorm(int aux) { TTNorm = aux; }
       void setBTNorm(int aux) { BTNorm = aux; }
       void setCTNorm(int aux) { CTNorm = aux; }
       void setRTNorm(int aux) { RTNorm = aux; }
       void setResTNorm(float aux) { ResTNorm = aux; }

       //fonction ecr et lect du poids et N° ordre
       int readNumOrdre()const { return NumOrdre; } //N° ordre
       int readPoids()const { return Poids; } //poids
       void setNumOrdre(int aux) { NumOrdre = aux;}
       void setPoids(int aux) { Poids = aux; }

       float TestAbsc(float absc , float refabsc);
       float FiltrephiSup(float curTSsup ,float  EpsiEq ,float  ProdAbsc, float TF );
       float calcEpsiF(float ProdAbsc,float TF);

       // date de chargement 
       void getChargingDate ( char [6] ) const ;
       void setChargingDate ( char [6] ) ;



//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data

protected:
  Batch *TheBatch;      //pointer to the batch object
  float AbscissaGap;    //abscissa difference between this product
			//and previous one (m)
  float ThermThick;     //thermal equivalent thickness (m)

  //modif JL Cousin
  int Caltodo1; //field necessary to know if the thermal thickness have been Calculate
  int Caltodo2; // caltodo=0 thethermal thickness haven't been calculate
		// caltodo=1 thethermal thickness have been calculate
  float HalfThermThick; //thermal equivalent thickness for half thickness (m)
  float LeftThermThick; //thermal equivalent thickness for left half thickness
  float RightThermThick;//thermal equivalent thickness for left half thickness
  float Maximumabscissa;//abscissa where the thermal thickness calculation is made
  float MaximumHalfThermThick;//maximum half thermal thickness

// end modif

  float Abscissa;       //product abscissa in furnace (m)
  float ResidTime; //seconds since charging
  float MeanTemp;  //mean temperature (øK)
  float TopSurfTemp;  //Top surface temperature (øK)
  float BotSurfTemp;   //Bottom surface temperature (øK)
  float CoreTemp;   //core temperature (øK)
  //21/04/92
  float *Enthzs;   //pointer to array of product enthalpy received
		   //in each upper zone
  float *Enthzi;   //pointer to array of product enthalpy received
		   //in each bottom zone
 //04/05/92
 float PhiSup;  //flux density received on top surface (last calculation)
 float PhiInf;  //flux density received on bot surface (last calculation)
 //15/05/92
 float PerteFeu; //perte au feu (cm)
 float Decarb;   //profondeur de decarburation (cm)
 //FD 16/08/93
 time_t BegZuTime ; //temps absolu (sec) correspondant a l'entree du produit
		    //dans la zone utile
 time_t EndZuTime ; //sortie de la zone utile
 float  ProdRate  ; //Cadence Equivalente calcul‚e du produit en zone utile
 //FD 08/02/94 for FALCK
 time_t BegZ3Time ; // temps absolu … l'entree dans la zone 3

 int ChargingTemp ; // Temperature d'enfournement (øK)

 int Identity ; // Nø d'identit‚ du produit (utile en multitaches)

//définition des données fin de zone de préchauffage
int MTPrech; //Temp moyenne °C
int TTPrech; //Temp Surf sup °C
int BTPrech; //Temp Surf inf °C
int CTPrech; //Temp Coeur °C
int RTPrech; //Temp Voute °C
float ResTPrech; //Temps sejour mn

//définition des données fin de zone d'égalisation
int MTNorm; //Temp moyenne °C
int TTNorm; //Temp Surf sup °C
int BTNorm; //Temp Surf inf °C
int CTNorm; //Temp Coeur °C
int RTNorm; //Temp Voute °C
float ResTNorm; //Temps sejour mn

//poids et N°Ordre
int NumOrdre;  //N° ordre
int  Poids;  //poids
// Objectifs (PF06/07/95)
int *Targets; // Tableau des objectifs du produit
// Date d'enfournement
char ChargingDate[6] ;
};

// Description -------------------------------------------------------------
// Previous product means product that have been charged before this one
//
// Constructor :
// 	Product (Batch *Bptr,float abs,float gap = -1,flaot nseccha = 0,
//               int Tc )
//
//	*Bptr -> pointer to the batch object
//      abs -> charging abscissa (discharging side border abscissa),
//	       default value 0
//	gap -> AbscissaGap value, default value -1 if no previous product
//      nseccha -> initial residence time (s), default value 0
//      Tc  -> Charging Temperature (øC)
//
// End ---------------------------------------------------------------------


#endif // ifndef __PRODUCT_H //

