import time
import serial
import io


# Initialized system and connect qtpy2040 with tty
ser = serial.Serial(
    port="/dev/tty.usbmodem101",  # input("please input your device path\n"),
    baudrate=115200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS,
)


def main():

    action = input("Read BOOT_PIN Press r;\nWrite Data Back to NioPixel Press w\n")
    actions = ["r", "w"]

    if action not in actions:
        print("Pleas enter only lower case r or w\n")
    elif action in actions:
        # read the data from tty
        if action == "r":
            speed = input("select speed,1X, 2x OR 0.5X\n")
            read_tty(speed)
        elif action == "w":
            speed = input("select speed, 1X, 2x OR 0.5X\n")
            write_tty(speed)


# record data from tty to sequencer.txt
def read_tty(speeds):
    speed = speeds
    print("Start to record the data from boot pin")

    # set read mode
    ser.write(bytearray("r", "ascii"))
    # create an array to store the data
    data_array = []
    start_time = time.time()
    # duration = int(input("record duration"))

    while True:
        # read the secquencer from terminal
        secquencer = ser.readline()
        # print(str(secquencer))

        # if I print 1 the format is b'1\r\n'
        # hence choose from 2 to -5 is our target data, but not include the 2 position
        # note that array is starting from 0
        secquencer = str(secquencer)[2:-5]

        # split the inner data by comma ","
        # row_data = secquencer.split(',')

        # combina the data to the new array
        print(secquencer)
        # print(data_array)

        # read for 10s
        if time.time() - start_time >= 5:
            if speed == '2X':
                data_array = up_motion(data_array)
            elif speed == '0.5X':
                data_array = slow_motion(data_array)
            print("Transmit the data into file")
            ser.write(bytearray("s", "ascii"))
            with open("sequencer.txt", "w") as file:
                for line in data_array:
                    file.write(f"{line}\n")
            ser.close()
            break
        else:
            data_array.append(secquencer)



def write_tty(speeds):
    speed = speeds
    data_write = []
    index = 0
    print("Start to display")
    ser.write(bytearray("w", "ascii"))
    # checking the connecting status
    while True:
        status = ser.readline()
        # print(str(status[1:-5]))
        if status == b"OK\r\n":
            print("Start to write back data")
            break

    with open("sequencer.txt", "r") as file:
        for line in file:
            data_write += line.rstrip()
        if speed == '1X':
            None
        elif speed == '2X':
            data_write = up_motion(data_write)
        elif speed == '0.5X':
            data_write = slow_motion(data_write)
        for index in range(len(data_write)):
            if data_write[index] == "0":
                ser.write(bytearray("0", "ascii"))
            elif data_write[index] == "1":
                ser.write(bytearray("1", "ascii"))
            time.sleep(0.05)
    ser.close()

def slow_motion(data_arrays):
    two_times_data = []
    for data in data_arrays:
        two_times_data.append(data)
        two_times_data.append(data)
    return two_times_data
    
def up_motion(data_arrays):
    half_data = []
    for index in range(len(data_arrays)):
        if index % 2 == 0:
            half_data.append(data_arrays[index])
    return half_data


if __name__ == "__main__":
    main()
