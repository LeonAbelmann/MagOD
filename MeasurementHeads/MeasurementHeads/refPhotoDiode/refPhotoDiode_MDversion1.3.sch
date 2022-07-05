EESchema Schematic File Version 4
LIBS:refPhotoDiode_MDversion1.3-cache
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
L Sensor_Optical:BPW21 D1
U 1 1 5BD1DA72
P 4950 3650
F 0 "D1" V 4946 3572 50  0000 R CNN
F 1 "BPW21" V 4855 3572 50  0000 R CNN
F 2 "BPW21:TO-5-2_Window" H 4950 3825 50  0001 C CNN
F 3 "http://techwww.in.tu-clausthal.de/site/Dokumentation/Dioden/Fotodioden/BPW21-Fotodiode.pdf" H 4900 3650 50  0001 C CNN
F 4 "BPW21R" H 0   0   50  0001 C CNN "Article number"
	1    4950 3650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5BD1DC48
P 5925 3850
F 0 "#PWR04" H 5925 3600 50  0001 C CNN
F 1 "GND" H 5930 3677 50  0000 C CNN
F 2 "" H 5925 3850 50  0001 C CNN
F 3 "" H 5925 3850 50  0001 C CNN
	1    5925 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5BD1DC83
P 5925 2800
F 0 "#PWR01" H 5925 2650 50  0001 C CNN
F 1 "+5V" H 5940 2973 50  0000 C CNN
F 2 "" H 5925 2800 50  0001 C CNN
F 3 "" H 5925 2800 50  0001 C CNN
	1    5925 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5BD1DD13
P 5925 4375
F 0 "C1" V 5673 4375 50  0000 C CNN
F 1 "2000p" V 5764 4375 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5963 4225 50  0001 C CNN
F 3 "~" H 5925 4375 50  0001 C CNN
F 4 "2905236" H 0   0   50  0001 C CNN "Article number"
	1    5925 4375
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5BD1DDDA
P 5925 4725
F 0 "R1" V 6132 4725 50  0000 C CNN
F 1 "100k" V 6041 4725 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5855 4725 50  0001 C CNN
F 3 "~" H 5925 4725 50  0001 C CNN
F 4 "2502674" H 0   0   50  0001 C CNN "Article number"
	1    5925 4725
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5775 4375 5775 4550
Wire Wire Line
	6075 4375 6075 4550
Wire Wire Line
	5775 4550 5150 4550
Connection ~ 5775 4550
Wire Wire Line
	5775 4550 5775 4725
Wire Wire Line
	6075 4550 6325 4550
Wire Wire Line
	6325 4550 6325 3800
Connection ~ 6075 4550
Wire Wire Line
	6075 4550 6075 4725
$Comp
L power:GND #PWR02
U 1 1 5BD1DE9E
P 4850 3650
F 0 "#PWR02" H 4850 3400 50  0001 C CNN
F 1 "GND" H 4855 3477 50  0000 C CNN
F 2 "" H 4850 3650 50  0001 C CNN
F 3 "" H 4850 3650 50  0001 C CNN
	1    4850 3650
	0    1    -1   0   
$EndComp
Wire Wire Line
	5150 3650 5150 4550
Wire Wire Line
	5725 3650 5150 3650
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5BD1E62F
P 6125 5225
F 0 "J1" H 6205 5217 50  0000 L CNN
F 1 "Conn_01x04" H 6205 5126 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical_SMD_Pin1Left" H 6125 5225 50  0001 C CNN
F 3 "https://de.farnell.com/multicomp/2212sm-04g-75-b1-pcr/buchsenleiste-4pos-1reihe-2-54mm/dp/2847903" H 6125 5225 50  0001 C CNN
F 4 "2847903" H 6125 5225 50  0001 C CNN "Article number"
	1    6125 5225
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5BD1E6D6
P 5300 5200
F 0 "J2" H 5380 5192 50  0000 L CNN
F 1 "Conn_01x04" H 5380 5101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical_SMD_Pin1Left" H 5300 5200 50  0001 C CNN
F 3 "https://de.farnell.com/multicomp/2212sm-04g-75-b1-pcr/buchsenleiste-4pos-1reihe-2-54mm/dp/2847903" H 5300 5200 50  0001 C CNN
F 4 "2847903" H 5300 5200 50  0001 C CNN "Article number"
	1    5300 5200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J3
