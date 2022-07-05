EESchema Schematic File Version 4
LIBS:RGBLed_MDversion-cache
EELAYER 29 0
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
P 5300 2750
F 0 "R1" V 5093 2750 50  0000 C CNN
F 1 "102R" V 5184 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5230 2750 50  0001 C CNN
F 3 "" H 5300 2750 50  0001 C CNN
F 4 "2407235" H 0   0   50  0001 C CNN "Article number"
	1    5300 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 2900 5525 2900
Wire Wire Line
	5450 3100 5525 3100
$Comp
L power:+5V #PWR0101
U 1 1 5BDDBB51
P 5925 3100
F 0 "#PWR0101" H 5925 2950 50  0001 C CNN
F 1 "+5V" V 5940 3228 50  0000 L CNN
F 2 "" H 5925 3100 50  0001 C CNN
F 3 "" H 5925 3100 50  0001 C CNN
	1    5925 3100
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
Text Label 5150 3100 2    50   ~ 0
G
Text Label 5150 3450 2    50   ~ 0
B
Text Label 4650 4200 2    50   ~ 0
R
Text Label 4650 4300 2    50   ~ 0
G
Text Label 4650 4400 2    50   ~ 0
B
Text Notes 2950 3150 0    50   ~ 0
Forward voltages for 20 mA:\nRed 2.2 V (Drop 2.8V@27mA=102 Ohm) Max 30 mA\nGreen 3.3V (Drop 1.7V@28mA=60.4 Ohm) Max 25 mA\nBlue 3.3 V (Drop 1.5V@30mA=56 Ohm) Max 30 mA
$Comp
L Device:R R2
U 1 1 5BDDBAB0
P 5300 3100
F 0 "R2" V 5093 3100 50  0000 C CNN
F 1 "60R4" V 5184 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5230 3100 50  0001 C CNN
F 3 "" H 5300 3100 50  0001 C CNN
F 4 "303433" H 0   0   50  0001 C CNN "Article number"
	1    5300 3100
	0    1    1    0   
$EndComp
Text Label 5150 2750 2    50   ~ 0
R
Wire Wire Line
	5450 2750 5450 2900
$Comp
L Device:R R3
U 1 1 5BDDBADA
P 5300 3450
F 0 "R3" V 5093 3450 50  0000 C CNN
F 1 "56R" V 5184 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5230 3450 50  0001 C CNN
F 3 "" H 5300 3450 50  0001 C CNN
F 4 "2303433" H 0   0   50  0001 C CNN "Article number"
	1    5300 3450
	0    1    1    0   
$EndComp
$Comp
L Device:LED_RABG D1
U 1 1 5BDDB818
P 5725 3100
F 0 "D1" H 5725 3597 50  0000 C CNN
F 1 "LED_RABG" H 5725 3506 50  0000 C CNN
F 2 "KingbrightRGB:L-154A4SUREQBFZGEW" H 5725 3050 50  0001 C CNN
F 3 "" H 5725 3050 50  0001 C CNN
F 4 "Mouser 604-WP154A43VBDZGCCA " H 0   0   50  0001 C CNN "Article number"
	1    5725 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3300 5525 3300
Wire Wire Line
	5450 3300 5450 3450
Connection ~ 5525 3300
Wire Wire Line
	5525 3300 5450 3300
Text Notes 5000 2450 0    50   ~ 0
KNIGHTBRIGHT WP154A4SEJ3VBDZGC/CA
$EndSCHEMATC
