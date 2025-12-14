#pragma once

#include <algorithm>
#include <optional>

#include "../Components/CombatStateComponent.h"

// Unreal-style stub representing the player-controlled pawn.
class ACombatantCharacter
{
public:
    ACombatantCharacter()
        : StateComponent(UCombatStateComponent::FStaminaSettings())
    {
    }

    void Tick(float DeltaSeconds)
    {
        StateComponent.Tick(DeltaSeconds);
        // In-engine: forward movement input, camera, etc.
    }

    void Attack()
    {
        if (LightAttackAction.has_value())
        {
            StateComponent.StartAction(LightAttackAction.value());
        }
    }

    void Parry()
    {
        if (ParryAction.has_value())
        {
            StateComponent.StartAction(ParryAction.value());
        }
    }

    void Dodge()
    {
        if (DodgeAction.has_value())
        {
            StateComponent.StartAction(DodgeAction.value());
        }
    }

    float GetHealth() const { return Health; }
    void ApplyDamage(float Amount)
    {
        Health = std::max(0.f, Health - Amount);
        // TODO: broadcast to HUD/animation
    }

    void SetActionProfiles(const Combat::FCombatActionData& InLightAttack,
                           const Combat::FCombatActionData& InParry,
                           const Combat::FCombatActionData& InDodge)
    {
        LightAttackAction = InLightAttack;
        ParryAction = InParry;
        DodgeAction = InDodge;
    }

private:
    float Health = 100.f;
    UCombatStateComponent StateComponent;

    std::optional<Combat::FCombatActionData> LightAttackAction;
    std::optional<Combat::FCombatActionData> ParryAction;
    std::optional<Combat::FCombatActionData> DodgeAction;
};
