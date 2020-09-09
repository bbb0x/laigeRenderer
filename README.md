# laigeRenderer
A DirectX 11 library that can render a Rainbow Track in 1 draw call.

<img src="https://raw.githubusercontent.com/bbb0x/laigeRenderer/master/assets/laigerenderer.png"/>


Telemetry applications analyzing race data usually use a Rainbow Track to show precise data of a lap and to hint in what part of a track it is possible to gain time.
The problem is that WPF and middle ware libraries offer no performant solutions to this issue. 
For this reason I decided to implement my own native c++ DirectX 11 based Rainbow Track renderer. It can be used within a C# WPF Application (sample in source code) via the exposed C functions.

# Building
Visual Studio 2019 project file is within the files, but you are welcome to use your own IDE.