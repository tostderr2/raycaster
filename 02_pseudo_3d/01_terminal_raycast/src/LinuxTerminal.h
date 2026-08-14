#pragma once

#include "Helper.h" // for Cell
#include <termios.h>
#include <vector>

// Puts stdin in raw mode, switches to the terminal's alternate screen
// buffer (so we never scribble over the user's scrollback), hides the
// cursor, and does an initial full clear.
void EnableRawMode(termios &orig_termios);

// Restores everything EnableRawMode changed, in reverse order.
void DisableRawMode(termios &orig_termios);

void linuxTerminalClose(bool rawModeEnabled, termios &orig_termios);

// Renders the Cell grid to the terminal.
//
// This diffs against the previous frame and only rewrites the rows that
// actually changed (typically just the HUD line or two while the player
// is standing still), positioning the cursor explicitly at the start of
// every row it rewrites rather than relying on the terminal's auto-wrap
// to land in the right place -- that assumption is what caused the
// tearing/blank-row/stale-row corruption in the previous version.
//
// Every byte handed to write() is guaranteed to actually be sent (short
// writes are looped, not discarded), and the whole frame is wrapped in a
// "synchronized update" escape sequence so terminals that support it
// (kitty, wezterm, iTerm2, ...) present the frame atomically instead of
// showing it mid-draw. On unsupported terminals it's a harmless no-op.
//
// If width/height differ from the previous call, the whole screen is
// cleared and fully redrawn once, then diffing resumes from there.
void DrawFrame(const std::vector<Cell> &screen, int width, int height);
