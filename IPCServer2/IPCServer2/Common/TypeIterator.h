//---------------------------------------------------------------------------
#ifndef TypeIteratorH
#define TypeIteratorH
//---------------------------------------------------------------------------
#pragma pack(push, 1)
template <class Type>
class CTypeIterator
{
public:

	CTypeIterator(Type& Target)
		: _Target(Target)
	{
	}

	CTypeIterator(const CTypeIterator& Source)
		: _Target(Source._Target),
			_Iterator(Source._Iterator)
	{
	}

	CTypeIterator& operator=(const CTypeIterator& rhs)
  {
    if (&rhs != this) {
      _Target = rhs._Target;
      _Iterator = rhs._Iterator;
    }
    return *this;
  }

	~CTypeIterator(){};

	void Reset() {
		_Iterator = _Target.begin();
	}
	void ToEnd() {
		_Iterator = _Target.end();
  }
	void To(typename Type::iterator Target) {
    _Iterator = Target;
  }
	void Next() {
		++_Iterator;
	}
	void operator++() {
		++_Iterator;
  }
	void operator--() {
    --_Iterator;
  }
	bool IsDone() {
		return _Iterator == _Target.end() ? true : false;
  }
	bool Undone() {
		return _Iterator == _Target.end() ? false : true;
  }
	typename Type::iterator Get() {
		return _Iterator;
	}
	typename Type::iterator operator->() {
		return _Iterator;
	}
	typename Type::iterator Find(const Type& Content)
	{
		bool found = false;

		_Iterator = _Target.begin();
		while (!_Iterator == _Target.end()) {
			if ((*_Iterator) == Content) {
  			found = true;
				break;
			}
			++_Iterator;
		}
		if (!found) {
			_Iterator = _Target.end();
		}
		return _Iterator;
  }

protected:
	Type&            _Target;
	typename Type::iterator   _Iterator;
};
//---------------------------------------------------------------------------
template <class Type>
class CTypeConstIterator
{
public:

	CTypeConstIterator(const Type& Target)
		: _Target(Target)
	{
	}

	CTypeConstIterator(const CTypeConstIterator& Source)
		: _Target(Source._Target),
			_Iterator(Source._Iterator)
	{
	}

	CTypeConstIterator& operator=(CTypeConstIterator& rhs)
  {
		if (&rhs != this) {
			_Target = rhs._Target;
			_Iterator = rhs._Iterator;
		}
		return *this;
	}

	~CTypeConstIterator(){};

	void Reset() {
		_Iterator = _Target.begin();
	}
  void ToEnd() {
    _Iterator = _Target.end();
  }
  void To(typename Type::iterator Target) {
    _Iterator = Target;
  }
	void Next() {
		++_Iterator;
	}
  void operator++() {
    ++_Iterator;
  }
	bool IsDone() {
		return _Iterator == _Target.end() ? true : false;
  }
	bool Undone() {
		return _Iterator == _Target.end() ? false : true;
  }
	typename Type::const_iterator Get() {
		return _Iterator;
	}
	typename Type::const_iterator operator->() {
    return _Iterator;
	}

	typename Type::const_iterator Find(const Type& Content)
	{
		bool found = false;

		_Iterator = _Target.begin();
		while (!_Iterator == _Target.end()) {
			if ((*_Iterator) == Content) {
  			found = true;
				break;
			}
			++_Iterator;
		}
		if (!found) {
			_Iterator = _Target.end();
		}
		return _Iterator;
	}

private:
	const Type&            _Target;
	typename Type::const_iterator   _Iterator;
};
//---------------------------------------------------------------------------
template <class ContainerT, class ElementT>
typename ContainerT::iterator find_iter(const ContainerT& Container, const ElementT& Element)
{
	CTypeIterator<ContainerT>  iter(Container);

	for (iter.Reset(); iter.Undone(); ++iter) {
		if ((*iter.Get()) == Element) {
      return iter.Get();
    }
	}
	iter.ToEnd();
	return iter.Get();
}
//---------------------------------------------------------------------------
template <class ContainerT, class ElementT>
typename ContainerT::iterator find_const_iter(const ContainerT& Container, const ElementT& Element)
{
	CTypeConstIterator<ContainerT>  iter(Container);

	for (iter.Reset(); iter.Undone(); ++iter) {
		if ((*iter.Get()) == Element) {
			return iter.Get();
		}
	}
	iter.ToEnd();
	return iter.Get();
}
//---------------------------------------------------------------------------
template <class MapT, class ElementT>
typename MapT::iterator find_map_iter(const MapT& Map, const ElementT& First)
{
	CTypeIterator<MapT>  iter(Map);

	for (iter.Reset(); iter.Undone(); ++iter) {
		if (iter->first == First) {
			return iter.Get();
		}
	}
	iter.ToEnd();
	return iter.Get();
}
//---------------------------------------------------------------------------
template <class MapT, class ElementT>
typename MapT::const_iterator find_map_const_iter(const MapT& Map, const ElementT& First)
{
	CTypeConstIterator<MapT>  iter(Map);

	for (iter.Reset(); iter.Undone(); ++iter) {
		if (iter->first == First) {
			return iter.Get();
		}
	}
	iter.ToEnd();
	return iter.Get();
}
//---------------------------------------------------------------------------
#pragma pack(pop)
#endif
