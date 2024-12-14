#ifndef TOOL_HPP
#define TOOL_HPP

#include <fmt/xchar.h>

#include <algorithm>
#include <string>

namespace tool {

const std::wstring SNUMS = L"０１２３４５６７８９";

inline int mid(int w, int n = 1) {
    double x = (w - n) / 2.0;
    return x == (int)x ? x : x + 1;
}

// closed open interval
// [x, z)
inline bool coInterval(int x, int y, int z) { return x <= y && y < z; }

inline int intLen(int x) { return std::to_string(x).length(); }

inline std::wstring toSnum(int x) {
    auto s = std::to_wstring(x);
    for (auto &c : s)
        c = SNUMS[c - L'0'];
    return s;
}

template <typename T, typename... Args>
inline bool is(T t, Args... args) {
    return ((t == args) || ...);
}

template <typename T, typename U>
typename T::iterator index(T &con, U value) {
    return std::find(con.begin(), con.end(), value);
}

template <typename T, typename U>
inline bool in(T &con, int start, U value) {
    return std::find(con.begin() + start, con.end(), value) != con.end();
}

template <typename T, typename U>
inline bool in(T &con, U value) {
    return in(con, 0, value);
}

} // namespace tool

#endif