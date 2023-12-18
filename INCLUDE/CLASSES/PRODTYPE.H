#ifndef __PRODTYPE_H
#define __PRODTYPE_H

// Contents ----------------------------------------------------------------
//
// product type class
//
// Description
//
// This class defines all data and functions for one product type
// in a steel industry furnace of ASCOMETAL.
//
// Creation : C.Martinet 16/06/92
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __SORTABLE_H
#include <Sortable.h>
#endif


// End Interface Dependencies ------------------------------------------------

// Class //

class ProdType : public Sortable
{
public:
//member functions
       //constructor
       ProdType(int,char *[],int,int,int);

       //destructor
       virtual ~ProdType();

//member functions
       //inline functions
       int number() { return Number; }
       char **theCostumerQual() { return TheCostumerQual; }
       int diam() { return Diam; }
       int tdef() { return Tdef; }
       int bisra() { return Bisra; }

       //other function
       void update(char *[],int d=0,int td=0,int b=0);
		//update the data with the arguments, 0 means no change

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual int             isLessThan( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
	int Number; //product type number
	char *TheCostumerQual[4]; //pointer to array of customer steel quality
	int Diam; //product diameter (mm)
	int Tdef; //discharging temperature target (øC)
	int Bisra; //steel quality

};

// Description -------------------------------------------------------------
// Constructor :
//      ProdType(int nb, char *qual[], int diam, int tdef, int bisra)
//
//      nb -> number of product type
//	*qual[] -> array of 4 character string (customer steel quality)
//                 one string may have up to 8 characters.
//      diam -> product diameter (mm)
//      tdef -> discharging temperature target (øC)
//      bisra -> steel quality (1 to 22)
// End ---------------------------------------------------------------------


#endif // ifndef __PRODTYPE_H //

