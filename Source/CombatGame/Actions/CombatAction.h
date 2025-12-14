#pragma once

#include <cstdint>
#include <functional>
#include <string>

// Unreal-style stub for data-driven combat actions.
// In-engine, convert to a USTRUCT with BlueprintType to expose to designers.
namespace Combat
{
    enum class ECombatPhase : uint8_t
    {
        Idle,
        Windup,
        Active,
        Recovery
    };

    struct FHitboxDefinition
    {
        // Name of the socket used as the origin for the sweep.
        std::string SocketName;
        // Radius or half-size depending on shape type (capsule/box).
        float Size = 15.f;
        // How many substeps to sweep per tick to avoid tunneling.
        int32_t SweepSubsteps = 2;
    };

    struct FCombatActionData
    {
        std::string Name;
        float WindupTime = 0.2f;
        float ActiveTime = 0.1f;
        float RecoveryTime = 0.3f;
        float StaminaCost = 15.f;
        float Damage = 20.f;
        float KnockbackImpulse = 600.f;
        bool bCanMoveDuringAction = false;
        float MovementSlowdown = 0.35f;
        FHitboxDefinition Hitbox;
    };

    // Helper to compute timing windows and callbacks for gameplay/animation sync.
    class FCombatActionTimeline
    {
    public:
        using PhaseCallback = std::function<void(ECombatPhase)>;

        explicit FCombatActionTimeline(const FCombatActionData& InData)
            : Data(InData)
        {
        }

        void OnPhaseChanged(PhaseCallback Callback)
        {
            PhaseListener = std::move(Callback);
        }

        // Advance timeline by DeltaSeconds; triggers callbacks when phase changes.
        void Tick(float DeltaSeconds)
        {
            Elapsed += DeltaSeconds;

            ECombatPhase NewPhase = ResolvePhase();
            if (NewPhase != CurrentPhase)
            {
                CurrentPhase = NewPhase;
                if (PhaseListener)
                {
                    PhaseListener(CurrentPhase);
                }
            }
        }

        void Reset()
        {
            Elapsed = 0.f;
            CurrentPhase = ECombatPhase::Idle;
        }

        ECombatPhase GetCurrentPhase() const { return CurrentPhase; }

        float GetNormalizedTime() const
        {
            const float Total = Data.WindupTime + Data.ActiveTime + Data.RecoveryTime;
            return Total > 0.f ? Elapsed / Total : 0.f;
        }

    private:
        ECombatPhase ResolvePhase() const
        {
            if (Elapsed < Data.WindupTime)
            {
                return ECombatPhase::Windup;
            }
            if (Elapsed < Data.WindupTime + Data.ActiveTime)
            {
                return ECombatPhase::Active;
            }
            if (Elapsed < Data.WindupTime + Data.ActiveTime + Data.RecoveryTime)
            {
                return ECombatPhase::Recovery;
            }
            return ECombatPhase::Idle;
        }

    private:
        FCombatActionData Data;
        float Elapsed = 0.f;
        ECombatPhase CurrentPhase = ECombatPhase::Idle;
        PhaseCallback PhaseListener;
    };
}
