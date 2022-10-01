#include <iostream>
#include <fstream>
#include <assert.h>
#include "my_serial.hpp"
#include <sstream>
#include <string>
#include <limits>


using namespace My_Tools;


struct Test_struct {
    bool a;
    uint64_t b;
    bool c;

    template <typename stream>
    void serialize(Serializer<stream> &serializer) {
        serializer.save(a, b, c);
    }

    template <typename stream>
    void load_ser(Deserializer<stream> &deserializer) {
        deserializer.load(a, b, c);
    }
};


struct Test_struct2 {
    bool a;

    template <typename stream>
    void serialize(Serializer<stream> &serializer) {
        serializer.save(a);
    }

    template <typename stream>
    void load_ser(Deserializer<stream> &deserializer) {
        deserializer.load(a);
    }
};


struct Test_struct3 {
    uint64_t a;

    template <typename stream>
    void serialize(Serializer<stream> &serializer) {
        serializer.save(a);
    }

    template <typename stream>
    void load_ser(Deserializer<stream> &deserializer) {
        deserializer.load(a);
    }
};


void is_Data_struct_correct() {
    Data empty;
    Data test {6, 7, true, false, false, 0, false, std::numeric_limits<unsigned long long>::max()};

    assert(test[0].num == 6 && !test[0].is_bool);
    assert(test[1].num == 7 && !test[1].is_bool);
    assert(test[2].num == true && test[2].is_bool);
    assert(test[3].num == false && test[3].is_bool);
    assert(test[4].num == false && test[4].is_bool);
    assert(test[5].num == 0 && !test[5].is_bool);
    assert(test[6].num == false && test[6].is_bool);
    assert(test[7].num == std::numeric_limits<unsigned long long>::max() && !test[7].is_bool);

    std::cout << "====================Struct Data test completed====================" << std::endl;
}


void Serial_direct_save() {
    uint64_t r_int;

    std::string r_bool;
    std::fstream fout("test.txt", std::fstream::out | std::fstream::trunc);

    Serializer<std::fstream> ser(fout);
    ser.save(6, 7, true, false, false, 0, false, std::numeric_limits<unsigned long long>::max(), 22);

    std::fstream fin("test.txt", std::fstream::in);

    fin >> r_int;
    assert(r_int == 6);
    fin >> r_int;
    assert(r_int == 7);
    fin >> r_bool;
    assert(r_bool == "true");
    fin >> r_bool;
    assert(r_bool == "false");
    fin >> r_bool;
    assert(r_bool == "false");
    fin >> r_int;
    assert(r_int == 0);
    fin >> r_bool;
    assert(r_bool == "false");
    fin >> r_int;
    assert(r_int == std::numeric_limits<unsigned long long>::max());
    fin >> r_int;
    assert(r_int == 22);
    
    std::cout << "====================Serial direct save completed====================" << std::endl;
}


void struct_save() {    
    uint64_t r_int;

    std::string r_bool;
    std::fstream fout("test.txt", std::fstream::out | std::fstream::trunc);

    Serializer<std::fstream> ser(fout);
    
    Test_struct m{true, 322332, false};
    m.serialize(ser);

    std::fstream fin("test.txt", std::fstream::in);


    fin >> r_bool;
    assert(r_bool == "true");
    fin >> r_int;
    assert(r_int == 322332);
    fin >> r_bool;
    assert(r_bool == "false");
    
    std::cout << "====================Struct save completed====================" << std::endl;
}


void Serial_direct_load() {
    Test_struct a;

    std::fstream fin("test.txt", std::fstream::in);
    Deserializer<std::fstream> deser(fin);

    std::fstream fout("test.txt", std::fstream::out | std::fstream::trunc);
    Serializer<std::fstream> ser(fout);
    ser.save(false, 5, true);

    a.load_ser(deser);

    assert(a.a == false);
    assert(a.b == 5);
    assert(a.c == true);

    std::cout << "====================Serial load completed====================" << std::endl;
}


void Sstream_test() {    
    std::string a;
    std::stringstream inout(a);

    Test_struct m{false, 1290, false};

    Serializer<std::stringstream> ser(inout);
    m.serialize(ser);

    assert(inout.str() == "false 1290 false ");

    m.a = m.b = m.c = 1;

    Deserializer<std::stringstream> deser(inout);
    m.load_ser(deser);

    assert(m.a == false);
    assert(m.b == 1290);
    assert(m.c == false);

    std::cout << "====================sstream completed====================" << std::endl;
}


void single_var_struct_test() {
    std::string a, b("true"), c("221");
    std::stringstream inout(a);

    Test_struct2 m1{false};                         //Struct with single bool variable inside
    Test_struct3 m2{1222};                          //Struct with single uint64_t var inside

    Serializer<std::stringstream> ser(inout);       
    
    m1.serialize(ser);
    assert(inout.str() == "false ");

    m2.serialize(ser);
    assert(inout.str() == "false 1222 ");

    std::stringstream out1(b);
    std::stringstream out2(c);

    Deserializer<std::stringstream> deser1(out1);
    Deserializer<std::stringstream> deser2(out2);

    m1.load_ser(deser1);
    m2.load_ser(deser2);

    assert(m1.a == true);
    assert(m2.a == 221);

    std::cout << "====================Single var struct completed====================" << std::endl;
}


void incorrect_types_test1() {
    std::string a1("false"), a2("555"), a3("trick"), a4("42trick"), a5("trick"), a6("42trick");
    
    std::stringstream inout1(a1);
    std::stringstream inout2(a2);
    std::stringstream inout3(a3);
    std::stringstream inout4(a4);
    std::stringstream inout5(a5);
    std::stringstream inout6(a6);

    Deserializer<std::stringstream> deser1(inout1);
    Deserializer<std::stringstream> deser2(inout2);
    Deserializer<std::stringstream> deser3(inout3);
    Deserializer<std::stringstream> deser4(inout4);
    Deserializer<std::stringstream> deser5(inout5);
    Deserializer<std::stringstream> deser6(inout6);

    Test_struct2 m1{false};
    Test_struct3 m2{44};

    try {
        m2.load_ser(deser1);
        assert(false);
    } catch (Wrong_Deserializer_Type &a) {}
    try {
        m1.load_ser(deser2);
        assert(false);
    } catch (Wrong_Deserializer_Type &a) {}
    try {
        m1.load_ser(deser3);
        assert(false);
    } catch (Wrong_Deserializer_Type &a) {}
    try {
        m1.load_ser(deser4);
        assert(false);
    } catch (Wrong_Deserializer_Type &a) {}
    
    try {
        m2.load_ser(deser5);
        assert(false);
    } catch (std::exception &a) {}

    m2.load_ser(deser6);

    std::cout << "====================Incorrect tests1 completed====================" << std::endl;
}


int main(void) {
    is_Data_struct_correct();   
    Serial_direct_save();
    struct_save();
    Serial_direct_load();
    Sstream_test();
    incorrect_types_test1();
 //   incorrect_types_test2();
    return 0;
}