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
L Sensor_Optical:BPW21 D1
U 1 1 5BD1DA72
P 5175 3650
F 0 "D1" V 5171 3572 50  0000 R CNN
F 1 "BPW21" V 5080 3572 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-5-2_Window" H 5175 3825 50  0001 C CNN
F 3 "http://techwww.in.tu-clausthal.de/site/Dokumentation/Dioden/Fotodioden/BPW21-Fotodiode.pdf" H 5125 3650 50  0001 C CNN
F 4 "BPW21R" H 5175 3650 50  0001 C CNN "Article number"
	1    5175 3650
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5BD1DC83
P 4650 1200
F 0 "#PWR01" H 4650 1050 50  0001 C CNN
F 1 "+5V" H 4665 1373 50  0000 C CNN
F 2 "" H 4650 1200 50  0001 C CNN
F 3 "" H 4650 1200 50  0001 C CNN
	1    4650 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5BD1DD13
P 5925 4375
F 0 "C2" V 5673 4375 50  0000 C CNN
F 1 "C" V 5764 4375 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5963 4225 50  0001 C CNN
F 3 "~" H 5925 4375 50  0001 C CNN
	1    5925 4375
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5BD1DDDA
P 5925 4725
F 0 "R1" V 6132 4725 50  0000 C CNN
F 1 "R" V 6041 4725 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5855 4725 50  0001 C CNN
F 3 "~" H 5925 4725 50  0001 C CNN
	1    5925 4725
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6075 4375 6075 4550
Wire Wire Line
	6075 4550 6325 4550
Connection ~ 6075 4550
Wire Wire Line
	6075 4550 6075 4725
$Comp
L power:GND #PWR03
U 1 1 5BD1DE9E
P 5075 3650
F 0 "#PWR03" H 5075 3400 50  0001 C CNN
F 1 "GND" H 5080 3477 50  0000 C CNN
F 2 "" H 5075 3650 50  0001 C CNN
F 3 "" H 5075 3650 50  0001 C CNN
	1    5075 3650
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 5BD1EB34
P 4950 1700
F 0 "C1" V 4698 1700 50  0000 C CNN
F 1 "0.1uF" V 4789 1700 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 4988 1550 50  0001 C CNN
F 3 "https://nl.farnell.com/wurth-elektronik/885012207016/cap-0-1-f-10v-10-x7r-0805/dp/2534051" H 4950 1700 50  0001 C CNN
F 4 "885012207016" H 4950 1700 50  0001 C CNN "Article number"
	1    4950 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4650 1675 4650 1700
Wire Wire Line
	4800 1700 4650 1700
Connection ~ 4650 1700
Wire Wire Line
	4650 1700 4650 1725
$Comp
L power:GND #PWR02
U 1 1 5BD1ED12
P 5100 1700
F 0 "#PWR02" H 5100 1450 50  0001 C CNN
F 1 "GND" H 5105 1527 50  0000 C CNN
F 2 "" H 5100 1700 50  0001 C CNN
F 3 "" H 5100 1700 50  0001 C CNN
	1    5100 1700
	0    -1   -1   0   
$EndComp
$Comp
L EMIshield:EMIshield U2
U 1 1 5BD574D7
P 8250 5950
F 0 "U2" H 8143 6175 50  0000 C CNN
F 1 "EMIshield" H 8143 6084 50  0000 C CNN
F 2 "EMIshield:Laird-BMI-S-202-F" H 8250 5950 50  0001 C CNN
F 3 "https://nl.farnell.com/laird/bmi-s-202-f/emi-shield-surface-mount/dp/2497717?ost=BMI-S-202-F&scope=partnumberlookahead&searchref=searchlookahead&krypto=j3iFSqeSfTjXXfOpoRIuPKM7UIy%2BvTJigm0QN3yDryZM8QlogRxdn7Dx%2FkTFeJac9aDEMj3hgo6Ckjf0zdpzKwE240w%2BZJbeuFggQWZTM5Tlw7e%2FvhZDy1IBFLLLr3mOigXCFiLX%2BR34zZM0QLutt0bvc5vS0aCbutfz8ghYSwpAlxVxBhOYLvahLZTRj4Kzk%2Fh0GwjeoOG1jspRoPLOa988bXmgja6AwIxVDkR5%2BAyKI7C6ljuhX6uQKMYtWbyJP%2FrB9czLNF2%2FXHtxLsgCJqP7O24wOqTGO8EK2nxIcX%2BCkae4EUYgTpdfeaXysltqpdf6Ok7cVpG4Y6eWQm8YTyrJQhEUlL9dNCrjuaRd7FNG%2FpkGh9CP6Nm2hra08KGtiNZq5EYHtxwVUGyYejInp4QxRvnBjB%2FwIY1nPd%2FRcjZWM9k8XYf18xPIeApT%2BuFyVIMT%2BBtSBxLa3ruUCmXYswozgjw9DtPR3wxmRiH0KI091VNYfPKPTCD3eocnrhVcI2YbFh%2BGp6V0EE80juCqZlepCl4lX44grZSO%2BhtMl3TsO66u%2BMAAfbWt%2FOpqgwZiDJJwObv6CNM6Rg3VSS940IV22GW1NXf5jzwy2oP8rpY%3D&ddkey=https%3Anl-NL%2FElement14_Netherlands%2Fw%2Fsearch" H 8250 5950 50  0001 C CNN
F 4 "BMI-S-202-F" H 8250 5950 50  0001 C CNN "Article number"
	1    8250 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5BD57541
