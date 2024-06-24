# Job Execution using Monitors in C

This program demonstrates how to execute processes in a specific order using monitors (mutexes and condition variables) in C.

## Process Order

The processes follow this execution order:

```
    _____P4____
   /           \
 P3------P5-----P8
   \_____P6____/

```

- **P3**: Runs first.
- **P4, P5, P6**: Run simultaneously after P3 completes.
- **P8**: Runs after P4, P5, and P6 complete.
