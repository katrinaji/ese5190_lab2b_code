#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "register.h"
#include <stdio.h>
#include <stdlib.h>

#define BOOT_PIN 21
#define address_len 8

ADDRESS hex_address;
VALUE value;
VALUE mask;
VALUE mask_value;

// Function to write, store and transform the input to an address format
ADDRESS InputAddress(){
    int counter = 0;
    char input_address[30];

    while(1){
        while (counter < address_len){ // Stop when counter value is 7
            char letter = getchar_timeout_us(0);
            if ((letter >= 0x20) && (letter <= 0x7E)){ // ASCII valid value for address: A-Z, 0-9
                printf("%c", letter);
                input_address[counter] = letter;
                counter ++;
            }

            // Some useful special cases:
            if (letter == 0x0D){ // "Return" key on Mac keyboard
                if (counter != address_len -1){
                    printf("\nInput address length is wrong.\n");
                    printf("Press RST and re-run the program.\n");
                    break;
                }
            }

            if (letter == 0x7F){ // "Delete" key on Mac keyboard
                printf("\b");
                counter = counter -1;
            }
        }

        // At this point the address is collected
        char letter = getchar_timeout_us(0);
        if (letter == 0x0D){ // "Return" key on Mac keyboard
            //Transform the array to a 32-bit address in hex form
            hex_address = (long int) strtoul(input_address, NULL, 16); // hexidecimal base is 16
            printf("\nThe input hex form address is %lx", hex_address);
        }

        if (gpio_get(BOOT_PIN) == 0){ // Out of WRITE MODE when button is pressed
            printf("\nExit WRITE ADDRESS MODE.\n");
            break;
        }   
    }
    return hex_address;
}

void WriteValue(ADDRESS address){
    char input_value[30];
    int idx = 0;

    while(1){
        char input = getchar_timeout_us(0);
        
        if ((input >= 0x20) && (input <= 0x7E)){ // ASCII valid value for address: A-Z, 0-9
            printf("%c", input);
            input_value[idx] = input;
            idx ++;
        }

        if (input == 0x7F){ // "Delete" key on Mac keyboard
            printf("\b");
            idx = idx -1 ;
        }
        
        if (input == 0x0D){ // "Return" key on Mac keyboard)
            printf("\nInput ends.\n");
            break;
        }
    }

    VALUE value = (long int) strtoul(input_value, NULL, 16); // hexidecimal base is 16
    printf("The new value at the address is %lx.\n", value);
    void register_write(ADDRESS address, VALUE value);
}


VALUE ApplyMask(ADDRESS address){
    char input_mask[30];
    int counter = 0;

    while(1){
        while (counter < address_len){ // Stop when counter value is 7
            char letter = getchar_timeout_us(0);
            if ((letter >= 0x20) && (letter <= 0x7E)){ // ASCII valid value for address: A-Z, 0-9
                printf("%c", letter);
                input_mask[counter] = letter;
                counter ++;
            }

            // Some useful special cases:
            if (letter == 0x0D){ // "Return" key on Mac keyboard
                if (counter != address_len -1){
                    printf("\nInput address length is wrong.\n");
                    printf("Press RST and re-run the program.\n");
                    break;
                }
            }

            if (letter == 0x7F){ // "Delete" key on Mac keyboard
                printf("\b");
                counter = counter -1;
            }
        }
        
        char letter = getchar_timeout_us(0);
        if (letter == 0x0D){ // "Return" key on Mac keyboard)
            printf("\nInput ends.\n");
            break;
        }
    }
        
        VALUE mask = (long int) strtoul(input_mask, NULL, 16); // hexidecimal base is 16
        mask_value = mask & value;
        printf("The new value after mask is %lx.\n", mask_value);
        
        return mask_value;
}

int main(){
    stdio_init_all();
    gpio_init(BOOT_PIN);
    gpio_set_dir(BOOT_PIN, GPIO_IN);

    while(stdio_usb_connected()!=true); // Only start when USB is connected
    
    //Input target address
    printf("WRITE ADDRESS MODE. Enter the address you want to read/write.\n");
    printf("To exit WRITE ADDRESS MODE, simply press the button.\n");
    InputAddress(); //return a (long int) hex_address
   
    VALUE readneed_address_value = register_read(hex_address);
    printf("The value of the address is %x\n", readneed_address_value);
    
    //Adding value
    printf("Entering WRITE MODE. Enter the hex value you want to re-write to the address.\n");
    printf("To exit WRITE MODE, simply press the button.\n");
    WriteValue(hex_address);

    //Adding Mask
    printf("Entering MASK MODE. Enter the hex form MASK you want to apply to the address.\n");
    printf("To exit MASK MODE, simply press the button.\n");
    ApplyMask(hex_address);
    
    return 0;

}
