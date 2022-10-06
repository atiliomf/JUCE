#pragma once


#define BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION(funcName, Type) \
    void funcName (Type);                                      \
    template < typename... Args >                              \
    void funcName (Type first, Args&&... rest)                 \
    {                                                          \
        funcName (first);                                      \
        funcName (std::forward< Args > (rest)...);             \
    }


#define BV_MUST_INHERIT_FROM(classToTest, requiredBaseClass) \
    std::enable_if_t< std::is_base_of< requiredBaseClass, classToTest >::value >* = nullptr
