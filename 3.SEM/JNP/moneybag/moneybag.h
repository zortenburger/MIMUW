#ifndef MONEYBAG_H
#define MONEYBAG_H

#include <iostream>
#include <sstream>


class Moneybag {
public:
    using coin_number_t = uint64_t;
    static constexpr coin_number_t COIN_NUMBER_MAX = UINT64_MAX;


// constructor
    constexpr Moneybag(const coin_number_t &l, const coin_number_t &s,
                       const coin_number_t &d) :
            livre(l), solidus(s), denier(d) {}

// copy constructor
    constexpr Moneybag(const Moneybag &m) = default;

// getters
    constexpr coin_number_t livre_number() const {
        return livre;
    }

    constexpr coin_number_t solidus_number() const {
        return solidus;
    }

    constexpr coin_number_t denier_number() const {
        return denier;
    }


// operator !=
    constexpr bool operator!=(const Moneybag &m) const {
        return !(*this == m);
    }

// operator =
    constexpr Moneybag &operator=(const Moneybag &m) {
        if (this != &m) {
            livre = m.livre;
            solidus = m.solidus;
            denier = m.denier;
        }
        return *this;
    }

// operator ==
    constexpr bool operator==(const Moneybag &m) const {
        return (livre == m.livre && solidus == m.solidus && denier == m.denier);
    }


// operator <=>
    constexpr std::partial_ordering operator<=>(const Moneybag &m) const {
        if (*this == m) {
            return std::partial_ordering::equivalent;
        }
        if (livre <= m.livre && solidus <= m.solidus && denier <= m.denier) {
            return std::partial_ordering::less;
        }
        if (livre >= m.livre && solidus >= m.solidus && denier >= m.denier) {
            return std::partial_ordering::greater;
        }
        return std::partial_ordering::unordered;
    }

// operator +=
    constexpr Moneybag &operator+=(const Moneybag &m) {
        if (denier < COIN_NUMBER_MAX - m.denier &&
            solidus < COIN_NUMBER_MAX - m.solidus &&
            livre < COIN_NUMBER_MAX - m.livre) {
            livre += m.livre;
            solidus += m.solidus;
            denier += m.denier;
            return *this;
        }
        else {
            throw std::out_of_range("Addition result is out of range");
        }
    }

// operator +
    constexpr Moneybag operator+(const Moneybag &m) const {
        Moneybag result = *this;
        result += m;
        return result;
    }

// operator -=
    constexpr Moneybag &operator-=(const Moneybag &m) {
        if (*this >= m) {
            livre -= m.livre;
            solidus -= m.solidus;
            denier -= m.denier;
            return *this;
        }
        else {
            throw std::out_of_range("Subtraction result is out of range");
        }
    }

// operator -
    constexpr Moneybag operator-(const Moneybag &m) const {
        Moneybag result = *this;
        result -= m;
        return result;
    }

// operator *=
    constexpr Moneybag &operator*=(const uint64_t &m) {
        if (m == 0 || std::max(std::max(livre, solidus), denier)
                      <= COIN_NUMBER_MAX / m) {
            livre *= m;
            solidus *= m;
            denier *= m;
            return *this;
        }
        else {
            throw std::out_of_range("Multiplication result is out of range");
        }
    }

// operator *uint_64
    constexpr Moneybag operator*(const uint64_t &m) const {
        Moneybag result = *this;
        result *= m;
        return result;
    }

// cast to bool
    constexpr explicit operator bool() const {
        return (livre > 0 || solidus > 0 || denier > 0);
    }


private:
    coin_number_t livre, solidus, denier;
};

//output
static std::ostream &operator<<(std::ostream &os, const Moneybag &m) {
    if (m.livre_number() == 1) {
        os << "(" << m.livre_number() << " livr, ";
    }
    else {
        os << "(" << m.livre_number() << " livres, ";
    }
    if (m.solidus_number() == 1) {
        os << m.solidus_number() << " solidus, ";
    }
    else {
        os << m.solidus_number() << " soliduses, ";
    }

    if (m.denier_number() == 1) {
        os << m.denier_number() << " denier)";
    }
    else {
        os << m.denier_number() << " deniers)";
    }
    return os;
}

// operator uint64_t*
static Moneybag operator*(const uint64_t &i, const Moneybag &n) {
    Moneybag result = n;
    result *= i;
    return result;
}

static const Moneybag Denier = Moneybag(0, 0, 1);
static const Moneybag Livre = Moneybag(1, 0, 0);
static const Moneybag Solidus = Moneybag(0, 1, 0);

class Value {
public:

// constructor with number of deniers
    constexpr explicit Value(const uint64_t &d) : deniers(d) {}

// constructor with MoneyBag
    constexpr explicit Value(const Moneybag &m) : deniers(
            __uint128_t(m.denier_number())
            + __uint128_t(m.solidus_number()) * __uint128_t(12)
            + __uint128_t(m.livre_number()) * __uint128_t(240)) {}

// empty constructor
    constexpr Value() : deniers(0) {}

// copy constructor
    constexpr Value(const Value &v) = default;

// operator =
    constexpr Value &operator=(const Value &v) {
        if (this != &v) {
            this->deniers = v.deniers;
        }
        return *this;
    }

// operator ==
    constexpr bool operator==(const Value &v) const {
        return (deniers == v.deniers);
    }

// operator == uint64
    constexpr bool operator==(const uint64_t &v) const {
        return (deniers == v);
    }

// operator <=>
    constexpr std::strong_ordering operator<=>(const Value &v) const {
        if (*this == v) {
            return std::strong_ordering::equal;
        }
        if (deniers < v.deniers) {
            return std::strong_ordering::less;
        }
        else {
            return std::strong_ordering::greater;
        }
    }

// operator <=>
    constexpr std::strong_ordering operator<=>(const uint64_t &v) const {
        if (deniers == v) {
            return std::strong_ordering::equal;
        }
        if (deniers < v) {
            return std::strong_ordering::less;
        }
        else {
            return std::strong_ordering::greater;
        }
    }

// string
    explicit operator std::string() const {
        if (deniers == 0) {
            return "0";
        }
        __uint128_t pom = deniers;
        std::string s;
        while (pom > 0) {
            s += char('0' + (pom % 10));
            pom /= 10;
        }
        std::reverse(s.begin(), s.end());
        return s;
    }

private:
    __uint128_t deniers;
};

#endif // MONEYBAG_H