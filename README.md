# 🏥 Hospital Navigation System (Web + C++ Backend)

An educational navigation system for a hospital campus that demonstrates classical data structures and algorithms in a realistic setting. The frontend is a Leaflet-based web app; the backend is a minimal C++ HTTP API implementing authentication, Dijkstra-based routing, history via a stack, and visited locations via a set.

—

## 🎯 What This Project Shows

- Shortest-path routing with Dijkstra’s algorithm on real coordinates
- Using core data structures in practice:
   - Stack: navigation history
   - Set: unique visited locations
   - Hash map: username/password authentication
- Simple C++ HTTP server with JSON responses consumed by a browser

—

## 🧩 Architecture

- Frontend: HTML/CSS/JS (Leaflet, Turf) under `Pages/`
- Backend: C++17 single-binary API server under `main.cpp` + `src/` + `include/`
- Map data: GeoJSON/OSM and curated points under `MapData/`

Server runs at `http://127.0.0.1:8081` and exposes:

- `POST /api/login` → `{ success: true|false }`
- `GET  /api/points` → list of points with `id,name,lat,lon`
- `POST /api/route` → `{ distance, path[] }`
- `GET  /api/history` → array of most recent origin→destination strings
- `GET  /api/visited` → array of unique visited destination names

—

## 🖥️ Quick Start

Two ways to try it:

1) Frontend-only demo (no backend required)
- Open `Pages/Navigation/index.html` in your browser.
- Pick a Start and Destination to see a computed route and distance.

2) Full stack (backend + login)
- Build and run the C++ server (see Build section below).
- Open `Pages/Login/login.html` and sign in with a sample account, e.g.:
   - Username: `safi`
   - Password: `safipass`
- You’ll be redirected to the navigation page.

—

## 🛠️ Build (Windows-focused)

Requirements:
- A C++17 compiler (one of):
   - Visual Studio 2019+ (MSVC developer command prompt)
   - MSYS2/MinGW-w64 with `g++`

Source layout to compile:
- Entry point: `main.cpp`
- Sources: everything in `src/`
- Headers: `include/`

MSVC (Developer Command Prompt):

```bat
cl /std:c++17 /O2 /I include main.cpp src\*.cpp /Fe:bin\hospital_nav.exe
```

MinGW-w64 (PowerShell/MSYS2 shell):

```bash
g++ -std=c++17 -O2 -I include main.cpp src/*.cpp -o bin/hospital_nav.exe -lws2_32
```

Run the server:

```bat
bin\hospital_nav.exe
```

You should see: `Starting C++ backend API server...` then `listening on http://127.0.0.1:8081`.

—

## 🧪 Verifying the API

Points:
```bash
curl http://127.0.0.1:8081/api/points
```

Login note: the current server expects simple key/value lines in the request body (not URL-encoded form). Example:

```bash
curl -X POST http://127.0.0.1:8081/api/login \
   -H "Content-Type: text/plain" \
   --data-binary '"user":"safi"\n"pass":"safipass"'
```

If you use the provided login page, make sure the server is running. If login fails, open `Pages/Navigation/index.html` directly to explore the routing UI without backend.

—

## 📚 Data Structures in Code

- Dijkstra: `src/dijkstra.cpp`, `include/dijkstra.h`
- Stack-based history: `src/stack.h` (templated) used in `main.cpp`
- Visited set: `src/visited.cpp`, `include/visited.h`
- Auth map: `src/auth.cpp`, `include/auth.h` (see sample users)

—

## 📁 Folder Guide

```
MapData/                 # OSM/GeoJSON + curated points
Pages/
   Login/                 # Login UI (talks to backend on 127.0.0.1:8081)
   Navigation/            # Map + routing UI (Leaflet + Turf)
StructuresAlgorithms/    # JS versions for didactic reference
Utils/                   # Frontend helpers
src/                     # C++ sources (server, dijkstra, json, etc.)
include/                 # C++ headers
bin/                     # Build output (created by you)
main.cpp                 # C++ API entrypoint
```

—

## 🔧 Troubleshooting

- Port already in use: adjust the port in `main.cpp` at `svr.listen("127.0.0.1", 8081);` and in `Pages/Login/login.js` `API_BASE`.
- CORS: server sends `Access-Control-Allow-Origin: *` for GET/POST/OPTIONS; ensure you hit `127.0.0.1`, not another host.
- MinGW link errors on Windows: add `-lws2_32`.
- Login payloads: the current backend expects simple `"key":"value"` lines; for browser form posts you may need to adjust the server parsing logic or send raw text as shown above.

—

## 👤 Author

Built with ❤️ by **Fahad Mustafa** — for educational use.

Screenshots: see `Screenshots/`.

