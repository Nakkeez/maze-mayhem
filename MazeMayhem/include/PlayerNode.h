#pragma once

#include "../../core-engine/core/include/CameraNode.h"

class PlayerNode : public CameraNode
{
public:
    enum class PlayerState {
        STATIONARY,
        JUMPING,
        FALLING
    };

    /*
     * Update player node and its children
     * @param frametime frame delta time
     */
    void Update(float frametime) override;

    /**
     * Set current state of the player object.
     * @param state the state to be set for the player object
     */
    void SetPlayerState(PlayerState state)
    {
        m_PlayerState = state;
    }

    /**
     * @return current state of the player object.
     */
    PlayerState GetPlayerState() const { return m_PlayerState; }

private:
    PlayerState     m_PlayerState;
};