#pragma once

#include <iostream>
#include <chrono>
#include <string>

namespace plemma
{

namespace chronometer
{

//using Clock = typename std::chrono::steady_clock;
using TimePoint = typename std::chrono::time_point<std::chrono::steady_clock>;

template <typename TimeUnitsToShow>
class SingleMeasurement
{
public:
    void Start() { begin_ = std::chrono::steady_clock::now(); }
    void Stop() { end_ = std::chrono::steady_clock::now(); }
    void Show(const std::string& process_name, const std::string& units_name)
    {
        std::cout << process_name << " took ";
        std::cout << std::chrono::duration_cast<TimeUnitsToShow> (end_ - begin_).count();
        std::cout << " " << units_name << std::endl;
    }
private:
    TimePoint begin_;
    TimePoint end_;
};

template <typename Function, typename TimeUnitsToShow>
void MeasureFunction(Function f, const std::string& process_name, const std::string& units_name)
{
    SingleMeasurement<TimeUnitsToShow> measurement;
    measurement.Start();
    f();
    measurement.Stop();
    measurement.Show(process_name, units_name);
}

} // namespace chronometer

} // namespace plemma
