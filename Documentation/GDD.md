# Darkness of Kabul 2.0
## Game Design Document

**Author:** Mohammad Faiaz AliKhail
**Engine:** Unreal Engine 5, C++ with Blueprint where it helps
**Platform:** Windows
**Version:** 0.2, October 2026

---

## At a glance

| | |
|---|---|
| Genre | First-person parkour and slingshot action |
| Setting | A quarter of Kabul overrun by zombies, played mostly across its rooftops |
| Goal | Destroy all ten growths before the spores arrive |
| Session | One level, about four minutes |
| You lose if | Your health runs out, or the timer does |
| Core skills | Moving fast, aiming a slingshot, and making a small supply of stones last |

## The story

It started with the gardens. Rooftop vines in the old quarter began growing where nothing should grow, through concrete and up the sides of buildings, overnight. By the time anyone understood it was spreading, the spores were already in the water and the air.

People who breathed too much of it didn't die. They got up again, slower and quieter, and they came looking for the living. Nobody had a better word for them, so everyone just said zombies.

The vines turned out to be the source. Wherever they grow thickest they knot into a single growth, something about the size of a market stall that feeds on whatever is near it and spits poison at anything alive that it can see. While a growth lives, the spores keep coming and the zombies keep getting made.

You grew up on these rooftops and you know them better than anyone left. You have a slingshot your grandfather made and a pouch of river stones. Guns draw every zombie in the district, and a sling doesn't. The wind is turning. In four minutes the spores will drift into the next quarter, and ten growths stand in the way.

## What the game is about

**Movement is the main attraction.** Running, jumping, sliding, vaulting and climbing should feel good even if you never fire a stone.

**The slingshot should feel like a real weapon.** I'm borrowing from the bow: drawing, holding steady, releasing. It takes a little learning, and it rewards you for it.

**Height is safety.** Zombies can't climb. The rooftops are where you catch your breath and the streets are where you pay for mistakes.

**Stones are scarce.** A stone that misses can be picked back up, which usually means walking somewhere you'd rather not.

## The loop

Move across the rooftops, find a growth, take it down, scavenge stones, move on. Clear all ten before the time runs out and you win. Lose all your health, or let the timer reach zero, and the spores win.

## Moving through the city

Most of the four minutes is spent moving, so movement gets the most polish. Each move below has to feel good on its own before anything is built on top of it.

| Move | What it does | What makes it feel right |
|---|---|---|
| Walk and sprint | Speed builds and fades over a fraction of a second instead of snapping | Acceleration ramps up and down, and the view opens up slightly at full sprint |
| Jump | A hop that covers about 7 m when sprinting | Hold for height, jump a moment after leaving a ledge, and a press just before landing still counts |
| Crouch | Slower and steadier, fits under low gaps | Calms the shake of the slingshot |
| Slide | Sprint, then crouch. You skid for about a second and slip under low obstacles | The camera drops, and you can't aim while sliding |
| Vault | Low obstacles like a stall or a waist-high wall are crossed without stopping | Only triggers when you're moving toward it, and you keep your speed |
| Mantle | Taller ledges within reach get climbed. Your hands go to the edge and your body pulls up | Quick, around half a second, with hands that land on the real edge |

The camera does a lot of the work. There's a small head bob while running, a dip on landing that scales with how far you fell, a lower view in a slide and a wider one at full sprint. None of it is big, and all of it gets noticed when it's missing.

Raising the slingshot slows you down and stops you sprinting, so moving and shooting are a real choice.

## The slingshot

A bow is a good model for a slingshot. Both store energy by pulling something back, both reward patience, and both punish shaky hands. So wherever archery has a good idea, the slingshot borrows it.

| From archery | In the slingshot |
|---|---|
| Raising the bow | Right mouse raises the sling and narrows the view a little, so aiming feels like focusing |
| Nocking an arrow | A stone is loaded into the pouch. A short reload sits between shots, which gives shooting a rhythm instead of a held button |
| Draw length | Left mouse draws the pouch back. The bands visibly stretch and it takes about 1.5 seconds to reach full draw. The further the draw, the faster the stone |
| Anchor point | At full draw the pouch always sits in the same spot near the cheek, so the same input gives the same shot |
| Holding at full draw | You can hold for about a second, then your arm tires and the crosshair starts to tremble. Hold to around 3.5 seconds and the band slips: the shot is cancelled, the camera kicks, and you have to raise the sling again |
| Steady hands | Standing still or crouching calms the shake. Moving makes it worse, and you can't draw while sprinting |
| Arrow drop | There's no trajectory line. Stones drop over distance, and a faint trail on the stone for the first moment helps you learn the arc |
| Release | The band snaps, the camera kicks a little, the view pulses, and a hit marker appears if something is struck |
| Finding your arrows | A stone that misses lands on the ground as a pickup. A stone that hits a zombie or a growth is gone |

