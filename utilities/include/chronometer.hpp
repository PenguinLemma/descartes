#pragma once

#include <iostream>
#include <chrono>
#include <string>

namespace plemma
{

namespace chronometer
{

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

// Class to facilitate measuring duration of chuncks of code.
template <typename TimeUnitsToShow>
class SingleMeasurement
{
public:
    // Start the chronometer
    void Start() { begin_ = Clock::now(); }
    // Stop the chronometer
    void Stop() { end_ = Clock::now(); }
    // Show results: process_name took time_elapsed units_name
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

// Free method to ease even more measuring the duration of anything
// with an operator(). It will create and start a SingleMeasurement,
// then call the function received and then stop and show the measurement.
// Format of written output is the same than SingleMeasurement::Show :
// process_name took time_elapsed units_name
template <typename Function, typename TimeUnitsToShow>
inline void MeasureFunction(Function f, const std::string& process_name, const std::string& units_name)
{
    SingleMeasurement<TimeUnitsToShow> measurement;
    measurement.Start();
    f();
    measurement.Stop();
    measurement.Show(process_name, units_name);
}

} // namespace chronometer

} // namespace plemma
