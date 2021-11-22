# Stop-Watch-using-AVR-
ATmega16 Microcontroller with frequency 1Mhz.
Timer1 in ATmega16 with CTC mode to count the Stop Watch time.
six common anode 7-segments.
7447 decoder
Stop Watch counting should start once the power is connected to the MCU.
External Interrupt INT0 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be reset.
External Interrupt INT1 with raising edge. Connect a push button with the external pull-down resistor. If a raising edge detected the Stop Watch time should be paused.
 External Interrupt INT2 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be resumed.
 
