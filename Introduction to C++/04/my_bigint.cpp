#include "my_bigint.hpp"


My_Tools::BigInt::BigInt(int32_t a) {
    big_int = new int32_t[100];
    max_size = 100;
    size = 0;
    int32_t residue;
    if (a < 0) {
        is_neg = true;
        big_int[0] = -(a % My_Tools::MAX_VAL);
        a += big_int[0];
        a = -a;
        ++size;
    } else {
        is_neg = false;
        big_int[0] = a % My_Tools::MAX_VAL;
        a -= big_int[0];
        ++size;
    }
    if (a) {
        a /= My_Tools::MAX_VAL;
        ++size;
        big_int[1] = a;
    }
}


My_Tools::BigInt::BigInt(const std::string &a) {
    max_size = a.size();
    if (!a.size()) {
        big_int = new int32_t[100];
        big_int[0] = 0;
        max_size = 100;
        size = 1;
    } else {
        if (a.size() < 100) {
            big_int = new int32_t[max_size * 4];
            max_size *= 4;
        } else {
            big_int = new int32_t[max_size];;
            max_size *= 2;
        }
        int32_t off = 0;
        
        if (a[0] == '-') {
            is_neg = true;
            off = 1;
            size = (a.size() - 1) / 9;
            if ((a.size() - 1) % 9) {
                size++;
            }
        } else {
            is_neg = false;
            size = a.size() / 9;
            if (a.size() % 9) {
                size++;
            }
        }
        int32_t upper_brd = a.size();
        int32_t cur_int = 0;
        int32_t cnt = 0;
        int32_t a_cnt = 0;
        for (int32_t i = a.size() - 9; i >= off; i -= 9) {
            cur_int = 0;
            for (int32_t j = 0; j < 9; ++j) {
                cur_int = cur_int * 10 + a[i + j] - '0';
            }
            big_int[cnt++] = cur_int;
        }
        cur_int = 0;
        if (off) {
            for (int32_t i = 1; i <= (a.size() - 1) % 9; ++i) {
                cur_int = cur_int * 10 + a[i] - '0';
            }
        } else {
            for (int32_t i = 0; i < a.size() % 9; ++i) {
                cur_int = cur_int * 10 + a[i] - '0';
            }
        }

        big_int[cnt] = cur_int;
    }
}


My_Tools::BigInt::~BigInt() {
    if (big_int != nullptr)
        delete[] big_int;
}


My_Tools::BigInt::BigInt(int32_t size, bool sign) {
    big_int = new int32_t[size];
    max_size = size;
    this->size = size - 1;
    is_neg = sign;
}


My_Tools::BigInt::BigInt(const BigInt &c, const int32_t offset) {
    size = c.size + offset;
    max_size = c.max_size + offset;
    is_neg = c.is_neg;
    big_int = new int32_t[max_size];
    int32_t cnt = 0;
    for (int32_t i = 0; i < size; ++i) {
        if (i < offset) {
            big_int[i] = 0;
        } else {
            big_int[i] = c.big_int[cnt++];
        }
    }
}


My_Tools::BigInt::BigInt(const BigInt &c) {
    max_size = c.max_size;
    is_neg = c.is_neg;
    size = c.size;
    big_int = new int32_t[max_size];

    for (int32_t i = 0; i < size; ++i) {
        big_int[i] = c.big_int[i];
    }
}


My_Tools::BigInt::BigInt(BigInt &&c) {
    big_int = c.big_int;
    size = c.size;
    max_size = c.max_size;
    is_neg = c.is_neg;
    c.big_int = nullptr;
    c.size = c.max_size = c.is_neg = 0;
}


My_Tools::BigInt& My_Tools::BigInt::operator=(const BigInt &c) {
    if (this == &c) {
        return *this;
    }
    if (big_int != nullptr) {
        delete[] big_int;
    }
    size = c.size;
    max_size = c.max_size;
    is_neg = c.is_neg;
    big_int = new int32_t[max_size];
    for (int32_t i = 0; i < size; ++i) {
        big_int[i] = c.big_int[i];
    }
    return *this;
}


My_Tools::BigInt& My_Tools::BigInt::operator=(BigInt &&c) {
    if (this == &c) {
        return *this;
    }
    if (big_int != nullptr) {
        delete[] big_int;
    }
    size = c.size;
    max_size = c.max_size;
    is_neg = c.is_neg;
    big_int = c.big_int;
    c.big_int = nullptr;
    c.size = c.max_size = c.is_neg = 0;
    return *this;
}


int32_t My_Tools::BigInt::Size() const {
    return size;
}


bool My_Tools::BigInt::abs_comp(const BigInt &a) const {
    if (size > a.size) {
        return true;
    } else if (size < a.size) {
        return false;
    } else {
        for (int32_t i = size - 1; i >= 0; --i) {
            if (big_int[i] < a.big_int[i]) {
                return false;
            } else if (big_int[i] > a.big_int[i]) {
                return true;
            }
        }
    }
    return true;
}


