#include "macro.hpp"
#include "noncopyable.hpp"

NS_UTILS_BEGIN

//func actuator wrapper
//RAII mechanism
//out scope desconstruct

template <typename Func>
class ScopeGuard : public Noncopyable {
public:
    ScopeGuard(Func f) : m_func(std::move(f)),m_active(true) {}
    ScopeGuard(ScopeGuard&& sg) : m_func(std::move(sg.m_func),m_active(sg.m_active){}
    ~ScopeGuard() { if(m_active) m_func(); }
public:
    void dismiss() { m_active = false; }

private:
    Func m_func;
    bool m_active;
};

template <typename Func>
ScopeGuard<Func> NewScopeGuard(Func f){
    return ScopeGuard<Func>(std::move(f));
}


NS_UTILS_END
