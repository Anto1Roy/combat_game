# Architecture Overview

## Goals
- PC-only focus leveraging UE5 features (Chaos physics, high-quality assets).
- Local multiplayer-first with networking-ready patterns (authoritative server model, replication hooks).
- Data-driven combat actions to iterate quickly.

## Project Layout (UE5 Recommendation)
- `/Source/CombatGame/` — C++ gameplay modules (characters, components, systems).
- `/Content/Blueprints/Characters/` — Blueprint wrappers for rapid iteration.
- `/Content/Data/CombatActions/` — Data assets for tuning timing, costs, damage.
- `/Content/Maps/Arena/` — Simple flat arena for MVP.
- `/Content/UI/Debug/` — Temporary HUD for health/stamina.

## Gameplay Layering
1. **Input Mapping Contexts** feed into player controllers with separate bindings for Player 1/Player 2.
2. **Character/Pawn** owns a `UCombatActionComponent` for action execution and a `UCombatStateComponent` for state gating.
3. **Combat System** resolves interactions (attack vs parry vs trade) and issues replicated events for health/stamina changes.
4. **Presentation** (Animation Blueprint, Niagara, UI) reacts to gameplay events but does not own logic.

## Replication Posture
- Mark authoritative logic on server functions (`ServerStartAction`, `ServerApplyHit`).
- Use `OnRep` callbacks for health/stamina to keep HUD in sync.
- Prefer deterministic action resolution functions shared between server and client prediction.

## Local Multiplayer Setup
- Enable split-screen in project settings; add two player start positions in the arena map.
- Provide input contexts for keyboard/mouse (P1) and gamepad (P2) out of the box.

## Extensibility
- Keep actions data-driven via data assets; allow designers to duplicate and tweak.
- Decouple hit detection into `UWeaponHitboxComponent` to swap between traces, sweeps, or overlap volumes.
- Use gameplay tags (optional) to label actions and states for future Ability System integration.
