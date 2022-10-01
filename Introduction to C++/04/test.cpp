#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <assert.h>
#include "my_bigint.hpp"


My_Tools::BigInt x1(5), x2(2e9), x3(2147483647), x4(-2147483648);
My_Tools::BigInt x5("1"), x6("-32"), x7("2147483647"), x8("-2147483648");
My_Tools::BigInt x9("1000000000"), x10("-437834678324682363486432862348638684628354328754238754387245387");
My_Tools::BigInt x11("743874329874239848723497248364639436982469823692369469286324624624382649853972287352858");


void end_of_test(const std::string &s, int len_output = 60) {
    std::string output(len_output, '=');
    output.replace((len_output - s.size()) / 2, s.size(), s);
    std::cout << output << std::endl;
}


void Correct_initialization() {

    std::cout << x1 << std::endl;
    std::cout << x2 << std::endl;
    std::cout << x3 << std::endl;
    std::cout << x4 << std::endl;
    std::cout << x5 << std::endl;
    std::cout << x6 << std::endl;
    std::cout << x7 << std::endl;
    std::cout << x8 << std::endl;
    std::cout << x9 << std::endl;
    std::cout << x10 << std::endl;
    std::cout << x11 << std::endl;
    
    assert(x1.to_String() == "5");
    assert(x2.to_String() == "2000000000");
    assert(x3.to_String() == "2147483647");
    assert(x4.to_String() == "-2147483648");
    assert(x5.to_String() == "1");
    assert(x6.to_String() == "-32");
    assert(x7.to_String() == "2147483647");
    assert(x8.to_String() == "-2147483648");
    assert(x9.to_String() == "1000000000");
    assert(x10.to_String() == "-437834678324682363486432862348638684628354328754238754387245387");
    assert(x11.to_String() == "743874329874239848723497248364639436982469823692369469286324624624382649853972287352858");

    end_of_test("Initialization test completed");
}


void Addition1() {
    My_Tools::BigInt a = x1 + 0;
    assert(a.to_String() == "5");
}


void Addition2() {
    My_Tools::BigInt a = x1 + x1;
    assert(a.to_String() == "10");
    a = a + x2;
    assert(a.to_String() == "2000000010");
}


void Addition3() {
    My_Tools::BigInt a = x6 + x1;
    assert(a.to_String() == "-27");
    a = x6 + x7;
    assert(a.to_String() == "2147483615");
}


void Addition4() {
    My_Tools::BigInt a = x6 + x7 + x6 + x6 + x6 + x1 + x8 + x5;
    assert(a.to_String() == "-123");
    a = x5 + -1;
    assert(a.to_String() == "0");
    a = x6 + My_Tools::BigInt("33");
    assert(a.to_String() == "1");

    end_of_test("Simple addition completed");
}


void Big_Seq_Addition() {

    My_Tools::BigInt a = x9 + x10;
    assert(a.to_String() == "-437834678324682363486432862348638684628354328754238753387245387");
    a = a + x11;
    assert(a.to_String() == "743874329874239848723496810529961112300106337259507120647639996270053895615218900107471");
    a = a + 10e3;
    assert(a.to_String() == "743874329874239848723496810529961112300106337259507120647639996270053895615218900117471");
    a = a + x9;
    assert(a.to_String() == "743874329874239848723496810529961112300106337259507120647639996270053895615219900117471");
    a = a + x1 + x2;
    assert(a.to_String() == "743874329874239848723496810529961112300106337259507120647639996270053895615221900117476");

    My_Tools::BigInt b = x10 + x11 + 10e3 + x9 + x2 + x9 + x1;
    assert(b.to_String() == a.to_String());

    end_of_test("Complex addition completed");
}


void Subtraction1() {
    My_Tools::BigInt a = x1 - 0;
    assert(a.to_String() == "5");
}


void Subtraction2() {
    My_Tools::BigInt a = x1 - x1;
    assert(a.to_String() == "0");
    a = a - x2;
    assert(a.to_String() == "-2000000000");
}


void Subtraction3() {
    My_Tools::BigInt a = x6 - x1;
    assert(a.to_String() == "-37");
    a = x6 - x7;
    assert(a.to_String() == "-2147483679");
}


void Subtraction4() {
    My_Tools::BigInt a = x6 - x7 - x6 - x6 - x6 - x1 - x8 - x5;
    assert(a.to_String() == "59");
    a = x5 - -1;
    assert(a.to_String() == "2");
    a = x6 - My_Tools::BigInt("33");
    assert(a.to_String() == "-65");

    end_of_test("Simple subtraction completed");
}


void Big_Seq_Subtraction() {

    My_Tools::BigInt a = x9 - x10;
    assert(a.to_String() == "437834678324682363486432862348638684628354328754238755387245387");
    a = a - x11;
    assert(a.to_String() == "-743874329874239848723496810529961112300106337259507120647639996270053895615216900107471");
    a = a - 10e3;
    assert(a.to_String() == "-743874329874239848723496810529961112300106337259507120647639996270053895615216900117471");
    a = a - x9;
    assert(a.to_String() == "-743874329874239848723496810529961112300106337259507120647639996270053895615217900117471");
    a = a - x1 - x2;
    assert(a.to_String() == "-743874329874239848723496810529961112300106337259507120647639996270053895615219900117476");

    My_Tools::BigInt b = x9 - x10 - x11 - 10e3 - x2 - x9 - x1;
    assert(b.to_String() == a.to_String());

    end_of_test("Complex subtraction completed");
}


