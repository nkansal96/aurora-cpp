# aurora-cpp
Aurora SDK for C++

# Development
Aurora uses cmake to build, test, and manage dependencies. To get started, run `cmake .` in the root directory of the project.

## Generating Documentation
Aurora uses Doxygen to generate documentation. To generate documentation, you must have Doxygen installed. Then run `make doc`. Documentation will be generated in the `/docs` directory.

## Testing
Aurora uses googletest to create and run tests. After building all tests with `make`, you can run all tests with `make test`.