The reticle does two jobs at once. It starts wide when the draw begins and tightens as the draw completes and your hands settle, so there's no separate bar to watch. Sound helps too: the band creaks, and the pitch rises as it stretches.

Stones leave from the pouch but land where the crosshair points, so close shots are accurate and long shots drop.

A stone does a quarter of a growth's health, so four stones destroy one. A zombie goes down after two. Hardened stones double that, which makes it two for a growth and one for a zombie.

## Enemies

**The growths.** There are ten. Each one sits where the vines are thickest and stays dormant until you step into its field of view with nothing in the way. Then it wakes and spits a glob of poison every second, aimed a little ahead of where you're running. A hit costs 10% of your health. You can tell what it's doing by looking at it: closed and slowly pulsing while dormant, opening and glowing when it notices you, shuddering as it fires. A destroyed growth stays destroyed.

**The zombies.** They wander the streets between the buildings. When one sees you it runs at you and swings once it's close, taking 15% of your health per hit with a pause between swings. They're slower than your sprint and faster than your walk, so you can't stroll away from them but you can outrun them. They can't climb or vault, so a well-timed vault or a mantle onto a roof loses them completely. They're there to keep you moving and to tempt you into spending stones you'd rather save for growths.

## Hazards

**Entangling roots.** A mat of grasping tendrils. Walking into it halves your speed for 30 seconds. They're usually laid across the easiest route, so spotting them early means choosing a harder one.

**Spore clouds.** Drifting spores clog the sling pouch and you can't draw for 30 seconds. They hang near growths on purpose, because they take away your only answer to one.

## Pickups

**Field dressing** restores your health to full. **Hardened stones** double your stone damage for 30 seconds. **Stone caches** add stones and are usually left somewhere exposed. **Single stones** are the ones you missed with. You start with 12 stones and can carry 24.

## Interface

The main menu has Start Game, Instructions and Quit. The pause menu has Resume, Restart and Back to Main Menu. There are win and lose screens.

The HUD shows the countdown timer, a health bar, how many growths are left, your stone count, and an icon for each buff or debuff you're carrying. The reticle shows draw and steadiness, as described above.

## The level

One level, built from a blank map. It's a cluster of old-city buildings joined by ledges, planks, awnings and rooftops, with narrow streets underneath where the zombies wander. It's laid out as a route that teaches before it tests.

- The first rooftops teach each move in turn: a sprint jump, a vault, a slide, a mantle. No growth is in range until they've all been used.
- The first growth is easy. It guards a clear approach and has cover close by.
- No growth can see the whole level, and each one has at least one approach that keeps you out of its sight.
- Two growths cover a courtyard crossing that needs a moving platform, which gives a timed ride under fire.
- Stone caches sit in exposed spots, so resupply costs risk.
- The last growth is on the tallest roof, reached by a run that uses everything you've learned.
- Everything can be reached on foot with no dead ends. The streets are connected for the zombies and the rooftops deliberately aren't.

The timer starts at four minutes. Three is the minimum, and playtesting will decide where it lands.

Before any art, the level is blocked out in coloured placeholder shapes for growths, hazards and pickups, kept in tidy World Outliner folders named Geometry, Turrets, Pickups and Hazards. Actors are named so the level can be read from the outliner, for example TurretPlaceholder3 or SlowHazardPlaceholder1.

## Animation

Animation carries a lot of the feel, so it gets planned early.

| Who | What's needed | Where it comes from |
|---|---|---|
| The player | Idle, walk, sprint, jump, fall, land, crouch, slide, vault, mantle | The new animation set |
| The slingshot arms | Raise, load, draw, hold, release, lower, slip | Adapted from bow animations, since the motion is nearly the same |
| Zombies | Idle, walk, run, attack, hit, crawl, death | The zombie pack from the first version |
| Growths | Dormant, alert, fire, destroyed | Simple authored animations or timelines |

