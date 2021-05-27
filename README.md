# MagOD
Magnetic Optical Density meter
Tijmen Hageman, Jordi Hendrix, Hans Keizer, Marcel Welleweerd, Dave
van As, Ruthvik Bandaru, Rob Krawinkel, Leon Abelmann.

Tijmen Hageman, Jordi Hendrix, Hans Keizer, Marcel Welleweerd, Dave
van As, Ruthvik Bandaru, Rob Krawinkel, Leon Abelmann.

KIST Europe, University of Twente

## Other sources
Next to software, also the source codes to create hardware are
available:

- Measurement head design, 3D print file (OpenSCAD, on [Thingiverse](https://www.thingiverse.com/thing:3122930) )
- Spice simulations, electronic diagrams, PCB layout (Spice, KiCad on
  [Dropbox](https://www.dropbox.com/sh/1dcai82byvd3hy0/AAA3AGXw0jbr9n2gzdcszufia))
- Laser cut pattern for housing electronics (DXF, on [Dropbox](https://www.dropbox.com/sh/1dcai82byvd3hy0/AAA3AGXw0jbr9n2gzdcszufia))

## Specifications
- Up to 800 samples per second, allows precise analysis of the rotation speed of magnetic bacteria
- Three-dimensional quasi-Helmholtz configuration, with fields up to 5 mT 
- Three color LED: red: 645 nm, 1000 mcd, green: 520 nm, 1700 mcd and blue 460 nm, 500 mcd
- Power supply with current feedback to compensate for coil heating, cut-off frequency 200 Hz
- Large area photodiode (sensitivity 0.5 mlx, with green LED theoretical max OD range of 5.8)
- Integrated pre-amplifier, SNR 83 dB (max OD range 4.0).
- Dedicated 16-bit ADS1115 DA converter
- Integrated temperature sensor
- Reference light detector
- Scattered light detector
- Fully programmable (ESP32 using Arduino IDE), open source
- User friendly spreadsheet recipe definition (csv format), with software to validate correctness
- Data collection on SD card, accessible by ftp over Wifi

## Additional info
- Chapter 8 in [Thesis](https://research.utwente.nl/en/publications/observing-magnetic-objects-in-fluids) Tijmen Hageman.
- [Presentation](https://youtu.be/TwwIsPg0qnI) by Leon Abelmann at Intermag2021

## How to join
Send mail to Leon (l.abelmann@utwente.nl) if you are interested, all skills are welcome.
