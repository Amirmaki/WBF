#ifndef __POLYNOME_H
#define __POLYNOME_H

// Contents ----------------------------------------------------------------
//
// Polynome class
//
// Description
// This class defines a polynome of degree Deg available
// between a lower and an upper bound.
//
//
// Creation C.Martinet 27/01/92
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __FSTREAM_H
#include <fstream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __OBJECT_H
#include <Object.h>
#endif

// End Interface Dependencies ------------------------------------------------

// Class //

class Polynome : public Object
{
public:
//member functions
       //constructor
       Polynome(ifstream&);
       Polynome(int,double,double,double *);

       //destructor
       virtual ~Polynome();

       void writePoly(ofstream&); //write the Polynome to a file

       double evaluate(double); //evaluate the polynome for sp. value
				      //returns the result
       int evalInBounds(double, double *); //returns 0 if the sp. value
				     //is not between the validity bounds
				     //else return 1 and evaluate the polyn.
       int isInBounds(double);       //returns 1 if the sp. value is between
				     //the validity bounds else returns 0
       int deg(); //return the Deg
       double lowerBound(); //return the LowerBound
       double upperBound(); //return the upperBound
       void   setLowerBound(double); //modify the LowerBound
       void   setUpperBound(double); //modify the UpperBound
       void   theCoeff(double *); //return the coefficient in the array
       void   setTheCoeff(double *); //modify the coefficients

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data

protected:
	short int Deg; //degree of the polynome
	double LowerBound; //Lower validity bound
	double UpperBound; //Upper validity bound
	double *TheCoeff;  //pointer to an array of Deg+1 coefficients
};

#endif // ifndef __POLYNOME_H //

