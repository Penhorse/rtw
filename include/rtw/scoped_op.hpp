#pragma once

#include <functional>

// 
// this is stupid
//

namespace rtw
{

class ScopedOp
{

public:

	using Op = std::function<void()>;

	class OpWrapper
	{

	public:

		OpWrapper() : op_(nullptr) {}
		OpWrapper(Op op) : op_(op) {}
		void operator()() { op_(); }
		operator bool() const { return bool(op_); }

	private:

		Op op_;

	};

	struct Begin : public OpWrapper
	{
		Begin() {}
		Begin(Op op) : OpWrapper(op) {}
	};

	struct End : public OpWrapper
	{
		End() {}
		End(Op op) : OpWrapper(op) {}
	};

	ScopedOp(Begin begin)
	{
		if(begin) begin();
	}

	ScopedOp(End end) :
		end_(end)
	{
	}

	ScopedOp(Begin begin, End end) :
		ScopedOp(end)
	{
		if(begin) begin();
	}

	ScopedOp(Op begin, Op end) :
		ScopedOp(Begin(begin), End(end))
	{
	}

	~ScopedOp()
	{
		if(end_) end_();
	}

private:

	End end_;

};

class Finally : private ScopedOp
{

public:

	Finally(ScopedOp::End end) :
		ScopedOp(end)
	{
	}
	
	Finally(ScopedOp::Op end) :
		ScopedOp(ScopedOp::End(end))
	{
	}
};
	
} // namespace rtw