void Multiplication1() {
    My_Tools::BigInt a = x1 * 0;
    assert(a.to_String() == "0");
    a = x1 * 1;
    assert(a.to_String() == x1.to_String());
}


void Multiplication2() {
    My_Tools::BigInt a = x1 * x1;
    assert(a.to_String() == "25");
    a = a * x2;
    assert(a.to_String() == "50000000000");
}


void Multiplication3() {
    My_Tools::BigInt a = x6 * x1;
    assert(a.to_String() == "-160");
    a = x6 * x7;
    assert(a.to_String() == "-68719476704");
}


void Multiplication4() {
    My_Tools::BigInt a = x6 * x7 * x6 * x6 * x6 * x1 * x8 * x5;
    assert(a.to_String() == "-24178516381033584425697280");
    a = x5 * -1;
    assert(a.to_String() == "-1");
    a = x6 * My_Tools::BigInt("33");
    assert(a.to_String() == "-1056");

    end_of_test("Simple Multiplication completed");
}


void Big_Seq_Multiplication() {

    My_Tools::BigInt a = x9 * x10;
    assert(a.to_String() == "-437834678324682363486432862348638684628354328754238754387245387000000000");
    a = a * x11;
    assert(a.to_String() == "-3256939779344764602210757351923275122855213196996159436920295360500979891300"
                            "65455247544917836611549416151161815597792424477116311760568162164701766046000000000");
    a = a * 10e3;
    assert(a.to_String() == "-3256939779344764602210757351923275122855213196996159436920295360500979891300"
                            "654552475449178366115494161511618155977924244771163117605681621647017660460000000000000");
    a = a * x9;
    assert(a.to_String() == "-3256939779344764602210757351923275122855213196996159436920295360500979891300"
                            "654552475449178366115494161511618155977924244771163117605681621647017660460000000000000000000000");
    a = a * x1 * x2;
    assert(a.to_String() == "-3256939779344764602210757351923275122855213196996159436920295360500979891300654552475449178366115"
                            "4941615116181559779242447711631176056816216470176604600000000000000000000000000000000");

    My_Tools::BigInt b = x9 * x10 * x11 * 10e3 * x2 * x9 * x1;
    assert(b.to_String() == a.to_String());

    end_of_test("Complex Multiplication completed");
}


void Combined_Operations() {
    My_Tools::BigInt a = x9 * x10 + x11 - x4 - x4 + x8 * x6 + x3;
    assert(a.to_String() == "743874329874239410888818923682275950549607475053684840931995870385628262608660449280537");

    end_of_test("Combined operations");
}


void Max_Val_overflow() {
    My_Tools::BigInt y1(1e9 - 1), y2(1e9), y3(1e9 + 1), y4("1000000000000000000000000000000000000");
    My_Tools::BigInt y5("1000000000000000000000000000000000001"), y6("999999999999999999999999999999999999");
    
    assert(y4 == y6 + 1);
    assert(y5 - 1 == y4);
    assert(y5 - 2 == y6);
    assert(y1 + 1 == y2);
    assert(y3 - 1e9 == 1);
    assert((y2 * 1e9).to_String() == "1000000000000000000");

}


void G_comparison() {
    assert(!(x1 > x2));      //pos < pos
    assert(x3 > x2);         //pos > pos
    assert(x3 > x4);         //pos > neg
    assert(!(x3 > x7));      //equal
    assert(!(x3 > x3));      //equal
    assert(!(x10 > x10));    //equal
    assert(!(x8 > x7));      //big_neg > little_pos
    assert(!(x6 > x7));      //little_neg > big_pos
    assert(x11 + 5 > x11);   //big_pos + 5 > big_pos
    assert(-x11 + 5 > -x11); //big_neg + 5 > big_neg
    assert(x1 > 3);          //bigint > int

    end_of_test("Greater operation completed");
}


void L_comparison() {
    assert(x1 < x2);            //pos < pos
    assert(!(x3 < x2));         //pos > pos
    assert(!(x3 < x4));         //pos > neg
    assert(!(x3 < x7));         //equal
    assert(!(x3 < x3));         //equal
    assert(!(x10 < x10));       //equal
    assert(x8 < x7);            //big_neg < little_pos
    assert(x6 < x7);            //little_neg < big_pos
    assert(x8 < x6);            //big_neg < little_neg
    assert(x11 - 5 < x11);      //big_pos - 5 < big_pos
    assert(!(-x11 + 5 < -x11)); //big_neg + 5 < big_neg
    assert(x1 < 6);             //bigint < int

    end_of_test("Less operation completed");
}


