# Darkness of Kabul 2.0: Technical Design

This is a map of what I'm going to build and why. There's no code in it on purpose. Each entry says what the thing is, why the game needs it, what it has to be able to do, and what to learn before building it. If I can explain the "why" out loud, I can build it. If I can't, I'm not ready to write it yet.

Every entry follows the same four labels:

- **Why.** The problem it solves. Without this, the entry shouldn't exist.
- **Needs.** The basic things it must hold and be able to do.
- **Learn first.** The Unreal ideas worth understanding before typing anything.
- **Works when.** A quick test that proves it, so I'm not guessing.

## Three rules

**One class, one job.** In the first version the player class grew past 1,100 lines because it handled movement, aiming, firing, health, settings, UI and resetting the world. Here the character reads input and owns its parts, and nothing else. Everything else lives in a component small enough to explain in a sentence.

**Share a concept once.** Health was written twice in the first version, once for the player and once for the zombie, with the same arithmetic in both. Two copies drift apart. Here there's one health component, and anything that can be hurt owns one.

**Layout in UMG, logic in C++.** The first version built every screen in C++, which was long and isn't how Unreal UI is normally made. Here the screens are UMG assets and C++ only supplies behaviour.

## Build order

| Step | Build | What it teaches |
|---|---|---|
| 1 | A blank C++ project that compiles | The module, the project file, the toolchain |
| 2 | Character, camera, Enhanced Input | Input mapping, possession, first-person setup |
| 3 | Walk, sprint and jump tuned on a test map | How the engine's movement numbers map onto the acceleration and damping in the GDD |
| 4 | Slide, vault and mantle | Traces, custom movement modes, montages |
| 5 | Health component and the damage interface | Components, interfaces, delegates |
| 6 | Slingshot, stone, ammo | The aim point, projectiles, state machines |
| 7 | One growth that works | Timers, line of sight, vision cones |
| 8 | Game mode and HUD | Match rules, UMG binding |
| 9 | Zombies | Navigation, AI controllers |
| 10 | Ten growths, hazards, pickups, status effects | Templates, reuse |
| 11 | Moving platform and IK | Base movement, animation graphs |
| 12 | Animation, audio, menus, art, packaged build | Polish and shipping |

Movement comes before combat on purpose. If it isn't fun to move, nothing built on top will rescue it. The first milestone, a blocked-out level with placeholders, a Blueprint moving platform and a non-functional HUD, needs almost no gameplay C++, so it happens alongside steps 1 to 3.

## Four techniques used everywhere

Four ideas show up in nearly every system. Learning each one once pays off many times.

**Line traces and sweeps.** Used for sight (growths and zombies), the aim point (slingshot), ledge detection (vault and mantle), and the ground under each foot (IK). A trace asks the physics world one question: what's the first thing along this line? Learn the difference between single and multi traces, channel traces and object traces, how to ignore actors, and why a sweep with a shape finds things a thin line slips past.

**Collision channels and presets.** Without them, every trace and overlap sees everything. Plan a small set: a projectile channel, a climbable trace channel for ledges, and presets for the player, zombies, projectiles, pickups and hazards. Decide on paper who blocks, overlaps or ignores whom before touching the editor.

**Timers instead of Tick.** Tick runs every frame on every actor. Perception and AI don't need that. Ten growths and a handful of zombies sensing five times a second is invisible to the player and far cheaper. Tick is for things that really change every frame, like the platform and the draw. Learn the timer handle, looping timers, and starting each one at a random delay so everything doesn't think on the same frame.

**Delegates.** A delegate is an event other code can subscribe to. The health component announces "I changed" without knowing a HUD exists, and the HUD listens. That's what lets UI, sound and gameplay stay separate. Learn dynamic multicast delegates, how to expose them to Blueprint, how to broadcast, how to bind (the bound function has to be a UFUNCTION), and why to unbind when a widget is destroyed.

## Enums

An enum says a thing is in exactly one state. A handful of booleans can describe impossible situations, like a growth that's alerted and destroyed and firing all at once. Each enum below replaces a cluster of booleans, and each is exposed to Blueprint so animation graphs can switch on it.

