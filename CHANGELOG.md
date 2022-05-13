# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

* Added CHANGELOG;
* Added README;
* Added game screenshots;
* Added `ball_entity.{c,h}` files with ball functions and variables;
* Added `player_entity.{c,h}` files with players functions and variables;
* Added `game.{c,h}` files with game handlers, functions and variables;
* Added `ui.{c,h}` files with funtions to handle ui elements;
* Added `config.h` file with config macros;
* Added `types.h` file types definitions;

### Fixed

### Changed

* Moved all functions from `main.c` to its independent files;

### Removed

* Removed `render_score` function;

## [0.1.0] - 2022-05-11

### Added

* Added render messages
* Added timer between rounds
* Added render score
* Added round feature
* Added score and play/pause features
* Added casting to floating variables
* Added players movement
* Added keyboard handlers
* Added players
* Added game events

### Fixed

### Changed

* Changed players and ball speed
* Changed colisions and events handlers
* Make ball bounce
* Make ball entity move

### Removed 

