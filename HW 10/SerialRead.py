#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial
import matplotlib.pyplot as plt

def main():
    ser = serial.Serial('/dev/ttyACM0',9600)
    ser.write('r')
    raw = []
    maf = []
    iir = []
    fir = []
    
    for i in range(0, 100):
        serial_vals = ser.readline().split()
        raw.append(serial_vals[1])
        maf.append(serial_vals[2])
        iir.append(serial_vals[3])
        fir.append(serial_vals[4])
        
    ser.close()
    
    #plot
    plt.plot(raw, label='raw')
    plt.plot(maf, label='maf')
    plt.plot(iir, label='iir')
    plt.plot(fir, label='fir')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main()
