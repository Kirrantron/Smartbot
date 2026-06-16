# SmartBot : A bluetooth controlled grass cutting machine using LPC2129  and HC-05 module (SMARTBOT)

An ARM7-based smart robotic system designed for wireless navigation and remote blade control. Powered by the NXP LPC2129 microcontroller, this robot interfaces with a custom Android Bluetooth application to execute mobility commands while featuring a high-priority hardware interrupt system to instantly toggle a cutting blade mechanism for safety and control.

---

## 🚀 Features

- **Wireless Control:** Full motion control (Forward, Backward, Left, Right) managed remotely via an Arduino Bluetooth Controller mobile app.
- **Persistent Status Display:** A 16x2 character LCD dynamically updates the current operational status with a smooth, scrolling initialization welcome screen.
- **Hardware Interrupt Safety:** Uses the `EINT0` external interrupt to instantly toggle the cutting blade mechanism (simulated via LED indicators) independently of the main firmware loop.
- **Dedicated Driver System:** Utilizes an L293D H-Bridge motor driver to handle the high-current demands of dual DC motors safely.

---

## 🛠️ Hardware Components

| Component | Description |
| :--- | :--- |
| **Microcontroller** | NXP LPC2129 (ARM7TDMI-S Core) |
| **Bluetooth Module** | HC-05 (Configured for UART Communication at 9600 Baud) |
| **Motor Driver** | L293D Push-Pull H-Bridge |
| **Actuators** | High-Torque DC Motors (for locomotion) |
| **Display Module** | HD44780-compliant 16x2 Character LCD |
| **Controller Interface** | Android Smartphone running *Arduino Bluetooth Controller* |
| **Safety Indicator** | LED (Simulating the Blade Motor Relay State) |

---

## 📌 Pin Mapping & Configuration

### 1. Liquid Crystal Display (LCD)
The LCD is operated in **8-bit parallel interface mode**:
- **Data Lines (DB0 - DB7):** `P0.2` to `P0.9` (`0xFF << 2`)
- **Enable (E):** `P0.10`
- **Register Select (RS):** `P0.11`

### 2. L293D Motor Driver Connections
- **Motor 1 & 2 Inputs:** `P0.12`, `P0.13`, `P0.14`, `P0.15`

### 3. UART0 (HC-05 Bluetooth Module)
- **TXD0 / RXD0:** `P0.0` and `P0.1` configured via `PINSEL0`

### 4. Blade Interrupt Mechanism
- **External Interrupt 0 (EINT0):** `P0.16` (Configured via `PINSEL1`)
- **Blade Status Indicator (LED0):** `P0.17`

---

## 💻 Firmware Architecture

The firmware is written in embedded C targeting the LPC2129 architecture. It is structured into distinct modular blocks:

- **`ext_int0_isr()`**: Fast Vector Interrupt Service Routine (`__irq`) assigned to Slot 0. It instantly intercepts standard program execution when the `EINT0` pin is triggered to flip the blade's operational state (`start ^= 1`).
- **UART Communication Pipeline**: Configured with a `9600` Baud Rate (`U0DLL = 97`) and `8N1` frame format to continuously parse incoming serial character streams.
- **Precision Delay Engine**: Leverages the internal **Timer 0 (`T0`)** register block to generate precise millisecond timing delays required for steady LCD signaling and motor stride intervals.

---

## 📱 Bluetooth Remote Mapping

The robot listens for standard character packets terminating with an `'S'` wrapper packet string format:

* 🔼 **`F`** $\rightarrow$ Move Forward
* 🔽 **`B`** $\rightarrow$ Move Backward
* ◀️ **`L`** $\rightarrow$ Pivot Left
* ▶️ **`R`** $\rightarrow$ Pivot Right

---

## 🛠️ How to Compile & Flash

1. **Software Framework:** Open the project file in **Keil uVision IDE**.
2. **Target Setup:** Ensure the target selection is configured for the **LPC2129** device under the ARM compiler toolchain.
3. **Build:** Click `Build Target (F7)` to output the production `.hex` binary file.
4. **Flashing:** Use **Flash Magic** utility or an appropriate JTAG debugger to interface with the LPC2129 bootloader configuration over an RS-232 serial interface to write the flash memory.

---

## 👨‍💻 Author
Designed and developed by **Kirran Vignesh**.
