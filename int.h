/*************************************************************************
 *
 *  Copyright (c) 2020 Rajit Manohar
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *
 **************************************************************************
 */
/*
	TODO: 
*/
#define BIGINT_TEST

#ifdef BIGINT_TEST
	#define UNIT_TYPE unsigned char
#else
	#define UNIT_TYPE unsigned long
#endif

#ifndef __COMMON_INT_H__
#define __COMMON_INT_H__

#include "misc.h"
#include <string>

/*
	Fun facts:
	 -2. BIGINT_TEST mode uses unsigned char, so the array up to 8 elements 
				can be compared with unsigned long.
   -1. Negative numbers are two's compliments.
		0. By default all numbers are unsigned and static.
				Use toSigned()/toDynamic() methods to switch sign/dynamic flags.
		1. Result is dynamic only if A is dynamic.
		2. Result is signed only if both variables are signed, otherwise both
				variables are promoted to unsigned. (ALWAYS)
		3. All variables are stored in multiples of unsigned longs in the current OS(!!!).
		4.a static and unsigned ==> zero extended and width const
		4.b dynamic and unsigned ==> zero extended and width varies
		4.c static and signed ==> sign extended and width remain the same
		4.d dymanic and signed ==> sign extended and width varies
				*** Sign extension assumes padding the sign bit if the var
						size is not multiple of UNIT_TYPE bitwidth ***
		5. Multiplication result is always dynamic
		6. Logic operation sign extend B if it is signed and shorter than A. 
				Use toUnsigned() method to clear sign extension and convert B to unsigned.
				Result inherits issigned value from A.
		7. Comparing signed and unsigned numbers promotes both to unsigned.
		8. Left shift extends dynamic numbers.
*/

#define BIGINT_BITS_ONE (8*sizeof (UNIT_TYPE))

class BigInt {
 public:
  BigInt ();			// default int is 1-bit wide, unsigned and dynamic
  
  BigInt (int w, int s, int d);	
  
  ~BigInt ();

  BigInt (BigInt &);		// copy constructor
  BigInt (BigInt &&);		// move constructor

  BigInt& operator=(BigInt &);         		// copy assignment
  BigInt& operator=(BigInt &&);	       		// move assignment
  BigInt& operator=(UNIT_TYPE &);  		// assignment
	BigInt& operator=(const std::string &);	// assignment (must start with 0x)
	void SetV (int, UNIT_TYPE);
    
  int operator<(BigInt &);	
  int operator<=(BigInt &); 
  int operator>(BigInt &);	
  int operator>=(BigInt &);	
  int operator==(BigInt &);	
  int operator!=(BigInt &);	
	
#ifdef BIGINT_TEST
	int operator==(unsigned long);
	int operator==(long);
	int operator!=(unsigned long);
	int operator!=(long);
#endif

  BigInt &operator+(BigInt &);	
  BigInt &operator-(BigInt &);	
  BigInt operator-(); 					
  BigInt operator*(BigInt &);		
  BigInt operator/(BigInt &);		
  BigInt operator%(BigInt &);		

  BigInt &operator&(BigInt &);	
  BigInt &operator|(BigInt &);	
  BigInt &operator^(BigInt &);	
  BigInt &operator~();					

  BigInt &operator<<(UNIT_TYPE x); 	
  BigInt &operator>>(UNIT_TYPE x);	
  BigInt &operator<<(BigInt &b); 	
  BigInt &operator>>(BigInt &b);	

	unsigned int nBit(unsigned long n);	//returns Nth bit

  int isNegative(); //0 - Non-negative, 1 - Negative

	int isSigned();		//return sign flag
	void toSigned();	//set sign flag and sign extend
	void toUnsigned();//reset sign flag and clear sign extension

	int isDynamic();	//return dynamic flag
	void toStatic();	//set dynamic flag
	void toDynamic();	//reset dynamic flag

	void setWidth (unsigned int); //set bitwidth with zero/sign extension
	unsigned int getWidth() { return width; };
	
  std::string sPrint ();	//print in hex to string
  void hPrint (FILE *fp);	//print in hex
	
 private:
  unsigned int issigned:1;	// 1 - signed, 0 - unsigned
  unsigned int isdynamic:1;	// 1 - dynamic(no overflows) 0 - static

  unsigned int width;			// actual bitwidth
  
  int len;			//number of unit type vars needed to store the value
  UNIT_TYPE *v;	// actual bits; 2's complement
								// rep. The number is sign-extended to
								// the maximum width of the rep

  inline int isOneInt() { return len == 1 ? 1 : 0; };

  void signExtend ();

	int isZero();	//number is all zeros
	int isOne();	//number is one

  void expandSpace(int amt); // expand bitwidth b by # of bits
	void squeezeSpace(int amt); // reduce bitwidth by # of bits
  void zeroClear ();

	UNIT_TYPE* getV() { return v; };
};


#endif /* __ACT_INT_H__ */
