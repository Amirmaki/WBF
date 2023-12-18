#ifndef __PRODTLST_H
#define __PRODTLST_H

// Contents ----------------------------------------------------------------
//
// prodtypelist class
//
// Description
//
// This class defines all data and functions for one list of product type
//
// Creation : C.Martinet 17/06/92 ASCOMETAL application
// Modified              22/01/93 add unSelect()
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __FSTREAM_H
#include <fstream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __SORTARRY_H
#include <SortArry.h>
#endif

#ifndef __PRODTYPE_
#include "prodtype.h"
#endif


// End Interface Dependencies ------------------------------------------------

// Class //
class ProdTypeList : public Object
{
public:
//member functions
       //constructor
       ProdTypeList();

       //destructor
       virtual ~ProdTypeList();

//member functions
       //inline functions
       SortedArray& theList() { return (SortedArray&)(*TheList); }
		//returns a reference to the type list

       ProdType* theSelected() { return TheSelected; }
		//returns a pointer to the selected type (used
		//at charging)

       int nbType() { return TheList->getItemsInContainer(); }

       void unSelect() { TheSelected = NULL; }


	//other function
	int readList(char *); //reads the list from a file, returns
	 //1 if all ok, else returns 0

	int writeList(char *); //writes the list in a file, returns
	//1 if all ok, else returns 0

	int addType(ProdType&); //adds a type to the list
	    //returns 1 if ok, returns 0 if duplicate number

	ProdType& getType(int); //returns a reference to the ProdType
	//object identified by its number. Returns NOOBJECT if no
	//type matches

	ProdType& detachType(int,int deleteToo=1); //detaches a type
	//from the list; delete it if deleteToo!=0 (default) and
	//return NOOBJECT, return a reference to it if deleteToo==0

	int setTheSelected(int); //makes the type specified by its
	//number as the selected type. Returns 1 if ok, else
	//returns 0 (unknown number)


//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
	SortedArray *TheList; //pointer to an array of product types

	ProdType *TheSelected; //pointer to the selected type
};

// Description -------------------------------------------------------------
// End ---------------------------------------------------------------------


#endif // ifndef __PRODTLST_H //

