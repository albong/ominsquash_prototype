This is an unfinished game engine written in C. It was forked to a private repo where development has continued and the code has changed extensively.

It is not really intended for public viewing at the moment, but as I am not comfortable publishing the latest version of the engine, this remains the only major C project I have for public display.

The code is inconsistent in its style (although, as documents indicate, some effort has been directed to correct this), and is generally of low quality. I made some decisions and fell into some habits at the start of the project that I no longer agree with. I also have done a lot of reading and learning about engine design and C in general, and would make a lot of different decisions if I were to start over.

It contains no makefile, but rather a Python script to do the compilation. The reason for this is that at the onset of the project my development partner, who was not a programmer, was using Windows, while I was using Cygwin (and later, Linux). Compiling C on Windows can be a challenge, so I wanted a fool-proof "just run this command" way for my partner to compile the code for testing that did not require installing lots of things.

There was a companion program for generating map files for the engine - it was a kind of "bootleg Electron" in that it was a web tool running from a locally hosted Python server. That code is not bad, but it is embarassing, so I moved it to a private repo. (The latest version of the project uses open source tools and a bevy of scripts to convert file formats)