| Enum | Values | Why it exists |
|---|---|---|
| Growth state | Dormant, Alerted, Firing, Destroyed | A growth is always in one of these. The AI and the animation graph both switch on it, and changing state is the one place enter and exit logic runs |
| Zombie state | Wandering, Chasing, Attacking, Dead | Same reasoning. The zombie animation graph needs to know whether to idle, run or swing |
| Slingshot state | Lowered, Raised, Drawing, FullDraw, Reloading, Slipped | The sling is a small state machine, like a bow. Writing the states down prevents things like drawing while reloading |
| Status effect | None, Slowed, WeaponJam, DamageBoost | Hazards and pickups each carry one value. The status component, the HUD icons and the movement component all key off it. Because a hazard simply carries an effect, there's no separate hazard enum |
| Pickup type | Health, DamageBoost, Ammo | One pickup class with a type is simpler than three classes while the only difference is a switch. If pickups ever behave very differently, split them then |
| Match phase | Playing, Victory, Defeat | Tells the game mode and HUD which screen to show. Pause isn't a phase because the engine already has one |
| Movement mode (custom) | Slide, Vault, Mantle | The movement component supports custom modes, each with its own physics step. That keeps parkour inside the engine's movement update instead of fighting it |

**Learn first.** UENUM with a uint8 backing type, UMETA for display names, BlueprintType so Blueprints can use it, switching on an enum, and using an enum as a map key.

## Interfaces

### Damageable

A promise that says "I can be hurt".

**Why.** A stone or a poison glob hits something, and it shouldn't care whether that something is the player (a character), a zombie (a character) or a growth (a plain actor). Without an interface the projectile needs a chain of casts that gets longer with every new enemy. With one, the projectile asks "can this be damaged?" and calls a single function. It has to be an interface rather than a shared base class because a character and a plain actor can't share a parent without one of them being forced into the wrong hierarchy.

**Needs.**
- A way to receive damage, taking the amount, the hit information and who caused it, and returning how much was actually taken
- A way to ask whether the thing is still alive

**Learn first.** Unreal's two-class interface pattern (a U class and an I class), BlueprintNativeEvent and why the implementing function gets an _Implementation suffix, and how to check that an actor implements an interface before calling it. Calling it on something that doesn't can crash or silently do nothing.

**Works when.** One stone hits a zombie, a growth and the player, and the same function runs each time.

### Interactable (optional)

**Why.** It's only worth having when more than one kind of thing is interactable. With pickups alone, a base class is simpler. If a second interactable appears, like a door or a switch for the platform, it earns its place. Building it just to practise is fine as long as I know that's the real reason.

## Templates

A template lets one piece of logic work for many types. My rule: only write one when I can name two places that need it.

### Timed modifier

A value that applies for a set time and then reverts by itself.

**Why.** Three effects follow the same recipe: a movement multiplier, a damage multiplier and a jam flag. Writing the timer logic three times invites three slightly different bugs. A template writes it once and works for floats and booleans alike.

**Needs.**
- Apply a value for a duration
- Count down each frame
- Report whether it's active and how much time is left
- Hand back the modified value while active and the base value otherwise

**Learn first.** Templates live entirely in headers, because the compiler builds each version from the full definition. They can't be marked as Unreal properties, so their data isn't saved or shown in the editor. Decide the stacking rule on paper: does a second application refresh the timer, extend it, or get ignored?

**Works when.** Applying a slow shows a countdown on the HUD, speed returns on its own, and stacking behaves the way I decided.

### Actor pool (later, optional)

A fixed set of pre-made actors that get reused instead of spawned and destroyed.

**Why.** Growths fire poison constantly. That's a few hundred projectiles in a typical match and a couple of thousand in theory. Unreal can spawn that many, but spawning has a cost and it shows up as hitches. Pooling is the standard answer. Measure first and build it only if profiling shows a problem.

