#include "big_integer_1_a.h"

////	Constructors for BigInt

BigInt::BigInt()
	: length(0)
{
	binaryNumber = new BigInt::binarySet(BigInt::DEFAULTSIZE, BigInt::coeff::BINARYARRAY);
}

BigInt::BigInt(int _bits)
{
	if (_bits < 0)
		throw "Invalid size";
	binaryNumber = new binarySet(_bits, BigInt::coeff::BITS);
}

BigInt::BigInt(const char * _digits)
	: digitString(_digits), length(strlen(_digits))
{
	binaryNumber = new binarySet((int)length);
	ToBinary();
}

BigInt & BigInt::operator = (const char * _digitString)
{
	BigInt * temp = new BigInt(_digitString);
	digitString = "";
	digitString = _digitString;
	if (temp->binaryNumber->sizeOfArray > binaryNumber->size())
		binaryNumber->GrowBinarySet(temp->binaryNumber->sizeOfArray - binaryNumber->size());
	length = strlen(_digitString);
	memcpy(binaryNumber->binaryBits, temp->binaryNumber->binaryBits, sizeof(unsigned long int) * temp->binaryNumber->sizeOfArray);
	return *this;
}

BigInt & BigInt::operator = (const BigInt & _b)
{
	//delete[] binaryNumber->binaryBits;
	if (!this->binaryNumber)
		binaryNumber = new BigInt::binarySet(BigInt::DEFAULTSIZE, BigInt::coeff::BINARYARRAY);

	if (_b.getSize() > this->getSize())
		binaryNumber->GrowBinarySet(_b.getSize() - this->getSize());
	memcpy(binaryNumber->binaryBits, _b.binaryNumber->binaryBits, sizeof(unsigned long int) * _b.binaryNumber->sizeOfArray);
	ToString();
    return *this;
}

BigInt & BigInt::operator = (BigInt && _b)
{
	
	if (_b.getSize() > this->getSize())
		binaryNumber->GrowBinarySet(_b.getSize() - getSize());
	memmove(binaryNumber->binaryBits, _b.binaryNumber->binaryBits, sizeof(unsigned long int) * _b.binaryNumber->sizeOfArray);
	ToString();
	return *this;
}

BigInt::~BigInt()
{
	//delete[] binaryNumber->binaryBits;
}

////	FUNCTIONS for BigInt

void BigInt::ToBinary()
{
	*binaryNumber += (unsigned long int)digitString[0] - 48;
	if (length > 1)
		for (int i = 0; i < length - 1; i++)
		{
			*binaryNumber *= BigInt::ten;
			*binaryNumber += (unsigned long int)digitString[i + 1] - 48;
		}
}

void BigInt::PrintBitSet()
{

	for (int i = binaryNumber->sizeOfArray - 1; i >= 0; i--)
		std::cout << std::bitset<32>((unsigned long int)binaryNumber->binaryBits[i]) << "\n";
	std::cout << "\n";
}


void BigInt::ToString()
{
	digitString = "";
	binarySet buf(binaryNumber->sizeOfArray, BigInt::coeff::BINARYARRAY);
	int tempBuf;

	memcpy(buf.binaryBits, binaryNumber->binaryBits, sizeof(unsigned long int) * binaryNumber->sizeOfArray);
	int i = 0;
	tempBuf = 0;
	do
	{
		tempBuf = buf % 10;
		digitString.push_back(tempBuf + '0');
		buf /= 10;
		tempBuf = 0;
		i++;
	} 
	while (buf >= 10);

	tempBuf = buf.binaryBits[0];
	digitString.push_back(tempBuf + '0');
	delete[] buf.binaryBits;
	std::reverse(digitString.begin(), digitString.end());
}

unsigned int BigInt::getNumBits(const binarySet & _number)
{
	for (int i = _number.sizeOfArray - 1; i >= 0; --i)
		for (int j = 0; j < 32; j++)
			if ((_number.binaryBits[i] >> (31 - j)) & 1)
				return (i + 1) * 32 - j;
}

bool BigInt::getBit(int _n)
{
	if (_n < 0 || _n > (32 * binaryNumber->sizeOfArray))
		throw "Out of range!";

	//	Использовал функцию из binarySet, чтобы не писать один и тот же код.
	return !binaryNumber->isPositiveResult(*binaryNumber, _n);
}

