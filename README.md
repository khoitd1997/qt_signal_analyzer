# Qt Signal Analyzer

Device Firmware and Qt front-end code for a basic signal analyzer

## Credits

Attributes for icons that I borrowed:

- Down Arrow: Icon made by [Lucy G](https://www.flaticon.com/authors/lucy-g) from www.flaticon.com
- Oscilloscope: Icon made by [Freepik](https://www.flaticon.com/authors/freepik) from www.flaticon.com
- Microchip and Analytics: Icon made by [Pixel perfect](https://www.flaticon.com/authors/pixel-perfect) from www.flaticon.com
- Stop button icon: Icon made by [Elegant Themes](https://www.flaticon.com/authors/elegant-themes) from www.flaticon.com
- Play button icon: Icon made by [Those Icons](https://www.flaticon.com/authors/those-icons) from www.flaticon.com

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