**Needs.**
- Setup with a class and a capacity
- Hand out an inactive actor, or nothing if the pool is empty
- Take one back by hiding it, disabling collision and stopping its tick
- Reset its state when it's reused

**Learn first.** Garbage collection. A plain C++ class holding actor pointers is invisible to Unreal's garbage collector, so the pooled actors can be deleted while the pool still thinks they exist. The owning UObject has to keep them in a UPROPERTY array. That one fact is the best lesson in this entry.

**Works when.** Firing a long burst keeps the actor count in the level flat.

## Components

Components are how the project avoids a god class. Each owns one concern, can be tested alone, and can be dropped onto any actor that needs it.

### Health component

One health pool. The player, the zombies and the growths all own one.

**Why.** It removes the duplicated health code from the first version, and it's the clearest example of "has a" beating "is a". The owner decides what running out means: a game over for the player, a ragdoll for a zombie, a collapse for a growth. The component only keeps the numbers.

**Needs.**
- A maximum and a current value
- Apply damage, clamped at zero, returning what was actually removed
- Restore to full
- Report a percentage for the HUD and whether it's depleted
- Announce a change every time, and announce depletion exactly once

**Learn first.** Creating a component in the constructor, the component's BeginPlay, editable properties with minimum values, and delegates.

**Works when.** Hitting it again and again past zero fires the depletion event only once.

### Ammo component

The stone supply.

**Why.** Ammo is the central pressure in the game, and keeping its rules in one place lets pickups, the slingshot and the HUD all talk to it without knowing about each other.

**Needs.**
- A starting count of 12 and a maximum of 24
- Spend a stone, reporting failure when empty so the slingshot can play a dry click
- Add stones, reporting how many were actually accepted
- Announce a change for the HUD
- Reset on restart

**Learn first.** Clamping integers, and deciding the design question up front: if the player is full, does a stone cache stay in the world? (My answer is yes, it stays.)

**Works when.** Picking up a cache at full ammo takes nothing and leaves the cache where it is.

### Slingshot component

Everything about the sling: raising, drawing, holding, releasing, reloading and slipping.

**Why.** It was the largest chunk of the first version's player class. As a component it can be read, tuned and explained on its own, and the character stays thin.

**Needs.**
- The slingshot state and the legal moves between states
- Draw progress from 0 to 1, and how long the draw has been held
- A steadiness value that rises as the draw is held and falls when standing still or crouched, turned into a small sway on the aim
- Tunable times for minimum draw, full draw, the slip point (around 3.5 seconds) and reload
- The stone class to spawn, and a socket on the arm mesh where the pouch sits
- The aim calculation, described below
- Announcements for presentation: draw started, released with strength, dry fire, slipped, reloaded
- Checks with other components: spend a stone from the ammo component, ask the status component whether the sling is jammed

**The aim calculation matters most.** The stone leaves the pouch, which sits off to the side of the camera, but the player aims with the crosshair, which sits at the centre. Fire along the camera's forward direction from the pouch and shots land beside the crosshair. The fix is to trace from the camera to find what the crosshair is pointing at, then aim from the pouch to that point. This removes the parallax at the aim point, and shots still drop at distance, which is intended.

**Learn first.** Ticking a component, line traces, skeletal mesh sockets, interpolation for a smooth draw and sway, and exposing hooks to Blueprint so sound, animation and camera kick stay out of the C++.

**Works when.** Aiming at a wall from 5 m, 20 m and 50 m, the near shot lands on the crosshair and the others drop by roughly what the GDD's maths predicts.

### Status effect component

Timed buffs and debuffs.

**Why.** Slow, jam and damage boost all work the same way, so one component owns the timed modifiers. It's also the only thing the HUD needs to ask about to draw its icons.

**Needs.**
- Apply or clear an effect by its enum value
- Report whether an effect is active and its remaining time
- A drag multiplier for movement (2.0 while slowed, because doubling the damping halves top speed) and a damage multiplier
- A list of active effects for the HUD
- Announce changes so icons appear and disappear without polling
- Clear everything on restart

**Learn first.** Using the timed modifier template as a member, and how delegates keep the HUD from checking every frame.

