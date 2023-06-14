#include <iostream>
#include <fstream>
#include <string>
#include <SerialStream.h>
#include <thread>

using namespace std;

// Serial port parameters
const string port1 = "/dev/ttyACM0"; // First serial port
const int baudRate = 4800; // Baud rate

// Function to read data from port1 and forward it to port2 and port3

void forwardDataToPorts(LibSerial::SerialStream& serialPort1) {
  char ch;
  const string port2 = "/home/pnovack/GPSTracks/newdata.txt"; // Second serial port
  const string port3 = "/home/pnovack/GPSTracks/Tracts-2-GPS.txt"; // Third serial port
  ofstream serialPort2(port2, ios::app);
  ofstream serialPort3(port3, ios::app);

  if (!serialPort2 || !serialPort3) {
    cout << "Failed to open output files." << endl;
    return;
  }

  while (serialPort1.get(ch)) {
    serialPort2 << ch << flush;
    serialPort3 << ch << flush;
    cout << ch;
  }

  serialPort2.close();
  serialPort3.close();
}

int main() {
  if (false) {
    // Open serial port
    LibSerial::SerialStream serialPort1;
    serialPort1.Open(port1);

    if (!serialPort1.good()) {
      cout << "Failed to open serial port." << endl;
      return 1;
    }

    // Configure serial port parameters
    serialPort1.SetBaudRate(LibSerial::BaudRate::BAUD_4800);
    serialPort1.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    serialPort1.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
    serialPort1.SetParity(LibSerial::Parity::PARITY_NONE);
    serialPort1.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    // Create a thread to forward data from port1 to port2 and port3
    thread forwardThread(forwardDataToPorts, std::ref(serialPort1));

    // Wait for 'q' key to stop the program
    cout << "Press 'q' to quit." << endl;

    char input;
    while (true) {
      cin.get(input);
      if (input == 'q' || input == 'Q') {
        break;
      }
    }

    // Signal the thread to stop
    forwardThread.join();

    // Close serial port
    serialPort1.Close();
  }


  return 0;
}
