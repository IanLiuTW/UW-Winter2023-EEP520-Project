#ifndef __FOLLOWER_AGENT__H
#define __FOLLOWER_AGENT__H

#include <iostream>
#include "enviro.h"

using namespace enviro;
using namespace std;

// Follower acts as a "follower" to the leader, and will move to protect the leader. It can also be swapped with the leader.
class FollowerController : public Process, public AgentInterface
{

public:
    FollowerController() : Process(), AgentInterface(), leader_x(0), leader_y(0), alive(true) {}

    void init()
    {
        // If there is a collision with the leader, goal, runner, the follower will respawn at random location.
        notice_collisions_with("Leader", [&](Event &e)
                               { if (leader_alive) respawn(); });
        notice_collisions_with("Goal", [&](Event &e)
                               { if (leader_alive) respawn(); });
        notice_collisions_with("Gunner", [&](Event &e)
                               { if (leader_alive) respawn(); });
        // If the follower collides with a bullet, it will be pushed back.
        notice_collisions_with("Bullet", [&](Event &e)
                               { track_velocity(-500, 0); });

        // Keep track of the leader's position and calculate the destination. The Destination is the midpoint between the leader and the gunner.
        watch("leader_position", [this](Event e)
              {
            leader_x = e.value()["x"];
            leader_y = e.value()["y"];
            dest_x = leader_x / 2;
            dest_y = leader_y / 2; });
        // If the follower is clicked, it will swap with the leader.
        watch("agent_click", [this](Event e)
              { 
                // If the leader is alive and this follower is clicked, emit a follower_swap_request event.
                if ( leader_alive && e.value()["id"] == id()){
                // This event tells the leader to swap with the follower and provides the follower's position.
                emit(Event("follower_swap_request", { { "x", position().x }, { "y", position().y } })); 
                // Move to the leader's position.
                teleport(leader_x, leader_y, 0);} });

        // Keep track of the leader's state.
        watch("leader_dead", [this](Event e)
              { leader_alive = false; });
        watch("leader_alive", [this](Event e)
              { leader_alive = true; });

        // Watch if a new game has started and respawn the follower.
        watch("respawn_follower", [this](Event e)
              { respawn(); });
        // Watch if this follower has been killed and call the kill function.
        watch("kill_follower", [this](Event e)
              { if (e.value()["id"] == id()) kill(); });

        // Respawn the follower for the first time.
        respawn();
    }
    void start() {}
    void update()
    {
        // Game over state. No movement.
        if (!leader_alive)
        {
            damp_movement();
            return;
        }
        // If the follower is alive, move toward the destination.
        if (alive)
        {
            // If one of the side sensors detects an obstacle, move back and away from it.
            // This prevents the follower from getting stuck on obstacles on the side.
            if (sensor_value(2) < keep_distance)
            {
                track_velocity(-5, -3);
            }
            // If the front sensors detect an obstacle, move back and away from it.
            // This prevents the follower from getting stuck on obstacles in front.
            else if (sensor_value(0) < keep_distance || sensor_value(1) < keep_distance)
                track_velocity(-5, 0);
            // Move toward the destination.
            else
                move_toward(dest_x, dest_y, 150, 150);

            // Tell others that this follower is alive and its id.
            emit(Event("follower_alive", {{"id", id()}}));
        }
    }
    void stop() {}

    // Respawn the follower at a random location and set its alive state to true.
    void respawn()
    {
        alive = true;
        // Randomly generate a position for the follower which is not too close to the center of the map.
        int x = rand() % 200 + 100;
        int y = rand() % 100 + 50;
        if (rand() % 2 == 0)
            x *= -1;
        if (rand() % 2 == 0)
            y *= -1;
        teleport(x, y, 0);
    }

    // Kill the follower and move it off the map and set its alive state to false.
    void kill()
    {
        alive = false;
        // Move it off the map and stop its movement.
        teleport(-1000, -1000, 0);
        damp_movement();
    }

private:
    double leader_x, leader_y;
    double dest_x, dest_y;
    bool alive;
    bool leader_alive = true;
    // The distance at which the follower will move away from an obstacle.
    int keep_distance = 20;
};

class Follower : public Agent
{
public:
    Follower(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    FollowerController c;
};

DECLARE_INTERFACE(Follower)

#endif