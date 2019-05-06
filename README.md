<p align="center">
<img src="resources/logos/logo_half.png" alt="Lodge logo"/>
</p>
<p align="center">
<a href="https://travis-ci.org/petermcneil/lodge"><img src="https://travis-ci.org/petermcneil/lodge.svg?branch=master"/></a>
<a href="https://github.com/petermcneil/lodge/releases"><img src="https://img.shields.io/github/release-pre/petermcneil/lodge.svg"/></a>
</p>

---

Lodge is a third year project, written by [Peter McNeil](https://pop.ski).

Traditionally video and subtitle files have been separated; requiring distributing many files to play a video with 
subtitles or for the video editor to burn the subtitles visibly into the video frame.
This project, Lodge, aims to fix this by using video steganography to "lodge" the subtitles 
inside individual video frames.

Lodge will consist of two software products for dealing with subtitling in videos. One program (Lodge Encoder) will be 
used by video editors or stenographers and will merge the subtitles into the video file. The other program 
(Lodge Viewer) will be used by the layperson while watching videos and will extract the subtitles from the video file 
and display them to the user. The two products together will provide a complete end-to-end workflow for subtitling.


Compiled for MacOS 10.12.+

Dependencies
--
For using the Lodge binaries on a system FFmpeg must be installed. Use Homebrew for easiest installation.

To build Lodge for development purposes the following dependencies need to be installed

    - Qt5
    - FFmpeg
    - Boost
    - Spdlog
    
The script `./configure` is provided for convenience of setting these up. It will use Homebrew to install these packages.

To build Lodge for releasing, Qt must be compiled statically. The script `qt.sh` in `scripts/` will download and compile it for you. Be warned
this process takes a substantial amount of time. Currently it is set up to use 8 cores - 4 physical, 4 logical - this can be changed if needed
by editing the script.


Building
---
There are two build systems in use in this project. CMake for the library portions and Qmake for the GUI portion. A top-level make file has been provided for convenience.

    - make build: builds all non-gui parts
    - make gui: builds everything and runs GUI
    - make tests: runs the tests
    - make read: runs a full write and read of a subtitle file
    - make run: runs a write of a subtitle file 

Detecting Lodge in video frames
---
A header will be written to each frame that Lodge writes to, of the format:

```
|L|(number of characters in file)|(filename of subtitle file)|(number of frames)|(frame number - zero indexed)|L|
```

Using
---

```
lodge read  -i [ --input ] pathToVideo (REQUIRED)  -o [ --output ] pathToSubtitle 
      write -i [ --input_video ] pathToInputVideo (REQUIRED) -s [ --input_subtitle ] pathToSubtitle (REQUIRED) -o [ --output_video ] pathToOutput
```


Contributions will not be accepted until final grade has been ratified with the University of Brighton and Exam board
---
