#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Symbols.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;

///	Evaluate
///
///	evaluates the supplied expression using the
///	supplied symbol substitutions.
///
///	SEE ALSO: Symbols
class Evaluate : public Algorithm
{
public:
	Evaluate() = default;

	virtual ~Evaluate() = default;

	float64_t operator()(Node const *node, Symbols const &symbols);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(NaturalLogarithm const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Power const *node) override;

private:
	Symbols symbols;
	double result = 0.0;

	template<class T> float64_t lhs(T const *node);

	template<class T> float64_t rhs(T const *node);

};

template<class T> inline float64_t Evaluate::lhs(T const *node)
{
	node->lhs->apply(this);
	return result;
}

template<class T> inline float64_t Evaluate::rhs(T const *node)
{
	node->rhs->apply(this);
	return result;
}

LUCID_XPR_END