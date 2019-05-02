#ifndef BulkH
#define BulkH
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
template <typename T>
class Bulk
{
public :
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;

  Bulk()
    : _Size(0), _Data(NULL)
  {
  }
  Bulk(unsigned int Size)
    : _Size(Size), _Data(new T[Size])
  {
  }
  Bulk(T* Ptr, unsigned int Size)
    : _Size(Size), _Data(new T[Size])
  {
    copy(Ptr, Ptr + Size, _Data);			
  }
  Bulk(const Bulk& Source)
    : _Size(Source._Size), _Data(new T[Source._Size])
  {
    copy(Source._Data, Source._Data + _Size, _Data);
  }
  ~Bulk()
  {
    if (NULL != _Data) {
      delete [] _Data;
    }
  }
  Bulk& operator=(const Bulk& Source)
  {
    if (&Source == this) {
      return *this;
    }
    Resize(Source._Size);
    copy(Source._Data, Source._Data + _Size, _Data);

    return *this;
  }
  bool operator==(const Bulk& Source) const
  {
    if (_Size != Source._Size) {
      return false;
    }
    for (unsigned int idx = 0; idx < _Size; ++idx) {
      if (_Data[idx] != Source[idx]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const Bulk& Source)  const
  {
    if (_Size != Source._Size) {
      return true;
    }
    for (unsigned int idx = 0; idx < _Size; ++idx) {
      if (_Data[idx] != Source[idx]) {
        return true;
      }
    }
    return false;
  }
  void Resize(unsigned int Size)
  {
		if (_Size != Size) {
			if (_Data != NULL) {
				delete [] _Data;
      }
      _Size = Size;
      _Data = new T[Size];
    }
  }
  pointer      Data() const { return _Data; }
  unsigned int Size() const { return _Size; }

  //reference operator[](unsigned int Index) { return *(_Data + Index); }
  reference operator[](int Index) { return *(_Data + Index); }
  const_reference operator[](int Index) const { return *(_Data + Index); }

  operator void*() { return reinterpret_cast<void*>(_Data); }
  operator char*() { return reinterpret_cast<char*>(_Data); }
  operator unsigned char*() { return reinterpret_cast<unsigned char*>(_Data); }

private :
  pointer        _Data;
  unsigned int   _Size;
};
//---------------------------------------------------------------------------
#endif
