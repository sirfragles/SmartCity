EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Smart Waste Bin"
Date "2018-12-06"
Rev "v01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Bao Vu"
$EndDescr
$Comp
L CONN_01X16 P4
U 1 1 5C0977F2
P 5500 5350
F 0 "P4" H 5500 6200 50  0000 C CNN
F 1 "CONN_01X16" V 5600 5350 50  0000 C CNN
F 2 "" H 5500 5350 50  0000 C CNN
F 3 "" H 5500 5350 50  0000 C CNN
	1    5500 5350
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X12 P5
U 1 1 5C09791A
P 5550 3450
F 0 "P5" H 5550 4100 50  0000 C CNN
F 1 "CONN_01X12" V 5650 3450 50  0000 C CNN
F 2 "" H 5550 3450 50  0000 C CNN
F 3 "" H 5550 3450 50  0000 C CNN
	1    5550 3450
	0    1    1    0   
$EndComp
$Comp
L CONN_01X07 P6
U 1 1 5C098828
P 6100 1750
F 0 "P6" H 6100 2150 50  0000 C CNN
F 1 "CONN_01X07" V 6200 1750 50  0000 C CNN
F 2 "" H 6100 1750 50  0000 C CNN
F 3 "" H 6100 1750 50  0000 C CNN
	1    6100 1750
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X03 P8
U 1 1 5C09896E
P 7700 2400
F 0 "P8" H 7700 2600 50  0000 C CNN
F 1 "CONN_01X03" V 7800 2400 50  0000 C CNN
F 2 "" H 7700 2400 50  0000 C CNN
F 3 "" H 7700 2400 50  0000 C CNN
	1    7700 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P2
U 1 1 5C0989E9
P 3000 2300
F 0 "P2" H 3000 2500 50  0000 C CNN
F 1 "CONN_01X03" V 3100 2300 50  0000 C CNN
F 2 "" H 3000 2300 50  0000 C CNN
F 3 "" H 3000 2300 50  0000 C CNN
	1    3000 2300
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 P1
U 1 1 5C098A9A
P 2750 4350
F 0 "P1" H 2750 4600 50  0000 C CNN
F 1 "CONN_01X04" V 2850 4350 50  0000 C CNN
F 2 "" H 2750 4350 50  0000 C CNN
F 3 "" H 2750 4350 50  0000 C CNN
	1    2750 4350
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 P9
U 1 1 5C098B3F
P 8550 4550
F 0 "P9" H 8550 4800 50  0000 C CNN
F 1 "CONN_01X04" V 8650 4550 50  0000 C CNN
F 2 "" H 8550 4550 50  0000 C CNN
F 3 "" H 8550 4550 50  0000 C CNN
	1    8550 4550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P3
U 1 1 5C098EAF
P 3750 2500
F 0 "P3" H 3750 2700 50  0000 C CNN
F 1 "CONN_01X03" V 3850 2500 50  0000 C CNN
F 2 "" H 3750 2500 50  0000 C CNN
F 3 "" H 3750 2500 50  0000 C CNN
	1    3750 2500
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X02 P10
U 1 1 5C0997BB
P 8650 3350
F 0 "P10" H 8650 3500 50  0000 C CNN
F 1 "CONN_01X02" V 8750 3350 50  0000 C CNN
F 2 "" H 8650 3350 50  0000 C CNN
F 3 "" H 8650 3350 50  0000 C CNN
	1    8650 3350
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P7
U 1 1 5C09A786
P 6800 4400
F 0 "P7" H 6800 4600 50  0000 C CNN
F 1 "CONN_01X03" V 6900 4400 50  0000 C CNN
F 2 "" H 6800 4400 50  0000 C CNN
F 3 "" H 6800 4400 50  0000 C CNN
	1    6800 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 5550 6250 5650
Wire Wire Line
	6250 5650 6400 5650
Wire Wire Line
	6400 5650 6400 3100
Wire Wire Line
	6400 3100 5900 3100
Wire Wire Line
	5900 3100 5900 3250
Wire Wire Line
	7000 4300 7100 4300
Wire Wire Line
	7100 4300 7100 3000
Wire Wire Line
	7100 3000 5800 3000
Wire Wire Line
	5800 3000 5800 3250
Wire Wire Line
	5800 1950 5800 2550
Wire Wire Line
	5800 2550 4300 2550
Wire Wire Line
	4300 2550 4300 5950
Wire Wire Line
	2350 5950 5050 5950
Wire Wire Line
	5050 5950 5050 5550
