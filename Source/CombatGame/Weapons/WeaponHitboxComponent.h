#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../Actions/CombatAction.h"

// Unreal-style stub to manage swept hit detection for melee weapons.
class UWeaponHitboxComponent
{
public:
    struct FHitResult
    {
        std::string HitActorName;
        float DamageApplied = 0.f;
        float KnockbackImpulse = 0.f;
    };

    using FOnHitSignature = std::function<void(const FHitResult&)>;

public:
    explicit UWeaponHitboxComponent(const Combat::FHitboxDefinition& InDefinition)
        : Definition(InDefinition)
    {
    }

    void SetOwnerName(const std::string& InOwner) { OwnerName = InOwner; }

    void BeginSwing(float Damage, float Knockback)
    {
        SwingDamage = Damage;
        SwingKnockback = Knockback;
        bHasRegisteredHit = false;
    }

    void EndSwing()
    {
        bHasRegisteredHit = false;
    }

    void SweepBetween(const std::string& StartSocket, const std::string& EndSocket)
    {
        if (bHasRegisteredHit)
        {
            return;
        }

        // Placeholder: in UE, perform SweepMultiByChannel from StartSocket to EndSocket with Definition.Size
        FHitResult Result;
        Result.HitActorName = "SimulatedOpponent";
        Result.DamageApplied = SwingDamage;
        Result.KnockbackImpulse = SwingKnockback;

        bHasRegisteredHit = true;

        if (OnHit)
        {
            OnHit(Result);
        }
    }

    void BindOnHit(FOnHitSignature InDelegate)
    {
        OnHit = std::move(InDelegate);
    }

private:
    Combat::FHitboxDefinition Definition;
    bool bHasRegisteredHit = false;
    float SwingDamage = 0.f;
    float SwingKnockback = 0.f;
    std::string OwnerName;
    FOnHitSignature OnHit;
};
