# Darkness of Kabul 2.0

A first-person parkour and slingshot game built in Unreal Engine 5 with C++.

The plant life of Kabul has been corrupted. Growths fused with the ruins spit poison at anything living in their sight, and the people caught in the spores have become zombies. The player has a slingshot, a small pouch of stones and four minutes to clear the quarter, mostly by moving across its rooftops.

## Status

In development.

## Focus

- Movement and parkour that feel good on their own: sprint, jump, slide, vault, mantle
- A slingshot modelled on the bow, with a real draw, a held breath and a tiring arm
- Stones as a scarce resource that can be recovered
- Line-of-sight enemies and zombies that can't climb, so height is safety
- Physics systems: kinematic projectiles, force-based motion, moving platforms, friction and drag
- Two-bone inverse kinematics for foot placement
- A complete loop: menu, play, win and lose, restart, quit

## Documentation

- [Game Design Document](Documentation/GDD.md), also available as a Word file
- [Technical Design](Documentation/TECHNICAL.md)

## Building

Requires Unreal Engine 5 and a C++ toolchain. Binary assets are tracked with Git LFS, so install it before cloning, otherwise the assets arrive as small pointer files.

```
git lfs install
git clone https://github.com/Faiazalikhail/DOK2.0.git
```
