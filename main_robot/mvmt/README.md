# Movement Handling

This is the updated movement handling interface built arround `FastAccelStepper`.
To install `FastAccelStepper` with `arduino-cli`:
- `arduino-cli lib install FastAccelStepper`

## To block or not to block
Currently the methods `moveDist` and `rotate` _are_ blocking, this allows
the robot to be controlled simply in such fashion:
```cpp
MovementHandler hdl = MovementHandler();
hdl.moveDist(...);
hdl.rotate(...);
hdl.moveDist(...);
```
If they weren't, the robot would try to execute all these movements at the same
time which would make a mess.

Steppers can be blocked individually with their move method:
```cpp
stepperL_->move(steps, true);
stepperR_->move(steps, true);
```
In the code above, the left stepper will run first, then it will be the right
stepper. This is obviously not what we want, we want both of them to run at the
same time.

To make these methods blocking, the current solution is:
```cpp
while (stepperL_->isRunning() || stepperR_->isRunning()); 
```

However, with these while loops we are blocked. We can't interrupt the steppers
while they are not finished.

**TODO**: Figure out a better way to handle this.

### API for blocking
Functions described above are blocking by default, to make them non blocking
specify `false` as a template parameter.

```cpp
  MovementHandler hdl = MovementHandler();
hdl.moveDist<false>(...);
hdl.rotate<false>(...);
hdl.moveDist<false>(...);
```

Here the robot will try to do these all at once.
