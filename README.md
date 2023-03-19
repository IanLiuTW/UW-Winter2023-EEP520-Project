# EEP_520_Winter2023 Final Project

## Project Goal

The objective of this project is to utilize Elma and Enviro to construct a multi-agent system. The completed project, along with comprehensive documentation and instructions for running the code, will be made available on GitHub. Through this project, I aim to enhance my skills in C++ programming. Additionally, I plan to push the boundaries of Elma's capabilities and expand Enviro to create a robust, enjoyable, and reliable system. Ultimately, my goal is to deliver a stable and substantial project that showcases the potential of these powerful tools.

## Project Description
This game is designed for a single player and features a protagonist, rewards, and guards. The protagonist serves as the leader, while the guards are the followers. The objective of the game is for the player to navigate the map and collect rewards, while avoiding the gunner who constantly shoots missiles at the protagonist.

To help the player evade the gunner, the guards try to move to the middle of the protagonist and the gunner, acting as both a shield and a companion that can be swapped with the protagonist for greater flexibility. The player cannot control the protagonist directly, but the leader will move automatically towards the reward. Additionally, the player can use a dash function to provide a short burst of speed, but must use it strategically as each use increases the speed of the bullet (missile).

Every time a reward is collected, one guard will depart, making the game more challenging. The player must aim to collect as many rewards as possible while also avoiding the missiles. The game features a "New Game" button located in the top right corner of the screen, which allows the player to start a new game at any time.

## Installation and Running

First, unzip the source code or clone the project to your local machine with the following command:

```shell
git clone https://github.com/IanLiuTW/520-Assignments.git
```

Go to the project directory:

```shell
cd /520-Assignments/project
```

In order to run the project, we need a environment with C++ and Elma installed. I assume the user has docker installed. Download the docker image and run the container. We can compile the code in the container with the following commands:

```shell
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.61 bash
```

Note: We are using enviro version 1.61. The image is available on Docker Hub: https://hub.docker.com/r/klavins/enviro

Now you are ready to compile and run the project. Run the following commands in the terminal:

```shell
make
```

## Usage of the Project

After the code is compiled, we can run the simulation. 
```shell
esm start
enviro
```

- Open `http://localhost` in your browser to see the simulation. 
- To stop the server, press `Ctrl+C` in the terminal.

### Controls

- Click the silver colored agent to swap with the follower.
- Click blank area to dash.
- Click the `New Game` button to start again.

### Agents
- Protagonist: Blue
- Follower: Silver
- Goal: Gold
- Gunner: Black
- Bullet: Black

### Tips 

- The score and the speed of the bullet are displayed on the top of the screen.
- There are two bullets in the game. The bullet from the gunner will push back the followers. Be sure to notice the dynamic and keep track of the bullets as they ricochet around the map.
- Game is over when the protagonist is hit by the bullet.
- Swap often to aboid potential danger.
- Dash to get out of trouble.
- As the game progresses, the bullets will become faster and faster, and you will have less and less followers to protect you. Be sure to collect the rewards as soon as possible.
- Followers will be teleported to a random location when they are in collision with the protagonist, preventing them to interfere with the protagonist's movement.
- End game would be you trying to collect as many goals as possible, even there's not follower left to protect you.

## Challenges and Solutions

### General

- Challenge: This is the first time I am using Elma and Enviro. I am not familiar with the syntax and the structure of the code. 
I have practiced using Elma in class assignments and the bonus question. I can try to build code blocks step by step and extend them. I have to read the documentation and the example code of Enviro and practice using it.
- Challenge: The StateMachine I created is not working as expected.
I have to look into the source code of Enviro and Elma to find out the reason. When creating a custom StateMachine, I should not implement the `init()` method with a empty block. This will result in the `init()` method of the parent class not being called. I can also call the `init()` method of the parent class in the `init()` method of the my custom class.

### Project
- Challenge: The followers are colliding with one another, sometimes even stuck with each other.
Add more range sensors to the followers to detect the obstacles in front of them. The followers will avoid the obstacles and move away from each other. Need to adjust the numbers of the range sensors and related parameters to make the followers move in a more natural way.
- Challenge: The game needs to be more interactive. The user should be able to control the leader.
Add a dash function and swap with follower function to the leader. The user can control with mouse. 
- Chllenage: The game is not challenging enough. 
Increase the speed and add richochet behavior to the bullet(missle). After each stage, the speed of the bullet will become even faster. The bullet will have push back effect to the followers to create a dynamic game play.
- Challenge: Need to let the player know the current stage and the number of rewards collected. 
I used an agent that is invisible and assign label to it. Now the game can give the player feedback with text about the current score and the speed of the bullet (which represents the difficulties).


## Acknowledgements 

### Libraries and Examples
- [Elma Repo](https://github.com/klavinslab/elma)
- [Elma Documentation](https://klavinslab.github.io/elma/index.html)
- [Enviro Repo](https://github.com/klavinslab/enviro)
- [Enviro Example](https://github.com/klavinslab/enviro/tree/master/examples)

### Coding Problem Solver
- [ChatGPT](https://chat.openai.com/chat) (For information and code examples)

### Documentation
- [C++ documentation - DevDocs](https://devdocs.io/cpp/)

## Project Rubric

[Project Instructions](https://github.com/sosper30/eep520/blob/main/Project/Project_assignment.md)

 - ~~5	Uses enviro, or extends enviro in some way.~~
 - ~~5	Code is well documented. Each method and class should have inline comments say what it does.~~
 - ~~5	Project is shared with us as a new project within a student's individual repo.~~
 - ~~5	Repo has a licence file (e.g. the mit licence~~
 - ~~5	Repo has a README.md file~~
 - ~~10	README has an overview of what the goal of the project is~~
 - ~~10	README has a description of key challenges and how they were addressed~~
 - ~~10	README describes how to install and run the code (assuming the user has docker installed)~~
 - ~~10	README describes how to run and/or use the project~~
 - ~~5	README acknowedges all sources of information or code used~~
 - ~~20	Project compiles~~
 - ~~20	Project runs without crashing. Occasional crashes will not result in partial deductions.~~
 - ~~20	Project shows a substantial behavior~~
 - ~~20	Behavior is mostly bug free~~

 *Total: 150*