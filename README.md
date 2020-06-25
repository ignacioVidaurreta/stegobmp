![Actions Status](https://github.com/ignacioVidaurreta/stegobmp/workflows/Ask%20Turing/badge.svg)

# StegoBMP
Program that can hide and find a file in a BMP file. 

## Usage

Make sure to compile the code by using
```bash
make all
```

This will generate a `stegobmp` file which is the executable of our program.
The usage varies if you are embedding or extracting.

**For embedding**
```bash
./stegobmp -embed -in file -p bmp_file -out output_bmp_file -steg <LSB1|LSB4|LSBI> \
          [-a <aes128|aes192|aes256|des>] [-m <ecb|efb|ofb|cbc>]
```
**Important: If you encrypt/decrypt without specifying a password, it won't encrypt the data, just hide it**
Default values:
- If you specify _algorithm_ and _password_ but not mode the default mode is CBC
- If you specify _mode_ and _password_ but not algorithm, the default algorithm is aes128
- If you only specify _password_ it will use aes-128-cbc by default.

**For extracting**
```bash
./stegobmp -extract -p bmp_file -out *output_bmp_file -steg <LSB1|LSB4|LSBI> \
          [-a *<aes128|aes192|aes256|des>] [-m <ecb|efb|ofb|cbc>] [-pass password]
```
Where:
* **-p** specifices the bmp cover image to be used 
* **-out** output of the programa. The cover image with the file hidden in it
* **-steg** the LSB algorithm to use
* **-a** encryption algorithm
* **-m** chaining mode 
* **-pass** password to encrypt with

## Authors
* Clara Guzzetti
* Marina Fuster
* Ignacio Vidaurreta
