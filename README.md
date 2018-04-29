# aurora-cpp
Aurora SDK for C++

# Development
Aurora uses cmake to build the Aurora library, build and run tests, and manage dependencies. To configure the project and download dependencies, run `cmake .` in the root directory of the project.

## Generating Documentation
Aurora uses Doxygen to generate documentation. To generate documentation, you must have Doxygen installed. Then run `make doc`. Documentation will be generated in the `/docs` directory.

## Testing
Aurora uses googletest to create and run tests. After building all tests with `make`, you can run all tests with `make test`.

## Static Analysis Checking
To prevent errors and improve performance, Aurora uses cppcheck to analyze all code for deficiencies. CPPCheck is automatically invoked on every build with `make`, and it can be explicityl invoked with `make cppcheck`. CMake handles downloading and building cppcheck during the configuration stage.
