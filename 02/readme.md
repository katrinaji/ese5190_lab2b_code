

Update:

The section is done. I am able to select any 32-bit address to read/write (even if not a valid RP2020 address).

![image](https://user-images.githubusercontent.com/114244957/202342964-e104f285-a76f-4b97-a3e5-271f85531245.png)
![image](https://user-images.githubusercontent.com/114244957/202343437-9cee4824-21a9-497b-95dc-4f1693f8fab0.png)

It can read/write any 32-bit value to this address.

![image](https://user-images.githubusercontent.com/114244957/202345899-e1e47492-4f3c-43c9-809e-9747f1dcd0c6.png)

It can also read/write using any of the atomic bit-setting aliases and a 32-bit mask.

![image](https://user-images.githubusercontent.com/114244957/202346122-2e558d43-28ab-4a90-b4a1-f4adf11dde72.png)

Since we are masking the value at 0xd0000004 (value 0) to the new value. The result will still be 0.

First Submission log:

I have successfully manage to put a mask on a register and change the register. The next step will be creating an interactive channel for me to input the register.