void BigInt::setBit(int _n, bool _bit)
{
	int topBit = _n % 32 - 1;
	if (topBit < 0)
		topBit = 31;
	int i = _n / 32 - (_n % 32 > 0 ? 0 : 1);
	if (_bit)
		binaryNumber->binaryBits[i] |= 1 << topBit;
	else
		binaryNumber->binaryBits[i] &= ~(1 << topBit);
}

////	OPERATORS for BigInt

BigInt & BigInt::operator + (const BigInt & _y) const
{
	int b = BigInt::getBits(length, _y.length);
	BigInt * sum = new BigInt;
	*binaryNumber + _y.binaryNumber;
	sum->binaryNumber = (*binaryNumber + _y.binaryNumber);

	return *sum;
}

////	Constructor for binary set

BigInt::binarySet::binarySet(const int _length, BigInt::coeff _divider)
{
	sizeOfArray = _length / _divider;
	sizeOfArray += (_length % _divider) > 0 ? 1 : 0;
	binaryBits = new unsigned long int[sizeOfArray];

	for (int i = 0; i < sizeOfArray; i++)
		binaryBits[i] = 0;
}
BigInt::binarySet::binarySet(const int _length)
{
	int s = (_length / 10) + (_length / 339) + 2;
	int dlsd = 10 - ((s - 1) / 3);	// dlsd - decimal less significant digit
	if (_length % 10 < dlsd)
		--s;

	sizeOfArray = s;
	binaryBits = new unsigned long int[sizeOfArray];

	for (int i = 0; i < sizeOfArray; i++)
		binaryBits[i] = 0;
}
BigInt::binarySet::binarySet(const int _length, const int _d)
	:sizeOfArray(_length)
{
	binaryBits = new unsigned long int[_length] {(unsigned long int)_d};
}
BigInt::binarySet::binarySet(binarySet && _bS) 
	: sizeOfArray(_bS.sizeOfArray)
{
	//delete[] binaryBits;
	binaryBits = _bS.binaryBits;
	_bS.binaryBits = nullptr;
}

BigInt::binarySet & BigInt::binarySet::operator = (binarySet && _bS)
{
	// Защита от перемещения на самого себя
	if (&_bS == this)
		return *this;

	int newSize = _bS.sizeOfArray;
	unsigned long int * newBits = _bS.binaryBits;

	_bS.sizeOfArray = sizeOfArray;
	_bS.binaryBits = nullptr;

	sizeOfArray = newSize;
	binaryBits = newBits;
}

BigInt::binarySet BigInt::binarySet::tempForInt(const int _d)
{
	return binarySet(1, _d);
}

void BigInt::binarySet::GrowBinarySet(const int diff)
{
	int nAllocatedNew = sizeOfArray + diff;
	unsigned long int * pNewData = new unsigned long int[nAllocatedNew];
	memset(pNewData, 0, sizeof(unsigned long int) * nAllocatedNew);
	memcpy(pNewData, binaryBits, sizeof(unsigned long int)*sizeOfArray);
	delete[] binaryBits;
	binaryBits = pNewData;
	sizeOfArray = nAllocatedNew;
}

////	Additional operators for binary set of BigInt

BigInt::binarySet BigInt::binarySet::operator * (const binarySet & _D) const
{
	binarySet buf((sizeOfArray > _D.sizeOfArray) ? sizeOfArray : _D.sizeOfArray, BigInt::coeff::BINARYARRAY);
	memcpy(buf.binaryBits, this->binaryBits, sizeof(unsigned long int) * sizeOfArray);
	binarySet _resultBuf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	
	for (int i = 0, shift = 0; i < sizeof(unsigned long int) * 8 * _D.sizeOfArray; i++)
	{
		if ((_D >> i) & 1)
		{
			leftShift(&buf, (i - shift));
			shift = i;
			_resultBuf += buf;
		}
	}
	delete[] buf.binaryBits;
	return _resultBuf;
}

