## Working
After the system is powered, it is ready for use. The buyer inserts a coin into the coin acceptor. The coin acceptor is programmed to determine the type of coin that has been inserted. 

The value of the coin is then sent to the microcontroller and using pre-callibrated values, the volume of oil(liquid) that can be bought using that coin is calculated by a function in the arduino code.

The controller then waits for an input signal from the ultrasonic sensor to know that a container has been placed at the dispensing point. Once this signal is established, the controller turns on the pump in the reservoir.

### Flow meter
The output hose from the  pump is connected to a flow meter which is then connected to another flow hose to the container. The flow meter uses Hall effect. Whenever a liquid flows through the valve it generates pulses. All we have to do is to measure the time for the pulses or to count the number of pulses in 1 second and then calculate the flow rate in liter per hour (l/hr). We then use a simple conversion formula to find the volume of the liquid which has passed through it. So, we continously run the pump until the volume that has passed through the flow meter is equal to the volume that was calculated from the coin value. Meanwhile, while doing this, we turn off the coin acceptor to prevent another coin from getting acccepted until the current operation is stopped.
