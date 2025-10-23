# Sign Language Translation System Based on Deep Learning
**The main function is in the application file directory**

This is a system based on the RT-Thread operating system using the STM32H750 as the main controller. It utilizes the RT-AK deep learning framework to encapsulate a trained sign language translation model into a library, which is embedded into the system to achieve sign language translation, with outputs visualized on a screen and also broadcast as speech. The system also features a function for the hearing-impaired to obtain the base station location in an emergency and send an alert message to emergency contacts. I am responsible for all kernel system development, including management of translation result objects, human-computer interaction, synchronization mechanisms, design of the RF module, and non-volatile storage.
### Object Management:
Uses a hash linked list to manage allocation with 50 initial page structure pointers. Each page stores object structures, which are inserted at the tail of the node in a doubly circular linked list. Space is allocated dynamically, and objects are displayed by traversing the page header nodes to achieve visualization. 
### Human-Computer Interaction:
Uses dual-channel ADC and one pull-up input GPIO to implement joystick-controlled multi-level menus, such as page turning and exiting the translation interface, changing emergency contacts, and deciding whether to send warning messages. Compared to buttons, it saves more interfaces.
### Synchronization mechanism:
Coordinate the operation of various modules using event sets, semaphores, mailboxes, etc., and implement logical operations with different priorities.
### RF module:
Communicate with the RF module via serial communication to achieve voice broadcast, obtain base station positioning, and use onboard phone number encoding and PDU encoding to send Chinese text messages. Power-off storage: Practices with SD card mounting, W25Q128, and on-chip FLASH were all successful; finally, SPI flash W25Q128 was chosen to store contact phone numbers.
