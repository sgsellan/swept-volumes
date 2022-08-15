# Swept Volumes via Spacetime Numerical Continuation
Public code release for ["Swept Volumes via Spacetime Numerical Continuation"](http://dgp.toronto.edu/~sgsellan/pdf/swept-volumes.pdf), presented at SIGGRAPH 2021 and authored by [Silvia Sellán](http://dgp.toronto.edu/~sgsellan/), [Noam Aigerman](https://noamaig.github.io) and [Alec Jacobson](http://www.cs.toronto.edu/~jacobson/). Please note that while this implementation of our method is hereby released under MIT License, the method itself is pending a US patent filed in 2021 by Adobe Inc.

# Important note

An up-to-date, maintained version of this code is implemented into our new python geometry processing library [Gpytoolbox](https://gpytoolbox.org). See the swept volume documentation and examples [here](https://gpytoolbox.org/copyleft/swept_volume/).

## Installation

To install this library, please start by cloning the repository recursively
```
git clone --recursive git@github.com:sgsellan/swept-volumes.git
```
After this, compile in release mode using `cmake` following the usual steps:
```
cd swept-volumes
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make all -j8
```
If all goes right, you'll find the executable `swept-volumes-gui` in the main repository directory. If not, we provide a MacOS precompiled binary in `precompiled/osx/bin/swept-volumes-gui` that you can move to the main repository directory.

## Use

You can begin by running `./swept-volumes-gui` with no arguments. A viewer should appear showing a bunny:

![](assets/viewer-1.png)

You can then use W, E and R to flip between translation, rotation and scaling to move the bunny around.

![](assets/viewer-2.png)

Press "B" to bake a keyframe in. A copy of the bunny will appear fixed in the current position, and you can move the original bunny around to select your second keyframe:

![](assets/viewer-3.png)

Keep doing this until you are happy with the trajectory you've built:

![](assets/viewer-4.png)

Remember to press "B" to bake the last frame. Then, press "S" to run our algorithm to compute the swept volume. Wait for a couple of seconds (up to a minute in this example) and you'll see our output (this took 25s in my MacBook):

![](assets/viewer-5.png)

Press "V" to compare to a stamping algorithm with 10 stamps. Keep pressing "V" to see 100 stamps and 1000 stamps.

![](assets/viewer-6.png)

If you want to go back to our output, press "C". You can now close the viewer window, all the files have been saved to `data/experiments/dd-mm-hh-mm` in `.obj` format.

Once you have tried this demo, there are many arguments you can pass to our gui, in whichever order. `-i path/to/shape.obj` will change the input shape (any libigl-readable format is accepted and there are no requirements on manifold-ness or close-ness, default is `data/bunny.obj`), `-e number` changes the edge-length of the grid used in our algorithm (lower will be slower and more precise, default is `0.02`). A sample call may be
```
./swept-volumes-gui -i path/to/your/shape.obj -e 0.01
```

A particularly interesting argument is `-load`, by which we can load the parameters and trajectory of a previous experiment. When using `-load`, the order of arguments does matter: every argument after `-load` will overwrite the loaded one. For example,
```
./swept-volumes-gui -i data/armadillo.obj -load data/experiments/experiment-name/
```
will ignore the `-i data/armadillo.obj` part and load whichever input was used in `experiment-name` experiment. However,
```
./swept-volumes-gui -load data/experiments/experiment-name/ -i data/armadillo.obj 
```
will recreate `experiment-name` *but with the armadillo as input*. We can even add new keyframes when loading a previous experiment, or run the same experiment with a finer grid size:
```
./swept-volumes-gui -load data/experiments/experiment-name/ -e 0.01 
```


## Known Issues
This released code uses marching cubes as a surface generation algorithm, while our paper examples use dual contouring. I plan on adding dual contouring functionality to this repository soon when I manage to clean our original research code :-). All other elements in the algorithm are reproduced here exactly as were used to produce the examples in our paper.

Please do not hesitate to contact [sgsellan@cs.toronto.edu](mailto:sgsellan@cs.toronto.edu) if you find any issues or bugs in this code, or you struggle to run it in any way.
