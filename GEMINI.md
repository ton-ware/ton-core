# TON Core — Agent Rules

1. **Learning-First Repository**: The user is building **TON Core** (`ton-core`) from scratch to learn.
   - **NEVER** create, edit, or overwrite C/C++ implementation files in `src/` (`.cpp`, `.hpp`, `.h`, `.c`, `.ixx`).
   - **NEVER** provide ready-to-copy-paste C++ implementation code in chat responses.
   - **ALLOWED**: You MAY create and edit unit tests and benchmarks in `test/`, build and configuration files (`CMakeLists.txt`, `CMakePresets.json`, `.clang-*`, `.github/**`, scripts), and documentation (`.md`).
2. **Active Skills**:
   - Activate the **`mentorship`** skill (`.agents/skills/mentorship/SKILL.md`) whenever explaining concepts, designing systems, debugging, or reviewing code.
   - Activate the **`cpp-cmake-guidelines`** skill (`.agents/skills/cpp-cmake-guidelines/SKILL.md`) whenever configuring CMake, `CMakePresets.json`, compiler flags, sanitizers, or reviewing SIMD/math architecture.
3. **Commit & Changelog Convention (Vendor-Agnostic Conventional Commits)**:
   - **Git Commits**: Follow Conventional Commits: `<type>(<scope>): <imperative-description>`. NEVER include provider-specific issue links (e.g., `#N`, `https://github.com/...`) in commit subjects or messages.
   - **Changelog**: Under [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) (`### Added` / `### Changed` / `### Fixed` / `### Removed`), format entries as:
     `- **<scope>**: <description>` (no provider issue links).
