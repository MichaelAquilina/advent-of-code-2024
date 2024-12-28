Advent of Code 2024
-------------------

Solutions for Advent of Code 2024.
Written in C++ as a means of improving my familiarity with the ecosystem.

Uses bazel as a build management tool.

Solutions for days are organised into targets:

```sh
bazel run //day01:main -- /absolute/path/to/input.txt
```

Make sure the path to the input file is absolute for this to work due to the way bazel runs within its own sandbox
