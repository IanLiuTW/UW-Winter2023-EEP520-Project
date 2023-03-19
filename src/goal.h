#ifndef __GOAL_AGENT__H
#define __GOAL_AGENT__H

#include <iostream>
#include "enviro.h"

using namespace enviro;
using namespace std;

// This is the controller for the goal. It is responsible for respawning the goal and notifying the others when the protagonist has reached the goal.
class GoalController : public Process, public AgentInterface
{

public:
    GoalController() : Process(), AgentInterface() {}

    void init()
    {
        // Watch if the protagonist has reached the goal and trigger the goal_reached function.
        notice_collisions_with("Leader", [&](Event &e)
                               { goal_reached(); });
        // Watch the protagonist's position and record it.
        watch("leader_position", [this](Event e)
              {
            leader_x = e.value()["x"];
            leader_y = e.value()["y"]; });
        // Watch which followers are alive and keep and record the next one to be removed.
        watch("follower_alive", [&](Event &e)
              { next_follower_to_removed = e.value()["id"]; });
        // Watch if the new game has started and respawn the goal.
        watch("respawn_goal", [&](Event &e)
              { respawn(); });
        // Respawn the goal for the first time.
        respawn();
    }
    void start() {}
    void update()
    {
        // Goal should not move.
        damp_movement();
        // Announce the goal's position.
        emit(Event("goal_position", {{"x", position().x}, {"y", position().y}}));
    }
    void stop() {}

    // This function is called when the protagonist has reached the goal.
    void goal_reached()
    {
        // Notify the others that the protagonist has reached the goal.
        emit(Event("goal_reached"));
        // Remove one of the followers from the game based on the next_follower_to_removed variable.
        emit(Event("kill_follower", {{"id", next_follower_to_removed}}));
        // Respawn the goal.
        respawn();
    }

    // This function is called when the goal needs to be respawned.
    void respawn()
    {
        // The goal will be respawned in a random location that is not too close to the protagonist.
        int x = rand() % 200 + 100;
        int y = rand() % 100 + 50;
        if (leader_x > 0)
            x *= -1;
        if (rand() % 2 == 0)
            y *= -1;
        teleport(x, y, 0);
    }

private:
    int next_follower_to_removed;
    double leader_x, leader_y;
};

class Goal : public Agent
{
public:
    Goal(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    GoalController c;
};

DECLARE_INTERFACE(Goal)

#endif