Wire Wire Line
	5900 1950 5900 2650
Wire Wire Line
	5900 2650 4500 2650
Wire Wire Line
	4500 2650 4500 5800
Wire Wire Line
	2150 5800 4850 5800
Wire Wire Line
	6000 1950 6000 3250
Wire Wire Line
	6100 1950 6100 3250
Wire Wire Line
	6200 1950 6200 2850
Wire Wire Line
	6200 2850 6500 2850
Wire Wire Line
	6500 2850 6500 5750
Wire Wire Line
	6500 5750 6150 5750
Wire Wire Line
	6150 5750 6150 5550
Wire Wire Line
	6300 1950 6300 2700
Wire Wire Line
	6300 2700 6650 2700
Wire Wire Line
	6650 2700 6650 5900
Wire Wire Line
	6650 5900 6050 5900
Wire Wire Line
	6050 5900 6050 5550
Wire Wire Line
	6400 1950 6400 2550
Wire Wire Line
	6400 2550 7250 2550
Wire Wire Line
	7250 2550 7250 6050
Wire Wire Line
	7250 6050 5950 6050
Wire Wire Line
	5950 6050 5950 5550
Wire Wire Line
	3950 2400 4050 2400
Wire Wire Line
	4050 2400 4050 5800
Connection ~ 4500 5800
Wire Wire Line
	3950 2600 4200 2600
Wire Wire Line
	4200 2600 4200 3100
Wire Wire Line
	4200 3100 5400 3100
Wire Wire Line
	5400 3100 5400 3250
Wire Wire Line
	2950 4200 5600 4200
Wire Wire Line
	5600 4200 5600 3700
Wire Wire Line
	5600 3700 8200 3700
Wire Wire Line
	8200 3700 8200 3300
Wire Wire Line
	8200 3300 8450 3300
Wire Wire Line
	2950 4500 5750 4500
Wire Wire Line
	5750 4500 5750 3850
Wire Wire Line
	5750 3850 8350 3850
Wire Wire Line
	8350 3850 8350 3400
Wire Wire Line
	8350 3400 8450 3400
Wire Wire Line
	2950 4300 4650 4300
Wire Wire Line
	4650 4300 4650 3000
Wire Wire Line
	4650 3000 5600 3000
Wire Wire Line
	5600 3000 5600 3250
Wire Wire Line
	2950 4400 3950 4400
Wire Wire Line
	3950 4400 3950 2850
Wire Wire Line
	3950 2850 5500 2850
Wire Wire Line
	5500 2850 5500 3250
Wire Wire Line
	3200 2200 5700 2200
Wire Wire Line
	5700 2200 5700 3250
Wire Wire Line
	3200 2300 3500 2300
Wire Wire Line
	3500 2300 3500 5800
Connection ~ 4050 5800
Wire Wire Line
	3200 2400 3200 5950
Connection ~ 4300 5950
Wire Wire Line
	6950 2300 7500 2300
Wire Wire Line
	6950 1250 6950 2300
Wire Wire Line
	6950 1250 2350 1250
Wire Wire Line
	2350 1250 2350 5950
Connection ~ 3200 5950
Wire Wire Line
	7500 2400 6850 2400
Wire Wire Line
	6850 2400 6850 1400
Wire Wire Line
	6850 1400 2150 1400
Wire Wire Line
	2150 1400 2150 5800
Connection ~ 3500 5800
Wire Wire Line
	7500 2500 7400 2500
Wire Wire Line
	7400 2500 7400 1050
Wire Wire Line
	7400 1050 1750 1050
Wire Wire Line
	1750 1050 1750 6150
Wire Wire Line
	1750 6150 5150 6150
Wire Wire Line
	5150 6150 5150 5550
Wire Wire Line
	8350 4500 7900 4500
Wire Wire Line
	7900 4500 7900 6350
Wire Wire Line
	7900 6350 5850 6350
Wire Wire Line
	5850 6350 5850 5550
Wire Wire Line
	8350 4600 8150 4600
Wire Wire Line
	8150 4600 8150 6450
Wire Wire Line
	8150 6450 5750 6450
Wire Wire Line
	5750 6450 5750 5550
Wire Wire Line
	4850 5800 4850 5550
Wire Wire Line
	8350 4400 8000 4400
Wire Wire Line
	8000 4400 8000 3850
Connection ~ 8000 3850
Wire Wire Line
	8350 4700 7700 4700
Wire Wire Line
	7700 4700 7700 3700
Connection ~ 7700 3700
$EndSCHEMATC
