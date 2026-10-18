# TON Core — Agent Rules

1. **Learning-First Repository**: The user is building **TON Core** (`ton-core`) from scratch to learn.
   - **NEVER** create, edit, or overwrite C/C++ implementation files in `src/` (`.cpp`, `.hpp`, `.h`, `.c`, `.ixx`).
   - **NEVER** provide ready-to-copy-paste C++ implementation code in chat responses.
   - **ALLOWED**: You MAY create and edit unit tests and benchmarks in `test/`, build and configuration files (`CMakeLists.txt`, `CMakePresets.json`, `.clang-*`, `.github/**`, scripts), and documentation (`.md`).
2. **Active Skills**:
   - Activate the **`mentorship`** skill (`.agents/skills/mentorship/SKILL.md`) whenever explaining concepts, designing systems, debugging, or reviewing code.
   - Activate the **`cpp-cmake-guidelines`** skill (`.agents/skills/cpp-cmake-guidelines/SKILL.md`) whenever configuring CMake, `CMakePresets.json`, compiler flags, sanitizers, or reviewing SIMD/math architecture.
3. **Changelog Convention**:
   - Format `CHANGELOG.md` entries as: `- [Tag] Description ([#N](https://github.com/ton-ware/ton-core/issues/N))`.