void E_NE_comparison() {
    assert(x1 != x2);
    assert(!(x3 == x2));
    assert(x3 == x3);
    assert(!(x3 != x3));
    assert(x11 == x11);
    assert(x5 == 1);
    assert(x1 == x5 + x5 + x5 + x5 + x5);
    assert(x1 == x5 * 5);
    assert(x9 == 1e9);
    assert(x11 != 392018);

    end_of_test("Equal/Not Equal operation completed");
}


void GE_comparison() {
    assert(!(x1 >= x2));      //pos < pos
    assert(x3 >= x2);         //pos > pos
    assert(x3 >= x4);         //pos > neg
    assert(x3 >= x7);      //equal
    assert(x3 >= x3);      //equal
    assert(x10 >= x10);    //equal
    assert(!(x8 >= x7));      //big_neg > little_pos
    assert(!(x6 >= x7));      //little_neg > big_pos
    assert(x11 + 5 >= x11);   //big_pos + 5 > big_pos
    assert(-x11 + 5 >= -x11); //big_neg + 5 > big_neg
    assert(x1 >= 5);          //bigint > int

    end_of_test("Greater/Equal operation completed");
}


void LE_comparison() {
    assert(x1 <= x2);            //pos < pos
    assert(!(x3 <= x2));         //pos > pos
    assert(!(x3 <= x4));         //pos > neg
    assert(x3 <= x7);            //equal
    assert(x3 <= x3);            //equal
    assert(x10 <= x10);          //equal
    assert(x8 <= x7);            //big_neg < little_pos
    assert(x6 <= x7);            //little_neg < big_pos
    assert(x8 <= x6);            //big_neg < little_neg
    assert(x11 - 5 <= x11);      //big_pos - 5 < big_pos
    assert(!(-x11 + 5 <= -x11)); //big_neg + 5 < big_neg
    assert(x1 <= 5);             //bigint < int

    end_of_test("Less/Equal operation completed");
}


void Unary_minus() {
    assert(-x1 == -5);
    assert(!(-x1 == x1));
    assert(-x2 == My_Tools::BigInt("-2000000000"));
    assert(-x9 == My_Tools::BigInt(0) - x9);
    assert(-x11 - x10 == -x10 - x11);

    end_of_test("Unary Minus completed");
}


void Copy_move_semantic() {

    My_Tools::BigInt m1 = My_Tools::BigInt(22);
    My_Tools::BigInt m2 = My_Tools::BigInt(14);
    My_Tools::BigInt &&m3 = My_Tools::BigInt(33);

    m1 = m2;
    
    assert(m1 == My_Tools::BigInt(14));
    assert(m2 == My_Tools::BigInt(14));

    m3 = std::move(m2);

    assert(m3 == My_Tools::BigInt(14));
    assert(m2.test_nullptr());

    end_of_test("Copy/Move");
}


void Very_Big_operation() {

    My_Tools::BigInt a("999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    My_Tools::BigInt b("123456789012346789012345678901234678901234567890123467890123456789012346789012345678901234678901234567890123467890");
    My_Tools::BigInt c("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

    My_Tools::BigInt result = a + b;
    assert(result.to_String() == "1123456789012346789012345678901234678901234567890123467890123456789012346789012345678901234678901234567890123467889");

    result = result * c;
    assert(result.to_String() == "1248285321124829765569272976556927421001371742100137186544581618654458163098902606309890260754334705075433470519875"
                                 "41838134554183690109739369010973924566529492456652948012208504801220850356776406035677640591233196159123319614679");
    result = result - a * b;
    assert(result.to_String() == "13717431001361875445816187544580631989026063198902507643347050764334695208779149520877913965322359396532235840976652"
                                 "94923566530479122085047912208603467764060346776415902331961590233197145788751714578875270134430727013443082569");
    result = result * c * b * a;
    assert(result.to_String() == "188167776102950888435959420638016486327285515008549097023072817437304963183317016979350810308407983863472421071822794"
                                 "405995769998813796055212651299476630032414671325339286811362906517505189827380533355679743862794576371392995641598886"
                                 "467183409339107188949329113763257300777045840511112240117767728816441726867634939210029074824201162405693247666337973"
                                 "911381326653618780835808098286159141860146571822794405995769998813796055212651299476630032414671325339287752201787032"
                                 "25963200717763654576217549922215141413848075508129589267871637133267963970300044202436494432245490");
    
    end_of_test("Super Big Operation");
}


int main(void) {
    Correct_initialization();

    Addition1();
    Addition2();
    Addition3();
    Addition4();
    Big_Seq_Addition();

    Subtraction1();
    Subtraction2();
    Subtraction3();
    Subtraction4();
    Big_Seq_Subtraction();

    Multiplication1();
    Multiplication2();
    Multiplication3();
    Multiplication4();
    Big_Seq_Multiplication();

    Unary_minus();

    Combined_Operations();

    Max_Val_overflow();

    G_comparison();
    L_comparison();
    E_NE_comparison();
    GE_comparison();
    LE_comparison();

    Copy_move_semantic();

    Very_Big_operation();

    return 0;
}