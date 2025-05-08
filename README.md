# Gravity Manipulation Third Person Game
![](https://github.com/Kotuon/jubilant_potato/blob/main/gravProject97.gif)

<!-- ## Table of Contents

* [Introduction](#introduction)
* [Technology Used](#technology-used)
* [Features](#features) -->

## Overview
A third-person action game prototype developed in Unreal Engine 5.5, featuring a fully dynamic gravity manipulation system. Built as a sandbox for experimenting with advanced Unreal Engine features, this project integrates a [custom-built ability framework](https://www.kelsonwysocki.com/winter-project2022) with quaternion-based player and camera rotation logic to maintain intuitive control and visual fidelity under any gravity orientation.

Separate experimental branches explore integration with UE5’s Gameplay Ability System (GAS) and motion-matching animation techniques.

## Technologies

Unreal Engine 5.5
* Visual Studio Code
* Niagara VFX
* Animation Blueprints
* Gameplay Ability System (GAS) (experimental branch)
* Motion Matching (experimental branch)

## Features

* __Fully Directional Gravity Manipulation:__ A physics-consistent system allowing gravity to shift in any direction, fully integrating into movement, animation, and combat.
* __Custom Ability Framework:__ A standalone system for defining and managing player abilities, designed to be modular and data-driven.
* __Data-Driven Combat System:__ Attacks and player actions are defined using data assets for rapid iteration and scalability.
* __Custom Camera and Targeting Systems:__ Designed to support unconventional orientations and gravity changes without disorienting the player.
* __Experimental GAS and Motion Matching Branches:__ Implementations exploring hybrid animation and ability systems using UE5’s GAS and motion matching features.
* __Advanced Animation Handling:__ Includes animation blueprints, notify states, and gameplay-linked animation events.
* __Dynamic Niagara Effects:__ Particle systems that respond to gameplay context and player actions.
