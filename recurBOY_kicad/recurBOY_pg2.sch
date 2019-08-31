EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:RCJ-024
LIBS:recurBOY-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SW_Push_Dual SW1
U 1 1 5D4ECF3B
P 7125 3700
F 0 "SW1" H 7175 3800 50  0000 L CNN
F 1 "BUTTON_1" H 7125 3430 50  0000 C CNN
F 2 "promicro:SW_TAC_12mm" H 7125 3900 50  0001 C CNN
F 3 "" H 7125 3900 50  0001 C CNN
	1    7125 3700
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x03_Top_Bottom J9
U 1 1 5D4EDBCC
P 6750 2675
F 0 "J9" H 6800 2875 50  0000 C CNN
F 1 "BUTTON_NAV" H 6825 2350 50  0000 C CNN
F 2 "custom_new:BUT_5_WAY" H 6750 2675 50  0001 C CNN
F 3 "" H 6750 2675 50  0001 C CNN
	1    6750 2675
	1    0    0    -1  
$EndComp
Text GLabel 7175 2550 2    60   Input ~ 0
BUTTON_NAV_IN_B
Text GLabel 7175 2700 2    60   Input ~ 0
BUTTON_NAV_IN_C
Text GLabel 7175 2825 2    60   Input ~ 0
BUTTON_NAV_IN_D
Text GLabel 7175 2950 2    60   Input ~ 0
BUTTON_NAV_IN_E
Text GLabel 7175 2425 2    60   Input ~ 0
BUTTON_NAV_IN_A
Wire Wire Line
	5950 2575 6550 2575
Wire Wire Line
	7050 2575 7050 2425
Wire Wire Line
	7050 2425 7175 2425
Wire Wire Line
	7050 2675 7100 2675
Wire Wire Line
	7100 2675 7100 2550
Wire Wire Line
	7100 2550 7175 2550
Wire Wire Line
	7050 2775 7150 2775
Wire Wire Line
	7150 2775 7150 2700
Wire Wire Line
	7150 2700 7175 2700
Wire Wire Line
	6550 2775 6550 2875
Wire Wire Line
	6550 2875 7050 2875
Wire Wire Line
	7050 2875 7050 2825
Wire Wire Line
	7050 2825 7175 2825
Wire Wire Line
	6550 2675 6500 2675
Wire Wire Line
	6500 2675 6500 2950
Wire Wire Line
	6500 2950 7175 2950
Text GLabel 7650 3700 2    60   Input ~ 0
BUTTON_IN_1
Wire Wire Line
	6525 3700 6925 3700
Wire Wire Line
	7325 3700 7650 3700
$Comp
L SW_Push_Dual SW2
U 1 1 5D567768
P 7125 4250
F 0 "SW2" H 7175 4350 50  0000 L CNN
F 1 "BUTTON_2" H 7125 3980 50  0000 C CNN
F 2 "promicro:SW_TAC_12mm" H 7125 4450 50  0001 C CNN
F 3 "" H 7125 4450 50  0001 C CNN
	1    7125 4250
	1    0    0    -1  
$EndComp
Text GLabel 7650 4250 2    60   Input ~ 0
BUTTON_IN_2
Wire Wire Line
	6525 4250 6925 4250
Wire Wire Line
	7325 4250 7650 4250
$Comp
L SW_Push_Dual SW3
U 1 1 5D5677DC
P 7125 4800
F 0 "SW3" H 7175 4900 50  0000 L CNN
F 1 "BUTTON_3" H 7125 4530 50  0000 C CNN
F 2 "promicro:SW_TAC_12mm" H 7125 5000 50  0001 C CNN
F 3 "" H 7125 5000 50  0001 C CNN
	1    7125 4800
	1    0    0    -1  
$EndComp
Text GLabel 7650 4800 2    60   Input ~ 0
BUTTON_IN_3
Wire Wire Line
	6525 4800 6925 4800
Wire Wire Line
	7325 4800 7650 4800