std::string transform_num(int32_t a) {
    std::string s(9, '0');
    size_t ind = 8;
    while(a) {
        s[ind--] = a % 10 + '0';
        a /= 10;
    }
    return s;
}


std::string My_Tools::BigInt::to_String() const {
    std::string a;
    if (is_neg) {
        a += '-';
    }
    a += std::to_string(big_int[Size() - 1]);
    for (int32_t i = Size() - 2; i >= 0; i--) {
        a += transform_num(big_int[i]);
    }
    return a;
}


int32_t My_Tools::BigInt::operator[](const int32_t a) const {
    return big_int[a];
}


std::ostream& My_Tools::operator<<(std::ostream &os, const My_Tools::BigInt &a) {
    if (a.is_neg) {
        os << '-';
    }
    os << a[a.Size() - 1];
    for (int32_t i = a.Size() - 2; i >= 0; i--) {
        os << std::setfill('0') << std::setw(9) << a[i];
    }
    return os;
}


My_Tools::BigInt My_Tools::BigInt::operator-() const {
    BigInt c = *this;
    c.is_neg = !c.is_neg;
    return c;
}


void My_Tools::BigInt::drop_leading_zeros() {
    for (int32_t i = size - 1; i > 0; --i) {
        if (big_int[i] == 0) {
            --size;
        } else {
            break;
        }
    }
}


void My_Tools::BigInt::drop_extra_memory() {
    int32_t *tmp = new int32_t[max_size / 2];
    for (int32_t i = 0; i < size; ++i) {
        tmp[i] = big_int[i];
    }
    delete[] big_int;
    big_int = tmp;
}


void My_Tools::BigInt::change_zero_sign() {
    if (!is_neg) {
        return;
    }
    for (int i = size - 1; i >= 0; --i) {
        if (big_int[i]) {
            return;
        }
    }
    is_neg = false;
}


void My_Tools::BigInt::fill_with_zeros() {
    for (int i = 0; i < max_size; ++i) {
        big_int[i] = 0;
    }
}


My_Tools::BigInt My_Tools::BigInt::BigInt_addition(const BigInt &a, const BigInt &b) const {
    BigInt c = BigInt(a.size + 1, a.is_neg);
    int32_t overflow = 0;
    for (int32_t i = 0; i < a.size; ++i) {
        c.big_int[i] = a.big_int[i] + overflow;
        overflow = 0;
        if (b.size > i) {
            c.big_int[i] += b.big_int[i];
            if (c.big_int[i] >= My_Tools::MAX_VAL) {
                overflow = 1;
                c.big_int[i] %= My_Tools::MAX_VAL;
            }
        } else if (c.big_int[i] >= My_Tools::MAX_VAL) {
            c.big_int[i] %= My_Tools::MAX_VAL;
            overflow = 1;
        }
    }
    if (overflow) {
        c.big_int[c.size] = overflow;
        c.size++;
    }
    if (c.max_size > c.size * 3) {
        c.drop_extra_memory();
    }
    c.drop_leading_zeros();
    c.change_zero_sign();
    return c;
}


My_Tools::BigInt My_Tools::BigInt::BigInt_subtraction(const BigInt &a, const BigInt &b) const {
    BigInt c = BigInt(a.size, a.is_neg);
    c.size = a.size;
    int32_t overflow = 0;
    for (int32_t i = 0; i < a.size; ++i) {
        c.big_int[i] = a.big_int[i] + overflow;
        overflow = 0;
        if (b.size > i) {
            c.big_int[i] -= b.big_int[i];
            if (c.big_int[i] < 0) {
                overflow = -1;
                c.big_int[i] = My_Tools::MAX_VAL + c.big_int[i];
            }
        } else if (c.big_int[i] < 0) {
            overflow = -1;
            c.big_int[i] = My_Tools::MAX_VAL + c.big_int[i];
        }
    }
    c.drop_leading_zeros();
    c.change_zero_sign();
    return c;
}


My_Tools::BigInt My_Tools::BigInt::operator+(const BigInt &a) const {
    bool comp = abs_comp(a);
    if (is_neg == a.is_neg) {
        if (comp) {
            return BigInt_addition(*this, a);
        } else {
            return BigInt_addition(a, *this);
        }
    } else {
        if (comp) {
            return BigInt_subtraction(*this, a);
        } else {
            return BigInt_subtraction(a, *this);

        }
    }
}


My_Tools::BigInt My_Tools::BigInt::operator-(const BigInt &a) const {
    bool comp = abs_comp(a);
    if (is_neg == a.is_neg) {
        if (comp) {
            return BigInt_subtraction(*this, a);
        } else {
            return BigInt_subtraction(-a, *this);
        }
    } else {
        if (comp) {
            return BigInt_addition(*this, a);
        } else {
            return BigInt_addition(-a, *this);
        }
    }
}


