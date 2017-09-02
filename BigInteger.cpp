/*
 *
 */

#include "BigInteger.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::string;


/*
 * ___________________________________________________________________________
 */
BigInteger::BigInteger() {
}


/*
 * ___________________________________________________________________________
 */
BigInteger::BigInteger(const BigInteger& big)
		: limbs_(big.getLimbs()) {
}


/*
 * ___________________________________________________________________________
 */
BigInteger::BigInteger(unsigned int num) {

	limbs_.push_back(num);
}


/*
 * ___________________________________________________________________________
 */
BigInteger::BigInteger(const string& str) {

	this->setFromString(str);
}


/*
 * ___________________________________________________________________________
 */
size_t BigInteger::getBitLength() const {

	size_t n = limbs_.size() - 1;
	while (limbs_[n] == 0 && n > 0) {
		--n;
	}

	size_t i = 0;
	if (limbs_[n] > 0) {
		i = limbBitSize;
		limb_t mask = msbMask;
		while (((limbs_[n] & mask) == 0) && i > 0) {
			--i;
			mask >>= 1;
		}
	}

	return n * limbBitSize + i;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::reduce() {

	/* remove all leading zero limbs */
	while (limbs_.size() > 0 && limbs_.back() == 0) {
		limbs_.pop_back();
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::truncate(size_t bitSize) {

	size_t nLimbs = (bitSize / limbBitSize);
	size_t nBits = bitSize % limbBitSize;
	if (nBits > 0) {
		++nLimbs;
	}
	while (limbs_.size() > nLimbs) {
		limbs_.pop_back();
	}
	if (nBits > 0) {
		limbs_[nLimbs - 1] &= ((limb_t)1 << nBits) - 1;
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::expand(size_t bitSize) {

	size_t nLimbs = (bitSize / limbBitSize);
	size_t nBits = bitSize % limbBitSize;
	if (nBits > 0) {
		++nLimbs;
	}
	while (limbs_.size() < nLimbs) {
		limbs_.push_back(0);
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::setZero() {

	limbs_.clear();
	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::operator=(const BigInteger& big) {

	limbs_.clear();

	size_t n = big.getLimbs().size();
	for (size_t i = 0; i < n; ++i) {
		limbs_.push_back(big.getLimbs()[i]);
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::operator=(unsigned int num) {

	limbs_.clear();
	limbs_.push_back(num);
	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::add(const BigInteger& big, bool carry) {

	size_t n1 = limbs_.size();
	size_t n2 = big.getLimbs().size();
	size_t i = 0;

	while (i < n1 || i < n2 || carry) {

		/* expand this big integer if necessary */
		if (i == n1) {
			limbs_.push_back(0);
			++n1;
		}

		/* get summands (limbs) */
		limb_t summand1 = limbs_[i];
		limb_t summand2 = big.getLimb(i);

		limb_t tmpSum = (carry ? 1 : 0) +
				(summand1 & ~msbMask) + (summand2 & ~msbMask);

		size_t tmpCarry = ((tmpSum & msbMask) > 0) ? 1 : 0;
		tmpCarry += ((summand1 & msbMask) > 0) ? 1 : 0;
		tmpCarry += ((summand2 & msbMask) > 0) ? 1 : 0;

		carry = tmpCarry > 1;
		if ((tmpCarry % 2) == 1) {
			tmpSum |= msbMask;
		} else {
			tmpSum &= ~msbMask;
		}

		limbs_[i] = tmpSum;

		/* move on to the next limb */
		++i;
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::subtract(const BigInteger& big) {

	if (*this >= big) {
		/* subtract by adding two's complement of subtrahend */
		size_t bitLength = this->getBitLength();
		BigInteger subtrahend(big);
		subtrahend.expand(bitLength);
		subtrahend.invert();
		this->add(subtrahend, true);
		this->truncate(bitLength);
	} else {
		this->setZero();
	}

	return this->reduce();
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::multiply(const BigInteger& mul) {

	BigInteger prod;

	size_t i = mul.getBitLength();
	while (i > 0) {
		prod <<= 1;
		if (mul.getBit(i - 1)) {
			prod += *this;
		}
		--i;
	}
	*this = prod;

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::divide(const BigInteger& divisor) {

	BigInteger quotient;
	BigInteger remainder(*this);

	if (*this >= divisor) {
		/* this number will never be smaller than zero */
		size_t n = this->getBitLength() - divisor.getBitLength();

		BigInteger s(divisor);
		s <<= n;

		/* school book division */
		for (n++; n > 0; --n) {
			quotient <<= 1;
			if (remainder >= s) {
				remainder -= s;
				quotient += 1;
			}
			s >>= 1;
		}
	}
	*this = quotient.reduce();

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::modulo(const BigInteger& divisor) {

	BigInteger remainder(*this);

	if (*this >= divisor) {
		/* this number will never be smaller than zero */
		size_t n = this->getBitLength() - divisor.getBitLength();

		BigInteger s(divisor);
		s <<= n;

		/* school book division */
		for (n++; n > 0; --n) {
			if (remainder >= s) {
				remainder -= s;
			}
			s >>= 1;
		}
	}
	*this = remainder.reduce();

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::exp(const BigInteger& exponent) {

	BigInteger power(1);

	size_t i = exponent.getBitLength();
	while (i > 0) {
		power.square();
		if (exponent.getBit(i - 1)) {
			power *= *this;
		}
		--i;
	}
	*this = power;

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::expmod(const BigInteger& big,
		const BigInteger& modulus) {

	BigInteger power(1);

	size_t i = big.getBitLength();
	while (i > 0) {
		power.square();
		power.modulo(modulus);
		if (big.getBit(i - 1)) {
			power *= *this;
			power.modulo(modulus);
		}
		--i;
	}
	*this = power.reduce();

	return *this;
}


/*
 * ___________________________________________________________________________
 */
bool BigInteger::invmod(const BigInteger& modulus) {

	BigInteger t;
	BigInteger r(modulus);
	BigInteger tNew(1);
	BigInteger rNew(*this);
	BigInteger q;
	BigInteger tmp;

	while (rNew != 0) {
		q = r / rNew;
		if (t >= q * tNew) {
			tmp = t - q * tNew;
		} else {
			cout << "Warning: t < q * tNew" << endl;
		}
		t = tNew;
		tNew = tmp;
		if (r >= q * rNew) {
			tmp = r - q * rNew;
		} else {
			cout << "Warning: r < q * rNew" << endl;
		}
		r = rNew;
		rNew = tmp;
	}

	bool success = false;
	if (r == 1) {
		*this = t.reduce();
		success = true;
	}
	return success;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::operator<<=(size_t bits) {

	/* shift by full digits */
	for (; bits >= limbBitSize; bits -= limbBitSize) {
		limbs_.insert(limbs_.begin(), 0);
	}

	limb_t carryOut;
	limb_t carryIn = 0;
	limb_t carrySetMask = ((limb_t)1 << bits) - 1;
	limb_t carryGetMask = carrySetMask << (limbBitSize - bits);

	/* shift by sub-limbs */
	size_t n = limbs_.size();
	for (size_t i = 0; i < n || carryIn != 0; ++i) {
		if (i < n) {
			carryOut = (limbs_[i] & carryGetMask) >> (limbBitSize - bits);
			limbs_[i] <<= bits;
			limbs_[i] |= carryIn & carrySetMask;
		} else {
			carryOut = 0;
			limbs_.push_back(carryIn & carrySetMask);
		}
		carryIn = carryOut;
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
BigInteger& BigInteger::operator>>=(size_t bits) {

	/* shift by full digits */
	for (; bits >= limbBitSize; bits -= limbBitSize) {
		limbs_.erase(limbs_.begin());
	}

	limb_t carryOut;
	limb_t carryIn = 0;
	limb_t carryGetMask = ((limb_t)1 << bits) - 1;
	limb_t carrySetMask = carryGetMask << (limbBitSize - bits);

	/* shift by sub-limbs */
	size_t n = limbs_.size();
	for (size_t i = n; i > 0; --i) {
		carryOut = (limbs_[i - 1] & carryGetMask) << (limbBitSize - bits);
		limbs_[i - 1] >>= bits;
		limbs_[i - 1] |= carryIn & carrySetMask;
		carryIn = carryOut;
	}

	return *this;
}


/*
 * ___________________________________________________________________________
 */
int BigInteger::compareTo(const BigInteger& big) const {

	size_t size1 = limbs_.size();
	size_t size2 = big.getLimbs().size();
	size_t size = size1 > size2 ? size1: size2;

	int compare = 0;

	for (size_t i = size; i > 0 && compare == 0; --i) {
		if (this->getLimb(i - 1) > big.getLimb(i - 1)) {
			compare = 1;
		} else if (this->getLimb(i - 1) < big.getLimb(i - 1)) {
			compare = -1;
		}
	}

	return compare;
}


/*
 * ___________________________________________________________________________
 */
bool BigInteger::getBit(size_t iBit) const {

	bool bit = false;

	size_t iLimb = iBit / limbBitSize;
	if (iLimb < limbs_.size()) {
		bit = (((limb_t)1 << (iBit % limbBitSize)) & limbs_[iLimb]) > 0;
	}

	return bit;
}


/*
 * ___________________________________________________________________________
 */
void BigInteger::setBit(size_t iBit, bool value) {

	size_t iLimb = iBit / limbBitSize;
	while (value && iLimb >= limbs_.size()) {
		limbs_.push_back(0);
	}

	if (value) {
		limbs_[iLimb] |= (limb_t)1 << (iBit % limbBitSize);
	} else {
		limbs_[iLimb] &= ~((limb_t)1 << (iBit % limbBitSize));
	}
}


/*
 * ___________________________________________________________________________
 */
bool BigInteger::setFromString(const string& in, size_t base) {

	size_t prefixBase = 0;

    // String indices (current and end)
	size_t i = 0;
	size_t n = in.length();

	/* infer conversion base from string prefix */
	string prefix = in.substr(0, 2);
	if (prefix == "0x" || prefix == "0X") {
		prefixBase = 16;
        i = 2;
	} else if (prefix == "0b" || prefix == "0B") {
		prefixBase = 2;
        i = 2;
	}

	if (base == 0) {
		if (prefixBase > 0) {
			/* use base from string prefix */
			base = prefixBase;
		} else {
			/* use default base 10 (decimal) */
			base = 10;
		}
	} else if (prefixBase > 0 && base != prefixBase) {
		/* conflict of conversion bases */
		return false;
	}

	string digits;
	if (base <= 16) {
		/* Conversion base up to 16: binary, decimal, hexadecimal */
		digits = "0123456789ABCDEF";
	} else if (base == 64) {
		/* Base64 */
		digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz0123456789+/";
	} else {
		/* invalid conversion base */
		return false;
	}

	BigInteger value;

	while (i < n) {

		/* multiply and ... */
		value *= base;

		/* get next character (digit) */
		char c = in[i++];
		if (base <= 16 && c >= 'a' && c <= 'z') {
			c -= 'a' - 'A';
		}

		size_t val = digits.find(c);
		if (val != string::npos && val < base) {
			/* ... add */
			value += val;
		} else {
			/* found invalid character */
    		return false;
		}
	}

    *this = value;

	return true;
}


/*
 * ___________________________________________________________________________
 */
void BigInteger::invert() {

	size_t n = limbs_.size();
	for (size_t i = 0; i < n; ++i) {
		limbs_[i] = ~limbs_[i];
	}
}


/*
 * ___________________________________________________________________________
 */
void BigInteger::printHexadecimal() const {

	string hex = "0123456789ABCDEF";

	if (limbs_.size() == 0) {
		cout << "0";
    }

	for (size_t i = limbs_.size(); i-- > 0;) {
		limb_t d = limbs_[i];
		for (size_t j = 0; j < limbBitSize; j += 4) {
			size_t v = (d & ((limb_t)0xF << (limbBitSize - 4)))
					>> (limbBitSize - 4);
			d <<= 4;
			cout << hex[v];
		}
	}
}


/*
 * ___________________________________________________________________________
 */
void BigInteger::printDecimal() const {

	size_t n = limbs_.size();
	for (size_t i = 0; i < n; ++i) {
		cout << i << ": " << limbs_[i] << endl;
	}
}


/*
 * ___________________________________________________________________________
 */
BigInteger::~BigInteger() {
}

