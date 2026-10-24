
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

constexpr double PI = 3.14159265358979323846;

// A simple combination of sine waves makes a repeatable test signal.
vector<double> generateSignal(int sampleCount, int sampleRate) {
    vector<double> signal(sampleCount, 0.0);

    for (int i = 0; i < sampleCount; ++i) {
        const double t = static_cast<double>(i) / sampleRate;
        signal[i] =
            0.60 * sin(2.0 * PI * 440.0 * t) +
            0.30 * sin(2.0 * PI * 660.0 * t) +
            0.10 * sin(2.0 * PI * 880.0 * t);
    }
    return signal;
}

// Add a delayed, quieter copy of the original signal.
// delaySamples determines the echo delay; echoGain controls its loudness.
vector<double> addEcho(const vector<double>& original,
                       int delaySamples,
                       double echoGain) {
    vector<double> result = original;

    for (int i = delaySamples; i < static_cast<int>(original.size()); ++i) {
        result[i] += echoGain * original[i - delaySamples];
    }
    return result;
}

// Add reproducible random noise so results are consistent across runs.
void addNoise(vector<double>& signal, double noiseAmplitude, unsigned seed) {
    mt19937 generator(seed);
    normal_distribution<double> noise(0.0, noiseAmplitude);

    for (double& sample : signal) {
        sample += noise(generator);
    }
}

// For each possible delay, compare y[t] with x[t-delay].
// The normalized correlation score is near 1 when the delayed patterns align.
// We skip delay 0 because we are searching for an echo, not the direct signal.
struct EchoResult {
    int delaySamples = 0;
    double score = -1.0;
};

EchoResult detectEcho(const vector<double>& original,
                      const vector<double>& observed,
                      int sampleRate,
                      double minDelaySeconds,
                      double maxDelaySeconds) {
    const int minDelay = max(1, static_cast<int>(minDelaySeconds * sampleRate));
    const int maxDelay = min(
        static_cast<int>(original.size()) - 1,
        static_cast<int>(maxDelaySeconds * sampleRate)
    );

    EchoResult best;

    for (int delay = minDelay; delay <= maxDelay; ++delay) {
        double dot = 0.0;
        double energyOriginal = 0.0;
        double energyObserved = 0.0;

        for (int i = delay; i < static_cast<int>(original.size()); ++i) {
            const double x = original[i - delay];
            const double y = observed[i];

            dot += x * y;
            energyOriginal += x * x;
            energyObserved += y * y;
        }

        const double denominator =
            sqrt(energyOriginal * energyObserved);

        if (denominator > 0.0) {
            const double score = dot / denominator;
            if (score > best.score) {
                best.score = score;
                best.delaySamples = delay;
            }
        }
    }

    return best;
}

int main() {
    const int sampleRate = 8000;       // samples per second
    const double durationSeconds = 2.0;
    const int sampleCount =
        static_cast<int>(sampleRate * durationSeconds);

    // Known echo settings for this demonstration.
    const double actualDelaySeconds = 0.25;
    const int actualDelaySamples =
        static_cast<int>(actualDelaySeconds * sampleRate);
    const double echoGain = 0.45;
    const double noiseAmplitude = 0.08;

    cout << "====================================\n";
    cout << "          ECHO DETECTIVE\n";
    cout << "   C++ Echo Detection Simulator\n";
    cout << "====================================\n\n";

    cout << "Generating test signal...\n";
    vector<double> original = generateSignal(sampleCount, sampleRate);

    cout << "Adding an echo and random noise...\n";
    vector<double> observed = addEcho(original, actualDelaySamples, echoGain);
    addNoise(observed, noiseAmplitude, 12345);

    cout << "Searching for echo delay...\n\n";
    EchoResult result = detectEcho(
        original, observed, sampleRate, 0.05, 0.60
    );

    cout << fixed << setprecision(4);
    cout << "Sample rate          : " << sampleRate << " samples/second\n";
    cout << "Signal duration      : " << durationSeconds << " seconds\n";
    cout << "Actual echo delay    : " << actualDelaySeconds << " seconds\n";
    cout << "Estimated echo delay : "
         << static_cast<double>(result.delaySamples) / sampleRate
         << " seconds\n";
    cout << "Correlation score    : " << result.score << "\n\n";

    const double estimatedDelay =
        static_cast<double>(result.delaySamples) / sampleRate;
    const double errorMs =
        abs(estimatedDelay - actualDelaySeconds) * 1000.0;

    cout << "Delay estimation error: " << errorMs << " ms\n";

    if (errorMs <= 10.0) {
        cout << "Result: Echo detected successfully.\n";
    } else {
        cout << "Result: A possible echo was found, but the estimate is not exact.\n";
    }

    cout << "\nNote: This demo uses generated signals, not microphone or MP3 audio.\n";
    return 0;
}