## How the physics works

Four physics mechanics carry the game: projectiles, character motion, moving platforms, and friction and drag. Two-bone IK handles the animation side. The equations aim for the right feel more than strict accuracy, and every number gets tuned by hand, but each one starts from a derivation so it has a reason.

Sketches for each mechanic live in Documentation/Sketches.

### 1. Kinematic projectiles

**Sketch:** `Sketches/01-projectiles.png`

**In the game.** Stones and the growths' poison are projectiles with gravity and nothing else: no steering, no drag. Draw length sets the launch speed, so a quick tap is a weak lob and a full draw is fast and flat.

**The idea.** A stretched band stores energy, E = ½kx². Some of it goes into the stone, ½mv² = ηE, where η is the share that isn't lost. Solving gives v = x√(ηk/m), so launch speed grows in proportion to how far the pouch is pulled. The game uses a simplified version of that:

```
d  = clamp( (t_held − t_min) / (t_full − t_min), 0, 1 )
v₀ = v_min + d · (v_max − v_min)
```

After release, gravity is the only force:

```
v(t) = v₀ + a·t
s(t) = s₀ + v₀·t + ½·a·t²        with a = (0, 0, −g)
```

Each frame the game steps forward with the trapezoid rule. For constant acceleration it gives exactly the same answer as the lines above, so the stepping adds no error of its own:

```
v_next = v + a·Δt
s_next = s + ½(v + v_next)·Δt  =  s + v·Δt + ½·a·Δt²
```

How far a stone drops over a flat shot of distance D at speed v:

```
t    = D / v
drop ≈ ½·g·t²
```

At 20 m a stone at 60 m/s drops about 0.54 m. At 30 m/s it drops about 2.2 m. That gap is what makes a full draw feel different from a quick one.

The growths aim ahead of a moving player:

```
t_flight  ≈ distance / v_projectile
aim_point = p_player + v_player · t_flight
```

**Tuning.** Launch speeds are picked so a full draw crosses the longest sightline in well under a second and a quick tap visibly drops. With no trajectory line, the arc has to be easy to read by eye.

### 2. Force-based character motion

**Sketch:** `Sketches/02-character-motion.png`

**In the game.** The player isn't moved by setting a speed. Input applies an acceleration and damping pulls the speed back down, so movement builds and fades the way it should. Jumps come from a launch velocity under gravity, with a few forgiving tricks on top.

```
a     = A · input − k · v          drive minus damping
v_max = A / k                      top speed
v(t)  = v_max · (1 − e^(−t/τ))     where τ = 1/k
```

With k = 9 per second the player reaches 90% of top speed in about a quarter of a second (t = 2.3τ). A sprint at 700 cm/s then needs A ≈ 6,300 cm/s², and a walk at 400 cm/s needs A ≈ 3,600 with the same k.

Jumping:

```
v₀     = √(2·g·h)                  h is the jump height
t_apex = v₀ / g
t_air  = 2·v₀ / g                  on flat ground
```

For a 120 cm jump with g = 980 cm/s², v₀ is about 485 cm/s and the jump lasts about a second. Sprinting at 700 cm/s, that covers about 7 m.

Four small tricks make it forgiving:

- **Variable height.** Releasing early cuts upward speed (v_y × 0.5). Height scales with v², so the remaining rise drops by about 75%.
- **Coyote time.** A jump still counts for about 0.1 s after leaving a ledge.
- **Jump buffer.** A press within 0.1 s before landing triggers a jump on landing.
- **Air control.** Sideways acceleration in the air is about 0.3 of what it is on the ground.

Vault and mantle are kinematic, meaning the character is moved along an eased path instead of being pushed by forces:

```
p(u) = p_start + (p_end − p_start) · (3u² − 2u³)       u = t / T,  T ≈ 0.5 s
```

Finding a ledge takes three checks: something solid ahead at a reachable height, a flat top, and enough room above it for the body.

**Tuning.** Everything gets adjusted by playing, but it starts from these numbers so each value has a reason.

### 3. Moving platforms

**Sketch:** `Sketches/03-oscillation.png`

**In the game.** A slab of broken scaffold slides back and forth between two points on a fixed period. The player times a jump onto it, rides it across a courtyard under fire from two growths, and steps off at the far side. Standing on it carries the player along.

The motion is simple harmonic:

