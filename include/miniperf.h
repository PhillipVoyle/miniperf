/*
Copyright 2020 Phillip Voyle

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <string>
#include <map>
#include <mutex>
#include <chrono>

namespace miniperf {

    template<typename TDuration>
    struct measurement
    {
        int count;
        TDuration total_duration;
        TDuration worst_duration;

        measurement():count(0), total_duration(0), worst_duration(0)
        {

        }
        measurement(
            int c,
            TDuration t,
            TDuration w):count(c), total_duration(t), worst_duration(w)
        {
        }
    };

    template<typename TClock>
    class generic_performance_tracker
    {
    public:
        typedef typename TClock::duration duration_t;
        typedef typename TClock::time_point time_point_t;
        typedef measurement<duration_t> measurement_t;
        typedef std::map<std::string, measurement_t> map_t;

        time_point_t begin_tracking() const {
            return TClock::now();
        }

        void end_tracking(const std::string& counter, time_point_t beginning) {
            auto end = TClock::now();
            auto durn = end - beginning;

            std::lock_guard<std::mutex> lock(mut_);
            auto it = durations_.find(counter);
            if (it == durations_.end())
            {
                measurement_t m = {1, durn, durn};
                durations_[counter] = m;
            }
            else
            {
                measurement_t& m = it->second;
                m.count++;
                m.total_duration += durn;
                if (durn > m.worst_duration)
                {
                    m.worst_duration = durn;
                }
            }
        }

        map_t measure()
        {
            std::lock_guard<std::mutex> lock(mut_);
            map_t result;
            std::swap(result, durations_);
            return result;
        }

    private:
        map_t durations_;
        std::mutex mut_;
    };


    using performance_tracker = generic_performance_tracker<std::chrono::high_resolution_clock>;
}