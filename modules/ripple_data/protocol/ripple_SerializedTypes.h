#ifndef RIPPLE_SERIALIZEDTYPES_H
#define RIPPLE_SERIALIZEDTYPES_H

// VFALCO TODO fix this restriction on copy assignment.
//
// CAUTION: Do not create a vector (or similar container) of any object derived from
// SerializedType. Use Boost ptr_* containers. The copy assignment operator of
// SerializedType has semantics that will cause contained types to change their names
// when an object is deleted because copy assignment is used to "slide down" the
// remaining types and this will not copy the field name. Changing the copy assignment
// operator to copy the field name breaks the use of copy assignment just to copy values,
// which is used in the transaction engine code.

enum PathFlags
{
	PF_END				= 0x00,		// End of current path & path list.
	PF_BOUNDARY			= 0xFF,		// End of current path & new path follows.

	PF_ACCOUNT			= 0x01,
	PF_OFFER			= 0x02,

	PF_WANTED_CURRENCY	= 0x10,
	PF_WANTED_ISSUER	= 0x20,
	PF_REDEEM			= 0x40,
	PF_ISSUE			= 0x80,
};

// VFALCO TODO make these non static or otherwise clean constants.
static const uint160 u160_zero(0), u160_one(1);
static inline const uint160& get_u160_zero() { return u160_zero; }
static inline const uint160& get_u160_one() { return u160_one; }

// VFALCO TODO replace these with language constructs, gah!
#define CURRENCY_XRP		get_u160_zero()
#define CURRENCY_ONE		get_u160_one()					// Used as a place holder.
#define CURRENCY_BAD		uint160(0x5852500000000000)		// Do not allow XRP as an IOU currency.
#define ACCOUNT_XRP			get_u160_zero()
#define ACCOUNT_ONE			get_u160_one()					// Used as a place holder.

class SerializedType
{
public:
	SerializedType() : fName(&sfGeneric) { ; }
	
    explicit SerializedType (SField::ref n) : fName(&n) { assert(fName); }

    virtual ~SerializedType() { }

	static UPTR_T<SerializedType> deserialize(SField::ref name)
	{ return UPTR_T<SerializedType>(new SerializedType(name)); }

	void setFName(SField::ref n) { fName = &n; assert(fName); }
	SField::ref getFName() const { return *fName; }
	std::string getName() const { return fName->fieldName; }

	virtual SerializedTypeID getSType() const { return STI_NOTPRESENT; }
	UPTR_T<SerializedType> clone() const { return UPTR_T<SerializedType>(duplicate()); }

	virtual std::string getFullText() const;
	virtual std::string getText() const // just the value
	{ return std::string(); }
	virtual Json::Value getJson(int /*options*/) const
	{ return getText(); }

	virtual void add(Serializer& s) const { ; }

	virtual bool isEquivalent(const SerializedType& t) const
	{ assert(getSType() == STI_NOTPRESENT); return t.getSType() == STI_NOTPRESENT; }

	void addFieldID(Serializer& s) const { s.addFieldID(fName->fieldType, fName->fieldValue); }

	SerializedType& operator=(const SerializedType& t);

	bool operator==(const SerializedType& t) const
	{ return (getSType() == t.getSType()) && isEquivalent(t); }
	bool operator!=(const SerializedType& t) const
	{ return (getSType() != t.getSType()) || !isEquivalent(t); }

	virtual bool isDefault() const	{ return true; }

protected:
    // VFALCO TODO make accessors for this
	SField::ptr	fName;

private:
	virtual SerializedType* duplicate() const { return new SerializedType(*fName); }
};

inline SerializedType* new_clone(const SerializedType& s) { return s.clone().release(); }
inline void delete_clone(const SerializedType* s) { boost::checked_delete(s); }
inline std::ostream& operator<<(std::ostream& out, const SerializedType& t) { return out << t.getFullText(); }

class STUInt8 : public SerializedType
{
public:

