# ArtNetStarGate


This program send RAM data as Art-Net frames.


1. need create 5 channel fixture with default dmx values (84, 104, 105, 101, 102) - name it RA (manufac. StarGate)
2. patch it on 8.508
3. run artnetstargate with Your program window name

Default in program
Program name "XX"
node ip "127.0.0.1"
bind - not set
fps - delay between frames "30"
nr universes to send "2"
Ra universe "8"



how to run

ArtNetStargate.exe "program window name" "node ip" "bind / or nobind" "fps" "universes" "Ra"
