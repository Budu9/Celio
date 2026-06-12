# Celio
 
Encode any file into a video. Decode the video back into the original file, byte for byte.
 
Celio reads a file as raw binary data, renders it as a sequence of black-and-white pixel frames, and assembles those frames into a video. The decoder reverses the process — extracting frames, sampling pixels, and reconstructing the original bytes exactly. Error correction is built into the encoding so the data survives video compression.
 
> **Status:** Work in progress.

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![FFmpeg](https://img.shields.io/badge/FFmpeg-007808?style=for-the-badge&logo=ffmpeg&logoColor=white)
![libpng](https://img.shields.io/badge/libpng-3D6117?style=for-the-badge)
![Make](https://img.shields.io/badge/Make-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)
 
## The idea
 
A video is just a sequence of images. An image is just a grid of pixels. And a file is just a sequence of bits. Celio collapses these three facts into one pipeline: any file becomes a visual bitstream, and that bitstream can be played, stored, and reversed.
 
```
file  →  bytes  →  bits  →  pixels  →  frames  →  video
video →  frames →  pixels →  bits  →  bytes  →  file
```
 

## Goals
 
- Encode any file type such as text, images, binaries and archives with no format-specific handling
- Perfect round-trip reconstruction, verified byte-for-byte against the original
- Resilience to standard video compression, not just lossless formats
- A clean command-line interface: `celio encode <file> <video>` and `celio decode <video> <file>`
- Written in C with minimal dependencies
## Design principles
 
- **Self-describing output.** The video itself carries everything the decoder needs like the file size, layout, encoding parameters.
- **Separation of concerns.** Encoding, error correction, pixel rendering, and video I/O are independent modules that can be tested in isolation.
- **Built in stages.** Each stage of the pipeline is verified end-to-end before the next is added. Raw bit rendering first, then error correction, then video compression resilience.
## Tech stack
 
- **C** — core encoding and decoding logic
- **libpng** — lossless frame output
- **ffmpeg** — video assembly and frame extraction (via subprocess pipe)
- **Hamming codes** — error correction for bit-level resilience