int BigInt::binarySet::operator >> (int _amount) const
{
	binarySet temp(sizeOfArray, BigInt::coeff::BINARYARRAY);
	memcpy(temp.binaryBits, this->binaryBits, sizeof(unsigned long int) * sizeOfArray);
	int i = 0;
	while (_amount >= 32)
	{
		++i;
		_amount -= 32;
	}
	temp.binaryBits[i] >>= _amount;
	return temp.binaryBits[i];
}

////	Operators and functions for conversion

BigInt::binarySet & BigInt::binarySet::operator *= (const int _N)
{
	*this = *this * _N;
	return *this;
}
BigInt::binarySet BigInt::binarySet::operator * (const int _N) const
{
	binarySet buf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	binarySet resbuf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	memcpy(buf.binaryBits, this->binaryBits, sizeof(unsigned long int) * sizeOfArray);
	for (int i = 0, shift = 0; i < sizeof(_N)*8; i++)
	{
		if (_N & (1 << i))
		{
			leftShift(&buf, i - shift);
			shift = i;
			resbuf += buf;
		}
	}
	delete[] buf.binaryBits;
	return resbuf;
}


void BigInt::binarySet::operator += (const int & _nextDigit)
{
	binarySet _y(sizeOfArray, BigInt::coeff::BINARYARRAY);
	_y.binaryBits[0] = (unsigned long int)_nextDigit;
	*this += _y;
}
void BigInt::binarySet::operator += (const binarySet & _d)
{
	*this = (*this + _d);
}
BigInt::binarySet BigInt::binarySet::operator + (const binarySet & _d)
{
	binarySet sum(sizeOfArray, BigInt::coeff::BINARYARRAY);
	binarySet sumBuf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	binarySet carry(sizeOfArray, BigInt::coeff::BINARYARRAY);
	sum = (binarySet &&)(*this ^ _d);
	carry = (binarySet &&)(*this & _d);
	while (carry != 0)
	{
		leftShift(&carry, 1);
		sumBuf = sum;
		sum = (sumBuf ^ carry);
		carry = (sumBuf & carry);
		
	}
	delete[] sumBuf.binaryBits;
	delete[] carry.binaryBits;
	return sum;
}
BigInt::binarySet * BigInt::binarySet::operator + (const binarySet * _d)
{
	binarySet * result = new binarySet(sizeOfArray, BigInt::coeff::BINARYARRAY);
	*result = *this + *_d;
	return result;
}
BigInt::binarySet BigInt::binarySet::operator - (const binarySet & _bS)
{
	binarySet result(sizeOfArray, BigInt::coeff::BINARYARRAY);
	binarySet * addCode = Inverse(_bS, _bS.sizeOfArray * 32);
	*addCode += 1;
	result = *this + *addCode;
	delete[] addCode->binaryBits;
	delete addCode;
	return result;
}

BigInt::binarySet BigInt::binarySet::operator ^ (const binarySet & _xorD)
{
	binarySet _xorBuf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	//	XOR every stage of the array
	for (int i = 0; i < sizeOfArray; i++)
		_xorBuf.binaryBits[i] = (unsigned long int)(this->binaryBits[i] ^ _xorD.binaryBits[i]);
	return _xorBuf;
}
BigInt::binarySet BigInt::binarySet::operator & (const binarySet & _andD)
{
	binarySet _andBuf(sizeOfArray, BigInt::coeff::BINARYARRAY);
	//	AND every stage of the array
	for (int i = 0; i < sizeOfArray; i++)
		_andBuf.binaryBits[i] = (unsigned long int)(this->binaryBits[i] & _andD.binaryBits[i]);
	return _andBuf;
}

void BigInt::binarySet::operator = (const int & _d)
{
	for (int i = 0; i < sizeOfArray; i++)
		this->binaryBits[i] = 0;
	this->binaryBits[0] = (unsigned long int)_d;
}

void BigInt::binarySet::operator = (const binarySet & _number)
{
	for (int i = 0; i < sizeOfArray; i++)
		this->binaryBits[i] = 0;
	for (int i = 0; i < _number.sizeOfArray; i++)
		this->binaryBits[i] = (unsigned long int)_number.binaryBits[i];
	//return *this;
}



