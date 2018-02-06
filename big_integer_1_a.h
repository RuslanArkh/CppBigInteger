#ifndef _BIG_INTEGER_H_ 
#define _BIG_INTEGER_H_ 

#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <cassert>

class BigInt
{
	enum coeff { BITS = (int)32, BINARYARRAY = (int)1 };

	class binarySet
	{
		friend BigInt;
		friend std::string;

		//	DATA BLOCK
		////////////////////////////////////////////////////////////////////////////////////

		int sizeOfArray;
		unsigned long int * binaryBits;

		/////////////////////////////////////////////////////////////////////////////////////
		//	END OF DATA BLOCK
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//	FUNCTIONS	AND OPERATORS

		//	Constructors
		explicit binarySet(const int _length, BigInt::coeff _divider);
		binarySet(const int);
		binarySet(const int, const int);
		//	Temporar object function for int initialization to binary set
		binarySet tempForInt(const int);
		// Copy Constructor
		//binarySet(binarySet &);
		//	Move constructors
		binarySet(binarySet && _bS);
		//	Destructor
		//~binarySet();
		binarySet & operator = (binarySet && _bS);


		//	Operators for general purposes
		binarySet operator + (const binarySet &);
		binarySet * operator + (const binarySet *);
		binarySet operator - (const binarySet &);
		binarySet operator / (const binarySet &) const;
		binarySet & operator % (const binarySet &) const;
		binarySet operator ^ (const binarySet & _xorD);
		binarySet operator & (const binarySet & _andD);
		int operator >> (int) const;

		void operator = (const binarySet &);

		void operator *= (const binarySet &);
		binarySet operator * (const binarySet &) const;

		//	Overloaded operators and functions for conversion and binary logic
		//	1. To Binary
        binarySet & operator *= (const int);
		binarySet operator * (const int) const;

		void operator += (const int & _nextDigit);
		void operator += (const binarySet & _d);


		bool operator != (const int & _cmpD);

		void operator = (const int & _d);

		void leftShift(binarySet * _range, int) const;

		//	2. To String

		binarySet operator / (const int);
		int operator % (const int);
        binarySet & operator /= (const int);
		bool operator >= (const int);

		bool isPositiveResult(const binarySet &, const int _numBits) const;
		void leftDeletion(binarySet * _range, const int _numBits) const;
		binarySet * Inverse(const binarySet & _bS, const int range) const;
		bool isNotZero(const binarySet &, int, const int) const;
		bool isNotMax(const binarySet &, int, const int) const;

		//	Дополнительные функции
		void GrowBinarySet(const int diff);
		int size() { return sizeOfArray; }
	};
	binarySet * binaryNumber;

	friend binarySet;
	friend std::ostream & operator << (std::ostream & _o, const BigInt & B);
	friend std::ostream & ShowBitSet();

	//	Pointer to working stage
	//	Length of array of string
	int length;
	std::string digitString;
	static const int DEFAULTSIZE = 8;
	const static unsigned short int ten = 10;
	static int getBits(int _length1, int _length2);
	
	void ToBinary();
	void ToString();

	
	
public:
	//	Constructors
	BigInt();
	BigInt(int _bits);
	BigInt(const char * _digits);
	BigInt & operator = (const char *);

	//	Copy constructor
	//BigInt(const BigInt & _b);
	//	Copy assignment
    BigInt & operator = (const BigInt & _b);

	//	Move constructors
	//BigInt(BigInt && _b) = delete;
	BigInt & operator = (BigInt && _b);

	//	Operators for arithmetics for BigInt
	BigInt & operator + (const BigInt & _y) const;

	//	Destructor
	~BigInt();

	void PrintBitSet();

	//friend std::ostream & operator << (std::ostream & _out, const BigInt & _b);
	void PrintBits();

	static unsigned int getNumBits(const binarySet &);

	//	Return bitset
    BigInt::binarySet & getBinarySet() { return *binaryNumber; };

	bool getBit(int);
	void setBit(int, bool);

	//	Length of string
	unsigned int getLength() { return length; };
	//	Add to string
	void StringPush(char c);
	//	Number of floors
	int getSize() const;


};

inline int BigInt::getSize() const
{
	return binaryNumber->size();
}

inline void BigInt::StringPush(char c)
{
	digitString.push_back(c);
}

inline std::ostream & operator << (std::ostream & _o, const BigInt & B)
{
	_o << B.digitString;
	return _o;
}

inline void BigInt::PrintBits()
{
	for (int i = binaryNumber->sizeOfArray - 1; i >= 0; i--)
		std::cout << std::bitset<32>((unsigned long int)binaryNumber->binaryBits[i]) << "\n";
}


#endif
