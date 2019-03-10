<p align="center">
<img src="extras/logos/logo_half.png" alt="Lodge logo"/>
</p>

---

[![Build Status](https://travis-ci.org/petermcneil/lodge.svg?branch=master)](https://travis-ci.org/petermcneil/lodge)

Lodge is a third year project, written by [Peter McNeil](https://pop.ski).

Traditionally video and subtitle files have been separated; requiring distributing many files to play a video with 
subtitles or for the video editor to burn the subtitles visibly into the video frame.
This project, Lodge, aims to fix this by using video steganography to "lodge" the subtitles 
inside individual video frames.

Lodge will consist of two software products for dealing with subtitling in videos. One program (Lodge Encoder) will be 
used by video editors or stenographers and will merge the subtitles into the video file. The other program 
(Lodge Viewer) will be used by the layperson while watching videos and will extract the subtitles from the video file 
and display them to the user. The two products together will provide a complete end-to-end workflow for subtitling.

Detecting Lodge in video frames
---
A header will be written to each frame that Lodge writes to, of the format:

```
|LODGE|(number of characters in file)|(file extension of subtitle file)|LODGE|
```

Using
---

```
lodge read  -i [ --input ] pathToVideo (REQUIRED)  -o [ --output ] pathToSubtitle 
      write -i [ --input_video ] pathToInputVideo (REQUIRED) -s [ --input_subtitle ] pathToSubtitle (REQUIRED) -o [ --output_video ] pathToOutput
```


Contributions will not be accepted until final grade has been ratified with the University of Brighton and Exam board
---