int BigInt::getBits(int _length1, int _length2)
{
	int b = (_length1 >= _length2) ?
		(_length1 * 3 + (_length1 / 3 + ((_length1 % 3 > 0) ? 1 : 0)) + 1)
		: (_length2 * 3 + (_length2 / 3 + ((_length2 % 3 > 0) ? 1 : 0)));
	return b;
}

void BigInt::binarySet::leftShift(binarySet * _range, int _shift) const
{
	int j = BigInt::getNumBits(*_range) / 32;
	unsigned int i = _shift / 32;
	if (i > j)
		while (j >= 0)
		{

			_range->binaryBits[i--] = _range->binaryBits[j];
			_range->binaryBits[j--] = 0;
		}
	
	_shift %= 32;
	if (_shift != 0)
		for (int k = _range->sizeOfArray - 1; k >= 0; k--)
		{
			_range->binaryBits[k] <<= _shift;
			if (k > 0)
				_range->binaryBits[k] |= (_range->binaryBits[k - 1] >> (32 - _shift));
		}
}
void BigInt::binarySet::leftDeletion(binarySet * _range, const int _numBits) const
{
	int i = _numBits / 32;
	int topNumBits = _numBits % 32;
	
	for (int j = i, k = topNumBits; j < _range->sizeOfArray; j++, k = 0)
	{
		while (k < 32)
		{
			_range->binaryBits[j] &= ~(1 << k++);
		}
	}
}

