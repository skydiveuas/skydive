// ========== robo3D_Lib ==========
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __FLAGS__
#define __FLAGS__

/**
 * =============================================================================================
 * Flags
 * =============================================================================================
 */
template <class _Tp>
class Flags
{
	_Tp value;

public:
	Flags(void);
	Flags(const _Tp _value);

	_Tp& getFlagsVector(void);
	const _Tp& getFlagsVector(void) const;

	bool getFlagState(const unsigned id) const;
	void setFlagState(const unsigned id, const bool state);
};

template <class _Tp>
Flags<_Tp>::Flags(void) :
value(0)
{
}

template <class _Tp>
Flags<_Tp>::Flags(const _Tp _value):
	value(_value)
{
}

template <class _Tp>
_Tp& Flags<_Tp>::getFlagsVector(void)
{
	return value;
}

template <class _Tp>
const _Tp& Flags<_Tp>::getFlagsVector(void) const
{
	return value;
}

template <class _Tp>
bool Flags<_Tp>::getFlagState(const unsigned id) const
{
	return (value & (1 << id)) != 0;
}

template <class _Tp>
void Flags<_Tp>::setFlagState(const unsigned id, const bool state)
{
	if (state)
	{
		value = (_Tp)((int)value | 1 << id);
	}
	else
	{
		value = (_Tp)((int)value & ~(1 << id));
	}
}

#endif // __FLAGS__