	STUInt8(unsigned char v = 0) : value(v) { ; }
	STUInt8(SField::ref n, unsigned char v = 0) : SerializedType(n), value(v) { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_UINT8; }
	std::string getText() const;
	Json::Value getJson(int) const;
	void add(Serializer& s) const { s.add8(value); }

	unsigned char getValue() const { return value; }
	void setValue(unsigned char v) { value = v; }

	operator unsigned char() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value == 0; }

private:
	unsigned char value;

	STUInt8* duplicate() const { return new STUInt8(*this); }
	static STUInt8* construct(SerializerIterator&, SField::ref f);
};

class STUInt16 : public SerializedType
{
public:

	STUInt16(uint16 v = 0) : value(v) { ; }
	STUInt16(SField::ref n, uint16 v = 0) : SerializedType(n), value(v) { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_UINT16; }
	std::string getText() const;
	Json::Value getJson(int) const;
	void add(Serializer& s) const { s.add16(value); }

	uint16 getValue() const { return value; }
	void setValue(uint16 v) { value=v; }

	operator uint16() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value == 0; }

private:
	uint16 value;

	STUInt16* duplicate() const { return new STUInt16(*this); }
	static STUInt16* construct(SerializerIterator&, SField::ref name);
};

class STUInt32 : public SerializedType
{
public:

	STUInt32(uint32 v = 0) : value(v) { ; }
	STUInt32(SField::ref n, uint32 v = 0) : SerializedType(n), value(v) { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_UINT32; }
	std::string getText() const;
	Json::Value getJson(int) const;
	void add(Serializer& s) const { s.add32(value); }

	uint32 getValue() const { return value; }
	void setValue(uint32 v) { value=v; }

	operator uint32() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value == 0; }

private:
	uint32 value;

	STUInt32* duplicate() const { return new STUInt32(*this); }
	static STUInt32* construct(SerializerIterator&, SField::ref name);
};

class STUInt64 : public SerializedType
{
public:

	STUInt64(uint64 v = 0) : value(v) { ; }
	STUInt64(SField::ref n, uint64 v = 0) : SerializedType(n), value(v) { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_UINT64; }
	std::string getText() const;
	Json::Value getJson(int) const;
	void add(Serializer& s) const { s.add64(value); }

	uint64 getValue() const { return value; }
	void setValue(uint64 v) { value=v; }

	operator uint64() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value == 0; }

private:
	uint64 value;

	STUInt64* duplicate() const { return new STUInt64(*this); }
	static STUInt64* construct(SerializerIterator&, SField::ref name);
};

// Internal form:
// 1: If amount is zero, then value is zero and offset is -100
// 2: Otherwise:
//   legal offset range is -96 to +80 inclusive
//   value range is 10^15 to (10^16 - 1) inclusive
//  amount = value * [10 ^ offset]

// Wire form:
// High 8 bits are (offset+142), legal range is, 80 to 22 inclusive
// Low 56 bits are value, legal range is 10^15 to (10^16 - 1) inclusive
class STAmount : public SerializedType
{
public:
	static const int cMinOffset		= -96, cMaxOffset = 80;
	static const uint64 cMinValue 	= 1000000000000000ull, cMaxValue = 9999999999999999ull;
	static const uint64 cMaxNative 	= 9000000000000000000ull;
	static const uint64 cMaxNativeN	= 100000000000000000ull; // max native value on network
	static const uint64 cNotNative 	= 0x8000000000000000ull;
	static const uint64 cPosNative 	= 0x4000000000000000ull;

	static uint64	uRateOne;

	STAmount(uint64 v = 0, bool isNeg = false) : mValue(v), mOffset(0), mIsNative(true), mIsNegative(isNeg)
	{ if (v == 0) mIsNegative = false; }

	STAmount(SField::ref n, uint64 v = 0, bool isNeg = false)
		: SerializedType(n), mValue(v), mOffset(0), mIsNative(true), mIsNegative(isNeg)
	{ ; }

	STAmount(SField::ref n, int64 v) : SerializedType(n), mOffset(0), mIsNative(true)
	{ set(v); }

