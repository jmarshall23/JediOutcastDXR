#ifndef hString_H
#define hString_H

#pragma warning(push, 3)
#include <string>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include <memory>
#include <cstddef>
#include <cstring>
#include <cassert>
#pragma warning(pop)

using namespace std;

class hstring
{
	int mId;

	void Init(const char* str);

public:
	hstring()
		: mId(0)
	{
	}

	hstring(const char* str)
	{
		Init(str);
	}

	hstring(const string& str)
	{
		Init(str.c_str());
	}

	hstring(const hstring& str)
		: mId(str.mId)
	{
	}

	operator string() const
	{
		return str();
	}

	const char* c_str(void) const;
	string str(void) const;

	hstring& operator=(const char* str)
	{
		Init(str);
		return *this;
	}

	hstring& operator=(const string& str)
	{
		Init(str.c_str());
		return *this;
	}

	hstring& operator=(const hstring& str)
	{
		mId = str.mId;
		return *this;
	}

	bool operator==(const hstring& str) const
	{
		return (mId == str.mId);
	}

	int compare(const hstring& str) const
	{
		return std::strcmp(c_str(), str.c_str());
	}

	bool operator<(const hstring& str) const
	{
		return (mId < str.mId);
	}

	int length() const
	{
		return static_cast<int>(std::strlen(c_str()));
	}
};

void TouchStringPool(void);

////////////
// MapPool
////////////
#define MAP_NODE_SIZE (32)

class CMapBlock;

class CMapPoolLow
{
	vector<CMapBlock*> mMapBlocks;
	vector<void*>      mFreeList;
	int                mLastBlockNum;

public:
	CMapPoolLow();
	~CMapPoolLow();

	void* Alloc();
	void  Free(void* p);
	void  TouchMem();
};

CMapPoolLow& GetMapPool();

template <class T>
class CMapPool
{
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	template <class U>
	struct rebind
	{
		using other = CMapPool<U>;
	};

	CMapPool() noexcept
		: mPool(GetMapPool())
	{
	}

	template <class U>
	CMapPool(const CMapPool<U>&) noexcept
		: mPool(GetMapPool())
	{
	}

	~CMapPool() = default;

	pointer address(reference value) const noexcept
	{
		return &value;
	}

	const_pointer address(const_reference value) const noexcept
	{
		return &value;
	}

	pointer allocate(size_type num)
	{
		assert(num == 1); // allocator designed for single-node allocations
		assert(sizeof(T) <= MAP_NODE_SIZE); // node must fit pool block
		return static_cast<pointer>(mPool.Alloc());
	}

	pointer allocate(size_type num, const void*)
	{
		return allocate(num);
	}

	void deallocate(pointer p, size_type num)
	{
		assert(num == 1);
		mPool.Free(p);
	}

	template <class U, class... Args>
	void construct(U* p, Args&&... args)
	{
		::new ((void*)p) U(std::forward<Args>(args)...);
	}

	template <class U>
	void destroy(U* p)
	{
		p->~U();
	}

	size_type max_size() const noexcept
	{
		return 1;
	}

private:
	CMapPoolLow& mPool;
};

template <class T1, class T2>
bool operator==(const CMapPool<T1>&, const CMapPool<T2>&) noexcept
{
	return true;
}

template <class T1, class T2>
bool operator!=(const CMapPool<T1>&, const CMapPool<T2>&) noexcept
{
	return false;
}

template <class K, class V, class Compare = less<K>>
class hmap : public map<K, V, Compare, CMapPool<pair<const K, V>>> {};

template <class K, class V, class Compare = less<K>>
class hmultimap : public multimap<K, V, Compare, CMapPool<pair<const K, V>>> {};

template <class K, class Compare = less<K>>
class hset : public set<K, Compare, CMapPool<K>> {};

template <class K, class Compare = less<K>>
class hmultiset : public multiset<K, Compare, CMapPool<K>> {};

template <class K>
class hlist : public list<K, CMapPool<K>> {};

#endif // hString_H