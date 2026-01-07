# C++ Backend

See the root README for full details. Quick build/run:

MSVC (Developer Command Prompt):

```bat
cl /std:c++17 /O2 /I ..\include ..\main.cpp ..\src\*.cpp /Fe:..\bin\hospital_nav.exe
```

MinGW-w64:

```bash
g++ -std=c++17 -O2 -I ../include ../main.cpp ../src/*.cpp -o ../bin/hospital_nav.exe -lws2_32
```

Run the server:

```bat
..\bin\hospital_nav.exe
```

Default: http://127.0.0.1:8081
