/*
Copyright 2020 Phillip Voyle

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <chrono>
#include <thread>
#include <iostream>
#include <iostream>
#include <miniperf.h>

using namespace std::chrono;

int main(int argc, char** argv) {

    miniperf::performance_tracker perf;
    auto start = perf.begin_tracking();

    std::this_thread::sleep_for(120ms);
    perf.end_tracking("first_duration", start);

    std::this_thread::sleep_for(1251ms);
    perf.end_tracking("second_duration", start);

    start = perf.begin_tracking();

    std::this_thread::sleep_for(120ms);
    perf.end_tracking("first_duration", start);

    std::this_thread::sleep_for(1251ms);
    perf.end_tracking("second_duration", start);

    auto results = perf.measure();

    for(auto kvp: results) {
        std::cout
            << kvp.first << ", total = " 
            << duration_cast<microseconds>(kvp.second.total_duration).count()
            << " microseconds, worst = "
            << duration_cast<microseconds>(kvp.second.worst_duration).count()
            << " microseconds, average = "
            << duration_cast<microseconds>(kvp.second.total_duration).count() / kvp.second.count
            << " microseconds"
            << std::endl;
    }

    return 0;
}