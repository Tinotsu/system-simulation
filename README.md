# Solar System and Launch Window calculation

## Build

`cmake --build build`
`./build/Application`

## Road Map

### *Make it work*

---

- [x] Initialize the project (copied from my repo
<https://github.com/Tinotsu/cpp-opengl>) - 13.09.2026

### Prototyping in 2D

- [x] Drawing a circle in 2D space - 13.09.2026

- [x] Add acceleration - 18.09.2026

- [x] Add collision on the border (remove later) - 19.09.2026

- [x] Fixed time step (delta t) and integrator - 18.09.2026

- [ ] GUI to pause, accelerate, set time for the simulation
- [ ] Making two objects orbiting around each other (Verlet/Leapfrog integration)
  - Newtonian gravity
  - Circular orbits
  - Conservation of energy
  - Angular momentum
  - Verlet / Leapfrog integration

- [ ] 3 body problems
  - N-body Newtonian gravity

### Solar system in 3D

- [ ] Moving camera
  - Linear algebra

- [ ] Transitioning objects from circle to sphere
- [ ] Light with shaders on the objects
- [ ] trace to visualize trajectories of the objects
- [ ] Setting the attributes (mass, trajectory, etc.) of the planets and the Sun
- [ ] Texture for each objects
- [ ] Stars in the background

### Launch Windows Calculation

- [ ] Button to set the camera in position to make the system look 2D
- [ ] SpaceShip object with trace
  - Two-body orbital mechanics
  - Kepler's laws
  - Vis-viva equation

- [ ] Calculate Hohmann transfer
  - Transfer orbit
  - Delta-v
  - Transfer time

- [ ] GUI to set the start and end of the spaceship - One way
- [ ] Calculate launch window
  - Orbital periods
  - Angular velocity
  - Phase angle

- [ ] Click option to set where the spaceship start

- [ ] Round trip
  - Lambert's problem
  - Numerical root finding

### Make it right

---

### Make it fast

---

- [ ] Replacing OpenGL draw-call with batch rendering

## Journal

### How to render a circle with a fragment shader

```SHADER
void main()
{
    vec2 uv = vec2(gl_fragcoord.xy)/vec2(iresolution) * 2.0 - 1.0;
    float aspect = float(iresolution.x) / float(iresolution.y);
    uv.x *= aspect;
    
    fragcolor.rg = uv;
    fragcolor.b = 0.5;
    
    float distance = step(length(localposition), 0.5);
    fragcolor = vec4(vec3(distance), 1.0);
    
    if (any(greaterthan(uv, vec2(1.0))) || any(lessthan(uv,vec2(-1, -1))))
    {
        fragcolor.rgb = vec3(0.0);
    }
}
```

TODO : explain this code

### How to make an object falling

First I tried to make the object moving to the bottom : `alt += 0.5f;` , alt
being the x coordinate of the object updated each frame.

To make the time less depending of the performance of the computer the code is
running on, I added a this:

```C++
constexpr double FIXED_DT = 1.0 / 60.0;

double accumulator = 0.0f;
auto previous = std::chrono::steady_clock::now();

while (running)
{
    auto current = std::chrono::steady_clock::now();

    double frameTime =
        std::chrono::duration<double>(current - previous).count();

    previous = current;
    accumulator += frameTime;

    while (accumulator >= FIXED_DT)
    {
        alt += -0.5f;
        accumulator -= FIXED_DT;
    }
}
```

It makes the simulation advancing of 1/60 seconds and the accumulator is here
to make sure the simulation run at a consistent speed regardless is rendering at
30 FPS or 3000 FPS.

The problem with the current movement of the object is that's the object is not
falling, what's actually describing its movement is a velocity but the action
of gravitation is an acceleration : $a = \frac{d^2x}{dt^2}$

```C++
while (accumulator >= FIXED_DT) {
    dt += FIXED_DT;
    alt += -0.5f * dt;
    accumulator -= FIXED_DT;
}
```

Now the object seems to fall when we start the simulation. But there is a problem,
the acceleration depend on how many updates the simulation perform, if `FIXED_DT`
change then `dt` change too. We should then start with a constant that could then
give us the next values.
On Earth, the constant for the acceleration of the gravitation is
$g = 9.81ms^-2$, we can start from that because it is constant.
Then :

$$a = \frac{dv}{dt} = \frac{d^2x}{dt^2}
\iff
dx = v\,dt
\quad\text{and}\quad
dv = a\,dt$$

In code :

```C++
v += g * FIXED_DT
alt -= v * FIXED_DT
```

And we removed `dt` because we don't need the accumulation time in our calculation.

### Collision

For simulating a bouncing ball effect for the object, we can simply inverse
the sens and decrease its velocity when it reach the bottom border of the simulation
window.

```C++
if (alt <= BOTTOM_BORDER) {
    v = -v * e;
}
```

In classical mechanics *e* is the coefficient of restitution what is the measure
of the springiness of collisions between two surfaces.

Since the ground is stationary:

$$e = \frac{|v_{\text{after}}|}{|v_{\text{before}}|}$$

In the code I chosen an arbitrary value because it's a random object.
