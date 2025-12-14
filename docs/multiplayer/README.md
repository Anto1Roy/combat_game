# Local Multiplayer & Networking Readiness

## Split-Screen MVP
- Enable split-screen in project settings; start with horizontal split for readability.
- Spawn two player controllers at BeginPlay; assign input contexts (P1 keyboard/mouse, P2 gamepad/secondary keyboard bindings).
- Keep camera FOV adjustable per viewport to maintain clarity in first-person.

## Listen-Server Option
- Allow launching as listen server with `?listen` URL option; second instance joins via IP for LAN testing.
- Reuse replication code paths even in local play to surface timing issues early.

## Replication Guidelines
- Execute authoritative combat logic on server-owned characters.
- Use RPCs for action start/stop; replicate condensed action state (ID + timestamps) rather than raw inputs.
- Replicate health/stamina with `OnRep` handlers to trigger HUD updates.

## Desync Mitigation for Future Online
- Centralize outcome resolution in a pure function so both server and clients run the same logic.
- Add configurable grace windows for parries/dodges to tolerate latency.
- Use Unreal's Network Emulation settings in PIE to test under simulated ping and packet loss.
