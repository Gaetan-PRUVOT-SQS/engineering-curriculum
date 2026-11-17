# engineering-curriculum

English · [Français](README.fr.md)

Engineering curriculum. We start from the Linux workstation, the compiler,
then a C program that shows the machine.

## Contents

- `docs/learning-objectives.md`: what we must be able to do again
- `docs/evaluation-rules.md`: how we decide a milestone is met
- `ROADMAP.md`: what comes next
- `PROGRESS.md`: what is already done
- `00-environment/`: the workstation used for this repository
- `projects/system-toolkit/`: the `system-info` program

## Get the repository

```
git clone https://github.com/Gaetan-PRUVOT-SQS/engineering-curriculum.git
cd engineering-curriculum
```

Build, test and run the program:

```
make test
make -C projects/system-toolkit
./projects/system-toolkit/build/system-info
```

`make test` at the root runs the `projects/system-toolkit` suite. You need
`gcc`, `make`, `gdb`, `valgrind`, `bats`, `shellcheck`, `checkbashisms`,
`shfmt`, `dash` and `bash`. Workstation details are in
`00-environment/README.md`. Program details are in
`projects/system-toolkit/README.md`.
