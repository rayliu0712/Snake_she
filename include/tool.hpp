#ifndef TOOL_HPP
#define TOOL_HPP

#include <format>
#include <ranges>
#include <string>
using std::wstring;

namespace tool {

const auto SNUMS = L"０１２３４５６７８９";

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
    return std::to_string(x).length();
}

inline wstring toSnum(int x)
{
    auto s = std::to_wstring(x);
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
inline bool in(const T& con, U value)
{
    return std::ranges::find(con, value) != con.end();
}

}

#endif