#include <iostream>
#include <random>
#include <thread>
#include <chrono>


// hardcoded arrays that contain the chances for pulling a 5* where the index is equal to the current pull number
// values are fixed point stored as int
// ie: 03000 = 0.3%
//     1000000 = 100%
// yes, dynamically generating these would be better, but hardcoding them is good enough and is fast
int chanceEvent[] = {
    00000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    03000,
    32235,
    61471,
    90706,
    119941,
    149176,
    178412,
    207647,
    236882,
    266118,
    295353,
    324588,
    353824,
    383059,
    412294,
    441529,
    470765,
    500000
};
int chanceStandard[] = {
    00000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    06000,
    64471,
    122941,
    181412,
    239882,
    298353,
    356824,
    415294,
    473765,
    532235,
    590706,
    649176,
    707647,
    766118,
    824588,
    883059,
    941529,
    1000000
};
uint64_t allPulls = 0;
uint64_t eventChars = 0;
std::mt19937 randall{ static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()) };


// calling this function where rollNumber is higher than 92 will result in an overflow or crash
int rollAt(int rollNumber, bool guarantee) {
    int outcome = randall() % 1000000;
    if (outcome <= chanceEvent[rollNumber]) {
        return(1);
    }
    if (outcome <= chanceStandard[rollNumber]) {
        if (guarantee) {
            return(1);
        }
        return(2);
    }
    return(0);
}

int pullUntilEvent() {
    // cur roll gets reset when getting a standard, cum roll does not
    int curRoll = 1;
    int cumRoll = 1;
    bool guarantee = false;
    roll:
    int returnVal = rollAt(curRoll, guarantee);
    if (returnVal == 1) {
        return(cumRoll);
    }
    if (returnVal == 2) {
        guarantee = true;
        curRoll = 1;
    }
    curRoll++;
    cumRoll++;
    goto roll;
}


int main()
{      
    auto startTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    auto lastDuration = duration;
    float avg;
    for (eventChars = 1; eventChars < 10000000; eventChars++) {
        if (eventChars % 100000 == 0) {
            lastDuration = duration;
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
            avg = (float)allPulls / eventChars;

            // uncomment the next line if you want to clear the console - this will result in ~6% slower execution speed, and is windows specific
            system("cls");

            std::cout << "Current event 5* count is " << eventChars << " and current pull count is " << allPulls << " for an average of " << avg << " pulls per event 5*" << std::endl
                      << "Time elapsed: " << duration << " | Currently averaging 100000 characters in: " << duration / ( eventChars / 100000 ) << std::endl;
        }
        allPulls += pullUntilEvent();
        std::this_thread::yield();
    }
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    avg = (float)allPulls / eventChars;
    std::cout << "Finished with event 5* count of " << eventChars << " and pull count of " << allPulls << " for an average of " << avg << " pulls per event 5*" << std::endl;
    return(0);
}