**Works when.** Walking into roots shows the slow icon with a countdown, and it disappears by itself.

### Movement component

A subclass of the engine's character movement component.

**Why.** Parkour needs physics beyond the defaults. Subclassing lets slide, vault and mantle run as custom movement modes inside the engine's own movement update, so collision, floor detection and moving platforms all keep working. The tempting alternative is moving the actor from the character's Tick, which fights the movement component and causes jitter.

**Needs.**
- Acceleration and damping values from the GDD's derivation
- A jump launch velocity worked out from the height I want
- The forgiveness timers: coyote time and the jump buffer
- Slide start speed and friction
- Vault and mantle reach limits, and the traces that find ledges
- Rules for when each move can start and when it ends
- Reading the status component's drag multiplier

**Learn first.** How the engine's walking and falling already use max acceleration, ground friction and braking, because my maths has to map onto those numbers. Then custom movement modes and their physics function, and why velocity is only safe to change inside the movement update.

**Fallback.** If custom modes prove too hard, vault and mantle can be root motion montages, where the animation itself moves the character. It's less flexible and much easier, and it can still look good.

**Works when.** Printing speed against time for a sprint start gives a curve close to the one in the GDD.

### Camera feel component (optional, recommended)

**Why.** Feel lives in the camera: the view widening on sprint and narrowing on aim, head bob, the landing dip, the slide lowering, the release kick and the tired-arm sway. Putting all of that in the character is exactly how it bloated before. This component listens for events and nudges the camera.

**Needs.**
- A target field of view per state, smoothly interpolated
- Small offsets for bob, landing dip and slide
- A kick on release and a sway input from the slingshot
- Every value tunable, plus one overall strength setting so it can be dialled down

**Learn first.** Interpolation, the idea of a spring and damper for natural settling, camera shake assets compared with manual offsets, and setting the field of view through the camera component.

**Works when.** Switching it off makes you feel something is missing immediately.

## Actors

### Character

**Why.** It's thin on purpose. It reads input, forwards it to components, owns them, and implements the damage interface. A rough heuristic: past about 300 lines, something belongs in a component.

**Needs.**
- A first-person camera and an arms mesh that only the owner can see
- The components above
- Input bindings for move, look, jump, sprint, crouch, aim and draw

**Learn first.** Enhanced Input (mapping contexts, actions and bindings), possession, first-person mesh visibility, and the capsule versus the mesh.

**Works when.** It moves and looks around, and removing any single component removes only that feature.

### Projectiles

A base class with two children, one for stones and one for poison.

**Why.** Movement, collision, lifetime and damage are shared. The children differ in what they ignore, how much they hurt, how they look and what happens on landing: a stone that misses becomes a pickup, and poison splashes. A flag on one class would grow into an ever-longer chain of ifs, so they're separate children.

**Needs.**
- A sphere collision and a projectile movement component with gravity and no bounce, matching the GDD's maths
- Launch with a direction and a speed
- A damage amount and a lifetime
- Ignoring whoever fired it
- Calling the damage interface on a hit, then deactivating

**Learn first.** The projectile movement component, collision presets, hit events versus overlap events, and tunnelling. At 60 m/s a stone moves a full metre every frame at 60 fps and is only about 10 cm across, so without swept collision it passes straight through thin walls.

**Works when.** Firing 100 stones at a thin wall at full speed sends none through.

### Growth

The stationary turret.

**Why.** A growth doesn't move, so a plain actor is lighter than a character. It's the only place sight, firing and state changes come together for the objective.

**Needs.**
- A mesh and a health component
- The growth state, with a single setter that runs enter and exit logic and tells Blueprint
- A vision range and a half-angle for the cone
- A fire interval of one second, and a projectile class
- A perception pass on a timer, not Tick, that checks range, the cone and a clear line of sight
- Aiming ahead of a moving target
- A weak reference to its current target
- On destruction, telling the game mode and stopping its timers

**Learn first.** Timers, the dot product for a cone check, line traces that ignore itself, and why the target is a weak pointer.

