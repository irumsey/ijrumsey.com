#include "Tests.h"
#include "RenderTest.h"
#include "MathTest.h"
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
	DAD_ADD_TEST(  "Math",   MathTest);
	DAD_ADD_TEST("Render", RenderTest);
}

Tests::~Tests()
{
}