```
x(t) = A · sin(ω·t + φ)            ω = 2π / T
v(t) = A·ω · cos(ω·t + φ)
a(t) = −A·ω² · sin(ω·t + φ) = −ω²·x(t)
```

A is half the travel distance, T is the period, and φ is a phase offset so several platforms can be staggered. The position is worked out from the clock every frame instead of being nudged forward, so it can't drift.

The platform is driven by the equation, not by forces, so the player's movement has to treat it as a moving base and inherit its speed:

```
v_player_world = v_player_input + v_platform
```

Getting this wrong is the classic bug where the player slides off a moving platform.

**Tuning.** The period is set so the ride takes a little longer than a growth's firing interval, which guarantees at least one shot unless the player breaks line of sight or destroys a growth first.

### 4. Friction and drag

**Sketch:** `Sketches/04-friction-drag.png`

**In the game.** Two places use it: the entangling roots, and the slide.

The roots raise the damping. In the motion model above, top speed is A/k, so doubling k halves it exactly:

```
v_max(roots) = A / (2k) = ½ · v_max
```

Easing k up over about half a second makes it feel like wading in rather than hitting a wall.

The slide uses Coulomb friction, which pulls the player down at a constant rate:

```
F = μ·m·g
a = −μ·g                           independent of mass
v(t)     = v₀ − μ·g·t
distance = v₀² / (2·μ·g)
```

A slide that starts at 800 cm/s with μ = 0.8 lasts about a second and covers about 4 m, then drops the player into a crouch walk.

**Tuning.** The roots need to hurt without feeling broken. Thirty seconds at half speed is long enough that the player tries to escape the area rather than push through, and that's the intent.

### Two-bone IK

**Sketch:** `Sketches/05-two-bone-ik.png`

**In the game.** The level is stairs, rubble and ledges, and a fixed walk animation leaves feet floating or sinking. Two-bone IK puts each foot on the real surface and tilts it to the slope. If time allows, it also places the hands on the edge during a mantle.

A trace straight down from each foot finds the real ground. The leg is two bones, a thigh L₁ and a shin L₂, and the knee bend comes from the law of cosines, where d is the distance from hip to the foot's target and the angle is the one inside the knee:

```
cos(knee) = (L₁² + L₂² − d²) / (2·L₁·L₂)
reach:      d ≤ L₁ + L₂
```

When a target is out of reach, the leg is as straight as it can get and the pelvis drops instead. The drop is the larger of the two feet's drops, so the body lowers to reach the lower foot instead of stretching one leg:

```
pelvis_drop = max(drop_left, drop_right)
```

A pole target controls the knee direction so it always bends forward.

**Why this technique.** A leg is two bones and one hinge, so there's a closed-form answer. It's cheaper and steadier than CCD or FABRIK, which solve by repeating steps, and the result shows up directly on the uneven ground the level is built from.

## Requirements checklist

| Requirement | Where it's met |
|---|---|
| 10 turrets, dormant until the player is in line of sight | The growths |
| Turrets fire a projectile every second | The growths, kinematic projectiles |
| A projectile costs the player 10% health | Poison |
| The player's weapon does 25% damage to a turret | Stones, four hits |
| A slowing hazard: half speed for 30 seconds | Entangling roots, friction and drag |
| A weapon jam hazard: 30 seconds | Spore clouds |
| A health pickup that restores full health | Field dressing |
| A damage pickup that doubles damage for 30 seconds | Hardened stones |
| Main menu: Start, Instructions, Quit | Interface |
| Pause menu: Resume, Restart, Main Menu | Interface |
| HUD: timer, health, turrets remaining, buff icons | Interface |
| A time limit of at least three minutes | Four minutes, tuned in playtesting |
| A moving platform that works | Moving platforms |
| A level from a blank map, designed around gameplay | The level |
| At least three physics mechanics | Projectiles, character motion, moving platforms, friction and drag |
| One of 2-bone IK, CCD or FABRIK | Two-bone IK |
| Principles and equations for each mechanic | The physics section |
| A sketch for each mechanic | Documentation/Sketches |
| Collecting and managing ammunition | Stone caches, recovered stones, the stone count |

## What this game doesn't try to do

It's one level, with no saves and no progression between levels. The first version of this project predicted trajectories and ricochets. Both are gone, so the effort goes into movement, aiming feel and a finished loop instead.
