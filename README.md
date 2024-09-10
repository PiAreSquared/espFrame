# espFrame

This project combines an ESP32 microcontroller with a framed E-ink display to create an "at-a-glance" information station. It includes custom code to drive the E-ink display. The code is responsible for refreshing the display with the latest data fetched from the weather and transit APIs. It utilizes libraries specifically designed for driving E-ink displays, ensuring optimal performance and compatibility. This station provides a convenient way for users to gather information and plan their day without the need for a smartphone.

## Project Overview

The espFrame (ESP32-based station) fetches live data from various APIs to display the current weather conditions and the bus schedule for a user-defined route. The E-ink display is chosen for its low power consumption and excellent readability under direct sunlight, making it perfect for both indoor and outdoor settings.

## Hardware Required

- ESP32 development board
- Framed E-ink display compatible with the ESP32
- Power supply for the ESP32 and the display
- Optional: Case or frame for mounting the display

## Software Requirements

- ESP-IDF or Arduino IDE for ESP32 development
- Libraries for driving the E-ink display
- HTTP client libraries for fetching data from weather and transit APIs

## Configuration

1. **Set up your development environment**: Ensure that you have the ESP-IDF or Arduino IDE installed and configured for ESP32 development.

2. **Connect the E-ink display to the ESP32**: Follow the display's documentation to correctly connect it to your ESP32 board.

3. **Install necessary libraries**: Make sure to install any required libraries for the E-ink display and HTTP requests.

## Building and Running

1. **Configure Wi-Fi and API settings**: In your project, include a configuration file or section where you can specify your Wi-Fi credentials and API keys for the weather and transit data sources.

2. **Build the project**: Compile your project using the ESP-IDF or Arduino IDE.

   For ESP-IDF:
    ```
    idf.py build
    ```

    For Arduino IDE:
    - Open the project in the Arduino IDE.
    - Select the correct board and port.
    - Click the "Upload" button to compile and upload the code to your ESP32.

3. **Flash the ESP32**: Flash the compiled binary to your ESP32 board.

## Usage

Once the ESP32 E-Ink Display Information Station is set up and running, it will automatically fetch and display the current weather conditions and bus schedule for the specified route. Users can glance at the display to quickly get the information they need without the need for a smartphone or computer.

