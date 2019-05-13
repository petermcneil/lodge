Sample videos
---

Videos found in sub-directories have been sourced from Pexels Videos. They are licensed
under the [Creative Commons Zero license](https://creativecommons.org/publicdomain/zero/1.0/), which gives
the project the right to download, edit, and re-distribute at will without attribution.

Despite the license, I still believe it is right and proper to give proper attribution.
The table below details the creator and gives a link to where you may download it yourselves.

|Video                        |Attribution  |Link                                                                 |
|-----------------------------|-------------|---------------------------------------------------------------------|
|Time Lapse Video Of Night Sky|Vimeo        |https://videos.pexels.com/videos/time-lapse-video-of-night-sky-857195|
|Aerial Shot of City          |Pixabay      |https://videos.pexels.com/videos/aerial-shot-of-city-854336          |

Converting to RAW H264
---
Because of the size of RAW videos this repository doesn't not contain any. To create some follow the below scripts

Create RAW screen capture
```bash
ffmpeg -f avfoundation -i "1:0" -c:v huffyuv <output>.mkv
```

Convert RAW screen capture to lossless H264
```bash
ffmpeg -i <input_video> -c:v libx264 -crf 0 -preset ultrafast -pix_fmt yuv422p <output>.mp4
```

Use the video with lodge

```
lodge write -i <input>.mp4 -o <output>.mp4 -s <input subtitle>
lodge read -i <output>.mp4
```