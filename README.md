# 2024-group-12

![pipeline badge](https://git.chalmers.se/courses/dit638/students/2024-group-12/badges/main/pipeline.svg)

## Dependencies

### System requirements
Ubuntu 22.04

### Tools
* build-essential
* cmake
* git
* Docker

## Getting started

To clone the repository, execute the following commands
```
git clone git@git.chalmers.se:courses/dit638/students/2024-group-12.git
cd 2024-group-12
```

Next, to build the project, create a build directory and run these commands to build the project
```
mkdir build
cd build
cmake ..
make
```

Subsequent builds only require you to run the ``make`` command, unless there have been changes to any of the CMake files

In order to build and run the tests, use the following
```
make test
```

## Way of working

We intend to use the git feature branch work flow, where we create a new branch for each issue which represents a single feature, piece of work or fix. Each feature branch is required to go through a code review by opening a merge request by the author, assigning another member for a review.
Code review comments are optional, the code review may either leave a comment or contact the author for further discussion on changes or improvements

### Commit messages

Commit messages are required to reference the issue number in the subject of the commit message.
The subject of the commit message is required, while the body is optional.

Template: 
```
#[issue ID]: [subject]

[optional body]
```

Example commit message for issue with id 12
```
#12: added more test coverage
```

## License
MIT License
