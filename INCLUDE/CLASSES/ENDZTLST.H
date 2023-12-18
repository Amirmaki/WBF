#ifndef __ENDZTLST_H
#define __ENDZTLST_H

// Contents ----------------------------------------------------------------
//
// endztargetlist class
//
// Description
//
// This class defines all data and functions for one list of target curves
//
// Creation : C.Martinet 18/06/92 ASCOMETAL application
// Modified              21/06/93 add unselect()
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __FSTREAM_H
#include <fstream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __SORTARRY_H
#include "SortArry.h"
#endif
#ifndef __SHDDEL_H
#include <ShdDel.h>
#endif 
#ifndef __ENDZTARG_H
#include "endztarg.h"
#endif


// End Interface Dependencies ------------------------------------------------

// Class //
class EndzTargetList : public Object
{
public:
//member functions
       //constructor
       EndzTargetList(int);

       //destructor
       virtual ~EndzTargetList();

//member functions
       //inline functions
       SortedArray& theList() { return (SortedArray&)(*TheList); }
		//returns a reference to the curve list

       int theSelectedProd() { return TheSelectedProd; }
		//returns the production used to activate the
		//curves

       int nbCurve() { return TheList->getItemsInContainer(); }

	//other function
	int readList(char *); //reads the list from a file, returns
	 //1 if all ok, else returns 0

	int writeList(char *); //writes the list in a file, returns
	//1 if all ok, else returns 0

	int addCurve(EndzTarget&); //adds a curve to the list
	    //returns 1 if ok, returns 0 if duplicate number and prod

	EndzTarget& getCurve(float,int,int); //returns a reference to the curve
	//object identified by its production and type number.
	//Returns NOOBJECT if no curve matches

	EndzTarget& existCurve(int,int,int);
	//old function getcurve created by Martinet

	EndzTarget& getActiveCurve(int); //returns a reference to the
	//present active curve for the specified product type number
	//Returns NOOBJECT if no curve matches

	EndzTarget& detachCurve(int,int,int,int  deleteToo=1); //detaches a curve
	//from the list; delete it if deleteToo!=0 (default) and
	//return NOOBJECT, return a reference to it if deleteToo==0

	void setTheSelectedProd(int); //makes the curves specified by its
	//production as the active curves. For each product type number
	//the active curve is the one which as the specified production
	//or which has the closest production (higher if possible)

	void unSelect(); //makes all curve unactive

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
	SortedArray *TheList; //pointer to an array of product types

	int TheSelectedProd; //production used to activate the curves

	EndzTarget *curve_tempo; //courbe temporaire
};

// Description -------------------------------------------------------------
// End ---------------------------------------------------------------------


#endif // ifndef __ENDZTLST_H //