**Works when.** Standing behind cover it never fires. Stepping out, it fires within about a second.

### Zombie

**Why.** It's a character so it gets movement, a capsule, a skeletal mesh and the animation setup for free. It's pressure on the player rather than the objective, so the AI stays simple.

**Needs.**
- An AI controller and a navigation mesh
- The zombie state
- A perception pass on a timer
- Chase on sight, then a melee swing when in range with a pause between swings
- A speed below the player's sprint and above the walk
- A health component, and a death using the zombie animations from the first version
- No ability to climb: the navigation mesh deliberately doesn't connect the rooftops

**Learn first.** The AI controller, navigation mesh bounds volumes, simple move-to calls, and the auto-possess setting. Unreal has an AI perception system, but a hand-rolled check on a timer is enough here and easier to understand.

**Works when.** A zombie loses you the moment you vault a wall or climb onto a roof.

**A common trap.** With no navigation mesh volume in the level, zombies never move at all. It's a level step, not a code step, and it's easy to forget.

### Pickup

**Why.** One class with a type does the job while pickups differ only by what they grant. Blueprint children supply the meshes and effects.

**Needs.**
- An overlap trigger
- The pickup type, a stone count and a duration
- On overlap by the player, grant the effect and remove itself, unless it can't be used (a cache at full ammo stays)
- A hook for the collection sound and effect

**Learn first.** Overlap events, the difference between block and overlap in a collision preset, and spawning an actor from code at an impact point (for recovered stones).

**Works when.** Collecting each type produces the right effect, and a missed stone appears where it landed.

### Hazard volume

**Why.** Roots and spore clouds are the same idea with a different effect, so one volume class that carries an effect covers both.

**Needs.**
- A trigger box
- An effect value and a duration
- Apply the effect once per entry, not every frame

**Learn first.** Begin-overlap fires once on entry, which is why it's the right event, and finding a component on another actor and checking it exists before using it.

**Works when.** Walking in applies the effect once, and leaving and re-entering applies it again.

### Oscillating platform

**Why.** It's the one moving thing the player stands on, and the physics reason is in the GDD: it has to carry the player.

**Needs.**
- Travel distance, period and phase offset, all editable
- A position worked out from elapsed time each frame, not nudged forward, so it can't drift
- A movable mobility setting

**Learn first.** Tick, sine, mobility, and how the movement component follows a moving base. The first milestone asks for a Blueprint version, so building that first and porting it to C++ later is a good way to see the difference.

**Works when.** The player rides it without sliding off, and two platforms with different phases stay out of step.

### Game mode

**Why.** One per level, and it's the natural owner of the match rules: the timer, the objective and the win or lose decision.

**Needs.**
- A match length of four minutes
- The set of all growths, found or registered at the start, and a set of destroyed ones
- Notifications for growth destroyed and player died
- The match phase
- Getters for time left and growths remaining, so the HUD can read them

**Learn first.** The difference between game mode and game state (state is where shared information conventionally lives, though in single player game mode is fine), and why counting by sets of weak pointers beats decrementing a number: a number can double-count, a set can't.

**Restarting.** Reloading the level is one line and can't miss anything. The first version restored the world by hand, which took hundreds of lines and kept missing things.

**Works when.** Destroying all ten ends the match once, as a win.

## Animation

### Animation instance

**Why.** It keeps presentation separate from gameplay. The character exposes state, the animation instance copies it into plain variables each update, and the graph only reads those. Gameplay logic never lives in the animation Blueprint.

**Needs.**
- Variables for speed, direction, falling, crouching, sliding, aiming and draw progress
- An update that reads the character and its movement component, with null checks because the pawn can be missing in the first frames

**Learn first.** The native initialise and update functions, state machines and transitions, blend spaces for locomotion (speed against direction), montages for one-shot actions (vault, mantle, release, reload), animation notifies for footsteps and the exact release moment, and layered blending per bone so the arms can aim while the legs run.

**A common trap.** Transitions that flicker. Fix them with a short minimum time in a state.

**Works when.** Printing the variables on screen matches what the character is doing.

