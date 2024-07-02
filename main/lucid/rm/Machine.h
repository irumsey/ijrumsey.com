#pragma once

#include <cinttypes>
#include <vector>
#include <list>
#include <lucid/core/Types.h>
#include <lucid/rm/Defines.h>
#include <lucid/rm/Graph.h>

LUCID_RM_BEGIN

///
///
///
class Machine
{
public:
	enum OPCODE
	{
		NEG  = 0x00,	// negate
		ADD  = 0x01,	// add
		SUB  = 0x02,	// subtract
		MUL  = 0x03,	// multiply

		INC  = 0x04,	// increment
		DEC  = 0x05,	// decrement

		NOT  = 0x06,	// logical not (complement)
		AND  = 0x07,	// logical and
		OR   = 0x08,	// logical or
		XOR  = 0x09,	// logical exclusive or
		LSL  = 0x0a,	// logical shift left
		LSR  = 0x0b,	// logical shift right

		BRA  = 0x0c,	// branch always
		BZ   = 0x0d,	// branch if zero
		BNZ  = 0x0e,	// branch if not zero
		BSE  = 0x0f,	// branch if stack is empty
		BSNE = 0x10,	// branch if stack is not empty

		MOV  = 0x11,	// move

		NEGF = 0x12,	// floating point negate
		ADDF = 0x13,	// floating point add
		SUBF = 0x14,	// floating point subtract
		MULF = 0x15,	// floating point multiply
		DIVF = 0x16,	// floating point divide

		CGI  = 0x17,	// create graph input
		CAGN = 0x18,	// create/adjust graph node
		CAGE = 0x19,	// create/adjust graph edge
		MHD  = 0x1a,	// move head
		
		RDGN = 0x1b,	// read graph node data
		RDGE = 0x1c,	// read graph edge data

		PUSH = 0x1d,	// push head/upstream/downstream
		POP  = 0x1e,	// pop head position

		NOP  = 0x1f,	// nop
	};

	enum { OPCODE_COUNT = 0x20 };

	typedef uint32_t Instruction;
	typedef std::vector<Instruction> Program;

	Machine();

	virtual ~Machine();

	Graph &graph();

	Graph const &graph() const;

	void execute(Program const &program, size_t limit = -1);

	void reset();

private:
	enum {  REGISTER_COUNT = 256 };
	enum {     MEMORY_SIZE = 0x00010000};

	size_t pc = 0;

	uint16_t *registers = nullptr;
	uint16_t *memory = nullptr;

	std::list<size_t> stack;
	size_t head[2] = { 0, 0, };

	Graph _graph;

	typedef void (Machine::*operation_t)(Instruction ins);
	static operation_t const operations[OPCODE_COUNT];

	void _neg(Instruction ins);

	void _add(Instruction ins);

	void _sub(Instruction ins);

	void _mul(Instruction ins);

	void _inc(Instruction ins);

	void _dec(Instruction ins);

	void _not(Instruction ins);

	void _and(Instruction ins);

	void _or(Instruction ins);

	void _xor(Instruction ins);

	void _lsl(Instruction ins);

	void _lsr(Instruction ins);

	void _bra(Instruction ins);

	void _bz(Instruction ins);

	void _bnz(Instruction ins);

	void _bse(Instruction ins);

	void _bsne(Instruction ins);

	void _mov(Instruction ins);

	void _negf(Instruction ins);

	void _addf(Instruction ins);

	void _subf(Instruction ins);

	void _mulf(Instruction ins);

	void _divf(Instruction ins);

	void _cgi(Instruction ins);

	void _cagn(Instruction ins);

	void _cage(Instruction ins);

	void _mhd(Instruction ins);

	void _rdgn(Instruction ins);

	void _rdge(Instruction ins);

	void _push(Instruction ins);

	void _pop(Instruction ins);

	void _nop(Instruction ins);

};

inline Graph &Machine::graph()
{
	return _graph;
}

inline Graph const &Machine::graph() const
{
	return _graph;
}

LUCID_RM_END
