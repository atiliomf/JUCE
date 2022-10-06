#pragma once

namespace bav
{
template < class Test, template < typename... > class Ref >
struct is_specialization : std::false_type
{
};

template < template < typename... > class Ref, typename... Args >
struct is_specialization< Ref< Args... >, Ref > : std::true_type
{
};


template < class Type >
constexpr bool is_container()
{
    return std::is_array< Type >() || is_specialization< Type, std::vector >() || is_specialization< Type, juce::OwnedArray >();
}

}  // namespace bav
