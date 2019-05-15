#pragma once
// TODO: add includes

namespace plemma
{

namespace glancy
{

// Class that has a similar behaviour to Catch2's `Approx()`.
// Class Container should have:
//  - value_type
//  - Methods `begin() const`, `end() const` and `size()`
//  - operator== (either as part of the class or as free method)
//  - operator<< (either as part of the class or as free method)
template <typename Container, typename DistanceFunction>
class ApproxContainer
{
public:
    typedef Container::value_type value_type;
    typedef value_type T;

    ApproxContainer(T epsilon, Container const & c, DistanceFunction dist = defdist) :
        tolerance_(epsilon), data_(c), dist_(dist)
    { }

    // Parametrized constructor that will use Container's
    // parametrized constructor (with 0 or more parameters)
    template<class... Args>
    ApproxContainer(T epsilon, DistanceFunction dist = defdist, Args const &... args) :
        tolerance_(epsilon), dist_(dist), data_(args...)
    { }

    // getter for the container
    constexpr Container const & Data() const noexcept { return data_; }
    // getter for the tolerance
    constexpr Container const & Tolerance() const noexcept { return tolerance_; }

    bool DistanceExceedsTolerance(Container const & c) const noexcept;

    constexpr std::size_t size() const noexcept { return std::size(data_); }
private:
    Container data_;
    DistanceFunction dist_;
    RealNum tolerance_;
};

// TODO: implement zip iterator, use it to be able to do this with std::find_if
template <typename Container, typename DistanceFunction>
constexpr bool ApproxContainer::DistanceExceedsTolerance(Container const & other) const noexcept
{
    auto it_this = std::begin(*this);
    auto it_this_end = std::end(*this);
    auto it_other = std::begin(other);
    while (it_this != it_this_end)
    {
        if (dist_(*it_this, *it_other) > tolerance_)
            return true;
        ++it_this;
        ++it_other;
    }
    return false;
}

template <typename Container, typename DistanceFunction>
constexpr bool operator==(Container const & c, ApproxContainer const & ac) noexcept
{
    if (c.size() != ac.size())  // this could be changed to an 'assert'
        return false;
    return !ac.DistanceExceedsTolerance(c);
}


} // namespace glancy

} // namespace plemma
