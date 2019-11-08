# Signal Analyzer Qt

This project uses Qt 5 and has a mix of QML, Javascrip, and C++, simple logic is generally handled by Javascript but heavy tasks like zero crossing detection for finding waveform frequency is handled by C++.

The application is intended to have 3 tabs, selectable by the icons on the left side bar:

- Live Mode: Which streams data from the MCU and display it and allows other things like adjusting cursors, recording data, etc.
- Analysis Mode: intended for more detailed analysis like FFT of the graph and things like that.
- Device Config Mode: This allows configuration and getting info from the MCU to fine tune sampling configuration.

Aside from the main tabs, there are also the top bar, which allows user to save graph pictures and data, help page and documentation, and other miscellaneous features.

There is also the bottom bar, which serves to report the result of user action(such as whether the data logger was started succesfully), show the current mode, and report the MCU being sampled.
