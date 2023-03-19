#ifndef __LEADER_AGENT__H
#define __LEADER_AGENT__H

#include <iostream>
#include "enviro.h"

using namespace enviro;
using namespace std;

// State for the leader when it is alive.
class Alive : public State, public AgentInterface
{
public:
    Alive() : State("Alive"), AgentInterface() {}
    void entry(const Event &e)
    {
        // Set the color of the leader to blue, which means it is alive.
        set_style({{"fill", "CornflowerBlue"}});
    }
    void during()
    {
        // Announce the leader's position to the others.
        emit(Event("leader_position", {{"x", position().x},
                                       {"y", position().y}}));
        // Announce the leader is alive.
        emit(Event("leader_alive"));
    }
    void exit(const Event &e) {}
};

// State for the leader when it is dead.
class Dead : public State, public AgentInterface
{
public:
    Dead() : State("Dead"), AgentInterface() {}

    void entry(const Event &e)
    {
        // Set the color of the leader to red, which means it is dead.
        set_style({{"fill", "red"}});
    }
    void during()
    {
        // Stop the leader's movement.
        damp_movement();
        // Announce the leader is dead.
        emit(Event("leader_dead"));
    }
    void exit(const Event &e) {}
};

// Leader is the protagonist of the game. It is a state machine that can be in two states: Alive and Dead.
// It can be swapped with the follower and dash when clicked. Its objective is to collect the goals.
class LeaderController : public StateMachine,
                         public AgentInterface
{
public:
    LeaderController() : StateMachine(), AgentInterface()
    {
        // Define the initial state and the transitions.
        set_initial(alive);
        add_transition("respawn_leader", dead, alive);
        add_transition("leader_dead", alive, dead);
    }

    void init()
    {
        // Watch if the leader has collided with the bullet. If so, the leader is dead and the game is over.
        notice_collisions_with("Bullet", [&](Event &e)
                               { emit(Event("leader_dead")); });

        // Watch the goal's position and record it.
        watch("goal_position", [this](Event e)
              { target_x = e.value()["x"];
                target_y = e.value()["y"]; });
        // Watch if the follower has been swapped with the leader and teleport the leader to the follower's position.
        watch("follower_swap_request", [this](Event e)
              { teleport(e.value()["x"], e.value()["y"], 0); });
        // Watch if there is a screen click and dash if the leader is alive.
        watch("screen_click", [this](Event e)
              { 
              // Only dash if the leader is alive.
              if (current().name() == "Alive"){
              // Dash in the direction the leader is facing.
              apply_force(2000, 0);
              // Announce the leader has dashed.
              emit(Event("leader_dash"));} });
        // Watch if the new game button has been clicked and trigger the respawn function.
        watch("button_click", [this](Event e)
              {if(e.value()["value"] == "new_game")
              respawn(); });

        // Respawns the leader for the first time.
        respawn();
        StateMachine::init();
    }

    void update()
    {
        // Keep the leader moving toward the goal.
        move_toward(target_x, target_y, 300, 80);
        StateMachine::update();
    }

    // Respawns the leader and tell other agents to respawn as well.
    void respawn()
    {
        // Randomly generate the leader's position which is not too close to the center of the screen.
        int x = rand() % 200 + 100;
        int y = rand() % 100 + 50;
        if (rand() % 2 == 0)
        {
            x *= -1;
        }
        if (rand() % 2 == 0)
        {
            y *= -1;
        }
        teleport(x, y, 0);

        // Letting other agents know to respawn. This sets everything back to the initial state.
        emit(Event("respawn_leader"));
        emit(Event("respawn_follower"));
        emit(Event("respawn_goal"));
        emit(Event("respawn_bullet"));
        emit(Event("respawn_score"));
    }

private:
    Alive alive;
    Dead dead;
    int target_x, target_y;
};

class Leader : public Agent
{
public:
    Leader(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    LeaderController c;
};

DECLARE_INTERFACE(Leader)

#endif