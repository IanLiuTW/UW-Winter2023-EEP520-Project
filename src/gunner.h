#ifndef __GUNNER_AGENT__H
#define __GUNNER_AGENT__H

#include "enviro.h"

using namespace enviro;

// The gunner is a agent that stays in the center of the screen and fires bullets at the leader.
class GunnerController : public Process, public AgentInterface
{

public:
    GunnerController() : Process(), AgentInterface() {}

    void init()
    {
        // Watch the leader's position and record it.
        watch("leader_position", [this](Event e)
              {
            leader_x = e.value()["x"];
            leader_y = e.value()["y"]; });
        // Watch if a bullet has been fired and teleport the gunner off map and back to the center of the screen.
        // This is done to prevent the gunner from being hit by its own bullets.
        watch("bullet_fired", [this](Event e)
              { teleport(-1000, -1000, angle());
              teleport(0, 0, angle()); });
    }
    void start() {}
    void update()
    {
        // Rotate the gunner to face the leader's position.
        move_toward(leader_x, leader_y, 0, 1000);
        // In case the gunner got pushed off its position, teleport it back to the center of the screen.
        teleport(0, 0, angle());
        // Emit the gunner's angle so that the bullets can be fired in the correct direction.
        emit(Event("gunner_angle", {{"angle", angle()}}));
    }
    void stop() {}

private:
    double leader_x, leader_y;
};

class Gunner : public Agent
{
public:
    Gunner(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    GunnerController c;
};

DECLARE_INTERFACE(Gunner)

#endif