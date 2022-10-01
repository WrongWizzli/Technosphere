#pragma once

#include <iostream>
#include <string>
#include <iomanip>


namespace My_Tools {

    const int32_t MAX_VAL = 1e9;

    class BigInt {

        private:
        bool is_neg;
        int32_t max_size;
        int32_t *big_int = nullptr;
        int32_t size = 0;
        BigInt(int32_t t, bool sign);
        BigInt(const BigInt &c, const int32_t offset);
        BigInt BigInt_addition(const BigInt &a, const BigInt &b) const;
        BigInt BigInt_subtraction(const BigInt &a, const BigInt &b) const;
        bool abs_comp(const BigInt &a) const;
        void drop_leading_zeros();
        void drop_extra_memory();
        void change_zero_sign();
        void fill_with_zeros();
        BigInt int32_mul(const int64_t a, const bool sign) const;
        int32_t operator[](const int32_t a) const;
        int32_t Size() const;


        public:
        BigInt(int32_t a);
        BigInt(const std::string &a);
        ~BigInt();
        BigInt(const BigInt &c);
        BigInt(BigInt &&c);
        std::string to_String() const;
        BigInt& operator=(const BigInt &c);
        BigInt& operator=(BigInt &&c);

        friend std::ostream& operator<<(std::ostream &os, const BigInt &a);

        BigInt operator-() const;
        BigInt operator+(const int32_t a) const;
        BigInt operator+(const BigInt &a) const;
        BigInt operator-(const int32_t a) const;
        BigInt operator-(const BigInt &a) const;
        BigInt operator*(const int32_t a) const;
        BigInt operator*(const BigInt &a) const;
        bool operator==(const int32_t a) const;
        bool operator!=(const int32_t a) const;
        bool operator<=(const int32_t a) const;
        bool operator<(const int32_t a) const;
        bool operator>=(const int32_t a) const;
        bool operator>(const int32_t a) const;
        bool operator==(const BigInt &a) const;
        bool operator!=(const BigInt &a) const;
        bool operator<=(const BigInt &a) const;
        bool operator<(const BigInt &a) const;
        bool operator>=(const BigInt &a) const;
        bool operator>(const BigInt &a) const;

        bool test_nullptr() const {return big_int == nullptr;}  //Test only fucntion!
    };
    
    
    std::ostream& operator<<(std::ostream &os, const BigInt &a);
}