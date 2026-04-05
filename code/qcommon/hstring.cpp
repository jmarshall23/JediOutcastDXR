#ifndef hString_H
#define hString_H

#pragma warning(push, 3)
#include <string>
#include <set>
#include <list>
#include <map>
#include <cstring>
#pragma warning(pop)

using namespace std;

class hstring
{
	std::string mStr;

	void Init(const char* str)
	{
		mStr = (str != nullptr) ? str : "";
	}

public:
	hstring()
		: mStr()
	{
	}

	hstring(const char* str)
	{
		Init(str);
	}

	hstring(const string& str)
		: mStr(str)
	{
	}

	hstring(const hstring& str)
		: mStr(str.mStr)
	{
	}

	operator string() const
	{
		return mStr;
	}

	const char* c_str(void) const
	{
		return mStr.c_str();
	}

	string str(void) const
	{
		return mStr;
	}

	hstring& operator=(const char* str)
	{
		Init(str);
		return *this;
	}

	hstring& operator=(const string& str)
	{
		mStr = str;
		return *this;
	}

	hstring& operator=(const hstring& str)
	{
		mStr = str.mStr;
		return *this;
	}

	bool operator==(const hstring& str) const
	{
		return mStr == str.mStr;
	}

	bool operator!=(const hstring& str) const
	{
		return mStr != str.mStr;
	}

	int compare(const hstring& str) const
	{
		return mStr.compare(str.mStr);
	}

	bool operator<(const hstring& str) const
	{
		return mStr < str.mStr;
	}

	int length() const
	{
		return static_cast<int>(mStr.length());
	}

	bool empty() const
	{
		return mStr.empty();
	}

	void clear()
	{
		mStr.clear();
	}
};

inline void TouchStringPool(void)
{
	// no-op
}

template <class K, class V, class Compare = less<K>>
class hmap : public map<K, V, Compare>
{
};

template <class K, class V, class Compare = less<K>>
class hmultimap : public multimap<K, V, Compare>
{
};

template <class K, class Compare = less<K>>
class hset : public set<K, Compare>
{
};

template <class K, class Compare = less<K>>
class hmultiset : public multiset<K, Compare>
{
};

template <class K>
class hlist : public list<K>
{
};

#endif // hString_H