U 1 1 5BD1E7FF
P 5300 5925
F 0 "J3" H 5380 5917 50  0000 L CNN
F 1 "Conn_01x06" H 5380 5826 50  0000 L CNN
F 2 "Connector_Molex:Molex_Pico-Clasp_501331-0607_1x06-1MP_P1.00mm_Vertical" H 5300 5925 50  0001 C CNN
F 3 "https://de.farnell.com/molex/501331-0607/stiftleiste-6pos-1-reihe-1mm/dp/1835980" H 5300 5925 50  0001 C CNN
F 4 "1835980" H 5300 5925 50  0001 C CNN "Article number"
	1    5300 5925
	1    0    0    -1  
$EndComp
Text Label 5100 6025 2    50   ~ 0
R
Text Label 5100 6125 2    50   ~ 0
G
Text Label 5100 6225 2    50   ~ 0
B
$Comp
L power:+5V #PWR0101
U 1 1 5BD1E976
P 5100 5725
F 0 "#PWR0101" H 5100 5575 50  0001 C CNN
F 1 "+5V" H 5115 5898 50  0000 C CNN
F 2 "" H 5100 5725 50  0001 C CNN
F 3 "" H 5100 5725 50  0001 C CNN
	1    5100 5725
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5BD1EA4C
P 5100 5825
F 0 "#PWR0102" H 5100 5575 50  0001 C CNN
F 1 "GND" H 5105 5652 50  0000 C CNN
F 2 "" H 5100 5825 50  0001 C CNN
F 3 "" H 5100 5825 50  0001 C CNN
	1    5100 5825
	0    1    1    0   
$EndComp
Text Label 5100 5925 2    50   ~ 0
A
Text Label 5100 5100 2    50   ~ 0
R
Text Label 5100 5200 2    50   ~ 0
G
Text Label 5100 5300 2    50   ~ 0
B
$Comp
L Device:C C2
U 1 1 5BD1EB34
P 5725 2850
F 0 "C2" V 5473 2850 50  0000 C CNN
F 1 "1.5uF" V 5564 2850 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5763 2700 50  0001 C CNN
F 3 "https://nl.farnell.com/multicomp/mc0805x155k6r3ct/cap-1-5-f-6-3v-10-x5r-0805/dp/1759410" H 5725 2850 50  0001 C CNN
F 4 "1759410" H 5725 2850 50  0001 C CNN "Article number"
	1    5725 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	5925 3200 5925 3225
Connection ~ 5925 3225
Wire Wire Line
	5925 3225 5925 3250
$Comp
L power:GND #PWR0104
U 1 1 5BD1ED12
P 5575 2850
F 0 "#PWR0104" H 5575 2600 50  0001 C CNN
F 1 "GND" H 5580 2677 50  0000 C CNN
F 2 "" H 5575 2850 50  0001 C CNN
F 3 "" H 5575 2850 50  0001 C CNN
	1    5575 2850
	0    1    1    0   
