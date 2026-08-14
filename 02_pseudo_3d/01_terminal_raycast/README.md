# Console FPS — overhauled

A first-person raycaster that renders straight to a truecolor terminal
(kitty, etc). Originally based on the OneLoneCoder "Code-It-Yourself! FPS"
tutorial; this version replaces the renderer and movement internals while
keeping the same 3-file layout.

## Build & run

```
make BUILD=release   # or BUILD=debug
./build/my_program
```

## Controls

| Key       | Action              |
|-----------|---------------------|
| W / S     | Move forward / back |
| A / D     | Turn left / right   |
| Q / E     | Strafe left / right |
| Space     | Sprint              |
| M         | Toggle minimap      |
| ESC       | Quit                |

