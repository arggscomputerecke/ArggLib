// PipeLines.cpp : Defines the entry point for the console application.
//

#include "ArggLib/proc.hh"
#include "ArggLib/param.hh"
#include "ArggLib/proc_tools.hh"

#include <sstream>
#include "ArggLib/ArggLib_Unit_Tests.hh"
#include "ArggLib/for_loop.hh"
#include "ArggLib/out_stream.hh"
#include "ArggLib/Evaluate.hh"
#include "ArggLib/Drop.hh"
#include "ArggLib/Modify.hh"
#include "ArggLib/Where.hh"
#include "ArggLib/has_member_helper.hh"
#include "ArggLib/OnEnd.hh"
#include "ArggLib/impl_do_begin_do_end.hh"
#include "ArggLib/OnStart.hh"
#include "ArggLib/constexpr_if.hh"
#include "ArggLib/Export_CSV.hh"
#include "ArggLib/convert2HashTable.hh"
#include "ArggLib/Import_CSV.hh"
#include "ArggLib/named_variable.hh"
#include <string>
#include <iostream>





using namespace std;

using namespace ArggLib;








ARGGLIB__DEFINE_TEST( processor_test1)
{
  





//	param() << 0 << 10 << 2 | proc() >> for_loop();// >> disp;
  
  cout << "display the numbers from 0 to 10\n";
  param() << 0 << 10 | proc() >> ArggLib::for_loop() >> ArggLib::display();
  cout << "==================\n\n";

  cout << "display the numbers from 0 to 10 step 2 \n";
  param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::display();
  cout << "==================\n\n";
  cout << "display the numbers from 0 to 10 step 2 \n";
  param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::out_stream(std::cout);
  cout << "==================\n\n";


  cout << "Streaming the numbers from 0 to 10 step 2 to out stream \n";

  std::stringstream out;
  param() << 0 << 10 << 2 | proc() >> ArggLib::for_loop() >> ArggLib::out_stream(out);

  std::string dummy(out.str());
  __Test("stream out", dummy, "0\n2\n4\n6\n8\n");

  cout << "==================\n\n";


      
}



ARGGLIB__DEFINE_TEST(processor_test2) {

  cout << "Streaming the numbers 1,2,5,6 to out stream \n";
  std::stringstream out;
  make_vec({ 1,2,5,6 }) | proc() >> ArggLib::for_loop() >> ArggLib::out_stream(out);
  __Test("stream out", out.str(), "1\n2\n5\n6\n");
  cout << "==================\n\n";

}


struct t1
{
  double x, y;

};

std::ostream& operator<<(std::ostream& out, t1& t) {
  out << "x: " << t.x << " y: " << t.y << std::endl;
  return out;
}

double operator*(t1& x, t1& y) {
  return x.x*y.x + x.y*y.y;
}


void _Fill(std::vector<t1>& vec, const std::string& delimer,  double x, double y) {
  vec.push_back(t1{ x, y });
}
ARGGLIB__DEFINE_TEST(processor_test3) {
  std::vector<t1> vec;

  cout << "Streaming the numbers 10 x 10  to out stream \n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> ArggLib::out_stream(vec);

  vec | proc() >> ArggLib::for_loop() >> square()>>  ArggLib::drop<0>()>> ArggLib::display();
  cout << "==================\n\n";
}

double test_times_two(double x) {
  return x * 2;
}

ARGGLIB__DEFINE_TEST(processor_test4) {
  std::vector<t1> vec;

  int i = 0;
  auto f = [&i](auto&e) { i++; return e*e; };
  auto f2 = [&i](auto&e,auto& e1) { i++; return e*e1; };

  cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify(f2) >> ArggLib::Fill(cout);\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> Modify(f2) >> ArggLib::out_stream(cout);
  cout << "==================\n\n";


  cout << "==================\n" << "param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2; }) >> Evaluate(f) >> ArggLib::Fill(cout);\n";
  param() | proc() >> for_loop(10) >> Where([](auto&e1) { return e1 % 2==0; }) >> Evaluate(f) >> ArggLib::out_stream(cout);
  cout << "==================\n\n";


  cout << "==================\n" << "param() | proc() >> for_loop(10) >> Evaluate(&test_times_two) >> ArggLib::Fill(vec);\n";
  param() | proc() >> for_loop(10) >> Evaluate(&test_times_two) >> ArggLib::out_stream(vec);
  cout << "==================\n\n";
  
  cout << "==================\n" << "vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::display();\n";
  vec | proc() >> ArggLib::for_loop() >> square() >> ArggLib::drop<0>() >> ArggLib::display();
  cout << "==================\n\n";
}




ARGGLIB__DEFINE_TEST(processor_test5) {
  cout << "==================\n" << "	auto p = proc() >> OnEnd([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnEnd([] { cout << \"3\\n\"; }) >> OnEnd([] { cout << \"4\\n\"; });\n\n";
  auto p = proc() >> OnEnd([] { cout << "1\n"; }) >> OnEnd([] { cout << "2\n"; }) >> OnEnd([] { cout << "3\n"; }) >> OnEnd([] { cout << "4\n"; });
  ArggLib_impl::unfold_end(p.m_pro);
  cout << "==================\n\n";
}


ARGGLIB__DEFINE_TEST(processor_test6) {
  cout << "==================\n" << "	auto p = proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnStart([] { cout << \"2\\n\"; }) >> OnStart([] { cout << \"3\\n\"; }) >> OnStart([] { cout << \"4\\n\"; });\n\n";
  auto p = proc() >> OnStart([] { cout << "1\n"; }) >> OnStart([] { cout << "2\n"; }) >> OnStart([] { cout << "3\n"; }) >> OnStart([] { cout << "4\n"; });
  ArggLib_impl::unfold_Start(p.m_pro);
  cout << "==================\n\n";
}


