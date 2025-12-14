# Animation & Procedural Layers

## Placeholder Strategy
- Use third-person template animations (punch/swing) as stand-ins.
- Annotate frames for windup/active/recovery to align with gameplay timing.
- Allow animation rate scaling based on stamina level.

## Parametric Adjustments
- Apply aim offsets or bone controllers to steer swing direction (left/right/up) from a base clip.
- Blend additive lean/tilt to reflect input-driven angles without new assets.
- Expose curve values for hitbox enable/disable in Animation Blueprint.

## Future-Proofing
- Keep animation triggers event-driven (e.g., `OnActionPhaseChanged`) to swap to final assets later.
- Reserve slots for VFX/SFX notifies when hits land or blocks occur.
- Document required poses for animators (idle, parry, stagger, dodge) with desired durations.
