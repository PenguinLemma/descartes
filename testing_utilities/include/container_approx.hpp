#pragma once

#include <sstream>
#include <string>

#include "catch.hpp"

namespace plemma::glancy {

template <typename Container>
class CompWiseApprox : public Catch::MatcherBase<Container>
{
  public:
    typedef typename Container::value_type value_type;
    typedef value_type T;

    CompWiseApprox(T epsilon, Container const& c) : data_(c), tolerance_(epsilon) {}

    template <class... Args>
    explicit CompWiseApprox(T epsilon, Args const&... args) : data_(args...), tolerance_(epsilon)
    {}

    // Performs the test for this matcher
    // TODO: implement zip iterator, use it to be able to do this with std::find_if(_not)
    bool match(Container const& other) const override
    {
        if (std::size(other) != std::size(data_))
            return false;
        auto it_this = std::begin(data_);
        auto it_this_end = std::end(data_);
        auto it_other = std::begin(other);
        while (it_this != it_this_end) {
            T distance_sq = (*it_this - *it_other);
            distance_sq *= distance_sq;
            if (distance_sq > tolerance_ * tolerance_)
                return false;
            ++it_this;
            ++it_other;
        }
        return true;
    }

    // Produces a string describing what this matcher does.
    // Format will be:
    // *values of lhs*
    // is component-wise approx to
    // (*values of rhs*)
    // with a tolerance of *value of tolerance_*
    [[nodiscard]] std::string describe() const override
    {
        std::ostringstream ss;
        ss << "\nis component-wise approx to\n(";
        auto const data_begin = std::begin(data_);
        auto const data_end = std::end(data_);
        for (auto it = data_begin; it != data_end; ++it) {
            if (it != data_begin)
                ss << ", ";
            ss << *it;
        }
        ss << ")\nwith a tolerance of " << tolerance_;

        return ss.str();
    }

  private:
    Container data_;
    T tolerance_;
};

// Builder function
template <typename Container, class... Args>
inline CompWiseApprox<Container> IsComponentWiseApprox(Args const&... args)
{
    return CompWiseApprox<Container>(args...);
}

}  // namespace plemma::glancy
