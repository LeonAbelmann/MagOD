EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector_Generic:Conn_01x04 J2
U 1 1 5BDDA98F
P 4850 4300
F 0 "J2" H 4930 4292 50  0000 L CNN
F 1 "Conn_01x04" H 4930 4201 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04_Pitch2.54mm" H 4850 4300 50  0001 C CNN
F 3 "~" H 4850 4300 50  0001 C CNN
	1    4850 4300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5BDDA9E0
P 4850 3800
F 0 "J1" H 4930 3792 50  0000 L CNN
F 1 "Conn_01x04" H 4930 3701 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04_Pitch2.54mm" H 4850 3800 50  0001 C CNN
F 3 "~" H 4850 3800 50  0001 C CNN
	1    4850 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5BDDBA4D
P 5300 2900
F 0 "R1" V 5093 2900 50  0000 C CNN
F 1 "R" V 5184 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5230 2900 50  0001 C CNN
F 3 "~" H 5300 2900 50  0001 C CNN
	1    5300 2900
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0101
U 1 1 5BDDBB51
P 5750 2900
F 0 "#PWR0101" H 5750 2750 50  0001 C CNN
F 1 "+5V" V 5765 3028 50  0000 L CNN
F 2 "" H 5750 2900 50  0001 C CNN
F 3 "" H 5750 2900 50  0001 C CNN
	1    5750 2900
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5BDDBE65
P 4650 4500
F 0 "#PWR0102" H 4650 4350 50  0001 C CNN
F 1 "+5V" V 4665 4628 50  0000 L CNN
F 2 "" H 4650 4500 50  0001 C CNN
F 3 "" H 4650 4500 50  0001 C CNN
	1    4650 4500
	0    -1   -1   0   
$EndComp
Text Label 5150 2900 2    50   ~ 0
G
Text Label 4650 4300 2    50   ~ 0
G
$Comp
L Device:LED D1
U 1 1 5CEFA5CE
P 5600 2900
F 0 "D1" H 5591 3116 50  0000 C CNN
F 1 "LED" H 5591 3025 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 5600 2900 50  0001 C CNN
F 3 "~" H 5600 2900 50  0001 C CNN
	1    5600 2900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