$EndComp
$Comp
L refPhotoDiode_MDversion1.3-rescue:EMIshield-EMIshield U2
U 1 1 5BD574D7
P 8250 5950
F 0 "U2" H 8143 6175 50  0000 C CNN
F 1 "EMIshield" H 8143 6084 50  0000 C CNN
F 2 "EMIshield:Wurth-3610213126S-EMIshield-13.26x12.3mm" H 8250 5950 50  0001 C CNN
F 3 "https://nl.farnell.com/wurth-elektronik/3610213126s/emi-shield-cabinet-13-26mm-x-12/dp/2918354?ost=3610213126S&scope=partnumberlookahead&searchref=searchlookahead&krypto=Q3vQa53%2BmVXDAr4BB0tXYOxhZTuJQmghT%2Bp4Vr46u1KynV6oXNdycdmS5skScsyREtp9n%2BXvHu6ptJEaISgDZQ8q0hT2Q9EYBuA6KMF%2BCB5y6Ihjws2Ro2R%2BLVqdTm9NVIuEPpLaGCm6kwScWYLopc5MxE%2BYQJcj3e7tlmdOzfTjbjE6xoP8o1VWhW9tSBjZ4RfSjbu2t9YPvf%2BcIlGs1eyQHjLZUEwsjyei6V3lO%2F8mQozZv5L5Clkb6qFFhiOotdiEUS4YSBEM8%2BHSHSxdKE5FV33HYeyQHmO94UuWj7jnZU0r1MjnO4kmKV%2F0SOEP%2Bh%2FqM0NJGyPU0nADX%2BhrLywdDUl4%2B8%2BHyR69iWj9MyvGoARitxFMulO34GyVOJ4taT5DY8YB8gQ0xUJxhD6faVGgfTe%2FIHW7BB30hiPdl6PpZq5mzRqLoLs0QZ8fjK%2Fzt8CRgBzqTa1GUBr%2FE8n95sXEJ%2BXyQWLpJrpuCVIeuYtu%2FHjB5tdTZTYUK2rwBlzFOiSWCEa4OcdTVTqoEHZwi5pnPKjKRMEImxDKPZnGG%2BpW6qX%2B7UIgm7L0LXYN5woZNzC4OaMz%2BXEOh%2B5oEnueU3ogB3hn%2BwiAgMKqPYZAUXM%3D&ddkey=https%3Anl-NL%2FElement14_Netherlands%2Fw%2Fsearch" H 8250 5950 50  0001 C CNN
F 4 "2918354" H 8250 5950 50  0001 C CNN "Article number"
	1    8250 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5BD57541
P 7900 5950
F 0 "#PWR0105" H 7900 5700 50  0001 C CNN
F 1 "GND" H 7905 5777 50  0000 C CNN
F 2 "" H 7900 5950 50  0001 C CNN
F 3 "" H 7900 5950 50  0001 C CNN
	1    7900 5950
	0    1    1    0   
$EndComp
Text Label 6400 3800 0    50   ~ 0
A
Wire Wire Line
	6400 3800 6325 3800
Connection ~ 6325 3800
Wire Wire Line
	6325 3800 6325 3550
$Comp
L power:+5V #PWR0103
U 1 1 5BDDBD30
P 5100 5400
F 0 "#PWR0103" H 5100 5250 50  0001 C CNN
F 1 "+5V" H 5115 5573 50  0000 C CNN
F 2 "" H 5100 5400 50  0001 C CNN
F 3 "" H 5100 5400 50  0001 C CNN
	1    5100 5400
	0    -1   -1   0   
$EndComp
Text Notes 6750 3250 0    50   ~ 0
ferrite bead + capacitor
Text Notes 6450 4550 0    50   ~ 0
860 hertz
Text Notes 5225 2000 0    50   ~ 0
https://www.analog.com/en/technical-articles/optimizing-precision-photodiode-sensor-circuit-design.html
Text Notes 5225 2100 0    50   ~ 0
https://www.analog.com/en/technical-articles/low-noise-single-supply-photodiode-amplifier.html
$Comp
L Device:Ferrite_Bead L1
U 1 1 5BF01159
P 5925 3050
F 0 "L1" H 6062 3096 50  0000 L CNN
F 1 "Ferrite_Bead" H 6062 3005 50  0000 L CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 5855 3050 50  0001 C CNN
F 3 "https://nl.farnell.com/murata/blm21bd272sn1l/ferrite-bead-0-8ohm-200ma-0805/dp/1515647" H 5925 3050 50  0001 C CNN
F 4 "1515647" H 5925 3050 50  0001 C CNN "Article number"
	1    5925 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5BF0151F
