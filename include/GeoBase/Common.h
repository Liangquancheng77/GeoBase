#pragma once
#include <cmath>
#include <random>
#include <chrono>

// 高精度圆周率 足够几何算法所有场景
constexpr double PI = 3.14159265358979323846;

//eps，全局统一管理
constexpr double EPS_ABS = 1e-9;
constexpr double EPS_REL = 1e-6;

// 角度转弧度
inline double DegToRad(double deg)
{
    return deg * PI / 180.0;
}

// 弧度转角度
inline double RadToDeg(double rad)
{
    return rad * 180.0 / PI;
}

// ---------------------- 随机数生成器（高质量，避免rand()偏差） ----------------------
inline std::mt19937& getRNG() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

// 生成[min, max]之间的随机浮点数
inline float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(getRNG());
}

inline double randomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(getRNG());
}

// ---------------------- 生成 [min, max] 随机整数（包括两端） ----------------------
inline int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max); // 整数专用
    return dist(getRNG());
}

// 高精度计时器（微秒级）
class Timer {
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    // 重置计时器
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    // 获取从开始到现在的时间（毫秒）
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // 转成毫秒（保留小数）
    }

    // 获取从开始到现在的时间（微秒）
    double elapsedUs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};