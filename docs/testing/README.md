# Testing & Debugging

## In-Editor Scenarios
- **Split-Screen PIE**: launch two players, verify both can perform attack/parry/dodge simultaneously.
- **Listen Server PIE**: run one server, one client to exercise replication paths.
- **Frame Rate Stress**: throttle FPS (`t.MaxFPS 30`) to confirm sweep-based hit detection still registers.

## Debug HUD
- Display health/stamina, current action state, and last hit result above each pawn.
- Toggle via console variable (e.g., `cg.DebugHUD 1`).

## Automation Hooks
- Blueprint function library for scripted sparring sequences to test timings.
- Unit-test friendly pure functions for action resolution and stamina calculations.