P 5725 3225
F 0 "C3" V 5473 3225 50  0000 C CNN
F 1 "0.1uF" V 5564 3225 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5763 3075 50  0001 C CNN
F 3 "https://nl.farnell.com/wurth-elektronik/885012207016/cap-0-1-f-10v-10-x7r-0805/dp/2534051" H 5725 3225 50  0001 C CNN
F 4 "2673150" H 5725 3225 50  0001 C CNN "Article number"
	1    5725 3225
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5BF01526
P 5575 3225
F 0 "#PWR0106" H 5575 2975 50  0001 C CNN
F 1 "GND" H 5580 3052 50  0000 C CNN
F 2 "" H 5575 3225 50  0001 C CNN
F 3 "" H 5575 3225 50  0001 C CNN
	1    5575 3225
	0    1    1    0   
$EndComp
Wire Wire Line
	5925 3225 5875 3225
Wire Wire Line
	5925 2800 5925 2850
Wire Wire Line
	5875 2850 5925 2850
Connection ~ 5925 2850
Wire Wire Line
	5925 2850 5925 2900
Connection ~ 5150 3650
$Comp
L Diode:1N4148 D2
U 1 1 5C960748
P 5000 3450
F 0 "D2" H 5000 3666 50  0000 C CNN
F 1 "1N4148" H 5000 3575 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 5000 3450 50  0001 C CNN
F 3 "~" H 5000 3450 50  0001 C CNN
F 4 "1776392" H 0   0   50  0001 C CNN "Article number"
	1    5000 3450
	1    0    0    -1  
$EndComp
Text Label 5925 3225 0    50   ~ 0
V
Wire Wire Line
	4850 3125 4850 3450
$Comp
L power:GND #PWR0107
U 1 1 5C960F1B
P 4850 3450
F 0 "#PWR0107" H 4850 3200 50  0001 C CNN
F 1 "GND" H 4855 3277 50  0000 C CNN
F 2 "" H 4850 3450 50  0001 C CNN
F 3 "" H 4850 3450 50  0001 C CNN
	1    4850 3450
	0    1    -1   0   
$EndComp
Connection ~ 4850 3450
$Comp
L Amplifier_Operational:OPA340NA U1
U 1 1 5CBAD05D
P 6025 3550
F 0 "U1" H 6366 3596 50  0000 L CNN
F 1 "OPA140" H 6366 3505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5925 3350 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa340.pdf" H 6025 3750 50  0001 C CNN
F 4 "595-OPA140AIDBVR" H 0   0   50  0001 C CNN "Article number"
	1    6025 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3125 5150 3450
$Comp
L Device:C C4
U 1 1 5C960B03
P 5000 3125
F 0 "C4" V 4748 3125 50  0000 C CNN
F 1 "1u" V 4839 3125 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5038 2975 50  0001 C CNN
F 3 "~" H 5000 3125 50  0001 C CNN
F 4 "9227792" H 0   0   50  0001 C CNN "Article number"
	1    5000 3125
	0    1    1    0   
$EndComp
Text Label 5300 3150 0    50   ~ 0
V
Connection ~ 5150 3450
$Comp
L Device:R R2
U 1 1 5C9606B3
P 5300 3300
F 0 "R2" V 5507 3300 50  0000 C CNN
F 1 "18k" V 5416 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5230 3300 50  0001 C CNN
F 3 "https://de.farnell.com/panasonic/erj6enf1802v/dickschichtwiderstand-18k-1-0/dp/2303679" H 5300 3300 50  0001 C CNN
F 4 "2303679" H 0   0   50  0001 C CNN "Article number"
	1    5300 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5150 3450 5300 3450
Wire Wire Line
	5300 3450 5725 3450
Connection ~ 5300 3450
$EndSCHEMATC