	STAmount(const uint160& uCurrencyID, const uint160& uIssuerID,
			uint64 uV = 0, int iOff = 0, bool bNegative = false)
		: mCurrency(uCurrencyID), mIssuer(uIssuerID), mValue(uV), mOffset(iOff), mIsNegative(bNegative)
	{ canonicalize(); }

	STAmount(const uint160& uCurrencyID, const uint160& uIssuerID,
			uint32 uV, int iOff = 0, bool bNegative = false)
		: mCurrency(uCurrencyID), mIssuer(uIssuerID), mValue(uV), mOffset(iOff), mIsNegative(bNegative)
	{ canonicalize(); }

	STAmount(SField::ref n, const uint160& currency, const uint160& issuer,
			uint64 v = 0, int off = 0, bool isNeg = false) :
		SerializedType(n), mCurrency(currency), mIssuer(issuer), mValue(v), mOffset(off), mIsNegative(isNeg)
	{ canonicalize(); }

	STAmount(const uint160& uCurrencyID, const uint160& uIssuerID, int64 v, int iOff = 0)
		: mCurrency(uCurrencyID), mIssuer(uIssuerID), mOffset(iOff)
	{
		set(v);
		canonicalize();
	}

	STAmount(SField::ref n, const uint160& currency, const uint160& issuer, int64 v, int off = 0)
		: SerializedType(n), mCurrency(currency), mIssuer(issuer), mOffset(off)
	{
		set(v);
		canonicalize();
	}

	STAmount(const uint160& uCurrencyID, const uint160& uIssuerID, int v, int iOff = 0)
		: mCurrency(uCurrencyID), mIssuer(uIssuerID), mOffset(iOff)
	{
		set(v);
		canonicalize();
	}

	STAmount(SField::ref n, const uint160& currency, const uint160& issuer, int v, int off = 0)
		: SerializedType(n), mCurrency(currency), mIssuer(issuer), mOffset(off)
	{
		set(v);
		canonicalize();
	}

	STAmount(SField::ref, const Json::Value&);

	static STAmount createFromInt64(SField::ref n, int64 v);

	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	bool bSetJson(const Json::Value& jvSource);

	static STAmount saFromRate(uint64 uRate = 0)
	{ return STAmount(CURRENCY_ONE, ACCOUNT_ONE, uRate, -9, false); }

	SerializedTypeID getSType() const	{ return STI_AMOUNT; }
	std::string getText() const;
	std::string getRaw() const;
	std::string getFullText() const;
	void add(Serializer& s) const;

	int getExponent() const				{ return mOffset; }
	uint64 getMantissa() const			{ return mValue; }

	// When the currency is XRP, the value in raw units. S=signed
	uint64 getNValue() const			{ if (!mIsNative) throw std::runtime_error("not native"); return mValue; }
	void setNValue(uint64 v)			{ if (!mIsNative) throw std::runtime_error("not native"); mValue = v; }
	int64 getSNValue() const;
	void setSNValue(int64);

	std::string getHumanCurrency() const;

	bool isNative() const		{ return mIsNative; }
	bool isZero() const			{ return mValue == 0; }
	bool isNonZero() const		{ return mValue != 0; }
	bool isNegative() const		{ return mIsNegative && !isZero(); }
	bool isPositive() const		{ return !mIsNegative && !isZero(); }
	bool isGEZero() const		{ return !mIsNegative; }
	bool isLegalNet() const		{ return !mIsNative || (mValue < cMaxNativeN); }
	operator bool() const		{ return !isZero(); }

	void negate()				{ if (!isZero()) mIsNegative = !mIsNegative; }
	void zero()					{ mOffset = mIsNative ? 0 : -100; mValue = 0; mIsNegative = false; }

	// Zero while copying currency and issuer.
	void zero(const STAmount& saTmpl)
	{ mCurrency = saTmpl.mCurrency; mIssuer = saTmpl.mIssuer; mIsNative = saTmpl.mIsNative; zero(); }
	void zero(const uint160& uCurrencyID, const uint160& uIssuerID)
	{ mCurrency = uCurrencyID; mIssuer = uIssuerID; mIsNative = !uCurrencyID; zero(); }