P 7900 5950
F 0 "#PWR08" H 7900 5700 50  0001 C CNN
F 1 "GND" H 7905 5777 50  0000 C CNN
F 2 "" H 7900 5950 50  0001 C CNN
F 3 "" H 7900 5950 50  0001 C CNN
	1    7900 5950
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5BDDEC06
P 5850 5200
F 0 "J1" H 5930 5242 50  0000 L CNN
F 1 "Conn_01x03" H 5930 5151 50  0000 L CNN
F 2 "Connector_Molex:Molex_Pico-Clasp_501331-0307_1x03-1MP_P1.00mm_Vertical" H 5850 5200 50  0001 C CNN
F 3 "https://nl.farnell.com/molex/501331-0307/connector-header-3pos-1row-1mm/dp/1835977?st=pico%20clasp" H 5850 5200 50  0001 C CNN
F 4 "501331-0307" H 5850 5200 50  0001 C CNN "Article number"
	1    5850 5200
	1    0    0    -1  
$EndComp
Text Label 5650 5300 2    50   ~ 0
A
$Comp
L power:+5V #PWR06
U 1 1 5BDDEC86
P 5650 5200
F 0 "#PWR06" H 5650 5050 50  0001 C CNN
F 1 "+5V" H 5665 5373 50  0000 C CNN
F 2 "" H 5650 5200 50  0001 C CNN
F 3 "" H 5650 5200 50  0001 C CNN
	1    5650 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5BDDECB4
P 5650 5100
F 0 "#PWR07" H 5650 4850 50  0001 C CNN
F 1 "GND" H 5655 4927 50  0000 C CNN
F 2 "" H 5650 5100 50  0001 C CNN
F 3 "" H 5650 5100 50  0001 C CNN
	1    5650 5100
	0    1    1    0   
$EndComp
Text Notes 3675 4150 0    50   ~ 0
Tweede trap erbij
Text Notes 3700 3975 0    50   ~ 0
Ferrite bead + cap
$Comp
L Device:Opamp_Dual_Generic U1
U 2 1 5BE9F049
P 7175 3650
F 0 "U1" H 7175 4017 50  0000 C CNN
F 1 "Opamp_Dual_Generic" H 7175 3926 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 7175 3650 50  0001 C CNN
F 3 "https://nl.mouser.com/ProductDetail/Texas-Instruments/OPA2140AID?qs=sGAEpiMZZMvtNjJQt4UgLazUAy7nLwWSG%252bHXmvkwFZI%3d" H 7175 3650 50  0001 C CNN
	2    7175 3650
	1    0    0    1   
$EndComp
$Comp
L Device:Opamp_Dual_Generic U1
U 3 1 5BE9F1E2
P 4750 2025
F 0 "U1" H 4750 2392 50  0000 C CNN
F 1 "Opamp_Dual_Generic" H 4750 2301 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 4750 2025 50  0001 C CNN
F 3 "https://nl.mouser.com/ProductDetail/Texas-Instruments/OPA2140AID?qs=sGAEpiMZZMvtNjJQt4UgLazUAy7nLwWSG%252bHXmvkwFZI%3d" H 4750 2025 50  0001 C CNN
	3    4750 2025
	1    0    0    -1  