$Comp
L SW_Push_Dual SW4
U 1 1 5D5677ED
P 7125 5350
F 0 "SW4" H 7175 5450 50  0000 L CNN
F 1 "BUTTON_4" H 7125 5080 50  0000 C CNN
F 2 "promicro:SW_TAC_12mm" H 7125 5550 50  0001 C CNN
F 3 "" H 7125 5550 50  0001 C CNN
	1    7125 5350
	1    0    0    -1  
$EndComp
Text GLabel 7650 5350 2    60   Input ~ 0
BUTTON_IN_4
Wire Wire Line
	6525 5350 6925 5350
Wire Wire Line
	7325 5350 7650 5350
$Comp
L GND #PWR030
U 1 1 5D5AEBFA
P 5950 2675
F 0 "#PWR030" H 5950 2425 50  0001 C CNN
F 1 "GND" H 5950 2525 50  0000 C CNN
F 2 "" H 5950 2675 50  0001 C CNN
F 3 "" H 5950 2675 50  0001 C CNN
	1    5950 2675
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2575 5950 2675
$Comp
L GND #PWR031
U 1 1 5D5AECB4
P 6525 5450
F 0 "#PWR031" H 6525 5200 50  0001 C CNN
F 1 "GND" H 6525 5300 50  0000 C CNN
F 2 "" H 6525 5450 50  0001 C CNN
F 3 "" H 6525 5450 50  0001 C CNN
	1    6525 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 5D5AECD2
P 6525 4900
F 0 "#PWR032" H 6525 4650 50  0001 C CNN
F 1 "GND" H 6525 4750 50  0000 C CNN
F 2 "" H 6525 4900 50  0001 C CNN
F 3 "" H 6525 4900 50  0001 C CNN
	1    6525 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 5D5AECF8
P 6525 4350
F 0 "#PWR033" H 6525 4100 50  0001 C CNN
F 1 "GND" H 6525 4200 50  0000 C CNN
F 2 "" H 6525 4350 50  0001 C CNN
F 3 "" H 6525 4350 50  0001 C CNN
	1    6525 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR034
U 1 1 5D5AED12
P 6525 3800
F 0 "#PWR034" H 6525 3550 50  0001 C CNN
F 1 "GND" H 6525 3650 50  0000 C CNN
F 2 "" H 6525 3800 50  0001 C CNN
F 3 "" H 6525 3800 50  0001 C CNN
	1    6525 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6525 3700 6525 3800
Wire Wire Line
	6525 4250 6525 4350
Wire Wire Line
	6525 4800 6525 4900
Wire Wire Line
	6525 5350 6525 5450
NoConn ~ 7325 5550
NoConn ~ 6925 5550
NoConn ~ 6925 5000
NoConn ~ 7325 5000
NoConn ~ 7325 4450
NoConn ~ 6925 4450
NoConn ~ 6925 3900
NoConn ~ 7325 3900
$Comp
L SW_Push_Dual SW5
U 1 1 5D5B1CE4
P 7100 5975
F 0 "SW5" H 7150 6075 50  0000 L CNN
F 1 "BUTTON_5" H 7100 5705 50  0000 C CNN
F 2 "promicro:SW_TAC_12mm" H 7100 6175 50  0001 C CNN
F 3 "" H 7100 6175 50  0001 C CNN
	1    7100 5975
	1    0    0    -1  
$EndComp
Text GLabel 7625 5975 2    60   Input ~ 0
BUTTON_IN_5
Wire Wire Line
	6500 5975 6900 5975
Wire Wire Line
	7300 5975 7625 5975
$Comp
L GND #PWR035
U 1 1 5D5B1CED
P 6500 6075
F 0 "#PWR035" H 6500 5825 50  0001 C CNN
F 1 "GND" H 6500 5925 50  0000 C CNN
F 2 "" H 6500 6075 50  0001 C CNN
F 3 "" H 6500 6075 50  0001 C CNN
	1    6500 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5975 6500 6075
NoConn ~ 7300 6175
NoConn ~ 6900 6175
$EndSCHEMATC