	int compare(const STAmount&) const;

	const uint160& getIssuer() const		{ return mIssuer; }
	STAmount* setIssuer(const uint160& uIssuer)	{ mIssuer	= uIssuer; return this; }

	const uint160& getCurrency() const	{ return mCurrency; }
	bool setValue(const std::string& sAmount);
	bool setFullValue(const std::string& sAmount, const std::string& sCurrency = "", const std::string& sIssuer = "");
	void setValue(const STAmount &);

	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return (mValue == 0) && mIssuer.isZero() && mCurrency.isZero(); }

	bool operator==(const STAmount&) const;
	bool operator!=(const STAmount&) const;
	bool operator<(const STAmount&) const;
	bool operator>(const STAmount&) const;
	bool operator<=(const STAmount&) const;
	bool operator>=(const STAmount&) const;
	bool isComparable(const STAmount&) const;
	void throwComparable(const STAmount&) const;

	// native currency only
	bool operator<(uint64) const;
	bool operator>(uint64) const;
	bool operator<=(uint64) const;
	bool operator>=(uint64) const;
	STAmount operator+(uint64) const;
	STAmount operator-(uint64) const;
	STAmount operator-(void) const;

	STAmount& operator+=(const STAmount&);
	STAmount& operator-=(const STAmount&);
	STAmount& operator+=(uint64);
	STAmount& operator-=(uint64);
	STAmount& operator=(uint64);

	operator double() const;

	friend STAmount operator+(const STAmount& v1, const STAmount& v2);
	friend STAmount operator-(const STAmount& v1, const STAmount& v2);

	static STAmount divide(const STAmount& v1, const STAmount& v2, const uint160& uCurrencyID, const uint160& uIssuerID);
	static STAmount divide(const STAmount& v1, const STAmount& v2, const STAmount& saUnit)
		{ return divide(v1, v2, saUnit.getCurrency(), saUnit.getIssuer()); }
	static STAmount divide(const STAmount& v1, const STAmount& v2)
		{ return divide(v1, v2, v1); }

	static STAmount multiply(const STAmount& v1, const STAmount& v2, const uint160& uCurrencyID, const uint160& uIssuerID);
	static STAmount multiply(const STAmount& v1, const STAmount& v2, const STAmount& saUnit)
		{ return multiply(v1, v2, saUnit.getCurrency(), saUnit.getIssuer()); }
	static STAmount multiply(const STAmount& v1, const STAmount& v2)
		{ return multiply(v1, v2, v1); }

	// Add, subtract, multiply, or divide rounding result in specified direction
	static STAmount addRound(const STAmount& v1, const STAmount& v2, bool roundUp);
	static STAmount subRound(const STAmount& v1, const STAmount& v2, bool roundUp);
	static STAmount mulRound(const STAmount& v1, const STAmount& v2,
		const uint160& currency, const uint160& issuer,	bool roundUp);
	static STAmount divRound(const STAmount& v1, const STAmount& v2,
		const uint160& currency, const uint160& issuer, bool roundUp);

	static STAmount mulRound(const STAmount& v1, const STAmount& v2, const STAmount& saUnit, bool roundUp)
		{ return mulRound(v1, v2, saUnit.getCurrency(), saUnit.getIssuer(), roundUp); }
	static STAmount mulRound(const STAmount& v1, const STAmount& v2, bool roundUp)
		{ return mulRound(v1, v2, v1.getCurrency(), v1.getIssuer(), roundUp); }
	static STAmount divRound(const STAmount& v1, const STAmount& v2, const STAmount& saUnit, bool roundUp)
		{ return divRound(v1, v2, saUnit.getCurrency(), saUnit.getIssuer(), roundUp); }
	static STAmount divRound(const STAmount& v1, const STAmount& v2, bool roundUp)
		{ return divRound(v1, v2, v1.getCurrency(), v1.getIssuer(), roundUp); }

	// Someone is offering X for Y, what is the rate?
	// Rate: smaller is better, the taker wants the most out: in/out
	static uint64 getRate(const STAmount& offerOut, const STAmount& offerIn);
	static STAmount setRate(uint64 rate);

	// Someone is offering X for Y, I try to pay Z, how much do I get?
	// And what's left of the offer? And how much do I actually pay?
	static bool applyOffer(
		const bool bSell,
		const uint32 uTakerPaysRate, const uint32 uOfferPaysRate,
		const STAmount& saOfferRate,
		const STAmount& saOfferFunds, const STAmount& saTakerFunds,
		const STAmount& saOfferPays, const STAmount& saOfferGets,
		const STAmount& saTakerPays, const STAmount& saTakerGets,
		STAmount& saTakerPaid, STAmount& saTakerGot,
		STAmount& saTakerIssuerFee, STAmount& saOfferIssuerFee);

	// Someone is offering X for Y, I need Z, how much do I pay
	static STAmount getPay(const STAmount& offerOut, const STAmount& offerIn, const STAmount& needed);

	static std::string createHumanCurrency(const uint160& uCurrency);
	static Json::Value createJsonCurrency(const uint160& uCurrency)
		// XXX Punted.
		{ return createHumanCurrency(uCurrency); }

	static STAmount deserialize(SerializerIterator&);
	static bool currencyFromString(uint160& uDstCurrency, const std::string& sCurrency);
	static bool issuerFromString(uint160& uDstIssuer, const std::string& sIssuer);

	Json::Value getJson(int) const;
	void setJson(Json::Value&) const;

	STAmount getRound() const;
	void roundSelf();