$EndComp
$Comp
L Device:Opamp_Dual_Generic U1
U 1 1 5BE9F237
P 6025 3550
F 0 "U1" H 5983 3596 50  0000 L CNN
F 1 "Opamp_Dual_Generic" H 5983 3505 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6025 3550 50  0001 C CNN
F 3 "https://nl.mouser.com/ProductDetail/Texas-Instruments/OPA2140AID?qs=sGAEpiMZZMvtNjJQt4UgLazUAy7nLwWSG%252bHXmvkwFZI%3d" H 6025 3550 50  0001 C CNN
F 4 "OPA2140AID" H 6025 3550 50  0001 C CNN "Article number"
	1    6025 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5BE9F297
P 4650 2325
F 0 "#PWR0101" H 4650 2075 50  0001 C CNN
F 1 "GND" H 4655 2152 50  0000 C CNN
F 2 "" H 4650 2325 50  0001 C CNN
F 3 "" H 4650 2325 50  0001 C CNN
	1    4650 2325
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5BE9F44C
P 7075 3125
F 0 "R3" V 7282 3125 50  0000 C CNN
F 1 "R" V 7191 3125 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7005 3125 50  0001 C CNN
F 3 "~" H 7075 3125 50  0001 C CNN
	1    7075 3125
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 5BE9F48E
P 7025 4225
F 0 "R4" V 7232 4225 50  0000 C CNN
F 1 "R" V 7141 4225 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6955 4225 50  0001 C CNN
F 3 "~" H 7025 4225 50  0001 C CNN
	1    7025 4225
	0    1    1    0   
$EndComp
Wire Wire Line
	6875 3750 6875 4225
Wire Wire Line
	7625 3650 7475 3650
Text Label 7625 3650 0    50   ~ 0
A
Wire Wire Line
	6625 3550 6875 3550
Wire Wire Line
	6325 3550 6325 4550
$Comp
L power:GND #PWR0102
U 1 1 5BE9FB70
P 7175 4225
F 0 "#PWR0102" H 7175 3975 50  0001 C CNN
F 1 "GND" H 7180 4052 50  0000 C CNN
F 2 "" H 7175 4225 50  0001 C CNN
F 3 "" H 7175 4225 50  0001 C CNN
	1    7175 4225
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5BEB2536
P 6475 3550
F 0 "R2" V 6682 3550 50  0000 C CNN
F 1 "3" V 6591 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6405 3550 50  0001 C CNN
F 3 "https://nl.farnell.com/panasonic-electronic-components/erj6geyj3r0v/res-3r-5-0-125w-0805-thick-film/dp/2323844RL" H 6475 3550 50  0001 C CNN
F 4 "ERJ6GEYJ3R0V" H 6475 3550 50  0001 C CNN "Article number"
	1    6475 3550
	0    -1   -1   0   
$EndComp
Connection ~ 6325 3550
$Comp
L Device:C C4
U 1 1 5BEB2596
P 6625 3700
F 0 "C4" V 6373 3700 50  0000 C CNN
F 1 "47uF" V 6464 3700 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6663 3550 50  0001 C CNN
F 3 "https://nl.farnell.com/murata/grm21br60j476me15l/cap-47-f-6-3v-20-x5r-0805/dp/2362109" H 6625 3700 50  0001 C CNN
F 4 "GRM21BR60J476ME15L" H 6625 3700 50  0001 C CNN "Article number"
	1    6625 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5BEB2629
P 6625 3850
F 0 "#PWR0103" H 6625 3600 50  0001 C CNN
F 1 "GND" H 6630 3677 50  0000 C CNN
F 2 "" H 6625 3850 50  0001 C CNN
F 3 "" H 6625 3850 50  0001 C CNN
	1    6625 3850
	1    0    0    -1  
$EndComp
Connection ~ 6625 3550
$Comp
L Device:Ferrite_Bead L1
U 1 1 5BEB286B
P 4650 1525
F 0 "L1" H 4787 1571 50  0000 L CNN
F 1 "Ferrite_Bead" H 4787 1480 50  0000 L CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 4580 1525 50  0001 C CNN
F 3 "https://nl.farnell.com/murata/blm21bd272sn1l/ferrite-bead-0-8ohm-200ma-0805/dp/1515647" H 4650 1525 50  0001 C CNN
F 4 "BLM21BD272SN1L" H 4650 1525 50  0001 C CNN "Article number"
	1    4650 1525
	1    0    0    -1  
