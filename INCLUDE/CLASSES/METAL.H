#ifndef __METAL_H
#define __METAL_H

// Contents ----------------------------------------------------------------
//
// Metal class
//
// Description
//
// This class defines 3 arrays of Polynome objects to define conductibility,
// density, and heat capacity (?chaleur massique) of a metal at various
// temperaures.
//
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __FSTREAM_H
#include <fstream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __ARRAY_H
#include <Array.h>
#endif

#ifndef __POLYNOME_H
#include "polynome.h"
#endif

// End Interface Dependencies ------------------------------------------------

// Class //

class Metal : public Object
{
public:
//member functions
       //constructor
       Metal(int code,int readInFile = 1);

       //destructor
       virtual ~Metal();

       void writeMetal(); //write all the data to a file
       int  code();  //return the metal code
       void addCond(const Polynome&);  //add Polynome objects to the
       void addDens(const Polynome&);  //arrays
       void addHeat(const Polynome&);
       double evalCond(double); //return the evaluation at specified
       double evalDens(double); //temperature
       double evalHeat(double);
       void   evalAll(double,double *,double *,double *); //evaluate
				//the 3 values with sp. temp., respectively
				// Cond,Dens,Heat

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
protected:
    int Code;         //metal code
    Array Conduct;   //conductivity polynome array
    Array Density;   //density polynome array
    Array HeatCap;   //heat capacity polynome array
};

// Description -------------------------------------------------------------
//
// Constructor Metal(int code,int readInFile = 1)
//
// If readInFile = 1 (default), the data for the Metal object will
// be read into a file named "BISRAxx.DAT" where xx is the code
// number ex) BISRA2.DAT
// If readInFile = 0, no data is read from the file
// Array objects keep empty until a program adds polynomes.
//
//
// first polynome should have its lowerBound equal to 0.
// Each array of Polynome object should have its LowerBound equal to
// previous polynome UpperBound.
// Evaluations are made the following way :
// if temperature is lower than first polynome lower bound,
// first polynome is evaluated with lower bound,
// else if temperature is greater than last polynome upper bound,
// last polynome is evaluated with upper bound,
// else array is iterated until the polynome upper bound is lower than
// the desired temperature, this polynome is then evaluated
// with the desired temperature.
// End ---------------------------------------------------------------------


#endif // ifndef __METAL_H //