private:
	uint160	mCurrency;		// Compared by ==. Always update mIsNative.
	uint160	mIssuer;		// Not compared by ==. 0 for XRP.

	uint64	mValue;
	int		mOffset;
	bool	mIsNative;		// Always !mCurrency. Native is XRP.
	bool	mIsNegative;

	void canonicalize();
	STAmount* duplicate() const { return new STAmount(*this); }
	static STAmount* construct(SerializerIterator&, SField::ref name);

	STAmount(SField::ref name, const uint160& cur, const uint160& iss, uint64 val, int off, bool isNat, bool isNeg)
		: SerializedType(name), mCurrency(cur), mIssuer(iss),  mValue(val), mOffset(off),
			mIsNative(isNat), mIsNegative(isNeg) { ; }

	void set(int64 v)
	{
		if (v < 0)
		{
			mIsNegative = true;
			mValue = static_cast<uint64>(-v);
		}
		else
		{
			mIsNegative = false;
			mValue = static_cast<uint64>(v);
		}
	}

	void set(int v)
	{
		if (v < 0)
		{
			mIsNegative = true;
			mValue = static_cast<uint64>(-v);
		}
		else
		{
			mIsNegative = false;
			mValue = static_cast<uint64>(v);
		}
	}
};

// VFALCO TODO Make static member accessors for these in STAmount
extern const STAmount saZero;
extern const STAmount saOne;

class STHash128 : public SerializedType
{
public:
	STHash128(const uint128& v) : value(v) { ; }
	STHash128(SField::ref n, const uint128& v) : SerializedType(n), value(v) { ; }
	STHash128(SField::ref n, const char *v) : SerializedType(n) { value.SetHex(v); }
	STHash128(SField::ref n, const std::string &v) : SerializedType(n) { value.SetHex(v); }
	STHash128(SField::ref n) : SerializedType(n) { ; }
	STHash128() { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_HASH128; }
	virtual std::string getText() const;
	void add(Serializer& s) const { s.add128(value); }

	const uint128& getValue() const { return value; }
	void setValue(const uint128& v) { value=v; }

	operator uint128() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value.isZero(); }

private:
	uint128 value;

	STHash128* duplicate() const { return new STHash128(*this); }
	static STHash128* construct(SerializerIterator&, SField::ref name);
};