My_Tools::BigInt My_Tools::BigInt::operator+(const int32_t a) const {
    return *this + BigInt(a);
}


My_Tools::BigInt My_Tools::BigInt::operator-(const int32_t a) const {
    return *this - BigInt(a);
}


My_Tools::BigInt My_Tools::BigInt::int32_mul(const int64_t a, const bool sign) const {
    BigInt c(size + 1, sign);
    c.size = size;
    int64_t mul, overflow = 0;
    for (int32_t i = 0; i < size; ++i) {
        mul = a * big_int[i] + overflow;
        if (mul >= My_Tools::MAX_VAL) {
            c.big_int[i] = mul % My_Tools::MAX_VAL;
            overflow = mul / My_Tools::MAX_VAL;
        } else {
            c.big_int[i] = mul;
            overflow = 0;
        }
    }
    if (overflow) {
        c.big_int[c.size++] = overflow;
    }
    return c;
}


My_Tools::BigInt My_Tools::BigInt::operator*(const BigInt &a) const {
    bool sign_mul = is_neg != a.is_neg;
    BigInt c(a.size * size + 1, sign_mul);
    c.fill_with_zeros();
    int32_t cnt = 0;
    for (int32_t i = 0; i < a.size; ++i) {
        BigInt tmp = BigInt(int32_mul(a[i], sign_mul), i);
        c = c + tmp;
    }
    return c;
}


My_Tools::BigInt My_Tools::BigInt::operator*(const int32_t a) const {
    return *this * BigInt(a);
}


bool My_Tools::BigInt::operator==(const BigInt &a) const {
    if (size != a.size || is_neg != a.is_neg) {
        return false;
    }
    for (int32_t i = 0; i < size; ++i) {
        if (big_int[i] != a.big_int[i]) {
            return false;
        }
    }
    return true;
}


bool My_Tools::BigInt::operator!=(const BigInt &a) const {
    if (size != a.size || is_neg != a.is_neg) {
        return true;
    }
    for (int32_t i = 0; i < size; ++i) {
        if (big_int[i] != a.big_int[i]) {
            return true;
        }
    }
    return false;
}


bool My_Tools::BigInt::operator<=(const BigInt &a) const {
    if (a.is_neg > is_neg) {
        return false;
    } else if (a.is_neg < is_neg) {
        return true;
    }
    if (size > a.size) {
        return is_neg;
    } else if (a.size > size) {
        return !is_neg;
    }
    for (int32_t i = a.size - 1; i >= 0; --i) {
        if ((is_neg && big_int[i] < a.big_int[i]) || (!is_neg && big_int[i] > a.big_int[i]))
            return false;
    }
    return true;
}


bool My_Tools::BigInt::operator<(const BigInt &a) const {
    if (*this == a) {
        return false;
    }
    if (a.is_neg > is_neg) {
        return false;
    } else if (a.is_neg < is_neg) {
        return true;
    }
    if (size > a.size) {
        return is_neg;
    } else if (a.size > size) {
        return !is_neg;
    }
    for (int32_t i = a.size - 1; i >= 0; --i) {
        if ((is_neg && big_int[i] < a.big_int[i]) || (!is_neg && big_int[i] > a.big_int[i]))
            return false;
    }
    return true;
}


bool My_Tools::BigInt::operator>=(const BigInt &a) const {
    if (a.is_neg < is_neg) {
        return false;
    } else if (a.is_neg > is_neg) {
        return true;
    }
    if (size < a.size) {
        return is_neg;
    } else if (a.size < size) {
        return !is_neg;
    }
    for (int32_t i = a.size - 1; i >= 0; --i) {
        if ((!is_neg && big_int[i] < a.big_int[i]) || (is_neg && big_int[i] > a.big_int[i]))
            return false;
    }
    return true;
}


bool My_Tools::BigInt::operator>(const BigInt &a) const {
    if (*this == a) {
        return false;
    }
    if (a.is_neg < is_neg) {
        return false;
    } else if (a.is_neg > is_neg) {
        return true;
    }
    if (size < a.size) {
        return is_neg;
    } else if (a.size < size) {
        return !is_neg;
    }
    for (int32_t i = a.size - 1; i >= 0; --i) {
        if ((!is_neg && big_int[i] < a.big_int[i]) || (is_neg && big_int[i] > a.big_int[i]))
            return false;
    }
    return true;
}


bool My_Tools::BigInt::operator==(const int32_t a) const {
    return *this == BigInt(a);
}


bool My_Tools::BigInt::operator!=(const int32_t a) const {
    return *this != BigInt(a);
}


bool My_Tools::BigInt::operator<=(const int32_t a) const {
    return *this <= BigInt(a);
}


bool My_Tools::BigInt::operator<(const int32_t a) const {
    return *this < BigInt(a);
}


bool My_Tools::BigInt::operator>=(const int32_t a) const {
    return *this >= BigInt(a);
}


bool My_Tools::BigInt::operator>(const int32_t a) const {
    return *this > BigInt(a);
}