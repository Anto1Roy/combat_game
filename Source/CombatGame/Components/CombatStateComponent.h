#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../Actions/CombatAction.h"

// Unreal-style stub for a component that enforces action gating and stamina usage.
class UCombatStateComponent
{
public:
    struct FStaminaSettings
    {
        float MaxStamina = 100.f;
        float RegenRate = 20.f;          // per second
        float RegenDelay = 0.75f;        // seconds after spend
    };

    struct FActionBuffer
    {
        std::string QueuedActionName;
        float TimeRemaining = 0.f;
    };

public:
    explicit UCombatStateComponent(const FStaminaSettings& InSettings)
        : StaminaSettings(InSettings)
        , CurrentStamina(InSettings.MaxStamina)
    {
    }

    void Tick(float DeltaSeconds)
    {
        TickStamina(DeltaSeconds);
        TickCurrentAction(DeltaSeconds);
        ProcessBufferedAction();
    }

    bool CanStartAction(const Combat::FCombatActionData& Action) const
    {
        return CurrentState == Combat::ECombatPhase::Idle && CurrentStamina >= Action.StaminaCost;
    }

    void StartAction(const Combat::FCombatActionData& Action)
    {
        if (!CanStartAction(Action))
        {
            BufferAction(Action.Name, 0.2f);
            return;
        }

        CurrentState = Combat::ECombatPhase::Windup;
        CurrentAction = Action;
        CurrentTimeline = Combat::FCombatActionTimeline(Action);
        CurrentTimeline.OnPhaseChanged([this](Combat::ECombatPhase NewPhase)
        {
            CurrentState = NewPhase;
        });

        SpendStamina(Action.StaminaCost);
    }

    void BufferAction(const std::string& ActionName, float BufferTime)
    {
        ActionBuffer.QueuedActionName = ActionName;
        ActionBuffer.TimeRemaining = BufferTime;
    }

    float GetStamina() const { return CurrentStamina; }
    float GetMaxStamina() const { return StaminaSettings.MaxStamina; }
    Combat::ECombatPhase GetCurrentState() const { return CurrentState; }
    const Combat::FCombatActionData* GetCurrentAction() const { return CurrentAction ? &CurrentAction.value() : nullptr; }

private:
    void TickCurrentAction(float DeltaSeconds)
    {
        if (!CurrentAction.has_value())
        {
            return;
        }

        CurrentTimeline.Tick(DeltaSeconds);

        if (CurrentTimeline.GetCurrentPhase() == Combat::ECombatPhase::Idle)
        {
            CurrentAction.reset();
            CurrentState = Combat::ECombatPhase::Idle;
        }
    }

    void TickStamina(float DeltaSeconds)
    {
        if (StaminaRegenDelayRemaining > 0.f)
        {
            StaminaRegenDelayRemaining -= DeltaSeconds;
            return;
        }

        CurrentStamina = std::min(StaminaSettings.MaxStamina, CurrentStamina + StaminaSettings.RegenRate * DeltaSeconds);
    }

    void SpendStamina(float Amount)
    {
        CurrentStamina = std::max(0.f, CurrentStamina - Amount);
        StaminaRegenDelayRemaining = StaminaSettings.RegenDelay;
    }

    void ProcessBufferedAction()
    {
        if (ActionBuffer.TimeRemaining <= 0.f)
        {
            return;
        }

        ActionBuffer.TimeRemaining -= 0.016f; // assume tick; in UE hook this to DeltaSeconds
        if (ActionBuffer.TimeRemaining <= 0.f)
        {
            // placeholder: actual lookup should resolve action data by name from a registry/data asset
            ActionBuffer.QueuedActionName.clear();
        }
    }

private:
    std::optional<Combat::FCombatActionData> CurrentAction;
    Combat::FCombatActionTimeline CurrentTimeline{ Combat::FCombatActionData() };

    Combat::ECombatPhase CurrentState = Combat::ECombatPhase::Idle;
    FStaminaSettings StaminaSettings;
    float CurrentStamina = 0.f;
    float StaminaRegenDelayRemaining = 0.f;

    FActionBuffer ActionBuffer;
};
