Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

~0.5mA

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?**

~0.49mA

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

No, for currents above 250uA, accuracy is down to +/- 0.01mA, which is the difference seen here.  LED current is determined by the output voltage (3.3V), series resistor (3k ohm), and voltage drop across LED (assumed to be ~1.8V).  Maximum current we would expect to see is 0.5mA based on (3.3V - 1.8V) = I * 3k.  Both Strong and Weak can supply over 0.5mA so we do not expect to see a difference here.

**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

4.48mA

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

4.64mA
