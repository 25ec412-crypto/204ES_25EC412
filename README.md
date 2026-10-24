# Echo Detective

**Echo Detective** is a beginner-friendly, software-only C++ project that simulates an echo in a digital signal and estimates the echo delay.

The program generates an original signal from sine waves, adds a delayed and quieter copy to simulate an echo, adds random noise, and searches for the delay with the highest normalized correlation score.

## Features

- Generates a test signal using three sine waves: 440 Hz, 660 Hz, and 880 Hz.
- Simulates an echo by adding a delayed, lower-amplitude copy of the original signal.
- Adds random noise to simulate interference.
- Estimates echo delay by testing candidate delays and calculating normalized correlation.
- Displays the actual delay, estimated delay, correlation score, and delay estimation error.
- Uses only standard C++ libraries. MATLAB, Python, Arduino, external libraries, and internet access are not required to run it.

## Requirements

- A C++ compiler supporting **C++17**, such as GCC/MinGW.
- A code editor such as Visual Studio Code (recommended but not required).

## Files

```text
Echo-Detective/
└── echo_detective.cpp
```

Save the provided source code as `echo_detective.cpp`.

## How It Works

1. **Generate the original signal:** The program combines sine waves at 440 Hz, 660 Hz, and 880 Hz.
2. **Add an echo:** A delayed copy of the original signal is added at a lower amplitude.
3. **Add noise:** Random values from a normal distribution are added to the samples.
4. **Search for the delay:** The program tests candidate delays from 0.05 to 0.60 seconds.
5. **Calculate correlation:** For each delay, it calculates a normalized correlation score between the original signal and the observed signal.
6. **Report the result:** It chooses the delay with the highest score and compares it with the known delay used in the simulation.

## Default Simulation Settings

| Setting | Value | Meaning |
|---|---:|---|
| Sample rate | 8,000 samples/second | Number of samples representing one second |
| Signal duration | 2 seconds | Length of the generated signal |
| Actual echo delay | 0.25 seconds | Delay deliberately added by the program |
| Echo delay in samples | 2,000 samples | 0.25 × 8,000 |
| Echo gain | 0.45 | Echo amplitude is 45% of the original |
| Noise amplitude | 0.08 | Standard deviation of the simulated noise |
| Delay search range | 0.05–0.60 seconds | Candidate delays tested by the detector |

The random number generator uses a fixed seed (`12345`) so the noise sequence is reproducible for the same implementation and settings.

## Compile and Run

### Windows (PowerShell, with GCC/MinGW installed)

Open the VS Code terminal in the folder containing `echo_detective.cpp`, then run:

```powershell
g++ -std=c++17 -O2 echo_detective.cpp -o echo_detective.exe
.\echo_detective.exe
```

### Linux or macOS

```bash
g++ -std=c++17 -O2 echo_detective.cpp -o echo_detective
./echo_detective
```

If the compiler command is not recognized, install a C++ compiler and ensure it is available in your terminal's PATH.

## Understanding the Output

The program prints:

- **Sample rate:** Samples generated per second.
- **Signal duration:** Length of the simulated signal in seconds.
- **Actual echo delay:** The delay inserted by the simulation.
- **Estimated echo delay:** The delay selected by the correlation search.
- **Correlation score:** The normalized similarity score for the selected delay.
- **Delay estimation error:** Absolute difference between the estimated and actual delays, converted to milliseconds.
- **Result:** A simple status message based on whether the error is at most 10 ms.

The error is calculated as:

```cpp
abs(estimatedDelay - actualDelaySeconds) * 1000.0
```

An error of `0.0000 ms` means the estimated delay equals the actual delay at the displayed precision. The 10 ms success threshold is a project-specific rule, not a universal standard.

## Important Limitations

- The project generates numerical signal samples; it does not record sound from a microphone.
- It does not load or analyze MP3 or WAV files.
- The detector searches only within the configured delay range.
- Correlation-based detection can be affected by the signal pattern and noise. The highest score is an estimate, not proof that a real-world echo exists.
- This is a simulation, not a validated real-world acoustic measurement system.

## Possible Future Improvements

- Add support for reading PCM WAV audio files using a custom C++ implementation.
- Simulate multiple echoes.
- Let the user change echo delay, echo gain, and noise level at runtime.
- Compare detection performance under different noise levels.
- Save samples and results to CSV or text files for later inspection.

## Concepts Demonstrated

- Functions and return values
- `vector` and references
- Structures (`struct`)
- Loops and conditional statements
- Mathematical functions and sine waves
- Random-number generation
- Normalized correlation
- Numeric conversion and formatted output

## License

MIT License

Copyright (c) 2026 25ec412-crypto
