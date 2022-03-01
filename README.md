# FOSS libraries

In my solution, I utilized the following open source libraries, and thus, I want to give
credit to the creators who helped me immensely when implementing my solution.

* https://github.com/martinus/robin-hood-hashing

* https://github.com/cameron314/concurrentqueue

---

# Introduction

This is a reference solution used in the VŠB-TU Ostrava Programming Contest 2021.
You should create your copy of the repository first using the `Fork` button (it can be found at the top of this page).
Once you have forked the repository you need a git client to download it to your computer.

```bash
$ git clone https://pgcontest.vsb.cz/gitlab/<your-username>/similarity-join.git
```

If you perform any changes in the code and you want to submit it to the testing server then run the following commands:

```bash
$ git add -u
$ git commit -m "commit message"
$ git push
```

After you push your changes, your program will be compiled, checked for correctness and evaluated
on our server. You can see the result in your GitLab repository (CI/CD -> Pipelines).

If you add a new file to the repository, you should add it to git using
```bash
$ git add <path-to-file>
```

## Linux
### Dependencies
You will need a recent C++ compiler (`GCC`, `Clang`) and `CMake` to compile the project.
On Ubuntu, you can install these with:
```bash
$ sudo apt-get install build-essential cmake
```

### Compilation
The `compile.sh` script compiles the source code.

It takes one argument with `Debug/Release` value (to compile the code in debug or optimized mode).
If you do not specify the compilation mode, `Release` will be used.

```bash
# Compile in Debug mode
$ ./compile.sh Debug

# Compile in Release mode
$ ./compile.sh Release
```

The script uses CMake for the compilation; therefore, the compilation settings are in the `CMakeLists.txt`.
The application will be built inside a directory called `build`.

### Execution
You can provide a path to a workload within the `workloads` directory which will be used as an input for the program.
If you do not specify the workload, `mini` will be used.

```bash
$ ./run.sh mini
```

If you want to make sure that the output is correct, you can use the `diff` utility to compare the program output with
the expected output stored in the `workloads/<workload>/output` file.

```bash
$ ./run.sh mini | diff -s - workloads/mini/output
```

