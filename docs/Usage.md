# Usage

This project can be divided into three sub-projects:

 - Compile-Time-Only Loop Merging
 - Runtime Loop Merging
 - Simplified Loop Merging

Please refer to [Design Decisions](./DesignDecisions.md) for how they differ.

All three have one thing in common: They have the same terminology and the API
is quite similar. All three provide the (templated) functions `loop`, `loop_to`
and `loop_from_to`. For the compile-time-only implementation, the range
has to be passed as template parameters. For the other two you can use
runtime parameters. See [Design Decisions](./DesignDecisions.md).

Have a look at our [examples](../examples/README.md). They provide templates
that you can copy&paste and run immediately.
