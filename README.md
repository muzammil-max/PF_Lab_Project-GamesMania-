# 🎮 GamesMania

<p align="center">
  <strong>A multi-game arcade platform built in C using Raylib</strong><br>
  <sub>🐧 Linux Supported | 🪟 Windows Supported</sub>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Linux-green?style=for-the-badge&logo=linux" alt="Linux Supported">
  <img src="https://img.shields.io/badge/Platform-Windows-blue?style=for-the-badge&logo=windows" alt="Windows Supported">
</p>

---

## 🚀 About

**GamesMania** is a multi-game arcade platform built completely in **C** using **Raylib**.  
It combines four classic games into one application with a shared menu, progress unlocking, scoring, and save/load support.

---

## 🎮 Games Included

- 🐤 **Flappy Bird**
- 🧩 **Tetris**
- 🏓 **Ping Pong**
- 🧱 **Brick Breaker**

---

## ✨ Features

- Multiple games in one project
- State-based game switching
- Progress-based unlock system
- Save and load support
- Background music and sound effects
- Keyboard-only controls
- Smooth rendering using Raylib
- Centered window support on launch

---

## 🧠 Core Concept

GamesMania uses a simple **state-driven engine**:

- `state` controls the screen being shown
- `gamestate` controls the active game
- score/progress unlocks new games

This makes the project modular and easy to expand.

---

## 🛠️ Tech Stack

- **C language**
- **Raylib**
- **Linux GCC**
- **MinGW (for Windows .exe builds)**

---

## 📂 Project Structure

```text
GamesMania/
├── main.c
├── engine.h
├── flappy.c
├── tetris.c
├── pingpong.c
├── brickbreaker.c
├── assets/
│   ├── audio/
│   ├── images/
│   └── ...
└── save.txt
