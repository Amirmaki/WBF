#ifndef __ENDZTARG_H
#define __ENDZTARG_H

// Contents ----------------------------------------------------------------
//
// endztarget type class
//
// Description
//
// This class defines all data and functions for one end of zone target
// curve in a steel industry furnace.
//
// Creation : C.Martinet 18/06/92  ASCOMETAL application
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __SORTABLE_H
#include "Sortable.h"
#endif


// End Interface Dependencies ------------------------------------------------

// Class //

class EndzTarget : public Sortable
{
public:
//member functions
       //constructor
       EndzTarget(int,int,int,int,int *);//cad,num,chT,nbz,ztab

       //destructor
       virtual ~EndzTarget();

//member functions
       //inline functions
       int production() { return Production; }
       int number() { return Number; }
       int chTemp() {return ChTemp;}
       int nbZone() { return NbZone; }
       int *theCurve() { return TheCurve; }
       int isActive() { return IsActive; }
       void activate() { IsActive = 1; }
       void desactivate() { IsActive = 0; }

       //other function
       void update(int *); //update the data with a new curve
       void update(int cad,int type,int ChT,int *ztab);

       int getTarget(int); //return the target for specified zone number

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual int             isLessThan( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
	int Production; //production in discharged products per hour
	int Number; //product type number
        int ChTemp; //charging temp 
	int NbZone; //number of zone with target
	int *TheCurve; //array of NbZone targets (øC)
	int IsActive; //1 if the curve is the present active curve for
		      //the product type number, 0 if not active

};

// Description -------------------------------------------------------------
// Constructor :
//      EndzTarget(int prod,int nb,int nbz,int *ztab)
//
//      prod -> production prod/hour
//      nb -> number of product type
//      nbz -> number of zone
//      ztab -> array of nbz targets (øC)
// End ---------------------------------------------------------------------


#endif // ifndef __ENDZTARG_H //