class STHash160 : public SerializedType
{
public:
	STHash160(const uint160& v) : value(v) { ; }
	STHash160(SField::ref n, const uint160& v) : SerializedType(n), value(v) { ; }
	STHash160(SField::ref n, const char *v) : SerializedType(n) { value.SetHex(v); }
	STHash160(SField::ref n, const std::string &v) : SerializedType(n) { value.SetHex(v); }
	STHash160(SField::ref n) : SerializedType(n) { ; }
	STHash160() { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_HASH160; }
	virtual std::string getText() const;
	void add(Serializer& s) const { s.add160(value); }

	const uint160& getValue() const { return value; }
	void setValue(const uint160& v) { value=v; }

	operator uint160() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value.isZero(); }

private:
	uint160 value;

	STHash160* duplicate() const { return new STHash160(*this); }
	static STHash160* construct(SerializerIterator&, SField::ref name);
};

class STHash256 : public SerializedType
{
public:
	STHash256(const uint256& v) : value(v) { ; }
	STHash256(SField::ref n, const uint256& v) : SerializedType(n), value(v) { ; }
	STHash256(SField::ref n, const char *v) : SerializedType(n) { value.SetHex(v); }
	STHash256(SField::ref n, const std::string &v) : SerializedType(n) { value.SetHex(v); }
	STHash256(SField::ref n) : SerializedType(n) { ; }
	STHash256() { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_HASH256; }
	std::string getText() const;
	void add(Serializer& s) const { s.add256(value); }

	const uint256& getValue() const { return value; }
	void setValue(const uint256& v) { value=v; }

	operator uint256() const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value.isZero(); }

private:
	uint256 value;

	STHash256* duplicate() const { return new STHash256(*this); }
	static STHash256* construct(SerializerIterator&, SField::ref);
};

// variable length byte string
class STVariableLength : public SerializedType
{
public:
	STVariableLength(Blob const& v) : value(v) { ; }
	STVariableLength(SField::ref n, Blob const& v) : SerializedType(n), value(v) { ; }
	STVariableLength(SField::ref n) : SerializedType(n) { ; }
	STVariableLength(SerializerIterator&, SField::ref name = sfGeneric);
	STVariableLength() { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	virtual SerializedTypeID getSType() const { return STI_VL; }
	virtual std::string getText() const;
	void add(Serializer& s) const { s.addVL(value); }

	Blob const& peekValue() const { return value; }
	Blob & peekValue() { return value; }
	Blob getValue() const { return value; }
	void setValue(Blob const& v) { value=v; }

	operator Blob () const { return value; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return value.empty(); }

private:
	Blob value;

	virtual STVariableLength* duplicate() const { return new STVariableLength(*this); }
	static STVariableLength* construct(SerializerIterator&, SField::ref);
};

class STAccount : public STVariableLength
{
public:
	STAccount(Blob const& v) : STVariableLength(v) { ; }
	STAccount(SField::ref n, Blob const& v) : STVariableLength(n, v) { ; }
	STAccount(SField::ref n, const uint160& v);
	STAccount(SField::ref n) : STVariableLength(n) { ; }
	STAccount() { ; }
	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{ return UPTR_T<SerializedType>(construct(sit, name)); }

	SerializedTypeID getSType() const { return STI_ACCOUNT; }
	std::string getText() const;

	RippleAddress getValueNCA() const;
	void setValueNCA(const RippleAddress& nca);

