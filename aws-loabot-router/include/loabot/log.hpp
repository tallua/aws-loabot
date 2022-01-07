#include <iostream>

namespace loabot::log {

template <typename _Tp>
void tlog(_Tp&& arg) { std::cout << arg; }

template <typename _First, typename... _Rest>
void tlog(_First&& first, _Rest&&... rest)
{
    std::cout << first;
    tlog(std::forward<_Rest>(rest)...);
}

template <typename..._Args>
void LOG(_Args&&... args)
{
    tlog(std::forward<_Args>(args)...);
    std::cout << std::endl;
}

}