### Foot and hand IK

**Why.** The maths is in the GDD. The practical reason is that stairs, rubble and ledges break a fixed animation.

**Needs.**
- A trace down from each foot, smoothed so it doesn't pop
- A pelvis drop equal to the larger of the two feet's drops
- A pole target so knees bend forward
- IK switched off while airborne, sliding or vaulting

**Learn first.** The two-bone IK node in the animation graph (or the Control Rig equivalent), and where the trace should run.

**Works when.** Walking up uneven stairs keeps both feet planted without the body bobbing.

### Zombie and growth animation

**Why.** Zombies use the same separation as the player, with a graph that switches on the zombie state and reuses the first version's pack. Growths are simple enough for a state-driven Blueprint with a timeline.

**Works when.** A zombie's animation always matches its state.

## UI

**Why.** Every screen is a UMG asset, so layout lives in the editor. C++ declares what it needs and supplies behaviour. This is the direct fix for the first version's hand-built interface.

**Needs.**
- Screens for the main menu, pause, instructions, the HUD, victory and defeat
- A C++ base for the HUD that declares required widgets by name, for the timer, health bar, growths remaining, stone count and status icons
- The reticle as part of the HUD, scaled by draw progress and steadiness
- Listening to the health, ammo and status delegates instead of checking every frame
- Menus that switch input to UI and back, and pause the game

**Learn first.** The bind-widget property and why names must match exactly (a mismatch is a compile error, which is a feature), creating a widget and adding it to the viewport, input modes, setting the game paused, and unbinding delegates when the widget is destroyed.

**Where it lives.** The main menu is its own level with its own game mode, and Start Game opens the play level. That's simpler than layering a menu over gameplay.

**Works when.** Taking damage updates the health bar the instant it happens, with no per-frame polling.

## Pointers and references

| Situation | Use | Why |
|---|---|---|
| A member this object owns and the editor should see | A TObjectPtr inside a UPROPERTY | Garbage collection sees it and keeps the object alive |
| A reference to something that might be destroyed (a target, a tracked growth) | A weak object pointer | It becomes null safely instead of dangling |
| A class picked in the editor | A TSubclassOf | Type-safe and editable in Blueprint |
| A parameter the function only reads | A const reference | No copy, and it can't be null |
| A parameter the function writes into | A non-const reference | Tells the reader it's an output |
| A short-lived local | A raw pointer | Fine inside one function, never stored across frames |
| A component on another actor | Find it, then check for null | The other actor might not have one |

The rule: if a pointer outlives the current function, it's either a TObjectPtr because this object owns it, or a weak pointer because it doesn't. A raw pointer stored as a member is a crash waiting for the actor to be destroyed.

## Naming and folders

- **Prefixes.** A for actors, U for components and other objects, E for enums, I for interfaces, F for structs, T for templates.
- **Assets.** BP_ for Blueprints, WBP_ for widgets, ABP_ for animation Blueprints, SM_ and SK_ for static and skeletal meshes, M_ and MI_ for materials, T_ for textures, L_ for levels, IA_ and IMC_ for input.
- **World Outliner.** Folders for Geometry, Turrets, Pickups and Hazards, with actors named so the level reads at a glance, such as TurretPlaceholder3.

## Asset credits

Keep a credits file from the first imported asset: what it is, who made it, where it came from, the licence and the date. The first version's records were incomplete and that blocked publishing. Writing one line at download time is far easier than rebuilding the trail months later.

## Risks and fallbacks

| Risk | Fallback |
|---|---|
| Custom movement modes are too hard | Root motion montages for vault and mantle, with a tuned movement component for everything else |
| IK takes too long | Feet only on stairs, skip hand IK |
| Zombie navigation misbehaves | Fewer zombies with a simple patrol and chase. They're pressure, not the objective |
| The draw doesn't feel right | Cut the sway first, then hold fatigue, and keep the draw and release core |
| Running out of time | Cut in this order: camera feel polish, the pool, the interactable interface, hand IK. Never cut movement or the slingshot |
