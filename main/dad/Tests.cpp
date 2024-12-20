#include "Tests.h"
#include "RenderTest.h"
#include "OrbitTest.h"
#include "UnitsTest.h"
#include "XprTest.h"
#include "MathTest.h"
#include "CoreTest.h"
#include "Test.h"
#include <lucid/core/Ctor.h>

///
///
///

#define DAD_ADD_TEST(name,test)									\
	_factory.add(name, new lucid::core::CtorStd<test,Test>());	\
	_tests.push_back(name)

///
///
///

Tests::Tests()
{
	DAD_ADD_TEST(  "Core",   CoreTest);
	DAD_ADD_TEST(  "Math",   MathTest);
	DAD_ADD_TEST(   "xpr",    XprTest);
	DAD_ADD_TEST( "Units",  UnitsTest);
	DAD_ADD_TEST( "Orbit",  OrbitTest);
#if false
	DAD_ADD_TEST("Render", RenderTest);
#endif
}

Tests::~Tests()
{
}
