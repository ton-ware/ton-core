# Contributing

To maintain a clean, maintainable, and vendor-agnostic git history across all repositories, we follow the **Conventional Commits** specification. This document serves as a human-friendly guide and cheatsheet.

## Table of Contents

- [1. Commit Message Format](#1-commit-message-format)
- [2. Commit Types Cheatsheet](#2-commit-types-cheatsheet)
- [3. Scopes Reference by Repository](#3-scopes-reference-by-repository)
- [4. Good vs. Bad Examples](#4-good-vs-bad-examples)
- [5. Changelog Guidelines (`CHANGELOG.md`)](#5-changelog-guidelines-changelogmd)
- [6. Branching Strategy & Workflow](#6-branching-strategy--workflow)
  - [Branch Roles](#branch-roles)
  - [Step-by-Step Task Lifecycle](#step-by-step-task-lifecycle)
- [7. Versioning & Dependency Bumping in CMake](#7-versioning--dependency-bumping-in-cmake)
  - [1. Project Version Bumping](#1-project-version-bumping)
  - [2. Third-Party Dependency Bumping (FetchContent)](#2-third-party-dependency-bumping-fetchcontent)
  - [3. Release Lifecycle (Dev to Master)](#3-release-lifecycle-dev-to-master)
- [8. Project Structure](#8-project-structure)

## 1. Commit Message Format

Every commit message must follow this structure:

```text
<type>(<scope>): <subject>

[optional body]

[optional footer]
```

### Core Rules:

1. **Imperative Mood**: Use imperative present tense in the subject line (*"add"*, not *"added"* or *"adds"*). Think: *"If applied, this commit will `<subject>`"*.
2. **All Lowercase**: `<type>`, `(<scope>)`, and the initial letter of `<subject>` are lowercase.
3. **No Trailing Period**: Do not end the subject line with a period (`.`).
4. **Line Length**: Keep the subject line under `72 characters` (ideally `<=50 characters`).
5. **Vendor-Agnostic**: **Never** include host-specific issue links (e.g. `#14`, `gh-14`, or `https://github.com/...`) in commit subjects or messages. Git history must remain portable across any git host (GitHub, GitLab, Gitea, or self-hosted).

## 2. Commit Types Cheatsheet


| Type | Purpose | When to Use | Example |
| :--- | :--- | :--- | :--- |
| **`feat`** | New feature or capability | Adding a new struct, math primitive, algorithm, or rasterization step. | `feat(math): add templated Point2 struct` |
| **`fix`** | Bug fix | Correcting a calculation, edge-case clipping, memory leak, or crash. | `fix(rasterizer): clamp bounding box to framebuffer dimensions` |
| **`test`** | Unit tests & benchmarks | Adding Catch2 test cases, property tests, or benchmark suites. | `test(color): add Catch2 unit tests for RGBA8888 conversions` |
| **`build`** | Build system & toolchain | Editing CMakeLists, presets, compiler flags, or dependencies. | `build(cmake): configure dev and dist presets with clang` |
| **`perf`** | Performance optimization | SIMD vectorization, cache tiling, branch elimination ($O(N)$ improvements). | `perf(rasterizer): vectorize horizontal span interpolation` |
| **`refactor`** | Structural code changes | Reorganizing code without changing public behavior or fixing bugs. | `refactor(math): replace manual indexing with structured bindings` |
| **`docs`** | Documentation | Adding markdown guides, header architecture comments, or diagrams. | `docs(color): explain RGB565 photopic sensitivity and endianness` |
| **`chore`** | Maintenance & housekeeping | Updating `.gitignore`, `.clang-format`, or directory scaffolding. | `chore(git): ignore build and compilation database artifacts` |


## 3. Scopes Reference by Repository

Scopes indicate the subsystem or module affected by the commit.

### Examples

* `cmake` — CMake build logic, flags, or configuration.
* `deps` — Third-party library updates or `FetchContent` configuration.
* `git` — Repository settings, `.gitignore`, hooks.
* `docs` — General documentation, READMEs, or architectural guides.
* `core` — Foundational primitive types (`I32`, `F32`, etc.) and engine constants.
* `math` — Linear algebra primitives (`Point2`, vectors, matrices, quaternions).
* `simd` — Hardware-accelerated SIMD wrappers (AVX2, NEON, Xtensa PIE).
* `memory` — Custom allocators, arenas, and memory pools.

## 4. Good vs. Bad Examples


| Status | Commit Subject | Why |
| :---: | :--- | :--- |
| ❌ Bad | `[Math] Add templated Point2 struct (#2)` | Tagged subsystem pushes verb; vendor-locked issue link (`#2`). |
| ❌ Bad | `[Add] RGB565 color representation` | Bracketed verb adds punctuation noise without semantic value. |
| ❌ Bad | `feat(math): added Point2 and fixed bug` | Past tense (*"added"*); combines two separate changes into one. |
| ❌ Bad | `fix: fixed the screen rendering bug.` | Missing scope; past tense; unnecessary trailing period. |
| ✅ Good | `feat(math): add templated Point2 struct` | Correct type, precise scope, present imperative verb. |
| ✅ Good | `feat(color): add RGB565 and RGBA8888 color representations` | Clean, readable, vendor-agnostic. |
| ✅ Good | `test(rasterizer): add Catch2 cases for negative coordinate clipping` | Focused scope, descriptive target. |
| ✅ Good | `build(cmake): link Catch2 statically in test preset` | Clear build maintenance. |


## 5. Changelog Guidelines (`CHANGELOG.md`)

Each repository maintains a [`CHANGELOG.md`](CHANGELOG.md) adhering to [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

Entries are recorded under `## [Unreleased]` and grouped by action:

* `### Added` — New user-facing features or public APIs.
* `### Changed` — Changes in existing functionality.
* `### Fixed` — Bug fixes.
* `### Removed` — Deprecated or removed capabilities.

### Entry Format:

Each bullet entry begins with the scope in bold, followed by a concise description (no vendor issue URLs):

```markdown
### Added

- **color**: RGB565 and RGBA8888 color representations with photopic bit conversions
- **math**: templated Point2 struct with equality, ordering, and indexing operators

### Fixed

- **rasterizer**: out-of-bounds pixel write during negative coordinate clipping
```

## 6. Branching Strategy & Workflow

We employ a 2-tier trunk branching model: `master` (production/releases) and `dev` (integration/latest).

```text
  master (protected) ----------------------* [v0.1.0 Release] ------------------->
                      \                   /
  dev (integration)    *---*---*---*-----*--------------------*---*------------->
                            \     /                            \     /
  task/* (features)          *---* (task/bresenham)             *---* (task/zbuffer)
```

### Branch Roles

1. **`master` — Protected Release Branch**:
   - Represents stable, release-ready code.
   - **Direct pushes are forbidden.**
   - Every merge from `dev` into `master` represents a release and is accompanied by an annotated Git tag (e.g. `v0.1.0`).
2. **`dev` — Active Integration Branch**:
   - The living development trunk containing the latest tested engine code.
   - Target for all feature/task PRs and merges.
   - Must remain buildable at all times across all 3 presets (`debug`, `dev`, `dist`) with 100% passing tests.
3. **`task/*` or `feat/*` — Feature & Task Branches**:
   - Short-lived branches created off `dev`.
   - Name format: `task/<short-description>` or `feat/<short-description>`.
   - Merged cleanly back into `dev` via squash-merge once complete.

### Step-by-Step Task Lifecycle

1. **Update and branch off `dev`**:
   ```bash
   git switch dev
   git pull origin dev
   git switch -c task/bresenham-lines
   ```
2. **Commit changes locally** using Conventional Commits:
   ```bash
   git commit -m "feat(rasterizer): add Bresenham integer line algorithm"
   git commit -m "test(rasterizer): add test cases for diagonal and steep slopes"
   ```
3. **Squash-merge into `dev`**:
   ```bash
   git switch dev
   git pull origin dev
   git merge --squash task/bresenham-lines
   git commit -m "feat(rasterizer): add Bresenham line algorithm and unit tests"
   git push origin dev
   git branch -D task/bresenham-lines
   ```

## 7. Versioning & Dependency Bumping in CMake

We adhere strictly to [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html) (`{MAJOR}.{MINOR}.{PATCH}`).

### 1. Project Version Bumping
The project version is declared in the root `CMakeLists.txt`:

```cmake
project("TON Core"
  VERSION 0.1.0
  LANGUAGES C CXX
)
```

CMake automatically derives the variables:

* `PROJECT_VERSION` (e.g., `0.1.0`)
* `PROJECT_VERSION_MAJOR` (e.g., `0`)
* `PROJECT_VERSION_MINOR` (e.g., `1`)
* `PROJECT_VERSION_PATCH` (e.g., `0`)

#### Version Increment Rules:

* **`MAJOR`** (`X.0.0`): Incompatible API breaks, architectural redesigns, non-backward-compatible type layouts.
* **`MINOR`** (`0.X.0` / `1.X.0`): Backward-compatible new features (e.g., adding `Vec3` or new color blend functions). *Note: in pre-`1.0` (`0.y.z`), breaking experimental changes may occur on minor increments.*
* **`PATCH`** (`0.1.X`): Backward-compatible bug fixes, performance optimizations, or internal refactors.

#### Commit format for project version bumps:

```text
build(cmake): bump project version to 0.1.0
```

### 2. Third-Party Dependency Bumping (FetchContent)

All external dependencies (e.g., `Catch2`, `SDL3`) fetched via CMake's `FetchContent` module must follow these rules:

1. **Strictly Pin Immutable Release Tags**:
   - **Always** specify an exact release tag (e.g., `GIT_TAG v3.5.2`) or commit hash.
   - **Never** use floating branches like `main`, `master`, or `HEAD`. Floating branches break build reproducibility and cache consistency.
   - Enable `GIT_SHALLOW TRUE` to minimize clone times and network usage.
2. **Commit Format for Dependency Updates**:
   Use the `build(deps)` scope with old and new versions:
   ```text
   build(deps): bump Catch2 from v3.5.2 to v3.6.0
   ```
3. **Mandatory 3-Tier Preset Verification**:
   Before merging any dependency or CMake version change into `dev`, verify that all three build presets configure, build, and pass tests without warnings or sanitizer triggers:
   ```bash
   cmake --workflow --preset dev
   cmake --workflow --preset debug
   cmake --workflow --preset dist
   ```

### 3. Release Lifecycle (Dev to Master)

When a milestone is complete on `dev`:

1. Bump the version in `CMakeLists.txt`.
2. Move relevant entries from `## [Unreleased]` into a new version section in `CHANGELOG.md`:
   ```markdown
   ## [0.1.0] - 2026-10-31
   ### Added
   - **math**: templated Point2 struct with arithmetic and indexing operators
   ```
3. Commit on `dev`: `build(cmake): prepare release v0.1.0`.
4. Merge `dev` into `master` (via PR or merge commit).
5. Create an annotated Git tag on `master`:
   ```bash
   git switch master
   git merge dev
   git tag -a v0.1.0 -m "Release v0.1.0"
   git push origin master --tags
   ```

## 8. Project Structure

```text
ton-core/
├── CMakeLists.txt                  # CMake main configuration
├── CMakePresets.json               # CMake presets
├── src/
│   └── core/                       # Core library root
│       ├── CMakeLists.txt
│       ├── consts.hpp              # Fundamental constants
│       ├── types.hpp               # Standardized type definitions
│       └── math/                   # Linear algebra foundation
│           └── point2.hpp          # Point2 struct
└── test/                           # Catch2 unit tests and microbenchmarks
    ├── CMakeLists.txt
    └── math/
        └── point2_tests.cpp        # Point2 struct tests
```