	void setValueH160(const uint160& v);
	bool getValueH160(uint160&) const;
	bool isValueH160() const;

private:
	virtual STAccount* duplicate() const { return new STAccount(*this); }
	static STAccount* construct(SerializerIterator&, SField::ref);
};

class STPathElement
{
private:
    // VFALCO Remove these friend declarations
    friend class STPathSet;
    friend class STPath;
    friend class Pathfinder;

public:
	enum {
		typeEnd			= 0x00,
		typeAccount		= 0x01,	// Rippling through an account (vs taking an offer).
		typeCurrency	= 0x10,	// Currency follows.
		typeIssuer		= 0x20,	// Issuer follows.
		typeBoundary	= 0xFF, // Boundary between alternate paths.
		typeValidBits	= (
			typeAccount
				| typeCurrency
				| typeIssuer
			),	// Bits that may be non-zero.
	};

public:
	STPathElement(const uint160& uAccountID, const uint160& uCurrencyID, const uint160& uIssuerID,
		bool forceCurrency = false)
			: mAccountID(uAccountID), mCurrencyID(uCurrencyID), mIssuerID(uIssuerID)
	{
		mType	=
			(uAccountID.isZero() ? 0 : STPathElement::typeAccount)
			| ((uCurrencyID.isZero() && !forceCurrency) ? 0 : STPathElement::typeCurrency)
			| (uIssuerID.isZero() ? 0 : STPathElement::typeIssuer);
	}

	STPathElement(unsigned int uType, const uint160& uAccountID, const uint160& uCurrencyID, const uint160& uIssuerID)
		: mType(uType), mAccountID(uAccountID), mCurrencyID(uCurrencyID), mIssuerID(uIssuerID)
	{
		;
	}

	int getNodeType() const				{ return mType; }
	bool isOffer() const				{ return mAccountID.isZero(); }
	bool isAccount() const				{ return !isOffer(); }

	// Nodes are either an account ID or a offer prefix. Offer prefixs denote a class of offers.
	const uint160& getAccountID() const	{ return mAccountID; }
	const uint160& getCurrency() const	{ return mCurrencyID; }
	const uint160& getIssuerID() const	{ return mIssuerID; }

	bool operator==(const STPathElement& t) const
	{
		return mType == t.mType && mAccountID == t.mAccountID && mCurrencyID == t.mCurrencyID &&
			mIssuerID == t.mIssuerID;
	}

private:
	unsigned int	mType;
	uint160			mAccountID;
	uint160			mCurrencyID;
	uint160			mIssuerID;
};

class STPath
{
public:
	STPath()		{ ; }
	STPath(const std::vector<STPathElement>& p) : mPath(p) { ; }

	void printDebug();
	int size() const									{ return mPath.size(); }
	bool isEmpty() const								{ return mPath.empty(); }
	const STPathElement& getElement(int offset) const	{ return mPath[offset]; }
	const STPathElement& getElement(int offset)			{ return mPath[offset]; }
	void addElement(const STPathElement &e)				{ mPath.push_back(e); }
	void clear()										{ mPath.clear(); }
	bool hasSeen(const uint160 &uAccountId, const uint160& uCurrencyID, const uint160& uIssuerID);
//	std::string getText() const;
	Json::Value getJson(int) const;

//	uint160 mCurrencyID;
//	uint160 mCurrentAccount; // what account is at the end of the path

	std::vector<STPathElement>::iterator begin()				{ return mPath.begin(); }
	std::vector<STPathElement>::iterator end()					{ return mPath.end(); }
	std::vector<STPathElement>::const_iterator begin() const	{ return mPath.begin(); }
	std::vector<STPathElement>::const_iterator end() const		{ return mPath.end(); }

	bool operator==(const STPath& t) const						{ return mPath == t.mPath; }

	void setCanonical(const STPath& spExpanded);

private:
    friend class STPathSet;
    friend class Pathfinder;

    std::vector<STPathElement> mPath;
};

inline std::vector<STPathElement>::iterator range_begin(STPath & x)
{
	return x.begin();
}

inline std::vector<STPathElement>::iterator range_end(STPath & x)
{
	return x.end();
}

inline std::vector<STPathElement>::const_iterator range_begin(const STPath& x)
{
	return x.begin();
}

inline std::vector<STPathElement>::const_iterator range_end(const STPath& x)
{
	return x.end();
}

namespace boost
{
    template<>
	struct range_mutable_iterator< STPath >
	{
		typedef std::vector<STPathElement>::iterator type;
	};

	template<>
	struct range_const_iterator< STPath >
	{
		typedef std::vector<STPathElement>::const_iterator type;
	};
}

// A set of zero or more payment paths
class STPathSet : public SerializedType
{
public:
	STPathSet () { ; }
	
