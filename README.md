# Qt Signal Analyzer

Device Firmware and Qt front-end code for a basic signal analyzer

## Features

- Display multiple waveform inputs received from device
- Collect and Analyze waveform statistics
- Manual and Dynamic scaling
- Control device sampling mode
- Saving and Loading data and graphs
- Allow selection of multiple devices
- Maybe some DSP features

## Structure

The repos have two major parts:

- Qt code: Code for the front-end GUI of the analyzer using Qt 5
- Device code: Device firmware for the TivaC, which will be the one collecting voltage data