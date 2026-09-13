# Solar System and Launch Window calculation

## Road Map

[x] Initialize the project (copied from my repo
<https://github.com/Tinotsu/cpp-opengl>) - 13.09.2026

### Prototyping in 2D

[ ] Drawing a circle in 2D space
[ ] Add acceleration

- Newtonian mechanics / Kinematics

[ ] Add collision on the border (remove later)
    - Collision mechanics
    - Momentum / impulse / coefficient of restitution

[ ] Fixed time step (delta t) and integrator
    - Differential equations
    - Euler Integration
    - Numerical error / time step

[ ] GUI to pause, accelerate, set time for the simulation
[ ] Making two objects orbiting around each other (Verlet/Leapfrog integration)
    - Newtonian gravity
    - Circular orbits
    - Conservation of energy
    - Angular momentum
    - Verlet / Leapfrog integration

[ ] 3 body problems
    - N-body Newtonian gravity

### Solar system in 3D

[ ] Moving camera
    - Linear algebra

[ ] Transitioning objects from circle to sphere
[ ] Light with shaders on the objects
[ ] trace to visualize trajectories of the objects
[ ] Setting the attributes (mass, trajectory, etc.) of the planets and the Sun
[ ] Texture for each objects
[ ] Stars in the background

### Launch Windows Calculation

[] Button to set the camera in position to make the system look 2D
[] SpaceShip object with trace
    - Two-body orbital mechanics
    - Kepler's laws
    - Vis-viva equation

[ ] Calculate Hohmann transfer
    - Transfer orbit
    - Delta-v
    - Transfer time

[ ] GUI to set the start and end of the spaceship - One way
[ ] Calculate launch window
    - Orbital periods
    - Angular velocity
    - Phase angle

[ ] Click option to set where the spaceship start

[ ] Round trip
    - Lambert's problem
    - Numerical root finding
