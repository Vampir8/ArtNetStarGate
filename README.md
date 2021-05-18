# ArtNetStarGate


This program send RAM data as Art-Net frames.


1. need create 5 channel fixture with default dmx values (84, 104, 105, 101, 102) - name it RA (manufac. StarGate)
2. patch it on 8.508
3. run artnetstargate with Your program window name

Default in program

Program name "MagicQ PC (Demo Mode)"

node ip "127.0.0.1"

bind - not set

fps - delay between frames "32"

nr universes to send "2"

Ra universe "8"





how to run



ex. ArtNetStargate.exe "my program window name"

this send 2 universe to 127.0.0.1

OR ADD MORE 

ArtNetStargate.exe "my program window name" 2.0.0.10 nobind 30 4 8

where 2.0.0.10 is artnet gate IP

bind or nobind (output port set or no)

30 - delay between frames

4 nr of universes to send

8 - is a universe where RA is patched



this is test with chamsys
https://www.youtube.com/watch?v=WGwr3fhAVYM

