# ESP8266 Google Fake Wi-Fi Captive Portal

This project demonstrates how to create a fake Wi-Fi captive portal using the ESP8266 microcontroller. It sets up a Wi-Fi network with a fake sign-in page that captures user credentials and displays them. This example is intended for educational purposes to showcase the ESP8266’s capabilities in handling web requests and Wi-Fi management.

## Features

- **Captive Portal**: Redirects users to a sign-in page when they connect to the ESP8266 Wi-Fi network.
- **Fake Sign-In Page**: Collects user email and password.
- **Credential Display**: Shows a list of captured credentials on a separate page.
- **Clear Functionality**: Allows resetting of the captured credentials list.
- **LED Indicator**: Blinks the onboard LED when credentials are submitted.

## Requirements

- **Hardware**: ESP8266 microcontroller (e.g., NodeMCU, Wemos D1 Mini)
- **Software**: Arduino IDE with ESP8266 board support

## Installation

1. **Install Arduino IDE**: Download from [Arduino's official website](https://www.arduino.cc/en/software).
2. **Add ESP8266 Board**:
   - Open Arduino IDE and go to `File` > `Preferences`.
   - Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the `Additional Boards Manager URLs` field.
   - Go to `Tools` > `Board` > `Boards Manager`, search for `ESP8266`, and install it.
3. **Upload Code**:
   - Connect your ESP8266 to your computer.
   - In Arduino IDE, select the appropriate board and port (`Tools` > `Board` and `Tools` > `Port`).
   - Copy and paste the code into a new Arduino sketch.
   - Click `Upload` to upload the code to your ESP8266.

## Usage

1. **Power On**: After uploading the code, power the ESP8266.
2. **Connect to Wi-Fi**: Connect to the Wi-Fi network named "Google Free WiFi".
3. **Access Captive Portal**: Open a web browser to be redirected to the fake sign-in page.
4. **Submit Credentials**: Enter an email and password. The credentials will be displayed on the `/pass` page.
5. **View Credentials**: Navigate to `/pass` to see captured credentials.
6. **Clear Credentials**: Navigate to `/clear` to reset the list of captured credentials.

## Disclaimer

This project is intended for educational use only. Unauthorized use for malicious purposes is illegal and unethical. Ensure you have permission before deploying similar systems.

## License

This project is based on the work by Mrwhitehat1 available at [https://github.com/Mrwhitehat1](https://github.com/Mrwhitehat1). Please refer to the original repository for licensing details.

## Author

Prajwal Gaikwad  
[prajwalgaikwad800@gmail.com](mailto:prajwalgaikwad800@gmail.com)

## Acknowledgements

- Thanks to Mrwhitehat1 for the foundational work and inspiration.
