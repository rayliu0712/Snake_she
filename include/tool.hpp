#ifndef TOOL_HPP
#define TOOL_HPP

#include <algorithm>
#include <fmt/xchar.h>
#include <string>

namespace tool {

using namespace std;

const wstring SNUMS = L"０１２３４５６７８９";

inline int mid(int w, int n = 1)
{
    double x = (w - n) / 2.0;
    return x == (int)x ? x : x + 1;
}

// closed open interval
// [x, z)
inline bool coInterval(int x, int y, int z)
{
    return x <= y && y < z;
}

inline int intLen(int x)
{
    return to_string(x).length();
}

inline wstring toSnum(int x)
{
    auto s = to_wstring(x);
    for (auto& c : s)
        c = SNUMS[c - L'0'];
    return s;
}

template <typename T, typename... Args>
inline bool is(T t, Args... args)
{
    return ((t == args) || ...);
}

template <typename T, typename U>
typename T::iterator index(T& con, U value)
{
    return find(con.begin(), con.end(), value);
}

template <typename T, typename U>
inline bool in(T& con, int start, U value)
{
    return find(con.begin() + start, con.end(), value) != con.end();
}

template <typename T, typename U>
inline bool in(T& con, U value)
{
    return in(con, 0, value);
}

}

#endif