$EndComp
Text Notes 5475 1600 0    50   ~ 0
https://www.murata.com/~/media/webrenewal/support/library/catalog/products/emc/emifil/c39e.ashx
$Comp
L Device:C C3
U 1 1 5BEB2C09
P 4950 1375
F 0 "C3" V 4698 1375 50  0000 C CNN
F 1 "1.5uF" V 4789 1375 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 4988 1225 50  0001 C CNN
F 3 "https://nl.farnell.com/multicomp/mc0805x155k6r3ct/cap-1-5-f-6-3v-10-x5r-0805/dp/1759410" H 4950 1375 50  0001 C CNN
F 4 "MC0805X155K6R3CT" H 4950 1375 50  0001 C CNN "Article number"
	1    4950 1375
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5BEB2C87
P 5100 1375
F 0 "#PWR0104" H 5100 1125 50  0001 C CNN
F 1 "GND" H 5105 1202 50  0000 C CNN
F 2 "" H 5100 1375 50  0001 C CNN
F 3 "" H 5100 1375 50  0001 C CNN
	1    5100 1375
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4650 1375 4800 1375
Wire Wire Line
	4650 1375 4650 1200
Connection ~ 4650 1375
Text Notes 8300 2000 0    50   ~ 0
Add AD conversion on PCB
$Comp
L Device:R R5
U 1 1 5C95F2CA
P 5650 3300
F 0 "R5" V 5857 3300 50  0000 C CNN
F 1 "R" V 5766 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5580 3300 50  0001 C CNN
F 3 "~" H 5650 3300 50  0001 C CNN
	1    5650 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:D D2
U 1 1 5C95F3AC
P 5225 3450
F 0 "D2" H 5225 3666 50  0000 C CNN
F 1 "D" H 5225 3575 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 5225 3450 50  0001 C CNN
F 3 "~" H 5225 3450 50  0001 C CNN
	1    5225 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5775 4375 5775 4550
Wire Wire Line
	5375 3650 5550 3650
Wire Wire Line
	5775 4550 5550 4550
Wire Wire Line
	5550 4550 5550 3650
Connection ~ 5775 4550
Wire Wire Line
	5775 4550 5775 4725
Connection ~ 5550 3650
Wire Wire Line
	5550 3650 5725 3650
$Comp
L power:GND #PWR0105
U 1 1 5C95FE19
P 5075 3450
F 0 "#PWR0105" H 5075 3200 50  0001 C CNN
F 1 "GND" H 5080 3277 50  0000 C CNN
F 2 "" H 5075 3450 50  0001 C CNN
F 3 "" H 5075 3450 50  0001 C CNN
	1    5075 3450
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5C95FEC2
P 5225 3075
F 0 "C5" V 4973 3075 50  0000 C CNN
F 1 "C" V 5064 3075 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5263 2925 50  0001 C CNN
F 3 "~" H 5225 3075 50  0001 C CNN
	1    5225 3075
	0    1    1    0   
$EndComp
Wire Wire Line
	5075 3450 5075 3075
Connection ~ 5075 3450
Wire Wire Line
	5375 3075 5375 3450
Wire Wire Line
	5725 3450 5650 3450
Wire Wire Line
	5375 3450 5650 3450
Connection ~ 5375 3450
Connection ~ 5650 3450
Text Label 4650 1700 2    50   ~ 0
Vdd
Text Label 5650 3150 2    50   ~ 0
Vdd
Wire Wire Line
	6875 3550 6875 3125
Wire Wire Line
	6875 3125 6925 3125
Connection ~ 6875 3550
Wire Wire Line
	7225 3125 7475 3125
Wire Wire Line
	7475 3125 7475 3650
Connection ~ 7475 3650
$Comp
L Device:R R6
U 1 1 5C963211
P 6725 4225
F 0 "R6" V 6932 4225 50  0000 C CNN
F 1 "R" V 6841 4225 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6655 4225 50  0001 C CNN
F 3 "~" H 6725 4225 50  0001 C CNN
	1    6725 4225
	0    1    1    0   
$EndComp
Connection ~ 6875 4225
Text Label 6575 4225 2    50   ~ 0
Vdd
$EndSCHEMATC