ARGGLIB__DEFINE_TEST(processor_test7) {
  cout << "==================\n" << "param() | proc() >> OnStart([] { cout << \"1\\n\"; }) >> OnEnd([] { cout << \"2\\n\"; }) >> OnStart([] {cout << \"3\\n\"; }) >> OnEnd([] {cout << \"4\\n\"; }) >> Evaluate([] { cout << \"eval\\n\"; return 1; });\n\n";
  param() | proc() >> OnStart([] { cout << "1\n"; }) >> OnEnd([] { cout << "2\n"; }) >> OnStart([] {cout << "3\n"; }) >> OnEnd([] {cout << "4\n"; }) >> Evaluate([] { cout << "eval\n"; return 1; });
  
  param() | proc()
    >> for_loop(10)
    >> OnStart([] { cout << "1\n"; })
    >> OnEnd([] { cout << "2\n"; })
    >> OnStart([] {cout << "3\n"; })
    >> OnEnd([] {cout << "4\n"; })
    //		>>drop<0>()
    >> Evaluate([](auto i) { cout << "eval\n";  return i*i; })
    >> drop<-2>()
    >> display();
  

  cout << "==================\n\n";

}




ARGGLIB__DEFINE_TEST(processor_test8) {

  cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV("test.csv");

  cout << "==================\n\n";

  cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test2.csv\", { \"x\",\"y\",\"z\" }););\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> for_loop(10) >> Export_CSV("test2.csv", { "x","y","z" },":  ");

  cout << "==================\n\n";

  cout << "==================\n" << "	param() | proc() >> for_loop(10) >> for_loop(10)  >> for_loop(10)  >> Export_CSV(\"test2.csv\", { \"x\",\"y\",\"z\" }););\n\n";
  param() | proc() >> for_loop(10) >> Export_CSV("test3.csv", ":  ", { "x"});

  cout << "==================\n\n";
}


ARGGLIB__DEFINE_TEST(processor_test9) {

  cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret["x"] = x; ret["y"] = y;  return ret; })>> Export_CSV("test4.csv");
  
  
  cout << "==================\n\n";


}


ARGGLIB__DEFINE_TEST(processor_test10) {

  cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  param() | proc() >> for_loop(10) >> for_loop(10) >> convert2HashTable<double>() >> Export_CSV("test4.csv");


  cout << "==================\n\n";


}

ARGGLIB__DEFINE_TEST(processor_test11) {

  cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  10 | proc() >> for_loop() >> Evaluate([](auto i) {return i*i; }) >> convert2HashTable({ "x","y" }) >> Export_CSV("test5.csv") >> display();


  cout << "==================\n\n";


}


ARGGLIB__DEFINE_TEST(processor_test12) {

  cout << "==================\n" << "param() | proc() >> for_loop(10) >> for_loop(10) >> Modify([](auto x, auto y) {  std::map<std::string, double> ret;  ret[\"x\"] = x; ret[\"y\"] = y;  return ret; })>> Export_CSV(\"test4.csv\");\n\n";
  
  param() | proc() >> Import_CSV("test4.csv", ';') >>  display();
  param() | proc() >> Import_CSV_as_HashTable("test4.csv", ';') >> display();

  cout << "==================\n\n";


}


template <typename FunBase_t>
class  fun_class1 {
public:
  FunBase_t m_funB;

  fun_class1(FunBase_t&& funB) : m_funB(std::move(funB)){}

};

template <typename T>
auto make_fun_class(T&& t) {
  return fun_class1<T>(std::move(t));
}

template <typename FunBase_t,typename Fun_t>
class  fun_class {
public:
  FunBase_t m_funB;
  Fun_t m_fun;
  fun_class(FunBase_t funB, Fun_t fun) : m_funB(std::move(funB)),m_fun(std::move(fun)) {}

};

template <typename T,typename T1>
auto make_fun_class(T t,T1 t1) {
  return fun_class<T,T1>(std::move(t),std::move(t1));
}


template <typename T, typename F>
auto operator<<(fun_class1<T> t, F f) {
  return make_fun_class(t.m_funB, [t, f](auto i) { return t.m_funB(f, i); });
}


template <typename T ,typename T1, typename F>
auto operator<<( fun_class<T,T1> t, F f) {
  return make_fun_class(t.m_funB, [f,t](auto i1) {return t.m_funB(f, t.m_fun(i1)); });
}

template <typename F, typename T,typename T1>
auto operator<<=(const F& f, fun_class<T,T1> t) {
  
  return t.m_fun(f);
}

template <typename F, typename T, typename T1>
auto operator<<=(F& f, fun_class<T, T1> t) {
  f = t.m_fun(f);
  return f;
}

auto _plus() {
  return  make_fun_class([](auto i1, auto i2) { return i1 + i2; });
}

auto _max() {
  return  make_fun_class([](auto i1, auto i2) { return  std::max(i1 , i2); });
}
ARGGLIB__DEFINE_TEST(processor_test13) {

  auto i = make_fun_class([](auto i1, auto i2) { return i1 + i2; });
  int j = 0;
  auto x = j <<= _max() << 1 << 2 << 3;
  auto x1 = 4 <<= _max() << 1 << 2 << 3;
  cout << "==================\n";
  param() | proc() >> Import_CSV_as_named_variables("test4.csv", ';') >> display();

  param() | proc() >> Import_CSV_as_HashTable("test4.csv", ';') >> display();
  cout << "==================\n\n";


}

