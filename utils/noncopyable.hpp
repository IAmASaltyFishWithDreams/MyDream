#ifndef __UTILS_NONCOPYABLE_H__
#define __UTILS_NONCOPYABLE_H__

#include "macro.hpp"

NS_UTILS_BEGIN

//notes control construct version
class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&);
    const Noncopyable& operator = (const Noncopyable&);
};

NS_UTILS_END
#endif

