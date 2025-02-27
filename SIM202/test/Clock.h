// #ifndef CLOCK_H
// #define CLOCK_H

// #include <string>
// #include <iostream>
// #include <chrono>
// #include <thread>

// class Clock {
// private:
//     int simulatedMinutes;
//     bool running;

// public:
//     Clock() : simulatedMinutes(8 * 60 + 30), running(false) {} // Démarre à 8h30

//     void start() {
//         running = true;
//         std::thread([this]() {
//             while (running && simulatedMinutes <= (17 * 60 + 5)) { // Jusqu'à 17h05
//                 std::this_thread::sleep_for(std::chrono::seconds(1)); // 1s = 1min
//                 simulatedMinutes+=5;
//             }
//         }).detach();
//     }

//     void stop() { running = false; }

//     std::string getTime() const {
//         int hours = simulatedMinutes / 60;
//         int minutes = simulatedMinutes % 60;
//         return std::to_string(hours) + "h" + (minutes < 10 ? "0" : "") + std::to_string(minutes);
//     }

//     bool isPreOpening() const { return simulatedMinutes >= 510 && simulatedMinutes < 600; } // 8h30-10h00
//     bool isOpening() const { return simulatedMinutes == 600; } // 10h00
//     bool isContinuous() const { return simulatedMinutes > 600 && simulatedMinutes < 1020; } // 10h00-17h00
//     bool isPreClosing() const { return simulatedMinutes >= 1020 && simulatedMinutes < 1025; } // 17h00-17h05
//     bool isClosed() const { return simulatedMinutes >= 1025; } // Après 17h05
// };

// #endif // CLOCK_H

#ifndef CLOCK_H
#define CLOCK_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class Clock {
private:
    int simulatedMinutes;
    bool running;
    const int minutesPerTick = 5; // 1s réelle = 5min simulées

public:
    Clock() : simulatedMinutes(8 * 60 + 30), running(false) {} // Démarre à 8h30

    void start() {
        running = true;
        std::thread([this]() {
            while (running && simulatedMinutes <= (17 * 60 + 5)) { // Jusqu'à 17h05
                std::this_thread::sleep_for(std::chrono::seconds(1)); // 1s réelle
                simulatedMinutes += minutesPerTick; // Avance de 5min simulées
            }
        }).detach();
    }

    void stop() { running = false; }
    
    void Timeskip(unsigned int T){
        simulatedMinutes+=T;
        std::this_thread::sleep_for(std::chrono::seconds(T/5));
    }

    std::string getTime() const {
        int hours = simulatedMinutes / 60;
        int minutes = simulatedMinutes % 60;
        return std::to_string(hours) + "h" + (minutes < 10 ? "0" : "") + std::to_string(minutes);
    }

    bool isPreOpening() const { return simulatedMinutes >= 510 && simulatedMinutes < 600; } // 8h30-10h00
    bool isOpening() const { return simulatedMinutes == 600; } // 10h00
    bool isContinuous() const { return simulatedMinutes > 600 && simulatedMinutes < 1020; } // 10h00-17h00
    bool isPreClosing() const { return simulatedMinutes >= 1020 && simulatedMinutes < 1025; } // 17h00-17h05
    bool isClosed() const { return simulatedMinutes >= 1025; } // Après 17h05
};

#endif // CLOCK_H