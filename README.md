# Qt Signal Analyzer

Device Firmware and Qt front-end code for a basic signal analyzer

![](image/2019-11-09-20-39-41.png)

## Structure

The repos have two major parts, documentations for each parts are in the README of each respective folder:

- ```qt/```: Code for the front-end GUI of the analyzer using Qt 5
- ```firmware/```: Device firmware for the STM32F4 data collector

## Features

- Display multiple waveform inputs received from device
- Collect and Analyze waveform statistics
- Manual and Dynamic scaling
- Saving and Loading data and graphs
- Allow selection of multiple devices
- Maybe some DSP features

### Live Mode

Live mode has these main components:

- Graph Control: Allow changing scaling and offset of X/Y axis, renaming graph, and changing their on/off status.
- Measurement and Logger: Allow measuring frequency, period, max, min, peak-to-peak of waveform, frequency and period uses zero-crossing detection and would probably be optimized in the future. The logger allows selecting which targets to log and report total time and points since the start of logging.
- Cursor Settings: Control the X/Y axis cursor, it reports the difference in X as well the specific position and difference in Y.
- Math: Draw additional waveform with sources being the signal currently recorded as arguments, currently allows, subtraction, sum, abs and log.

All components are organized as drop-down menu for convenience and except for the cursor one(cursor uses Javascript as its logic handler), has its own thread in order to minimize data processing time. When user interacts, QML will usually invoke a C++ function to update the data structure describing the states, and at certain frequency, the measurements or graph is redrawn to reflect the current options and data. The rate of refresh is being optimized to potentially make it < 100 ms.

## Credits

Attributes for icons that I borrowed:

- Down Arrow: Icon made by [Lucy G](https://www.flaticon.com/authors/lucy-g) from www.flaticon.com
- Oscilloscope: Icon made by [Freepik](https://www.flaticon.com/authors/freepik) from www.flaticon.com
- Microchip and Analytics: Icon made by [Pixel perfect](https://www.flaticon.com/authors/pixel-perfect) from www.flaticon.com
- Stop button icon: Icon made by [Elegant Themes](https://www.flaticon.com/authors/elegant-themes) from www.flaticon.com
- Play button icon: Icon made by [Those Icons](https://www.flaticon.com/authors/those-icons) from www.flaticon.com