BigInt::binarySet * BigInt::binarySet::Inverse(const binarySet & _bS, const int range) const
{
	binarySet * inverseVersion = new binarySet(_bS.sizeOfArray, BigInt::coeff::BINARYARRAY);
	memcpy(inverseVersion->binaryBits, _bS.binaryBits, sizeof(unsigned long int) * _bS.sizeOfArray);
	for (int i = 0, k = 0; k < range;)
	{
		inverseVersion->binaryBits[i] = inverseVersion->binaryBits[i] ^ (1 << (k++ % 32));
		if (k % 32 == 0)
			++i;
	}
	return inverseVersion;
}
bool BigInt::binarySet::isPositiveResult(const binarySet & _bS, const int _numBits) const
{
	int topBits = _numBits % 32 - 1;
	if (topBits < 0)
		topBits = 31;
	int i = _numBits / 32 - (_numBits % 32 > 0 ? 0 : 1);
	if ((_bS.binaryBits[i] >> topBits) & 1)
		return false;
	else
		return true;
}
bool BigInt::binarySet::isNotZero(const binarySet & _bSDividend, int _range, const int _numBits) const
{
	int topBits = _numBits % 32 - 1;
	if (topBits < 0)
		topBits = 31;
	int i = _numBits / 32 - (_numBits % 32 > 0 ? 0 : 1);
	while (_range > 0)
	{
		if ((_bSDividend.binaryBits[i] >> topBits--) & 1)
		{
			return true;
		}
		if (topBits < 0)
		{
			topBits = 31;
			--i;
		}
		--_range;
	}
	return false;
}
bool BigInt::binarySet::isNotMax(const binarySet & _bSDividend, int _range, const int _numBits) const
{
	int topBits = _numBits % 32 - 1;
	if (topBits < 0)
		topBits = 31;
	int i = _numBits / 32 - (_numBits % 32 > 0 ? 0 : 1);
	if (_numBits == 162)
		std::cout << "";
	while (_range > 0)
	{
		if (!((_bSDividend.binaryBits[i] >> topBits--) & 1))
		{
			return true;
		}
		if (topBits < 0)
		{
			topBits = 31;
			--i;
			assert(i >= 0);
		}
		
		--_range;
	}
	return false;
}
BigInt::binarySet & BigInt::binarySet::operator /= (const int _N)
{
	*this = *this / _N;
	return *this;
}
BigInt::binarySet BigInt::binarySet::operator / (const int _N)
{
	return *this / tempForInt(_N);
}
BigInt::binarySet BigInt::binarySet::operator / (const binarySet & _dividerBuf) const
{
	binarySet * _dividendBuf = new binarySet(sizeOfArray + 5, BigInt::coeff::BINARYARRAY);
	memcpy(_dividendBuf->binaryBits, binaryBits, sizeof(unsigned long int) * sizeOfArray);

	binarySet * _tempDividerBuf = new binarySet(sizeOfArray + 5, BigInt::coeff::BINARYARRAY);
	memcpy(_tempDividerBuf->binaryBits, _dividerBuf.binaryBits, sizeof(unsigned long int) * _dividerBuf.sizeOfArray);

	int dividendBits = BigInt::getNumBits(*_dividendBuf);
	int dividerBits = BigInt::getNumBits(*_tempDividerBuf);

	binarySet * addCode = Inverse(*_tempDividerBuf, dividerBits);
	*addCode += 1;

	leftShift(_tempDividerBuf, dividendBits - dividerBits);
	leftShift(addCode, dividendBits - dividerBits);
	binarySet * _resultBuf = new binarySet((dividendBits % 32 > 0 && (dividendBits / 32 + 1) == sizeOfArray)
		? sizeOfArray + 5
		: sizeOfArray + 6, BigInt::coeff::BINARYARRAY);
	binarySet result(sizeOfArray, BigInt::coeff::BINARYARRAY);
	int max = 0;
	bool isPos = true;
	for (int i = 0; i <= dividendBits - dividerBits; i++)
	{
		if (!isPos)
		{
			*_resultBuf = (binarySet &&)(*_dividendBuf + *_tempDividerBuf);
		}
		else
		{
			*_resultBuf = (binarySet &&)(*_dividendBuf + *addCode);
		}

		// If overflow happened - add bit
		if (isPositiveResult(*_resultBuf, dividendBits))
		{
			result.binaryBits[0] |= 1;
			if (!isNotZero(*_resultBuf, dividerBits, dividendBits))
			{
				++dividendBits;
				++dividerBits;
				int topBits = dividendBits % 32 - 1;
				if (topBits < 0)
					topBits = 31;
				int j = (dividendBits - 1) / 32;
				addCode->binaryBits[j] |= (1 << topBits);
			}
			isPos = true;
		}
		else
		{
			if (!isNotMax(*_resultBuf, dividerBits, dividendBits))
			{
				++dividendBits;
				++dividerBits;
				int topBits = dividendBits % 32 - 1;
				if (topBits < 0)
					topBits = 31;
				int j = (dividendBits - 1) / 32;
				addCode->binaryBits[j] |= (1 << topBits);
				
			}
			isPos = false;
		}
		//	Then left shift the bitset to 1
		if (i < (dividendBits - dividerBits))
			leftShift(&result, 1);
		memcpy(_dividendBuf->binaryBits, _resultBuf->binaryBits, sizeof(unsigned long int) * _dividendBuf->sizeOfArray);

		leftShift(_dividendBuf, 1);
		leftDeletion(_dividendBuf, dividendBits);
		*_resultBuf = 0;
	}
	
	delete[] addCode->binaryBits;
	delete addCode;
	delete[] _dividendBuf->binaryBits;
	delete _dividendBuf;
	delete[] _tempDividerBuf->binaryBits;
	delete _tempDividerBuf;
	delete[] _resultBuf->binaryBits;
	delete _resultBuf;
	return result;
}

int BigInt::binarySet::operator % (const int _D)
{
	
	binarySet divisionResult(sizeOfArray, BigInt::coeff::BINARYARRAY);
	divisionResult = (binarySet &&)(*this / _D);
	
	binarySet * buffer = new binarySet(sizeOfArray, BigInt::coeff::BINARYARRAY);
	*buffer = *this - (divisionResult * tempForInt(_D));
	leftDeletion(buffer, BigInt::getNumBits(tempForInt(_D)));	
	int result = buffer->binaryBits[0];
	delete[] buffer->binaryBits;
	delete buffer;
	delete[] divisionResult.binaryBits;
	return result;
}



bool BigInt::binarySet::operator >= (const int _N)
{
	for (int i = sizeOfArray - 1; i >= 0; --i)
	{
		if (this->binaryBits[i] > 0 && i > 0)
			return true;
		else if (i == 0 && this->binaryBits[i] >= _N)
			return true;
	}
	return false;
}
bool BigInt::binarySet::operator != (const int & _cmpD)
{
	int count = 0;
	for (int i = 0; i < sizeOfArray; i++)
		if (binaryBits[i] != _cmpD)
			++count;
	if (count > 0)
		return true;
	else
		return false;
}
