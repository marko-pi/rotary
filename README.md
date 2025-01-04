# Yet another algorithm for rotary encoder control

Two innovative algorithms for rotary encoder control are presented. The detailed description of the algorithms is [here](https://www.pinteric.com/rotary.html).

## Four state algorithm

The four state algorithm uses interrupts to track state changes and affirms a rotation if all four states of the full cycle have been detected in the correct order. The code is very compact.

## Two state algorithm

The two state algorithm uses interrupts to detect the neutral and inverse states and affims a rotation when both states have been reached by the correct sequence of switch changes.  Although the code is more extensive, it is probably the optimal algorithm when it comes to reducing the computational load - up to a maximum of six interrupts are required to detect a complete cycle in the presence of switch bouncing.
