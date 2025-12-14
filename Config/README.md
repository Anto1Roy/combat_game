# Input & Tuning Config

## Input Mapping Suggestions
- **Player 1 (Keyboard/Mouse)**: WASD move, Mouse for look, Left Click Attack, Right Click Parry, Space Dodge.
- **Player 2 (Keyboard)**: Arrow keys move, Numpad 0 Attack, Numpad Enter Parry, Right Shift Dodge.
- **Player 2 (Gamepad)**: Left Stick move, Right Stick look, RT Attack, LT Parry, A Dodge.

## Console Variables (Proposed)
- `cg.Stamina.RegenRate`
- `cg.Stamina.DelayAfterSpend`
- `cg.Combat.DebugDraw`
- `cg.Hitbox.SweepSubsteps`

## Data Assets
- Create `PrimaryDataAsset` entries per action with durations, stamina cost, damage, knockback, and hitbox profiles.
