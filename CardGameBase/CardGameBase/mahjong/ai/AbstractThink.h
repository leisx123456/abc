#pragma once

#pragma once

class IAbstractThink
{
public:
	IAbstractThink();
	~IAbstractThink();

	virtual void thinkHu() = 0;

	virtual void thinkOutCard() = 0;
};
