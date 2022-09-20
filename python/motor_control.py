from binascii import unhexlify
import serial
import serial.tools.list_ports
import time
from crcmod import mkCrcFun
# import keyboard

class MotorController:
    def __init__(self, port):
        self.ser = serial.Serial()
        self.port = port
        # 360/~1.8 (angle/nema_step) * 27 gear reduction * 32 microsteps 
        self.full_revolution_steps = 198*27*32
        
    def int_hex2(self, int):
        return '{0:0{1}X}'.format(int, 2)

    def int_hex4(self, int):
        return '{0:0{1}X}'.format(int, 4)

    def int_hex8(self, int):        
        return '{0:0{1}X}'.format(int, 8)

    def int_0x2(self, int):
        return '0x{0:0{1}X}'.format(int, 2)

    def int_0x4(self, int):
        return '0x{0:0{1}X}'.format(int, 4)

    def int_0x8(self, int):
        return '0x{0:0{1}X}'.format(int, 8)

    def crc16_modbus(self, s):
        crc16 = mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
        return self.get_crc_value(s, crc16)

    def str_to_hex(self, hex_string):
        # hex_string = "0xFF"
        an_integer = int(hex_string, 16)
        hex_value = hex(an_integer)
        return an_integer

    def get_crc_value(self, s, crc16):
        data = s.replace(' ', '')
        crc_out = hex(crc16(unhexlify(data))).upper()
        str_list = list(crc_out)
        if len(str_list) == 5:
            str_list.insert(2, '0')  # 位数不足补0
        if len(str_list) == 4:
            str_list.insert(2, '00')  # 位数不足补0
        crc_data = ''.join(str_list[2:])
        # return crc_data[:2] + ' ' + crc_data[2:]
        # return crc_data[2:] + crc_data[:2]
        return self.str_to_hex("0x" + crc_data[2:]), self.str_to_hex("0x" + crc_data[:2])


    def forward_n_step(self, n):
        addr = 1
        func_code = 16
        reg_addr = 2004  # forward
        num_regs = 2
        data_len = 4
        reg_value = n

        data_to_crc = self.int_hex2(addr) + self.int_hex2(func_code) + self.int_hex4(reg_addr) + self.int_hex4(
            num_regs) + self.int_hex2(data_len) + self.int_hex8(reg_value)

        p1, p2 = self.crc16_modbus(data_to_crc)

        data_with_crc = data_to_crc + self.int_hex2(p1) + self.int_hex2(p2)

        mylist = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        for i in range(len(mylist)):
            mylist[i] = self.str_to_hex('0x' + data_with_crc[i * 2:(i + 1) * 2])

        self.ser_send(mylist)


    def backward_n_step(self, n):
        addr = 1
        func_code = 16
        reg_addr = 2006  # backward
        num_regs = 2
        data_len = 4
        reg_value = n

        data_to_crc = self.int_hex2(addr) + self.int_hex2(func_code) + self.int_hex4(reg_addr) + self.int_hex4(
            num_regs) + self.int_hex2(data_len) + self.int_hex8(reg_value)

        p1, p2 = self.crc16_modbus(data_to_crc)

        data_with_crc = data_to_crc + self.int_hex2(p1) + self.int_hex2(p2)

        mylist = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        for i in range(len(mylist)):
            mylist[i] = self.str_to_hex('0x' + data_with_crc[i * 2:(i + 1) * 2])

        self.ser_send(mylist)

    def go_position(self, position):
        addr = 1
        func_code = 16
        reg_addr = 2002  # position mode
        num_regs = 2
        data_len = 4
        reg_value = position

        data_to_crc = self.int_hex2(addr) + self.int_hex2(func_code) + self.int_hex4(reg_addr) + self.int_hex4(
            num_regs) + self.int_hex2(data_len) + self.int_hex8(reg_value)
        # print(data_to_crc)
        p1, p2 = self.crc16_modbus(data_to_crc)

        data_with_crc = data_to_crc + self.int_hex2(p1) + self.int_hex2(p2)
        # print(data_with_crc)
        mylist = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        for i in range(len(mylist)):
            mylist[i] = self.str_to_hex('0x' + data_with_crc[i * 2:(i + 1) * 2])

        self.ser_send(mylist)

    def move_angle(self, angle):
        steps_to_take = round(self.full_revolution_steps/360*angle)
        if angle < 0:
            self.backward_n_step(abs(steps_to_take))
        if angle > 0:
            self.forward_n_step(abs(steps_to_take))

    def reset(self):
        mylist = [0x01, 0x06, 0x07, 0xD0, 0x00, 0x01, 0x48, 0x87]
        self.ser_send(mylist)
        
    def ser_send(self, data):
        ser = self.ser
        ser.port = self.port
        # 9600,N,8,1
        ser.baudrate = 9600
        ser.bytesize = serial.EIGHTBITS  # number of bits per bytes
        ser.parity = serial.PARITY_NONE  # set parity check
        ser.stopbits = serial.STOPBITS_ONE  # number of stop bits
        ser.timeout = 0.5  # non-block read 0.5s
        ser.writeTimeout = 0.5  # timeout for write 0.5s
        ser.xonxoff = False  # disable software flow control
        ser.rtscts = False  # disable hardware (RTS/CTS) flow control
        ser.dsrdtr = False  # disable hardware (DSR/DTR) flow control
        try:
            ser.open()
        except Exception as ex:
            print("open serial port error " + str(ex))
            exit()
        if ser.isOpen():
            try:
                ser.flushInput()  # flush input buffer
                ser.flushOutput()  # flush output buffer
                print('sending:', data)
                ser.write(serial.to_bytes(data))
                time.sleep(0.5)  # wait 0.5s
                # read 8 byte data
                response = ser.read(8)
                print("read 8 byte data:")
                print(response)
                ser.close()
            except Exception as e1:
                print("communicating error " + str(e1))
        else:
            print("open serial port error")

    def list_com_ports():
        ports = serial.tools.list_ports.comports()

        for port, desc, hwid in sorted(ports):
            print("{}: {} [{}]".format(port, desc, hwid))

# ID, Current State, Open, Close



# Also physically label the IDs
if __name__ == "__main__":
    mc = MotorController('COM5')

    while(True):
        # mc.go_position(70)
        mc.move_angle(-133)
        # mc.backward_n_step(mc.full_revolution_steps)
        time.sleep(5)
        mc.move_angle(133)
        time.sleep(5)
    