    explicit STPathSet(SField::ref n) : SerializedType(n) { ; }
	
    explicit STPathSet(const std::vector<STPath>& v) : value(v) { ; }
	
    STPathSet(SField::ref n, const std::vector<STPath>& v) : SerializedType(n), value(v) { ; }
	
    static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
	{
        return UPTR_T<SerializedType>(construct(sit, name));
    }

//	std::string getText() const;
	void add(Serializer& s) const;
	virtual Json::Value getJson(int) const;

	SerializedTypeID getSType() const					{ return STI_PATHSET; }
	int size() const									{ return value.size(); }
	const STPath& getPath(int off) const				{ return value[off]; }
	STPath& peekPath(int off)							{ return value[off]; }
	bool isEmpty() const								{ return value.empty(); }
	void clear()										{ value.clear(); }
	void addPath(const STPath& e)						{ value.push_back(e); }

	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const						{ return value.empty(); }

	void printDebug();

	std::vector<STPath>::iterator begin()				{ return value.begin(); }
	std::vector<STPath>::iterator end()					{ return value.end(); }
	std::vector<STPath>::const_iterator begin() const	{ return value.begin(); }
	std::vector<STPath>::const_iterator end() const		{ return value.end(); }

private:
	std::vector<STPath> value;

	STPathSet* duplicate() const { return new STPathSet(*this); }
	static STPathSet* construct(SerializerIterator&, SField::ref);
};

inline std::vector<STPath>::iterator range_begin(STPathSet & x)
{
	return x.begin();
}

inline std::vector<STPath>::iterator range_end(STPathSet & x)
{
	return x.end();
}

inline std::vector<STPath>::const_iterator range_begin(const STPathSet& x)
{
	return x.begin();
}

inline std::vector<STPath>::const_iterator range_end(const STPathSet& x)
{
	return x.end();
}

namespace boost
{
    template<>
	struct range_mutable_iterator< STPathSet >
	{
		typedef std::vector<STPath>::iterator type;
	};

	template<>
	struct range_const_iterator< STPathSet >
	{
		typedef std::vector<STPath>::const_iterator type;
	};
}

class STVector256 : public SerializedType
{
public:
	STVector256() { ; }
	STVector256(SField::ref n) : SerializedType(n) { ; }
	STVector256(SField::ref n, const std::vector<uint256>& v) : SerializedType(n), mValue(v) { ; }
	STVector256(const std::vector<uint256>& vector) : mValue(vector) { ; }

	SerializedTypeID getSType() const { return STI_VECTOR256; }
	void add(Serializer& s) const;

	static UPTR_T<SerializedType> deserialize(SerializerIterator& sit, SField::ref name)
		{ return UPTR_T<SerializedType>(construct(sit, name)); }

	const std::vector<uint256>& peekValue() const { return mValue; }
	std::vector<uint256>& peekValue() { return mValue; }
	virtual bool isEquivalent(const SerializedType& t) const;
	virtual bool isDefault() const	{ return mValue.empty(); }

	std::vector<uint256> getValue() const			{ return mValue; }
	int size() const								{ return mValue.size(); }
	bool isEmpty() const							{ return mValue.empty(); }

	const uint256& at(int i) const					{ assert((i >= 0) && (i < size())); return mValue.at(i); }
	uint256& at(int i)								{ assert((i >= 0) && (i < size())); return mValue.at(i); }

	void setValue(const STVector256& v)				{ mValue = v.mValue; }
	void setValue(const std::vector<uint256>& v)	{ mValue = v; }
	void addValue(const uint256& v)					{ mValue.push_back(v); }
	bool hasValue(const uint256& v) const;
	void sort()										{ std::sort(mValue.begin(), mValue.end()); }

	Json::Value getJson(int) const;

private:
	std::vector<uint256>	mValue;

	STVector256* duplicate() const { return new STVector256(*this); }
	static STVector256* construct(SerializerIterator&, SField::ref);
};

#endif
// vim:ts=4