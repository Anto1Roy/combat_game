# Combat System Design

## Core Loop
1. Windup: character enters action-specific anticipation pose; hitboxes disabled.
2. Active: hitbox sweeps are enabled for the defined window (single confirmed hit per swing).
3. Recovery: input buffered but gated until recovery ends.

## Action Data Fields
- `Name` / `GameplayTag`
- `WindupTime`, `ActiveTime`, `RecoveryTime`
- `StaminaCost`, `Damage`, `KnockbackImpulse`
- `bCanMoveDuringAction`, `MovementSlowdown`
- `HitboxDefinition` (capsule/box, relative sockets, sweep frequency)

## Hit Detection Strategy
- Use `UWeaponHitboxComponent` to manage collision shapes separate from weapon meshes.
- Sweep capsule traces between previous and current socket transforms to avoid tunneling.
- Lock out additional hits after first valid overlap per active window to prevent multi-hit spam.
- When simultaneous hits occur, resolve via priority: Parry > Trade > BothTakeDamage.

## Stamina and Health
- `Stamina` drains per action, regenerates over time with a short delay after spending.
- `Health` reduced on confirmed hit; emit events for HUD and VFX.
- Low-stamina effects: slower action speed, reduced knockback output.

## Debug and Tuning
- Display current state and stamina above each character (Debug HUD widget).
- Log action transitions with timestamps for balance review.
- Provide console variables for hitbox debug drawing and stamina regeneration rate.
