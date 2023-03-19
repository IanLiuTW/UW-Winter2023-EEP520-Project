#ifndef __BULLET_AGENT__H
#define __BULLET_AGENT__H

#include <iostream>
#include "enviro.h"

using namespace enviro;
using namespace std;

// Controller for a bullet. It is responsible for moving the bullet.
class BulletController : public Process, public AgentInterface
{

public:
    BulletController() : Process(), AgentInterface() {}

    void init()
    {
        // Watch for the gunner angle and set the firing angle to that value.
        watch("gunner_angle", [this](Event e)
              { firing_angle = e.value()["angle"]; });
        // Watch if the protagonist has reached the goal and increase the bullet speed.
        watch("goal_reached", [this](Event e)
              {firing_speed+=50; bullet_speed += 50; });
        // Watch if the protagonist has swapped with a follower and increase the bullet speed.
        watch("follower_swap_request", [this](Event e)
              {firing_speed+=30; bullet_speed += 30; });
        // Watch if the protagonist has dashed and increase the bullet speed.
        watch("leader_dash", [this](Event e)
              {firing_speed+=10; bullet_speed += 10; });
        // Watch if a new game has started and respawn the bullet.
        watch("respawn_bullet", [this](Event e)
              { respawn(); });
        // respawn the bullet for the first time.
        respawn();
    }
    void start() {}
    void update()
    {
        // If the bullet has not been fired.
        if (!fired)
        {
            // Set the bullet's position to the center of the map and set its angle to the firing angle.
            teleport(0, 0, firing_angle);
            // Set the counter of ricochets to 5.
            ricochet_count = 5;
            // Fire the bullet.
            apply_force(firing_speed, 0);
            // Emit an event to notify the gunner that the bullet has been fired.
            emit(Event("bullet_fired"));
            fired = true;
        }
        // If the bullet has hit a thing, decrease the counter of ricochets.
        else if (sensor_value(0) < 10 || sensor_value(1) < 10)
        {
            ricochet_count--;
            // If the counter of ricochets is 0, stop the bullet to let it to be fired again.
            if (ricochet_count == 0)
                fired = false;
        }
        // If the bullet is in firing mode, apply the bullet speed to it.
        else
        {
            apply_force(bullet_speed, 0);
        }
        // Annoucnce the bullet speed to other agents.
        emit(Event("bullet_speed", {{"speed", bullet_speed}}));
    }
    void stop() {}

    // Respawn the bullet. Set related variables to their initial values.
    void respawn()
    {
        firing_angle = 0;
        bullet_speed = 300;
        firing_speed = 400;
        fired = false;
    }

private:
    double firing_angle;
    bool fired;
    int ricochet_count;
    int bullet_speed, firing_speed;
};

class Bullet : public Agent
{
public:
    Bullet(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    BulletController c;
};

DECLARE_INTERFACE(Bullet)

#endif