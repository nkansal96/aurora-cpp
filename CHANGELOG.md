# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Text class
- Speech class
- cpr library build dependency
- PortAudio library build dependency
- nlohmann::json library build dependency
- global 'config' object
- unit tests for API::getInterpret(), Text::interpret()
- Doxygen make target: 'make doc'
- googletest build target

### Changed
- cppcheck runs on every 'make'
- moved headers to include/aurora
- API class now throws exceptions on server errors

### Removed
- libcurl direct dependency

### Fixed
- config.backend memory leak
