#ifndef __BigInteger_H__
#define __BigInteger_H__

#include <vector>
#include <string>

/* ==========================================================================
 *
 *
 *
 * ========================================================================== */

class BigInteger {

protected:

	// TODO: Add description
	typedef unsigned long limb_t;


private:

	// TODO: Add description
	static const size_t limbBitSize = sizeof(limb_t) * 8;

	// TODO: Add description
	static const limb_t msbMask = (limb_t)1 << (limbBitSize - 1);


	// TODO: Add description
	std::vector<limb_t> limbs_;


	// TODO: Add description
	inline const std::vector<limb_t>& getLimbs() const {

		return limbs_;
	}

	// TODO: Add description
	inline limb_t getLimb(size_t i) const {

		return (i < limbs_.size()) ? limbs_[i] : 0;
	}


	// TODO: Add description
	void invert();

	// TODO: Add description
	BigInteger& reduce();

	// TODO: Add description
	BigInteger& expand(size_t bitSize);


	// TODO: Add description
	BigInteger& add(const BigInteger& big, bool carry);

	// TODO: Add description
	const BigInteger& divide(const BigInteger& divisor,
			BigInteger* quotient, BigInteger* remainder) const;


public:

	// TODO: Add description
	BigInteger();

	// TODO: Add description
	BigInteger(const BigInteger& big);

	// TODO: Add description
	BigInteger(unsigned int num);

	// TODO: Add description
	BigInteger(const std::string& str);


	// TODO: Add description
	size_t getBitLength() const;


	// TODO: Add description
	BigInteger& setZero();

	// TODO: Add description
	BigInteger& truncate(size_t bitSize);


	// TODO: Add description
	BigInteger& operator=(const BigInteger& big);

	// TODO: Add description
	BigInteger& operator=(unsigned int num);

	// TODO: Add description
	inline BigInteger& operator=(const std::string& str) {

		this->setFromString(str, 0);
		return *this;
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	inline BigInteger& add(const BigInteger& big) {

		return this->add(big, false);
	}

	// TODO: Add description
	inline BigInteger& operator+=(const BigInteger& big) {

		return this->add(big, false);
	}

	// TODO: Add description
	inline BigInteger& operator+=(unsigned int num) {

		return this->add(BigInteger(num), false);
	}

	// TODO: Add description
	inline BigInteger& operator+=(const std::string& str) {

		return this->add(BigInteger(str), false);
	}

	// TODO: Add description
	inline BigInteger operator+(const BigInteger& big) const {

		return BigInteger(*this).add(big, false);
	}

	// TODO: Add description
	inline BigInteger operator+(unsigned int num) const {

		return BigInteger(*this).add(BigInteger(num), false);
	}

	// TODO: Add description
	inline BigInteger operator+(const std::string& str) const {

		return BigInteger(*this).add(BigInteger(str), false);
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& subtract(const BigInteger& big);

	// TODO: Add description
	inline BigInteger& operator-=(const BigInteger& big) {

		return this->subtract(big);
	}

	// TODO: Add description
	inline BigInteger& operator-=(unsigned int num) {

		return this->subtract(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger& operator-=(const std::string& str) {

		return this->subtract(BigInteger(str));
	}

	// TODO: Add description
	inline BigInteger operator-(const BigInteger& big) const {

		return BigInteger(*this).subtract(big);
	}

	// TODO: Add description
	inline BigInteger operator-(unsigned int num) const {

		return BigInteger(*this).subtract(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger operator-(const std::string& str) const {

		return BigInteger(*this).subtract(BigInteger(str));
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& multiply(const BigInteger& big);

	// TODO: Add description
	inline BigInteger& operator*=(const BigInteger& big) {

		return this->multiply(big);
	}

	// TODO: Add description
	inline BigInteger& operator*=(unsigned int num) {

		return this->multiply(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger& operator*=(const std::string& str) {

		return this->multiply(BigInteger(str));
	}

	// TODO: Add description
	inline BigInteger operator*(const BigInteger& big) const {

		return BigInteger(*this).multiply(big);
	}

	// TODO: Add description
	inline BigInteger operator*(unsigned int num) const {

		return BigInteger(*this).multiply(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger operator*(const std::string& str) const {

		return BigInteger(*this).multiply(BigInteger(str));
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& divide(const BigInteger& big);

	// TODO: Add description
	inline BigInteger& operator/=(const BigInteger& big) {

		return this->divide(big);
	}

	// TODO: Add description
	inline BigInteger& operator/=(unsigned int num) {

		return this->divide(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger& operator/=(const std::string& str) {

		return this->divide(BigInteger(str));
	}

	// TODO: Add description
	inline BigInteger operator/(const BigInteger& big) const {

		return BigInteger(*this).divide(big);
	}

	// TODO: Add description
	inline BigInteger operator/(unsigned int num) const {

		return BigInteger(*this).divide(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger operator/(const std::string& str) const {

		return BigInteger(*this).divide(BigInteger(str));
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& modulo(const BigInteger& big);

	// TODO: Add description
	inline BigInteger& operator%=(const BigInteger& big) {

		return this->modulo(big);
	}

	// TODO: Add description
	inline BigInteger& operator%=(unsigned int num) {

		return this->modulo(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger& operator%=(const std::string& str) {

		return this->modulo(BigInteger(str));
	}

	// TODO: Add description
	inline BigInteger operator%(const BigInteger& big) const {

		return BigInteger(*this).modulo(big);
	}

	// TODO: Add description
	inline BigInteger operator%(unsigned int num) const {

		return BigInteger(*this).modulo(BigInteger(num));
	}

	// TODO: Add description
	inline BigInteger operator%(const std::string& str) const {

		return BigInteger(*this).modulo(BigInteger(str));
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	inline BigInteger& square() {

		return (*this) *= (*this);
	}

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& exp(const BigInteger& exponent);


	// TODO: Add description
	BigInteger& expmod(const BigInteger& exponent,
			const BigInteger& modulus);

	/* _____________________________________________________________________ */

	// TODO: Add description
	bool invmod(const BigInteger& modulus);

	/* _____________________________________________________________________ */

	// TODO: Add description
	BigInteger& operator<<=(size_t bits);

	// TODO: Add description
	BigInteger& operator>>=(size_t bits);


	// TODO: Add description
	int compareTo(const BigInteger& big) const;

	// TODO: Add description
	inline bool operator==(const BigInteger& big) const {

		return this->compareTo(big) == 0;
	}

	// TODO: Add description
	inline bool operator!=(const BigInteger& big) const {

		return this->compareTo(big) != 0;
	}

	// TODO: Add description
	inline bool operator>(const BigInteger& big) const {

		return this->compareTo(big) == 1;
	}

	// TODO: Add description
	inline bool operator>=(const BigInteger& big) const {

		return this->compareTo(big) >= 0;
	}

	// TODO: Add description
	inline bool operator<(const BigInteger& big) const {

		return this->compareTo(big) == -1;
	}

	// TODO: Add description
	inline bool operator<=(const BigInteger& big) const {

		return this->compareTo(big) <= 0;
	}


	// TODO: Add description
	bool getBit(size_t iBit) const;

	// TODO: Add description
	void setBit(size_t iBit, bool value);


	// TODO: Add description
	bool setFromString(const std::string& in, size_t base = 0);


	// TODO: Add description
	void printDecimal() const;

	// TODO: Add description
	void printHexadecimal() const;


	// TODO: Add description
	virtual ~BigInteger();

};

#endif
